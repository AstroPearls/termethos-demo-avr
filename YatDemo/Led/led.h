#ifndef __LED__
#define __LED__
#include "..\common.h"

// Automatic Turn OFF duration
#define  ATO_TIME_COUNT 60000	// 2 minutes
//#define  ATO_TIME_COUNT 146484	// 5 minutes

#define BLINK_INCREMENT 2
#define MIN_BLINK_RATE	250	// 1 ON/OFF cycle a second
#define MAX_BLINK_RATE	4	// 60 ON/OFF cycles a second

// states of the led
typedef enum {
	// LED is OFF
	OFF,
	// Led is ON
	ON,
	// pulse mode (this is normal mode, its called pulse as it will auto off after sometime to save battery)
	PULSE,
	// Blink
	Blink
} LedState;

typedef struct tagLEDSTATE
{
	WORD		_dwATOTimeCount;
	BYTE		_bIntensityLevel;
	BYTE		_state;					// 0: OFF, 1:Pulse
	BOOL		_fONPhase;				// in the ON part of the blinking cycle
	BYTE		_bBlinkRate;			// blink frequency
	BYTE		_bBlinkTimeCount;
} LEDINFO;

extern LEDINFO g_LED;

void LED_Initialize(void);
void LED_ON(void);		// ON continuously
void LED_OFF(void);		// OFF continuously
void LED_Toggle(void);	// ON if was OFF otherwise OFF
void LED_Pulse(void);	// ON for sometime then auto off to save power
void LED_Blink(void);	// Blinking light
inline BOOL LED_GetState(void) {return g_LED._state;}
void LED_Dim(void);
void LED_UnDim(void);
void LED_BlinkRateInc(void);
void LED_BlinkRateDec(void);
void LED_OnTick(void);

#endif //__LED__
