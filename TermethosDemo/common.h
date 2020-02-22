/////////////////////////////////////////////////////////////////////////////////
// This file contains tings that is usually common and used from different files
// only including this file everywhere will avoid including these definitions
// in a lot of different places
/////////////////////////////////////////////////////////////////////////////////

#ifndef __COMMON_DEFINITIONS_
#define __COMMON_DEFINITIONS_
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h> 
#include <avr/pgmspace.h>

#define SYSCLK	8000000UL

#define TRUE	1
#define FALSE	0
#define HIGH(x) (x>>8)
#define LOW(x)  (x&0x00FF)

#define ARRAYSIZE(x) sizeof(x)/sizeof(x[0])

#define __cli()	asm volatile ( "in		__tmp_reg__, __SREG__" :: );	\
				asm volatile ( "cli" :: );								\
				asm volatile ( "push	__tmp_reg__" :: )

#define __rei()	asm volatile ( "pop		__tmp_reg__" :: );				\
				asm volatile ( "out		__SREG__, __tmp_reg__" :: )

#define __NOP	asm volatile ( "nop" :: );

#define __WD_Reset() asm volatile ( "wdr" :: );

/*
#define	__SLEEP()	MCUCR |= _BV(SE);    \
					__asm__ __volatile__ ( "sleep" "\n\t" :: );    \
					MCUCR &= ~_BV(SE);
*/
#define	__SLEEP()	__asm__ __volatile__ ( "sleep" "\n\t" :: );

#define _(x)	((x) ^ 0xFF)

typedef uint8_t		BYTE;
typedef uint16_t	WORD;
typedef uint32_t	DWORD;
typedef uint8_t		BOOL;


#endif	//__COMMON_DEFINITIONS_
