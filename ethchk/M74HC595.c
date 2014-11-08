#include "M74HC595.h"

static void M74HC595_ShiftOut_MSB( unsigned char data );
static void M74HC595_ShiftOut_LSB( unsigned char data );

void
M74HC595_Init()
{
	M74HC595_DDR |= _BV(LATCH_PIN);
	M74HC595_DDR |= _BV(DATA_PIN);
	M74HC595_DDR |= _BV(CLOCK_PIN);

	M74HC595_PORT |= _BV(LATCH_PIN); /* register is active */
	M74HC595_PORT &= ~_BV(CLOCK_PIN); /* data are not send to shift register */
}

inline void
M74HC595_ShiftOut( unsigned char data, M74HC595_ByteOrder_e byteOrder )
{
    switch(byteOrder)
    {
    case M74HC595_LSB: M74HC595_ShiftOut_LSB( data ); break;
    case M74HC595_MSB: M74HC595_ShiftOut_MSB( data ); break;
    default:;
    }
}

void
M74HC595_ShiftOut_MSB( unsigned char data )
{
	M74HC595_PORT &= ~_BV(LATCH_PIN);

	for( int i = 7; i >=0; --i )
	{
		if( data & (1 << i) )
			M74HC595_PORT |= _BV(DATA_PIN);
		else
			M74HC595_PORT &= ~_BV(DATA_PIN);

		M74HC595_PORT |= _BV(CLOCK_PIN);
		M74HC595_PORT &= ~_BV(CLOCK_PIN);
	}
	M74HC595_PORT |= _BV(LATCH_PIN);
}

void
M74HC595_ShiftOut_LSB( unsigned char data )
{
	M74HC595_PORT &= ~_BV(LATCH_PIN);

	for( int i = 0; i <= 7; ++i )
	{
		if( data & (1 << i) )
			M74HC595_PORT |= _BV(DATA_PIN);
		else
			M74HC595_PORT &= ~_BV(DATA_PIN);

		M74HC595_PORT |= _BV(CLOCK_PIN);
		M74HC595_PORT &= ~_BV(CLOCK_PIN);
	}
	M74HC595_PORT |= _BV(LATCH_PIN);
}

