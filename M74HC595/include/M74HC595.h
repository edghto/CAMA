#include <avr/interrupt.h>
#include <avr/io.h>

#define LATCH_PIN PB0
#define DATA_PIN  PB1
#define CLOCK_PIN PB2
#define M74HC595_PORT PORTB
#define M74HC595_DDR  DDRB
#define M74HC595_TOOGLE_CLOCK ( M74HC595_PORT |= _BV(CLOCK_PIN), M74HC595_PORT &= ~_BV(CLOCK_PIN) )

void M74HC595_Reset();
void M74HC595_Init();
void M74HC595_ShiftOut( unsigned char data );
