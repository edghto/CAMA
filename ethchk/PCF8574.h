#ifndef PCF8574_H_INCLUDED
#define PCF8574_H_INCLUDED

#if 0
#include "i2c.h"
#else
#include "i2cmaster.h"
#endif

/* Address of PCF8574: 0 1 0 0 A2 A1 A0 */
inline unsigned char PCF8574_ADDR(unsigned char x)
{
    return ( 0x20 | x );
}

typedef struct
{
    unsigned char addr;
} PCF8574_Data_s;

typedef enum
{
    PIN01 = 0x01,
    PIN02 = 0x02,
    PIN03 = 0x04,
    PIN04 = 0x08,
    PIN05 = 0x10,
    PIN06 = 0x20,
    PIN07 = 0x64,
    PIN08 = 0x80
} PCF8574_Pin_e;

typedef enum
{
    PCF8574_NOERROR,
    PCF8574_ERROR
} PCF8574_Error_e;

PCF8574_Error_e PCF8574_Init( PCF8574_Data_s* data, unsigned char addr );
PCF8574_Error_e PCF8574_Get( PCF8574_Data_s* data, unsigned char* value );
PCF8574_Error_e PCF8574_Set( PCF8574_Data_s* data, unsigned char value );


#endif // PCF8574_H_INCLUDED
