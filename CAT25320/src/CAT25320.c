#include <CAT25320.h>
#include <spi.h>

static CAT52320_Error CAT52320_MemoryRW( CAT52320_Operations op, uint16_t address, uint8_t* buffer, uint16_t size );

#define CAT52320_SELECT()   { CAT25320_PORT &= ~CAT25320_NCS; }
#define CAT52320_DESELECT() { CAT25320_PORT |=  CAT25320_NCS; }

void CAT52320_Init( void )
{
	USI_SPI_Init();
	CAT25320_DDR |= CAT25320_NCS | CAT25320_NHOLD | CAT25320_NWP;
	CAT52320_DESELECT();

/*
	//Left as reference for futre use, i.e. software implememtnation
    CAT25320_DDR |= CAT25320_SCK | CAT25320_SI; // SI == MOSI
    CAT25320_DDR |= CAT25320_NHOLD | CAT25320_NCS | CAT25320_NWP;
    CAT25320_PORT |= CAT25320_NHOLD | CAT25320_NCS | CAT25320_NWP;
    CAT25320_PORT &= ~CAT25320_SCK;
*/
}

CAT52320_Error CAT52320_WriteStatusRegister( uint8_t StatusRegister )
{
    CAT52320_Error error = CAT52320_NOERROR;
    uint8_t msg[2] = { CAT52320_OP_WRSR, StatusRegister };

    CAT52320_SELECT()
    if( USI_SPI_Transfer( msg, 2 ) )
    {
        error = CAT52320_ERROR;
    }
    CAT52320_DESELECT();

    return error;
}

CAT52320_Error CAT52320_ReadStatusRegister( uint8_t* StatusRegister )
{
    CAT52320_Error error = CAT52320_NOERROR;
    uint8_t msg[2] = { CAT52320_OP_RDSR, *StatusRegister };

    CAT52320_SELECT()
    if( USI_SPI_Transfer( msg, 2 ) )
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
    uint8_t msg[1] = { CAT52320_OP_WREN };

    CAT52320_SELECT()
    if( USI_SPI_Transfer( msg, 1 ) )
    {
        error = CAT52320_ERROR;
    }
    CAT52320_DESELECT();

    return error;
}

CAT52320_Error CAT52320_DisableWriteOpertation( void )
{
    CAT52320_Error error = CAT52320_NOERROR;
    uint8_t msg[1] = { CAT52320_OP_WRDI };

    CAT52320_SELECT()
    if( USI_SPI_Transfer( msg, 1 ) )
    {
        error = CAT52320_ERROR;
    }
    CAT52320_DESELECT();

    return error;
}

static CAT52320_Error CAT52320_MemoryRW( CAT52320_Operations op, uint16_t address, uint8_t* buffer, uint16_t size )
{
	uint8_t msg[1] = { op };
	uint8_t splitedAddr[2] = { 
		CAT52320_ADDRESS_HIGHBITS(address),
		CAT52320_ADDRESS_LOWBITS(address)
	};

	CAT52320_SELECT()
	if( USI_SPI_Transfer( msg, 1 ) )
	{
		return CAT52320_ERROR;
	}
	if( USI_SPI_Transfer( splitedAddr, 2 ) )
	{
		return CAT52320_ERROR;
	}
	if( USI_SPI_Transfer( buffer, size ) )
	{
		return CAT52320_ERROR;
	}
    CAT52320_DESELECT();

    return CAT52320_NOERROR;
}

CAT52320_Error CAT52320_WriteMemoryPage( uint16_t address, uint8_t* buffer, uint16_t size )
{
	CAT52320_Error error = 
		CAT52320_MemoryRW( CAT52320_OP_WRITE, address, buffer, size );

    return error;
}

CAT52320_Error CAT52320_ReadMemory( uint16_t address, uint8_t* buffer, uint16_t size )
{
	CAT52320_Error error = 
		CAT52320_MemoryRW( CAT52320_OP_READ, address, buffer, size );

    return error;
}

CAT52320_Bool CAT52320_isReady( void )
{
    CAT52320_Bool ready = CAT52320_FALSE;
    uint8_t statusReg;

    CAT52320_SELECT()
    if( CAT52320_NOERROR == CAT52320_ReadStatusRegister( &statusReg ) )
    {
        ready = statusReg & CAT52320_NRDY ? CAT52320_FALSE : CAT52320_TRUE;
    }
    CAT52320_DESELECT();

    return ready;
}

CAT52320_Error CAT52320_EnableWriteProtection( void )
{
    CAT52320_Error error = CAT52320_ERROR;
    uint8_t statusReg;

    CAT52320_SELECT()
    if( CAT52320_NOERROR != CAT52320_ReadStatusRegister( &statusReg ) )
    {
        statusReg |= CAT52320_WPEN;
        error = CAT52320_WriteStatusRegister( statusReg );
    }
    CAT52320_DESELECT();

    return error;
}

CAT52320_Error CAT52320_DisableWriteProtection( void )
{
    CAT52320_Error error = CAT52320_ERROR;
    uint8_t statusReg;

    CAT52320_SELECT()
    if( CAT52320_NOERROR == CAT52320_ReadStatusRegister( &statusReg ) )
    {
        statusReg &= ~CAT52320_WPEN;
        error = CAT52320_WriteStatusRegister( statusReg );
    }
    CAT52320_DESELECT();

    return error;
}

CAT52320_Error CAT52320_setBlockProtection( CAT52320_BlockProtectionRange range )
{
    CAT52320_Error error = CAT52320_ERROR;
    uint8_t statusReg;

    CAT52320_SELECT()
    if( CAT52320_NOERROR == CAT52320_ReadStatusRegister( &statusReg ) )
    {
        statusReg &= ~( CAT52320_BP1 | CAT52320_BP2 );
        statusReg |= ( range << 2 );
        error = CAT52320_WriteStatusRegister( statusReg );
    }
    CAT52320_DESELECT();

    return error;
}

