#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <spi.h>

#ifdef USI_SPI_SLAVE_MODE

static inline unsigned char isSelected()
{
	unsigned char s;
#if defined(USI_SPI_SS_INT0)
	s = PIND & _BV(PD2);
#elif defined(USI_SPI_SS_INT1)
	s = PIND & _BV(PD3);
#elif defined(USI_SPI_SS_PCINT)
	s = PINB & ( 1 << USI_SPI_SS_PCINT_N );
#endif 

#ifdef USI_SPI_USE_NCS
	return 0 == s;
#else
	return 0 != s;
#endif
}

#endif  /* end of USI_SPI_SLAVE_MODE */

void USI_SPI_Init( void )
{
#ifdef USI_SPI_SLAVE_MODE

	#if defined(USI_SPI_SS_INT0)
		MCUCR = _BV(ISC00);
		GIMSK = _BV(INT0);
	#elif defined(USI_SPI_SS_INT1)
		MCUCR = _BV(ISC10);
		GIMSK = _BV(INT1);
	#elif defined(USI_SPI_SS_PCINT)
		GIMSK = _BV(PCIE);
		PCMSK = ( 1 << USI_SPI_SS_PCINT_N );
	#endif

	USICR = _BV(USIWM0) | _BV(USICS1);  //enables SPI
	DDRB |= USI_SPI_MOSI;  //DO as output
	DDRB &= ~USI_SPI_MISO; //DI as input
	DDRB &= ~USI_SPI_USCK; //SCK as input

#else /* Master mode */
	/*      three-wire,    ext clk,     use USITC */
	USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICLK);
	DDRB |= USI_SPI_MOSI;  //DO as output
	DDRB |= USI_SPI_USCK;  //USCK as output
	DDRB &= ~USI_SPI_MISO; //DI as input
#endif

#ifdef USI_SPI_MODE1
	USICR |= _BV(USICS0);
#endif
}

unsigned char USI_SPI_TransferByte( unsigned char msg )
{
    USISR = 0xf0; // reset counter to 0 and clears all flags
    USIDR = msg;

    do
    {
        USICR |= _BV(USITC);
//		USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
    } while( !( USISR & _BV(USIOIF) ) );

    return USIDR;
}

unsigned char USI_SPI_Transfer( unsigned char* msg, unsigned int msg_size )
{
    do
    {
        *msg = USI_SPI_TransferByte( *msg );
        ++msg;
		_delay_ms( USI_SPI_BYTE_DELAY );
    } while (--msg_size);

    return 0;
}

#ifdef USI_SPI_SLAVE_MODE

#if defined(USI_SPI_SS_INT0)
ISR( INT0_vect )
#elif defined(USI_SPI_SS_INT1)
ISR( INT1_vect )
#elif defined(USI_SPI_SS_PCINT)
ISR( PCINT_vect )
#else
#error Something is wrong
#endif
{
	if( isSelected() )
	{
		USISR =  _BV(USIOIF); //Reset counter and overflow flag
		USICR |= _BV(USIOIE); //Enable overflow interrupt
	}
	else
	{
		USICR &= ~_BV(USIOIE); //Disable overflow interupt
	}
}

ISR( USI_OVERFLOW_vect )
{
	unsigned char data = USIDR;
	USIDR = USI_SPI_callback( data );
	USISR =  _BV(USIOIF); //reser interrupt flag
}

#endif /* end of USI_SPI_SLAVE_MODE */

