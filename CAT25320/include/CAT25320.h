#ifndef CAT25320_H_INCLUDED
#define CAT25320_H_INCLUDED

#include <avr/io.h>
#include <util/delay.h>

#define CAT25320_DDR   DDRB
#define CAT25320_PORT  PORTB
#define CAT25320_SCK   _BV(PB7)
#define CAT25320_SI    _BV(PB6)  //PB6 (DO/PCINT6)
#define CAT25320_SO    _BV(PB5)  //PB5 (DI/SDA/PCINT5)
#define CAT25320_NHOLD _BV(PB3)
#define CAT25320_NCS   _BV(PB4)
#define CAT25320_NWP   _BV(PB2)

#define CAT52320_ADDRESS_LOWBITS(addr) ((uint8_t) addr & 0xff )
#define CAT52320_ADDRESS_HIGHBITS(addr) ((uint8_t) ( addr >> 8 ) )

typedef enum
{
    CAT52320_WPEN = 0b10000000,
    CAT52320_BP1  = 0b00001000,
    CAT52320_BP2  = 0b00000100,
    CAT52320_WEL  = 0b00000010,
    CAT52320_NRDY = 0b00000001
} CAT52320_SR_Bits;

typedef enum
{
    CAT52320_NOERROR,
    CAT52320_ERROR
} CAT52320_Error;

typedef enum
{
    CAT52320_FALSE,
    CAT52320_TRUE
} CAT52320_Bool;

typedef enum
{
    CAT52320_BP_NONE    = 0b00,
    CAT52320_BP_QUARTER = 0b01,
    CAT52320_BP_HALF    = 0b10,
    CAT52320_BP_FULL    = 0b11
} CAT52320_BlockProtectionRange;

typedef enum
{
    CAT52320_OP_WREN  = 0b00000110,
    CAT52320_OP_WRDI  = 0b00000100,
    CAT52320_OP_RDSR  = 0b00000101,
    CAT52320_OP_WRSR  = 0b00000001,
    CAT52320_OP_READ  = 0b00000011,
    CAT52320_OP_WRITE = 0b00000010
} CAT52320_Operations;


void CAT52320_Init( void );

CAT52320_Error CAT52320_WriteStatusRegister( uint8_t StatusRegister );

CAT52320_Error CAT52320_ReadStatusRegister( uint8_t* StatusRegister );

CAT52320_Error CAT52320_EnableWriteOpertation( void );

CAT52320_Error CAT52320_DisableWriteOpertation( void );

/*
 * It will write only one page (32 bytes). If the size exceeds on page size
 * the write will roll over to the beginning of current page.
 */
CAT52320_Error CAT52320_WriteMemoryPage( uint16_t address, uint8_t* buffer, uint16_t size );

/*
 * If read will reach the highest memory address, it will 
 * roll over and continue to read from the lowest address.
 */
CAT52320_Error CAT52320_ReadMemory( uint16_t address, uint8_t* buffer, uint16_t size );

CAT52320_Bool CAT52320_isReady( void );

CAT52320_Error CAT52320_EnableWriteProtection( void );

CAT52320_Error CAT52320_DisableWriteProtection( void );

CAT52320_Error CAT52320_setBlockProtection( CAT52320_BlockProtectionRange range );

#endif // CAT25320_H_INCLUDED

