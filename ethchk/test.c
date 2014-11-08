#define F_CPU 1000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "PCF8574.h"
#include "M74HC595.h"

#define DELAY 1000

void
testShiftRegister( void )
{
	const unsigned char COUNT = 0xf;
	M74HC595_Init();

	while( 1 )
	{
		for( unsigned char i = 0; i <= COUNT; ++i )
		{
			M74HC595_ShiftOut( i, M74HC595_MSB );
			_delay_ms(DELAY);
		}
	}
}

void
testI2COutput( void )
{
	PCF8574_Data_s output;
	const unsigned char COUNT = 0xf;
	PCF8574_Init( &output, PCF8574_ADDR( 0b000 ) );
	while( 1 )
	{
		for(unsigned char i = 0; i <= COUNT; ++i )
		{
			PCF8574_Set( &output, i );
			_delay_ms(DELAY);
		}
	}
}

void
testI2CInput( void )
{
	PCF8574_Data_s input;

	M74HC595_Init();
	PCF8574_Init( &input, PCF8574_ADDR( 0b000 ) );

	while( 1 )
	{
		unsigned char pins = 0;

		PCF8574_Get( &input, &pins );
		M74HC595_ShiftOut( pins, M74HC595_MSB );
		_delay_ms(100);
	}
}

int
test_main( void )
{
	unsigned char testNumber = 0;

	DDRD &= ~_BV(PD0);
	DDRD &= ~_BV(PD1);
	DDRD |= _BV(PD2);

	PORTD &= ~_BV(PD2);

	testNumber = (((PIND & _BV(PD0)) != 0) << 1 ) | ((PIND & _BV(PD1)) != 0);

	switch(testNumber)
	{
		case 1:
			testShiftRegister();
			break;
		case 2:
			testI2COutput();
			break;
		case 3:
			testI2CInput();
			break;
		default:
			while( 1 )
			{
				PORTD |= _BV(PD2);
				_delay_ms(200);
				PORTD &= ~_BV(PD2);
				_delay_ms(200);
			}
	}

	return 0;
}
