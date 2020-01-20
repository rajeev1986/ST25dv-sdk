/**
  ******************************************************************************
  * @file    scope.h 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   Header for scope.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty  
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SCOPE_H
#define __SCOPE_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define SCOPE_CHAN1_TIMERCHAN         TIM_CHANNEL_3
#define SCOPE_PWM1_PIN                GPIO_PIN_10
#define SCOPE_PWM1_PIN_PORT           GPIOB
#define SCOPE_PWM1_CLOCK_ENABLE()     __HAL_RCC_TIM2_CLK_ENABLE()
#define SCOPE_PWM1_CLOCK_DISABLE()    __HAL_RCC_TIM2_CLK_DISABLE()
#define SCOPE_PWM1_FORCE_RESET()      __HAL_RCC_TIM2_FORCE_RESET()
#define SCOPE_PWM1_RELEASE_RESET()    __HAL_RCC_TIM2_RELEASE_RESET()
#define SCOPE_PWM1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define SCOPE_PWM1_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()

#define SCOPE_CHAN2_TIMERCHAN         TIM_CHANNEL_4
#define SCOPE_PWM2_PIN                GPIO_PIN_11
#define SCOPE_PWM2_PIN_PORT           GPIOB
#define SCOPE_PWM2_CLOCK_ENABLE()     __HAL_RCC_TIM2_CLK_ENABLE()
#define SCOPE_PWM2_CLOCK_DISABLE()    __HAL_RCC_TIM2_CLK_DISABLE()
#define SCOPE_PWM2_FORCE_RESET()      __HAL_RCC_TIM2_FORCE_RESET()
#define SCOPE_PWM2_RELEASE_RESET()    __HAL_RCC_TIM2_RELEASE_RESET()
#define SCOPE_PWM2_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define SCOPE_PWM2_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ST25DV_Scope_Start( void );
void PWM1_Callback( void );
void PWM2_Callback( void );

#endif /* __SCOPE_H */

/************************ (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
