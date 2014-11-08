#ifndef SPI_H
#define SPI_H

typedef enum 
{
	USI_SPI_MODE0,
	USI_SPI_MODE1
} USI_SPI_Mode_t;

typedef enum
{
	USI_SPI_SS_INT0,
	USI_SPI_SS_INT1,
	USI_SPI_SS_PCINT
} USI_SPI_SS_t;

typedef struct
{
	USI_SPI_Mode mode;
	unsigned char isSlave; /* 0 - Master, 1 - Slave */
	unsigned char isNCS;   /* if 1 then transistion of SS pin from high to low enables transmission */
	USI_SPI_SS_t ssPin;    /* pin used as Slave Select */
	unsigned char pcintN;  /* 0..7 - number of pin if USI_SPI_SS_PCINT is used as ssPin */
} USI_SPI_Config_t;

/* Initialize interrupt driven SPI */
void USI_SPI_Init( USI_SPI_Config_t& config );

unsigned char USI_SPI_TransferByte( unsigned char msg );

unsigned char USI_SPI_Transfer( unsigned char* msg, unsigned int msg_size );

#endif
