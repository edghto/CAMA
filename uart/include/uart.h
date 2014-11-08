#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include <avr/interrupt.h>

#ifndef F_CPU
#error F_CPU is required
#endif

#define UART_BAUD 2400L
#define UART_UBBR_VALUE ((F_CPU/(UART_BAUD<<4))-1)

#define UART_DELAY 10

#define UART_INVALID_CHAR 0x80
#define UART_VALID_CHAR_MASK ( ~0x80 )

#define DISABLE_READ

/* If interrupt is meant to be used then sei() has to be invoked after init */
void UART_Init( unsigned int baud );

unsigned int UART_Transmit( const char* data );

#ifndef DISABLE_READ
unsigned int UART_Receive( char* data, unsigned int len );
#endif

#endif // UART_H_INCLUDED
