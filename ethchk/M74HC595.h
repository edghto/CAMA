#include <avr/interrupt.h>
#include <avr/io.h>

#define LATCH_PIN PB0
#define DATA_PIN  PB1
#define CLOCK_PIN PB2
#define M74HC595_PORT PORTB
#define M74HC595_DDR  DDRB

typedef enum
{
    M74HC595_LSB,
    M74HC595_MSB
} M74HC595_ByteOrder_e;

void M74HC595_Init();
void M74HC595_ShiftOut( unsigned char data, M74HC595_ByteOrder_e byteOrder );
