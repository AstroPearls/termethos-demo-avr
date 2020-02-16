/*
 * YatDemo.c
 *
 * Created: 8/1/2014 4:14:20 AM
 *  Author: Mohamed
 */ 

#include "common.h"
#include "System\system.h"
#include "Led\led.h"
#include "Timer\timer.h"
#include "USART\usart.h"
#include "Shell\shell.h"

//--------------------------------------------------------------------------------------------------
//                                      PROGRAM  ENTRY  POINT
//--------------------------------------------------------------------------------------------------
int main(void)
{
	Shell_Initialize();
	USART_Initialize();
	USART_WriteString("\x1b[2J\n");							// Clear Screen sequence followed by one empty line
	USART_WriteString("!#Welcome to YatDemo system\n");		// Welcome/Branding header

	// Disable interrupts
	cli();

	// Initializes all the subsystems we have (LED, Timer...etc)
	Sys_Initialize();

	// Enable interrupts
	sei();

	USART_WriteString("!IYatDemo system initialized\n");

	// wait fro shell commands...forever!
	Shell_EnterLoop();
}

/************************************************************************
 * Idea here is that we run in FAST PWM mode, this means:
 *  - TOP of the timer is FF and we'll always get TIM0_OVF interrupt
 *    (comes even when OCA is disconnected)
 *  - We'll use OCRA to control the pulse width, hence intensity of light
 *    this will have no impact on TIM0_OVF interrupt rate, as it 
 *    always triggers at FF.
 * So, two birds (tick interrupt and PWM) with one stone (TIM0)
 * Notes:
 *  - TIM0 configured such that TIM0_OVF triggers every ~2ms
 *  - Due to compiler warning, I'm using _VECTOR() rather than using
 *    TIM0_OVF_vect value (and value in header seems to be wrong!!!)
 *  - I got value 16 from datasheet (datasheet starts from 1, hence
 *    it is 17 there)
 ************************************************************************/
ISR(_VECTOR(16))
{
	// LED processing
	LED_OnTick();
}
