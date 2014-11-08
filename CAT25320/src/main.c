#include <util/delay.h>
#include <CAT25320.h>
#include <uart.h>

#define BUTTON _BV(PD5)

void stringify( unsigned char c, unsigned char* buffer )
{
    unsigned char i = 0;
    while( i < 8 )
    {
        buffer[7-i] = (c & (1<<i)) ? '1' : '0';
        ++i;
    }
    buffer[8] = '\0';
}

int main( int argc, char* argv[] )
{
    unsigned char str[9];
    UART_Init( UART_UBBR_VALUE );
    CAT52320_Init();

    sei();

    while( PIND & BUTTON ) _delay_ms( 100 );

    UART_Transmit("start\r\n");

    for(;;)
    {
        unsigned char reg = 0b01110000;
        if( CAT52320_NOERROR == CAT52320_ReadStatusRegister( &reg ) )
        {
            stringify( reg, str );
            UART_Transmit( str );
        }
        else
            UART_Transmit( "error" );

        UART_Transmit("\r\n");
        _delay_ms( 1000 );
    }

    return 0;
}

