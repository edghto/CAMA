#include <uart.h>

#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include "circbuffer.h"

static CircBuffer_t gOutputCircBuffer;
#ifndef DISABLE_READ
static CircBuffer_t gInputCircBuffer;
#endif

void UART_Init( unsigned int baud )
{
    /* Set baud rate */
    UBRRH = ( uint8_t ) ( baud >> 8 );
    UBRRL = ( uint8_t ) baud;

    /* Enable receiver and transmitter */
    UCSRB = ( 1 << RXEN ) | ( 1 << TXEN );

    /* Set frame format to 8 data bits, no parity, 1 stop bit */
    UCSRC = ( 1 << UCSZ1 ) | ( 1 << UCSZ0 );

    /* Enable UART receive interrupt */
#ifndef DISABLE_READ
    UCSRB |= (1 << RXCIE );
    CircBuffer_Init( &gInputCircBuffer );
#endif

    CircBuffer_Init( &gOutputCircBuffer );
}

unsigned int UART_Transmit( const char* data )
{
    unsigned int i = 0;
    for( ; *data; ++data, ++i )
        if( CIRCBUFFER_NOERROR != CircBuffer_Write( &gOutputCircBuffer, *data ) )
            break;

    UCSRB |= (1 << UDRIE );

    return i;
}

#ifndef DISABLE_READ
unsigned int UART_Receive( char* data, unsigned int len )
{
    unsigned int i = 0;
    unsigned char c = '\n';
    while( i < len )
    {
        if( !CircBuffer_IsEmpty( &gInputCircBuffer ) )
        {
//            c = UART_ReadByte();
            CircBuffer_Read( &gInputCircBuffer, &c );
#ifdef SKIP_INVALID_CHARS
            if( !( c & UART_VALID_CHAR_MASK ) )
                break;
#endif
            data[i++] = c;
        }
        else
            break;

        if( c == '\n' )
            break;
    }

    return i;
}
#endif

#ifndef DISABLE_READ
ISR( USART_RX_vect )
{
    volatile unsigned char data = UDR;
    //TODO error check
    if( CIRCBUFFER_NOERROR != CircBuffer_Write( &gInputCircBuffer, data ) )
        PORTB |= _BV(PB4);
}
#endif

ISR( USART_UDRE_vect )
{
    unsigned char data;

    if( CIRCBUFFER_NOERROR == CircBuffer_Read( &gOutputCircBuffer, &data ) )
        UDR = data;
    else
        UCSRB &= ~(1 << UDRIE);
}

#ifdef THIS_IS_WORKING
/* left for reference */
ISR( USART_RX_vect )
{
    unsigned char data = UDR;
    while( !( UCSRA & ( 1 << UDRE ) ) ) ;
    UDR = data;
}
#endif
