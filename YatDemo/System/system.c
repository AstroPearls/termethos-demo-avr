#include "system.h"
#include "..\Led\led.h"
#include "..\Timer\timer.h"

extern LEDINFO g_LED;
extern BYTE g_LogScale[];

void Sys_Initialize(void)
{
	// initialize Sys internal members
	Sys_SetOpMode(normal);

	// initialize I/O:
	//   Output initially set to turn led OFF (DP0 = 0)
	//   PD0 is direction to output (DDD0 = 1)
	//   I use the pin as source as it has more current than sink (see datasheet)
	PORTD = 0; //_BV(PIND6);
	DDRD = _BV(DDD6);
	
	// PB0 for debug LED
	PORTB = 0;
	DDRB = _BV(PINB0);
	
	// initialize LED
	LED_Initialize();

	// initialize timer - must be done first,we start it later
	Timer_Initialize();
}
