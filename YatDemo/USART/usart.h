#ifndef USART_H_
#define USART_H_

void USART_Initialize(void);
void USART_WriteByte(BYTE b);
void USART_WriteString(char* psz);

#endif /* USART_H_ */