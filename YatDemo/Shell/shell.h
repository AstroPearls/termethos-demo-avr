#ifndef SHELL_H_
#define SHELL_H_

#define BufferSize 16

typedef struct tagShell {
	char buffer[BufferSize + 2]; // enforce double null termination
	volatile BOOL commandReady;
	BYTE index;
} Shell;

void Shell_Initialize(void);
void Shell_AddChar(char c);
void Shell_EnterLoop(void);

#endif /* SHELL_H_ */