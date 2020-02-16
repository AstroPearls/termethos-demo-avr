#include "commands.h"
#include <string.h>
#include "..\Led\led.h"
#include "..\USART\usart.h"

SHELL_COMMAND(led, "led", ShellCommand_LED);
SHELL_COMMAND(help, "help", ShellCommand_Help);

PShellCommand g_ShellCommands[] = { &led, &help };
	
void Command_InvalidCommand(void) {
	USART_WriteString("!EInvalid command!\n\n");
}

void ShellCommand_LED(char* arg1, char* arg2) {
	if(!arg1 || !*arg1) {
		Command_InvalidCommand();
		return;
	}

	if(strcmp(arg1, "on") == 0 || arg1[0] == 's') {
		LED_ON();
		return;
	}
	
	if(strcmp(arg1, "off") == 0) {
		LED_OFF();
		return;
	}
	
	if(arg1[0] == 'b') {
		LED_Blink();
		return;
	}
	
	if(arg1[0] == 'i') {
		if(arg2[0] == '+') {
			LED_UnDim();
			return;
		}
		if(arg2[0] == '-') {
			LED_Dim();
			return;
		}
		
		Command_InvalidCommand();
		return;
	}
	
	if(arg1[0] == 'f') {
		if(arg2[0] == '+') {
			LED_BlinkRateInc();
			return;
		}
		if(arg2[0] == '-') {
			LED_BlinkRateDec();
			return;
		}
		
		Command_InvalidCommand();
		return;
	}
	
	Command_InvalidCommand();
}

void ShellCommand_Help(char* arg1, char* arg2) {
	USART_WriteString("\nhelp        show help\n");
	USART_WriteString("led on      Turn LED on\n");
	USART_WriteString("led off     Turn LED off\n");
	USART_WriteString("led i +     Increase intensity\n");
	USART_WriteString("led i -     Decrease intensity\n");
	USART_WriteString("led f +     Increase blink rate\n");
	USART_WriteString("led f -     decrease blink rate\n");
	USART_WriteString("led s       Mode = Steady\n");
	USART_WriteString("led b       Mode = Blink\n\n");
}