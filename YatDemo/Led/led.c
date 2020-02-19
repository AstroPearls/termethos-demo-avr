#include <stdio.h>
#include "led.h"
#include "..\Timer\timer.h"
#include "..\USART\usart.h"

LEDINFO g_LED;
// these are the intensity level in log scale - our eye work in logarithmic fashion!
const BYTE g_LogScale[19] PROGMEM = {0, 2, 3, 4, 5, 7, 9, 12, 16, 21, 28, 37, 48, 64, 84, 111, 147, 193, 255};

void LED_UpdateState(LedState state);
void LED_ReportIntensityLevel();
	
void LED_Initialize(void) {
	g_LED._bIntensityLevel = 0x09;
	g_LED._bBlinkTimeCount = g_LED._bBlinkRate = 64;
	LED_UpdateState(OFF);
	Timer_SetOCR(pgm_read_byte(&g_LogScale[g_LED._bIntensityLevel]));
}

void LED_Pulse(void) {
	g_LED._dwATOTimeCount = ATO_TIME_COUNT;
	LED_UpdateState(PULSE);
	Timer_ConnectA();
}

void LED_OFF(void) {
	Timer_DisconnectA();
	LED_UpdateState(OFF);
}

void LED_ON(void) {
	LED_UpdateState(ON);
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
	LED_UpdateState(Blink);
	Timer_ConnectA();
	g_LED._fONPhase = TRUE;
}

BOOL LED_Dim(void) {
	BOOL minReached = TRUE;
	if(g_LED._bIntensityLevel > 0) {
		g_LED._bIntensityLevel--;
		LED_ReportIntensityLevel();
		minReached = FALSE;
	}

	// there are cases above where intensity level didn't change and hence we don't need to 
	Timer_SetOCR(pgm_read_byte(&g_LogScale[g_LED._bIntensityLevel]));
	
	return minReached;
}

BOOL LED_UnDim(void) {
	BOOL maxReached = TRUE;
	if(g_LED._bIntensityLevel < 18) {
		g_LED._bIntensityLevel++;
		LED_ReportIntensityLevel();
		maxReached = FALSE;
	}

	// there are cases above where intensity level didn't change and hence we don't need to
	Timer_SetOCR(pgm_read_byte(&g_LogScale[g_LED._bIntensityLevel]));
	
	return maxReached;
}

BOOL LED_BlinkRateInc(void) {
	WORD orig = g_LED._bBlinkRate;
	g_LED._bBlinkRate -= BLINK_INCREMENT;
	if(g_LED._bBlinkRate < MAX_BLINK_RATE || g_LED._bBlinkRate > orig) {// overflow
		g_LED._bBlinkRate = MAX_BLINK_RATE;
		return TRUE;
	}
	
	return FALSE;
}

BOOL LED_BlinkRateDec(void) {
	WORD orig = g_LED._bBlinkRate;
	g_LED._bBlinkRate += BLINK_INCREMENT;
	if(g_LED._bBlinkRate > MIN_BLINK_RATE || g_LED._bBlinkRate < orig) {// underflow
		g_LED._bBlinkRate = MIN_BLINK_RATE;
		return TRUE;
	}
	
	return FALSE;
}

void LED_ReportIntensityLevel() {
	int level = g_LED._bIntensityLevel;
	char temp[16];
	
	sprintf(temp, "@{1:%d}", level);
	USART_WriteString(temp);
}

void LED_UpdateState(LedState state) {
		g_LED._state = state;
		switch(state) {
			case OFF:
				USART_WriteString("@{0:OFF}");
				break;
			case ON:
				USART_WriteString("@{0:ON}");
				break;
			case Blink:
				USART_WriteString("@{0:Blink}");
				break;
			case PULSE:
				USART_WriteString("@{0:Pulse}");
				break;
		}
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