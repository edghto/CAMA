#ifndef CIRCBUFFER_H_INCLUDED
#define CIRCBUFFER_H_INCLUDED

#define CIRCBUFFER_SIZE 20

typedef enum
{
    CIRCBUFFER_NOERROR,
    CIRCBUFFER_BAD_PARAM,
    CIRCBUFFER_OVERFLOW,
    CIRCBUFFER_UNDERFLOW
} CircBuffer_Error_t;

typedef struct
{
    unsigned char buffer[CIRCBUFFER_SIZE];
    unsigned int producer;
    unsigned int consumer;
    unsigned int count;
    unsigned int size;
} CircBuffer_t;

CircBuffer_Error_t CircBuffer_Init( CircBuffer_t* circBuffer );

CircBuffer_Error_t CircBuffer_Write( CircBuffer_t *circBuffer, unsigned char data );

CircBuffer_Error_t CircBuffer_Read( CircBuffer_t *circBuffer, unsigned char* data );

void CircBuffer_Flush( CircBuffer_t *circBuffer );

unsigned char CircBuffer_IsEmpty( CircBuffer_t *circBuffer );


#endif // CIRCBUFFER_H_INCLUDED
