#ifndef COMMANDS_H_
#define COMMANDS_H_

typedef void (*ShellCommandFn)(char* arg1, char* arg2);

typedef struct tagShellCommand {
	char name[8];
	ShellCommandFn commandFn;
} ShellCommand, *PShellCommand;

extern PShellCommand g_ShellCommands[];

#define SHELL_COMMAND(c, n, f) ShellCommand c = { \
	.name = n, \
	.commandFn = f \
}

void Command_InvalidCommand(void);

/*
 * led on		// Turn LED on
 * led off		// Turn LED off
 * led i +		// Increase intensity
 * led i -		// Decrease intensity
 * led f +		// Increase blink rate
 * led f -		// decrease blink rate
 * led s		// Mode = Steady
 * led b		// Mode = Blink
*/
void ShellCommand_LED(char* arg1, char* arg2);

void ShellCommand_Help(char* arg1, char* arg2);

void ShellCommand_Burst(char* arg1, char* arg2);

#endif /* COMMANDS_H_ */