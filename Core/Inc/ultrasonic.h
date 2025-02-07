/*
 * ultrasonic.h
 *
 *  Created on: Feb 6, 2025
 *      Author: ghkdw
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "main.h"

void initUltrasonic(TIM_HandleTypeDef *htim);
uint16_t getDistance();

#endif /* INC_ULTRASONIC_H_ */
