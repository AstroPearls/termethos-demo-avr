#ifndef __SYSTEM__
#define __SYSTEM__
#include "..\common.h"

// to save SRAM, I use OCR0B register to store the mode of operation as i don't use it otherwise.
// this not only saved SRAM, but FLASH too!!!
typedef enum { normal=0x00, blink=0x01} OpMode;

void Sys_Initialize(void);
inline OpMode Sys_GetOpMode(void) {return OCR0B;}
inline void Sys_SetOpMode(OpMode mode) {OCR0B = mode;}

#endif //__SYSTEM__
