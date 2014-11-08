#include <circbuffer.h>


CircBuffer_Error_t
CircBuffer_Init( CircBuffer_t* circBuffer )
{
    CircBuffer_Error_t error = CIRCBUFFER_NOERROR;

    if( circBuffer )
    {
        circBuffer->producer = 0;
        circBuffer->consumer = 0;
        circBuffer->count    = 0;
        circBuffer->size     = CIRCBUFFER_SIZE;
    }
    else
        error = CIRCBUFFER_BAD_PARAM;

    return error;
}

CircBuffer_Error_t
CircBuffer_Write( CircBuffer_t *circBuffer, unsigned char data )
{
    if( !circBuffer )
        return CIRCBUFFER_BAD_PARAM;

    if( circBuffer->count >= circBuffer->size )
        return CIRCBUFFER_OVERFLOW;

    circBuffer->buffer[circBuffer->producer] = data;
    circBuffer->producer = ( circBuffer->producer + 1 ) % CIRCBUFFER_SIZE;
    circBuffer->count += 1;

    return CIRCBUFFER_NOERROR;
}

CircBuffer_Error_t
CircBuffer_Read( CircBuffer_t *circBuffer, unsigned char* data )
{
    if( !circBuffer || !data )
        return CIRCBUFFER_BAD_PARAM;

    if( circBuffer->count <= 0 )
    {
        circBuffer->count = 0; //in case count goes under 0
        return CIRCBUFFER_UNDERFLOW;
    }

    *data = circBuffer->buffer[circBuffer->consumer];
    circBuffer->consumer = ( circBuffer-> consumer + 1 ) % CIRCBUFFER_SIZE;
    circBuffer->count -= 1;

    return CIRCBUFFER_NOERROR;
}

void
CircBuffer_Flush( CircBuffer_t *circBuffer )
{
    if( circBuffer )
    {
        circBuffer->producer = 0;
        circBuffer->consumer = 0;
        circBuffer->count    = 0;
    }
}

unsigned char
CircBuffer_IsEmpty( CircBuffer_t *circBuffer )
{
    if( !circBuffer )
        return 1;

    return circBuffer->count == 0;
}
