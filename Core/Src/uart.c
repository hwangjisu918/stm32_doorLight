/*
 * uart.c
 *
 *  Created on: Feb 4, 2025
 *      Author: ghkdw
 */

#include "uart.h"

// for circular buffer
#define 	rxBufferMax 100					// 버퍼의 크기
uint8_t 	rxBuffer[rxBufferMax];	// 버퍼
uint16_t 	rxBufferWriteIndex;			// 버퍼 쓰기 인덱스
uint16_t 	rxBufferReadIndex;			// 버퍼 읽기 인덱스
char			rxChar;									// 수신 문자 임시보관

UART_HandleTypeDef *myHuart;

int _write(int file, char* p, int len){
	HAL_UART_Transmit(myHuart, (uint8_t *)p, len, 10);
	return len;
}

// 수신 인터럽드 콜백
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	// 수신 문자를 원형버퍼에 저장후 인덱스 1증가
	rxBuffer[rxBufferWriteIndex++] = rxChar;
	// 인덱스가 최대값에 도달시 0으로 리셋
	rxBufferWriteIndex %= rxBufferMax;
	// 수신 인터럽트 준비
	HAL_UART_Receive_IT(myHuart, (uint8_t*)&rxChar, 1);
}

void initUart(UART_HandleTypeDef *inHuart){
	myHuart = inHuart;
	// 수신 인터럽트 준비
	HAL_UART_Receive_IT(myHuart, (uint8_t*)&rxChar, 1);
}

char getUart(){
	char result;
	// polling 방식의 문자 수신
	// HAL_UART_Receive(myHuart, (uint8_t *)&result, 1, 1);
	// interrupt 방식의 문자 수신
	if(rxBufferWriteIndex == rxBufferReadIndex) return 0;
	// 원형 버퍼에서 1개의 문자를 꺼내고, 수신 인덱스 1증가
	result = rxBuffer[rxBufferReadIndex++];
	// 수신 인덱스가 최대값일때 0으로 리셋
	rxBufferReadIndex %= rxBufferMax;
	return result;
}

char *getMessage(){
	static char message[100];
	static uint8_t messageCount = 0;
	char ch = getUart();
	if(ch != 0){
		if(ch == '\n'|| ch == '\r'){
			memset(message, 0, 100);
			messageCount = 0;
		}
		else if(ch >= 0x20){
			message[messageCount++] = ch;
		}
	}
	return message;
}
