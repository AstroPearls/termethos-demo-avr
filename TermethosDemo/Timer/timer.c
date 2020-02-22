#include "..\common.h"
#include "timer.h"

/************************************************************************
 * Timer initialized as follows:
 *    Given: System Clock = 8MHz
 *    Start with:
 *       - OCA disconnected (COM0A1 = COM0A0 = 0)
 *       - Mode = Fast PWM, Top=OCRA, (WGM00:02=7)
 *       - Enable overflow interrupt (TOIE0 = 1)
 *       - Timer Prescaler = 64 (CS00:02 = 3)
 *    Result:
 *       Overflow freq = 8MHz/64/256 = 488.281Hz
 *       Overflow event every ~2ms
 ************************************************************************/
void Timer_Initialize(void) {
	TCCR0A = /*_BV(COM0A1) | _BV(COM0A0) | _BV(WGM02) | */_BV(WGM01) | _BV(WGM00);
	TCCR0B = /*_BV(CS02) |*/ _BV(CS01) | _BV(CS00);
	TIMSK0 = _BV(TOIE0);
}

void Timer_ConnectA(void) {
	TCCR0A |= (_BV(COM0A1) /*| _BV(COM0A0)*/);
}

void Timer_DisconnectA(void) {
	// TODO: use macros instead of this
	TCCR0A &= 0x3F;
}