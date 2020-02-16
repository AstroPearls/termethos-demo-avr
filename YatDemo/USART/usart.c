#include "..\common.h"
#include "usart.h"
#include "..\Shell\shell.h"

void USART_Initialize(void)
{
	// Baud rate
	const WORD baudRate = 12;
	UBRR0H = (BYTE)(baudRate >> 8);
	UBRR0L = (BYTE)(baudRate);

	// Enable Rx, Tx, and receive complete interrupt
	UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
	
	// 8 data bits, no parity (default), 1 stop bit (default)
	UCSR0C =  _BV(UCSZ01) | _BV(UCSZ00);
}

void USART_WriteByte(BYTE b)
{
	/* Wait for empty transmit buffer */
	while(!(UCSR0A & _BV(UDRE0))){}
	/* Put data into buffer, sends the data */
	UDR0 = b;
}

void USART_WriteString(char* psz)
{
	while(*psz) {
		USART_WriteByte(*psz);
		psz++;
	}
}

ISR(_VECTOR(18))
{
	Shell_AddChar(UDR0);
}