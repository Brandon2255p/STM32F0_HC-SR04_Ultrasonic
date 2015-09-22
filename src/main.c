/**
  ******************************************************************************
  * @file    TIM/TIM_OnePulse/main.c
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "HCSR04.h"
/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_OnePulse
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//uint16_t PrescalerValue = 0;
uint8_t flagRise=0, flagFall=0;
/* Private function prototypes -----------------------------------------------*/
static void TIM_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

  /* TIM Configuration */
  HCSR04_t ultrasonic;
  ultrasonic.ECHO_GPIOx = GPIOA;
  ultrasonic.ECHO_GPIO_Pin = GPIO_Pin_1;
  ultrasonic.TRIGGER_GPIOx = GPIOA;
  ultrasonic.TRIGGER_GPIO_Pin = GPIO_Pin_0;
  HCSR04_Init();
  HCSR04_InitEcho();
  debugInit();

  debugSend("Hellow\n");
  /* Infinite loop */



uint32_t i;
  while (1)
  {
    HCSR04_Read(&ultrasonic);
    _printfLngU("Main: ", ultrasonic.Distance);
    for(i=0; i<4000000; i++);

  }
}




