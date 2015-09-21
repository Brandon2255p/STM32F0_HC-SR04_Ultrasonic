/**
  ******************************************************************************
  * @file    HCSR04.h
  * @author  Brandon Piner
  * @version V1.0
  * @date    21-September-2015
  * @brief   This file provides firmware functions to use a HCSR04
  *          ultrasonic distance sensor
  */

#ifndef HCSR04_H_INCLUDED
#define HCSR04_H_INCLUDED

#include "stm32f0xx_gpio.h"

typedef struct {
	float Distance;              /*!< Distance measured from sensor in centimeters */
	GPIO_TypeDef* ECHO_GPIOx;    /*!< Pointer to GPIOx PORT for ECHO pin. Meant for private use only */
	uint16_t ECHO_GPIO_Pin;      /*!< GPIO Pin for ECHO pin. Meant for private use only */
	GPIO_TypeDef* TRIGGER_GPIOx; /*!< Pointer to GPIOx PORT for TRIGGER pin. Meant for private use only */
	uint16_t TRIGGER_GPIO_Pin;   /*!< GPIO Pin for ECHO pin. Meant for private use only */
} HCSR04_t;



#endif /* HCSR04_H_INCLUDED */
