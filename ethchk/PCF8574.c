#include "PCF8574.h"
#include <avr/io.h>

PCF8574_Error_e
PCF8574_Init( PCF8574_Data_s* data, unsigned char addr )
{
    data->addr = addr;
    i2c_init();
    return PCF8574_NOERROR;
}

PCF8574_Error_e
PCF8574_Get( PCF8574_Data_s* data, unsigned char* value )
{
    PCF8574_Error_e error = PCF8574_NOERROR;
    unsigned char addr = ( ( data->addr << 1 ) | 1 ); /* addr + read*/

    if( i2c_start( addr ) )
        error = PCF8574_ERROR;
    *value = i2c_read( 0 ); //data (8bit)
    i2c_stop();

    return error;
}

PCF8574_Error_e
PCF8574_Set( PCF8574_Data_s* data, unsigned char value )
{
    PCF8574_Error_e error = PCF8574_NOERROR;
    unsigned char addr = ( data->addr << 1 );  /* addr + write(not read)*/

    if( i2c_start( addr ) )
        error = PCF8574_ERROR;

    if( i2c_write( value ) )
        error = PCF8574_ERROR;

    i2c_stop();

    return error;
}

