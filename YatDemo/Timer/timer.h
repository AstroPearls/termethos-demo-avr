#ifndef __TIMER__
#define __TIMER__
#include "..\common.h"

//--------------------------------------------
// Timer APIs
//--------------------------------------------
void Timer_Initialize(void);
inline void Timer_SetOCR(BYTE ocr) { OCR0A = ocr; }
void Timer_ConnectA(void);
void Timer_DisconnectA(void);

#endif	//__TIMER__
