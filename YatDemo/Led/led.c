#include "led.h"
#include "..\Timer\timer.h"

LEDINFO g_LED;
// these are the intensity level in log scale - our eye work in logarithmic fashion!
const BYTE g_LogScale[19] PROGMEM = {0, 2, 3, 4, 5, 7, 9, 12, 16, 21, 28, 37, 48, 64, 84, 111, 147, 193, 255};
	
void LED_Initialize(void) {
	g_LED._bIntensityLevel = 0x09;
	g_LED._bBlinkTimeCount = g_LED._bBlinkRate = 64;
	g_LED._state = OFF;
	Timer_SetOCR(pgm_read_byte(&g_LogScale[g_LED._bIntensityLevel]));
}

void LED_Pulse(void) {
	g_LED._dwATOTimeCount = ATO_TIME_COUNT;
	g_LED._state = PULSE;
	Timer_ConnectA();
}

void LED_OFF(void) {
	Timer_DisconnectA();
	g_LED._state = OFF;
}

void LED_ON(void) {
	g_LED._state = ON;
	Timer_ConnectA();
}

void LED_Toggle(void) {
	if (g_LED._state == ON) {
		LED_OFF();
	} else {
		LED_ON();
	}
}

void LED_Blink(void) {
	g_LED._state = Blink;
	Timer_ConnectA();
	g_LED._fONPhase = TRUE;
}

void LED_OnTick(void) {
	// Auto turn off logic, if in pulse mode
	if(LED_GetState() == PULSE) {
		g_LED._dwATOTimeCount--;
		if(g_LED._dwATOTimeCount == 0) {
			LED_OFF();
			g_LED._dwATOTimeCount = ATO_TIME_COUNT;
		}
	}
	
	// blinking logic
	if(LED_GetState() == Blink) {
		g_LED._bBlinkTimeCount--;
		if(g_LED._bBlinkTimeCount == 0) {
			g_LED._bBlinkTimeCount = g_LED._bBlinkRate;
			if(g_LED._fONPhase) {
				Timer_DisconnectA();
				g_LED._fONPhase = FALSE;
			} else {
				Timer_ConnectA();
				g_LED._fONPhase = TRUE;
			}
		}
	}
}

void LED_Dim(void) {
	if(g_LED._bIntensityLevel < 18) {
		g_LED._bIntensityLevel++;
	}

	// there are cases above where intensity level didn't change and hence we don't need to 
	Timer_SetOCR(pgm_read_byte(&g_LogScale[g_LED._bIntensityLevel]));
}

void LED_UnDim(void) {
	if(g_LED._bIntensityLevel > 0) {
		g_LED._bIntensityLevel--;
	}

	// there are cases above where intensity level didn't change and hence we don't need to
	Timer_SetOCR(pgm_read_byte(&g_LogScale[g_LED._bIntensityLevel]));
}

void LED_BlinkRateInc(void) {
	g_LED._bBlinkRate -= BLINK_INCREMENT;
	if(g_LED._bBlinkRate < MAX_BLINK_RATE) {// overflow
		g_LED._bBlinkRate = MAX_BLINK_RATE;
	}
}

void LED_BlinkRateDec(void) {
	g_LED._bBlinkRate += BLINK_INCREMENT;
	if(g_LED._bBlinkRate > MIN_BLINK_RATE) {// underflow
		g_LED._bBlinkRate = MIN_BLINK_RATE;
	}
}
