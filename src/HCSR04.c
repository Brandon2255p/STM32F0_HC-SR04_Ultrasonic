/**
  ******************************************************************************
  * @file    HCSR04.c
  * @author  Brandon Piner
  * @version V1.0
  * @date    21-September-2015
  * @brief   This file provides firmware functions to use a HCSR04
  *          ultrasonic distance sensor
  */


#include "HCSR04.h"

void HCSR04_Init(HCSR04_t * HCSR04)
{
  uint16_t prescalerValue = 0;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* TIM2_CH1 pin (PA.00) and TIM2_CH2 pin (PA.01) configuration */
  GPIO_InitStructure.GPIO_Pin = HCSR04->ECHO_GPIO_Pin |HCSR04->TRIGGER_GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect TIM pins to AF2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_2);
  //GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_2);

  /* --------------------------------------------------------------------------
    TIM2 configuration: One Pulse mode
    The external signal is connected to TIM2_CH2 pin (PA.01),
    The Rising edge is used as active edge,
    The One Pulse signal is output on TIM2_CH1 pin (PA.05)
    The TIM_Pulse defines the delay value
    The (TIM_Period -  TIM_Pulse) defines the One Pulse value.

    TIM2 input clock (TIM2CLK) is set to APB1 clock (PCLK1)
      TIM2CLK = PCLK1
      PCLK1 = HCLK
      => TIM2CLK = HCLK  = SystemCoreClock

    TIM2CLK = SystemCoreClock, we want to get TIM2 counter clock at 24 MHz:
     Prescaler = (TIM2CLK / TIM2 counter clock) - 1
     Prescaler = (SystemCoreClock  /24 MHz) - 1

    The Autoreload value is 65535 (TIM2->ARR), so the maximum frequency value
    to trigger the TIM2 input is 24000000/65535 = 366.2 Hz.

    The TIM_Pulse defines the delay value, this value is fixed to:
       delay =  CCR1/TIM2 counter clock = 682.6 us.

    The (TIM_Period - TIM_Pulse) defines the One Pulse value, this value is fixed to:
       One Pulse value = (TIM_Period - TIM_Pulse) / TIM2 counter clock = 2.04 ms.

    Note:
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.

  --------------------------------------------------------------------------- */

  /* Compute the prescaler value */
  prescalerValue = (uint16_t) ((SystemCoreClock ) / 10000000) - 1;//10MHz

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 20120;
  TIM_TimeBaseStructure.TIM_Prescaler = prescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM2 PWM2 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 20000;//2ms delay
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  /* TIM4 configuration in Input Capture Mode

  TIM_ICStructInit(&TIM_ICInitStructure);

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0;

  TIM_ICInit(TIM2, &TIM_ICInitStructure);
*/
  /* One Pulse Mode selection */
  TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Single);
}

void HCSR04_InitEcho()
{
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);

    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI0 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/** \brief Sends 10us pulse on trigger pin
 *
 * \param
 * \param
 * \return
 *
 */
void HCSR04_Trigger()
{
    TIM2->CR1|=TIM_CR1_CEN;
}
