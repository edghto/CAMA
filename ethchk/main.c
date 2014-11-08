#define F_CPU 1000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "PCF8574.h"
#include "M74HC595.h"

/* RJ-45 has 8 pins */
#define ETHCHK_NUM 8

#define DELAY 200
#define BLINK_DELAY 200

#define GREEN_PIN PD0
#define RED_PIN   PD1

#define PATTERN_TYPE_PIN PD2

#define GREEN_PIN_ON  PORTD |= _BV(GREEN_PIN)
#define RED_PIN_ON    PORTD |= _BV(RED_PIN)
#define GREEN_PIN_OFF PORTD &= ~_BV(GREEN_PIN)
#define RED_PIN_OFF   PORTD &= ~_BV(RED_PIN)

static PCF8574_Data_s gPCF8574_Data;

typedef enum
{
    ETHCHK_NOERROR,
    ETHCHK_EIO,
    ETHCHK_TEST_FAIL
} EthChk_Error_t;

typedef struct
{
    unsigned char in, out;
} EthChk_Result_t;

static EthChk_Result_t gResults[ETHCHK_NUM];

/* there are two possible patterns */
#define NUM_OF_PATTERNS 2

typedef enum
{
    ETHCHK_STRAIGHTTHRU = 0,
    ETHCHK_CROSSOVER
} EthChk_PatternType_t;

/* on i-th position contains expected value */
static const char gPatterns[NUM_OF_PATTERNS][ETHCHK_NUM] =
{
    {
        0, 1, 2, 3, 4, 5, 6, 7    /* straight-thru */
    },
    {
        2, 5, 0, 3, 4, 1, 6, 7    /* crossover*/
    }
};

static EthChk_PatternType_t gPatternType = ETHCHK_STRAIGHTTHRU;

EthChk_Error_t EthChk_Init()
{
	if( PCF8574_NOERROR != PCF8574_Init( &gPCF8574_Data, PCF8574_ADDR( 0b000 ) ) )
        return ETHCHK_EIO;

	M74HC595_Init();

	DDRD |= _BV(GREEN_PIN);
	DDRD |= _BV(RED_PIN);
	DDRD &= ~_BV(PATTERN_TYPE_PIN);

	GREEN_PIN_OFF;
	RED_PIN_OFF;

	/* blink red */
	RED_PIN_ON;
    _delay_ms(BLINK_DELAY);
	RED_PIN_OFF;
    _delay_ms(BLINK_DELAY);

    /* blink green */
    GREEN_PIN_ON;
    _delay_ms(BLINK_DELAY);
    GREEN_PIN_OFF;

    gPatternType = (PIND & _BV(PATTERN_TYPE_PIN)) != 0 ?
        ETHCHK_STRAIGHTTHRU : ETHCHK_CROSSOVER;

    return ETHCHK_NOERROR;
}

EthChk_Error_t EthChk_Write( unsigned char value )
{
    M74HC595_ShiftOut( value, M74HC595_MSB );
    return ETHCHK_NOERROR;
}

EthChk_Error_t EthChk_Read(  unsigned char *value )
{
    return PCF8574_NOERROR != PCF8574_Get( &gPCF8574_Data, value )
            ? ETHCHK_EIO : ETHCHK_NOERROR;
}

int EthChk_byteToPin( unsigned char byte )
{
    int i;
    for ( i = 0; ( byte >> i ) && i <= ETHCHK_NUM; ++i );

    return i > ETHCHK_NUM ? -1 : i - 1;
}

#ifdef DEBUG
void blinker( unsigned char byte, unsigned char color )
{
    for( int i = 0; i < byte; ++i )
    {
        if( color )
        {
            GREEN_PIN_ON;
            _delay_ms(BLINK_DELAY);
            GREEN_PIN_OFF;
            _delay_ms(BLINK_DELAY);
        }
        else
        {
            RED_PIN_ON;
            _delay_ms(BLINK_DELAY);
            RED_PIN_OFF;
            _delay_ms(BLINK_DELAY);
        }

    }
}
#endif

EthChk_Error_t EthChk_Test()
{
    const unsigned int COUNT = 0xff;
    unsigned char i = 0;
    EthChk_Error_t error = ETHCHK_NOERROR;

    /* testing and getting results */
    for( unsigned int out = 1; out <= COUNT; out<<=1 )
    {
        unsigned char in;
        error = EthChk_Write( out );
        if( ETHCHK_NOERROR != error )
            break;

        _delay_ms(DELAY);

        error = EthChk_Read( &in );
        if( ETHCHK_NOERROR != error )
            break;

        gResults[i].out = (unsigned char) out; /* pin out was set */
        gResults[i].in  = in;  /* pin in was received */
        i += 1;
    }

    /* verifying results */
    if( ETHCHK_NOERROR == error )
    {
        for( i = 0; i < ETHCHK_NUM; ++i )
        {
            int outPin = EthChk_byteToPin( gResults[i].out );
            int inPin  = EthChk_byteToPin( gResults[i].in );

            if( -1 == outPin || -1 == inPin )
            {
                error = ETHCHK_EIO;
                break;
            }

            /* For pin "outPin" we should get value "gPatterns[patternType][outPin]"
             * and we have received "inPin". Is it correct? Lets test it ;) */
            if( gPatterns[gPatternType][outPin] != inPin )
            {
                error = ETHCHK_TEST_FAIL;
                break;
            }
        }
    }

    return error;
}

int
main( void )
{
	EthChk_Error_t error = ETHCHK_NOERROR;

    error = EthChk_Init();
    if( ETHCHK_NOERROR != error )
        goto error_label;

    error = EthChk_Test();
    if( ETHCHK_NOERROR !=  error )
        goto error_label;

    RED_PIN_OFF;
    GREEN_PIN_ON;
	for( ;; )
        _delay_ms(DELAY * 10);

error_label:
    GREEN_PIN_OFF;

    switch( error )
    {
    case ETHCHK_TEST_FAIL:
        RED_PIN_ON;
        for( ;; );
        break;
    default:
        for( ;; )
        {
            RED_PIN_ON;
            _delay_ms(BLINK_DELAY);
            RED_PIN_OFF;
            _delay_ms(BLINK_DELAY);
        }

    }

	return 0;
}
