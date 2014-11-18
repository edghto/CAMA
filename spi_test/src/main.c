#include <spi.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char USI_SPI_callback( unsigned char data )
{
	return ~data;
}

#define SS_PIN PD2
#define NSS_HIGH { PORTD |= _BV(SS_PIN); }
#define NSS_LOW  { PORTD &= ~_BV(SS_PIN); }

int main( int argc, char* argv[] )
{
	USI_SPI_Init();
	DDRD |= _BV(SS_PIN);
    NSS_HIGH;
	sei();

#if USI_SPI_SLAVE_MODE
	for(;;) _delay_ms(1000);
#else
	for( unsigned char i = 0 ;; i=(i>255?0:i+1) )
	{
		NSS_LOW;
		_delay_ms(100);
		//USI_SPI_Transfer( msg, 2 );
		USI_SPI_TransferByte( i );
		_delay_ms(100);
		NSS_HIGH;
		_delay_ms(900);
	}
#endif

	return 0;
}
