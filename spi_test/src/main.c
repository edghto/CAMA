#include <spi.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main( int argc, char* argv[] )
{
	spi_init();
	sei();

	for(;;) _delay_ms(1000);

	return 0;
}
