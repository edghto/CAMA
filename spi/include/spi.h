#ifndef SPI_H
#define SPI_H

/* Pin configuration for Attiny 2313 */
#define USI_SPI_MISO _BV(PB5)
#define USI_SPI_MOSI _BV(PB6)
#define USI_SPI_USCK _BV(PB7)

/*
 * Enables delay between sending next byte.
 */
//#define USI_SPI_ENABLE_BYTE_DELAY

/*
 * Delay in miliseconds between transfer of next byte.
 * Only for Master mode, but make notice, that for correct working,
 * Slave mode also requires some delay, but it have to be managed
 * by master.
 * Requires USI_SPI_ENABLE_BYTE_DELAY to be defined.
 */
#define USI_SPI_BYTE_DELAY 200

/*
 * Uncomment define USI_SPI_SLAVE_MODE to enable SPI slave mode.
 * By default driver works as Master.
 */
//#define USI_SPI_SLAVE_MODE

/*
 * This has effect only in Slave mode.
 * This is interrutpt driven implementation, so interrupt pin is
 * needed to notify uC about transmission.
 * To do that a Slave Select Pin needs to be configured.
 * Uncomment ONE of following defines.
 */
#define USI_SPI_SS_INT0
//#define USI_SPI_SS_INT1
//#define USI_SPI_SS_PCINT

/*
 * When using USI_SPI_SS_PCINT as SS pin you need to define
 * Only values from 0 to 7 are valid, but may noticed, that
 * PICNT pins overlaps with SPI pins (DO, DI, USCK), so in
 * fact only 0-4 pins can be used.
 */
//#define USI_SPI_SS_PCINT_N 0

/*
 * When using Slave mode.
 * You can change behavior of SS pin to work as Not Select Cable.
 * So HIGH state of SS pin will indicate that uC is NOT selected,
 * whereas LOW state will indicate uC is selected.
 * To enable this feature uncomment following define.
 */
//#define USI_SPI_USE_NCS

/*
 * Uncommoment following define to enablr SPI Mode 1, a.k.a (0,1).
 * It works for both Master and Slave configuration.
 */
//#define USI_SPI_MODE1


#if defined(USI_SPI_SLAVE_MODE)
	#if !defined(USI_SPI_SS_INT0) && !defined(USI_SPI_SS_INT1) && !defined(USI_SPI_SS_PCINT)
		#error Slave Select Pin wasnt configures: USI_SPI_SS_INT0, USI_SPI_SS_INT1, USI_SPI_SS_PCINT
	#endif
#endif

#if defined(USI_SPI_SS_PCINT)
	#if !defined(USI_SPI_SS_PCINT_N)
		#error If USI_SPI_SS_PCINT is used than USI_SPI_SS_PCINT_N needs to be defined: 0..7
	#endif
#endif

#ifdef USI_SPI_ENABLE_BYTE_DELAY
    #ifndef USI_SPI_BYTE_DELAY
        #error USI_SPI_BYTE_DELAY needs to be defined
    #endif
#endif

/*
 * This is a callback declaration which has to be implemented
 * by user, when utilizing Slave mode.
 * This callback will be invoked after every 8bit transmission.
 * The parameter is 8bit data received thru SPI. It should
 * return 8bit that should be transmitted back to Master.
 */
#ifdef USI_SPI_SLAVE_MODE
unsigned char USI_SPI_callback( unsigned char data );
#endif

/* Initialize interrupt driven SPI */
void USI_SPI_Init( void );

unsigned char USI_SPI_TransferByte( unsigned char msg );

unsigned char USI_SPI_Transfer( unsigned char* msg, unsigned int msg_size );

#endif

