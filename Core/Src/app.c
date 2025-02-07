/*
 * app.c
 *
 *  Created on: Feb 4, 2025
 *      Author: ghkdw
 */

#include "app.h"

//장치 핸들 선언
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim11;

int timeoutDoor;

void SysticCallback()
{
	if (timeoutDoor > 0) timeoutDoor--;
}



uint16_t count_time = 99;

// us지연함수
void delay_us(uint16_t time){
	htim11.Instance->CNT = 0;
	while(htim11.Instance->CNT < time);
}

void app(){
	// uart 초기화
	initUart(&huart2);
	// 초음파 초기화
	initUltrasonic(&htim3);
	// 타이머 초기화
	HAL_TIM_Base_Start(&htim11);
	while(1){
		// 초음파 값 읽기
		uint16_t distance = getDistance();
		printf("%d\n", distance);
		if (distance <= 5) {
			count_time = 100;
			HAL_GPIO_WritePin(GPIOA, LED_Pin, 1);
		}
		else if(distance > 5)	{
			count_time--;
		}

		if (count_time == 0)
		{
			HAL_GPIO_WritePin(GPIOA, LED_Pin, 0);
		}
		HAL_Delay(95);

	}
}



