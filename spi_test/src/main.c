#include <spi.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char USI_SPI_callback( unsigned char data )
{
	return ~data;
}

int main( int argc, char* argv[] )
{
	USI_SPI_Init();
	sei();

	for(;;) _delay_ms(1000);

	return 0;
}
