#include "..\common.h"
#include "commands.h"
#include <string.h>
#include "shell.h"
#include "..\USART\usart.h"

static Shell g_shell;

void Shell_Reset(void);
void Shell_ProcessCommand(void);
BOOL Shell_FindToken(char** pBuffer);
void Shell_HandleCommand(char* command, char* arg1, char* arg2);

void Shell_Initialize(void) {
	Shell_Reset();
}

void Shell_AddChar(char c) {
	// if we have pending command, ignore coming data!
	if (g_shell.commandReady) {
		return;
	}

	// if buffer is full, ignore its contents and reset it!
	if (g_shell.index == BufferSize) {
		Shell_Reset();
		return;
	}
	
	// if we see \n we have a command ready so terminate the string, otherwise just append
	if (c == '\n') {
		g_shell.buffer[g_shell.index++] = 0;
		g_shell.commandReady = TRUE;
	} else {
		g_shell.buffer[g_shell.index++] = c;
	}
}

void Shell_EnterLoop(void) {
	// never return!
	while(TRUE) {
		// wait for command
		while(!g_shell.commandReady);
		
		// Process command
		Shell_ProcessCommand();
	}
}

void Shell_Reset(void) {
	memset(g_shell.buffer, 0, ARRAYSIZE(g_shell.buffer));
	g_shell.index = 0;
	g_shell.commandReady = FALSE;
}

// format is like: <command> [arg1] [arg2]
void Shell_ProcessCommand(void) {
	char *pBuffer = g_shell.buffer;

	// Find and terminate command token. Reset and ignore data if no command token
	char *command = g_shell.buffer;
	if(!Shell_FindToken(&pBuffer)) {
		Shell_Reset();
		return;
	}
	
	// Find and terminate arg1 token. Reset and ignore data if no arg1
	char *arg1 = pBuffer;
	Shell_FindToken(&pBuffer);
	
	// Find and terminate arg2 token. Arg2 is optional, so ignore if not found
	char *arg2 = pBuffer;
	Shell_FindToken(&pBuffer);

	// we have a command, handle it
	Shell_HandleCommand(command, arg1, arg2);
}

// Following 3 possibilities can happen
//  1. We found space, hence token is not last thing in the string.
//  2. We found zero, hence token is last thing in the string.
//  3. we found nothing
// we return true for 1,2 and false for 3.
// NOTE: this is not secure and assume string is double null terminated
BOOL Shell_FindToken(char** ppBuffer) {
	char* pBuffer =  *ppBuffer;
	// return false if empty string
	if(!*pBuffer) {
		return FALSE;
	}

	// find space or zero terminating token
	while(*pBuffer != ' ' && *pBuffer != '\0') {
		pBuffer++;
	}
	
	// terminate token in case it was space and prep for next token
	*pBuffer++ = 0;
	*ppBuffer = pBuffer;
	return TRUE;
}

void Shell_HandleCommand(char* command, char* arg1, char* arg2) {
	// TODO: replace 2 with compile time sizeof
	for(BYTE i = 0; i < 2; i++) {
		if(strcmp(g_ShellCommands[i]->name, command) ==0) {
			g_ShellCommands[i]->commandFn(arg1, arg2);
			Shell_Reset();
			return;
		}
	}
	
	Shell_Reset();
	Command_InvalidCommand();
}
