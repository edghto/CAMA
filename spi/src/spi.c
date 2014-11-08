#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <spi.h>

static unsigned char isSelected = 0;

void spi_init( void )
{
	USICR = _BV(USIWM0) | _BV(USICS1);

	MCUCR = _BV(ISC00);
	GIMSK = _BV(INT0);

	DDRB |=  _BV(PB6); //DO as output
	DDRB &= ~_BV(PB5); //DI as input
	DDRB &= ~_BV(PB7); //SCK as input

	DDRB |= _BV(PB2);
}

unsigned char spi_transfer_byte( unsigned char msg )
{
    USISR = 0xf0;         // reset counter to 0 and clears all flags
    USIDR = msg;

    do
    {
        /*      three-wire,    ext clk,     use USITC,    toggle USCK */
        USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
    } while( !( USISR & _BV(USIOIF) ) );

    return USIDR;
}

unsigned char spi_transfer( unsigned char* msg, unsigned int msg_size )
{
    do
    {
        *msg = spi_transfer_byte( *msg );
        ++msg;
    } while (--msg_size);

    return 0;
}

ISR( INT0_vect )
{
	isSelected = PIND & _BV(PD2);
		
	if( isSelected )
	{
		PORTB |= _BV(PB2);
		USISR =  _BV(USIOIF); //Reset counter and overflow flag
		USICR |= _BV(USIOIE); //Enable overflow interrupt
	}
	else
	{
		PORTB &= ~_BV(PB2);
		USICR &= ~_BV(USIOIE); //Disable overflow interupt
	}
}

ISR( USI_OVERFLOW_vect )
{
	unsigned char data = USIDR;
	USIDR = ~data;
	USISR =  _BV(USIOIF); //reser interrupt flag
}

