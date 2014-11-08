#include <CAT25320.h>
#include <spi.h>

#define CAT52320_SELECT()   { CAT25320_PORT &= ~CAT25320_NCS; }
#define CAT52320_DESELECT() { CAT25320_PORT |=  CAT25320_NCS; }

void CAT52320_Init( void )
{
    CAT25320_DDR |= CAT25320_SCK | CAT25320_SI;
    CAT25320_DDR |= CAT25320_NHOLD | CAT25320_NCS | CAT25320_NWP;
    CAT25320_PORT |= CAT25320_NHOLD | CAT25320_NCS | CAT25320_NWP;

    CAT25320_PORT &= ~CAT25320_SCK;
}

CAT52320_Error CAT52320_WriteStatusRegister( unsigned char StatusRegister )
{
    CAT52320_Error error = CAT52320_NOERROR;
    unsigned char msg[2] = { CAT52320_OP_WRSR, StatusRegister };

    CAT52320_SELECT()
    if( spi_transfer( msg, 2 ) )
    {
        error = CAT52320_ERROR;
    }
    CAT52320_DESELECT();

    return error;
}

CAT52320_Error CAT52320_ReadStatusRegister( unsigned char* StatusRegister )
{
    CAT52320_Error error = CAT52320_NOERROR;
    unsigned char msg[2] = { CAT52320_OP_RDSR, *StatusRegister };

    CAT52320_SELECT()
    if( spi_transfer( msg, 2 ) )
    {
        error = CAT52320_ERROR;
    }
    CAT52320_DESELECT();

    *StatusRegister = msg[1];

    return error;
}

CAT52320_Error CAT52320_EnableWriteOpertation( void )
{
    CAT52320_Error error = CAT52320_NOERROR;
    unsigned char msg[1] = { CAT52320_OP_WREN };

    CAT52320_SELECT()
    if( spi_transfer( msg, 1 ) )
    {
        error = CAT52320_ERROR;
    }
    CAT52320_DESELECT();

    return error;
}

CAT52320_Error CAT52320_DisableWriteOpertation( void )
{
    CAT52320_Error error = CAT52320_NOERROR;
    unsigned char msg[1] = { CAT52320_OP_WRDI };

    CAT52320_SELECT()
    if( spi_transfer( msg, 1 ) )
    {
        error = CAT52320_ERROR;
    }
    CAT52320_DESELECT();

    return error;
}

CAT52320_Error CAT52320_WriteMemory( unsigned char* buffer, unsigned int size )
{
    return CAT52320_ERROR;
}

CAT52320_Error CAT52320_ReadMemory( unsigned char* buffer, unsigned int size )
{
    return CAT52320_ERROR;
}

CAT52320_Bool CAT52320_isRead( void )
{
    CAT52320_Bool ready = CAT52320_FALSE;
    unsigned char statusReg;

    CAT52320_SELECT()
    if( CAT52320_NOERROR == CAT52320_ReadStatusRegister( &statusReg ) )
    {
        ready = statusReg & CAT52320_NRDY ? CAT52320_FALSE: CAT52320_TRUE;
    }
    CAT52320_DESELECT();

    return ready;
}

CAT52320_Error CAT52320_EnableWriteProtection( void )
{
    CAT52320_Error error = CAT52320_ERROR;
    unsigned char statusReg;

    CAT52320_SELECT()
    if( CAT52320_NOERROR != CAT52320_ReadStatusRegister( &statusReg ) )
    {
        statusReg |= CAT52320_WPEN;
        if( CAT52320_NOERROR != CAT52320_ReadStatusRegister( &statusReg ) )
            error = CAT52320_NOERROR;
    }
    CAT52320_DESELECT();

    return error;
}

CAT52320_Error CAT52320_DisableWriteProtection( void )
{
    CAT52320_Error error = CAT52320_ERROR;
    unsigned char statusReg;

    CAT52320_SELECT()
    if( CAT52320_NOERROR != CAT52320_ReadStatusRegister( &statusReg ) )
    {
        statusReg &= ~CAT52320_WPEN;
        if( CAT52320_NOERROR != CAT52320_ReadStatusRegister( &statusReg ) )
            error = CAT52320_NOERROR;
    }
    CAT52320_DESELECT();

    return error;
}

CAT52320_Error CAT52320_setBlockProtection( CAT52320_BlockProtectionRange range )
{
    CAT52320_Error error = CAT52320_ERROR;
    unsigned char statusReg;

    CAT52320_SELECT()
    if( CAT52320_NOERROR != CAT52320_ReadStatusRegister( &statusReg ) )
    {
        statusReg &= ~( CAT52320_BP1 | CAT52320_BP2 );
        statusReg |= ( range << 2 );
        if( CAT52320_NOERROR != CAT52320_ReadStatusRegister( &statusReg ) )
            error = CAT52320_NOERROR;
    }
    CAT52320_DESELECT();

    return error;
}

