#include <avr/interrupt.h>
#include <avr/io.h>
#include <M74HC595.h>

int 
main( void )
{
	M74HC595_Init();
	M74HC595_ShiftOut( 0b11000101 ); /* 0,2,6,7 pin is on */

	while( 1 );

	return 0;
}
