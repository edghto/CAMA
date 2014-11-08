#include <M74HC595.h>

void 
M74HC595_Reset()
{
	
}

void 
M74HC595_Init()
{
	M74HC595_DDR |= _BV(LATCH_PIN);
	M74HC595_DDR |= _BV(DATA_PIN);
	M74HC595_DDR |= _BV(CLOCK_PIN);

	M74HC595_PORT |= _BV(LATCH_PIN); /* register is active */
	M74HC595_PORT &= ~_BV(CLOCK_PIN); /* data are not send to shift register */
}

void 
M74HC595_ShiftOut( unsigned char data )
{
	M74HC595_PORT &= ~_BV(LATCH_PIN);
	for( unsigned int i = 7; i >=0; --i )
	{
		if( data >> i )
			M74HC595_PORT |= _BV(DATA_PIN);
		else
			M74HC595_PORT &= ~_BV(DATA_PIN);
		M74HC595_TOOGLE_CLOCK;
	}
	M74HC595_PORT |= _BV(LATCH_PIN);
}

