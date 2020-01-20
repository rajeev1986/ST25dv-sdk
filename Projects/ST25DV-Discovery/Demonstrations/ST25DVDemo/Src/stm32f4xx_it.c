/**
  ******************************************************************************
  * @file    stm32f4xx_it.c 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   Main Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "wifi_module.h"
#include "st25dx_spwf_wifi.h"
#include "st25dx_bluenrg_ble.h"
#include "hci.h"
#include "fw_command.h"
#include "st25dv_features_demo.h"
#include "scope.h"
#include "st25dx_discovery.h"

/** @defgroup ST25Discovery_IT ST25 Discovery interrupt routines
  * @ingroup ST25_Discovery_Demo
  * @brief This module defines all the required interrupt routines for the ST25DV demo.
  * @details Interrupts routines used in the ST25DVdemo:
  *           - System tick (ms timer)
  *           - SPI3 for the BlueNRG bluetooth module
  *           - USART3 for the Wifi module
  *           - GPO interrrupt from ST25DV
  *
  * @note     The hardware exceptions handler are defined in the same file but are documented in a sub-module to enforce readability.
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @brief  Current millisecond count */
volatile uint32_t ms_counter = 0;
/** @brief Polling variable for the ST25DV GPO interrupt, updated from GPO interrupt callback. */ 
volatile uint8_t GPO_Activated = 0;
/** @brief  Timer handle from st25_spwf_wifi.c */
extern TIM_HandleTypeDef    TimHandle;
/** @brief  Timer handle from st25_spwf_wifi.c */
extern TIM_HandleTypeDef    PushTimHandle;
/** @brief  Timer handle from scope.c */
extern TIM_HandleTypeDef    Tim2Handle;
/** @brief  Global detect flag for ST25DV-W demo from main.c */
extern uint8_t ST25DV_pwm;
/** @brief  Global detect flag for UserButtonStatus from scope.c */
extern uint8_t UserButtonStatus;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup CortexM4_Exceptions_Handlers Cortex-M4 exceptions handlers
  * @brief    This module defines handlers for the Cortex-M4 hardware exceptions.
  * @details  Hardware exceptions handlers are called when a specific situation occurs on the HW,
  *           such as unknown instruction, bus error,...
  * @{
  */

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/** @} */

/**
  * @brief  This function handles System Tick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  Wifi_SysTick_Isr();
  ms_counter++;
  HAL_SYSTICK_IRQHandler();
  ST25DV_DEMO_Led2_Pwm();
/* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}


/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIMx_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
  
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIMp_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&PushTimHandle);
}

/**
* @brief  Period elapsed callback in non blocking mode
*         This timer is used for calling back User registered functions with information
* @param  htim : TIM handle
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{ 
  Wifi_TIM_Handler(htim);
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/


/**
  * @brief  This function handles External line interrupt request for BlueNRG bluetooth module.
  */
void EXTI2_IRQHandler(void)
{
  BSP_BlueNRG_IRQHandler();
}

/**
  * @brief  This function handles USART3 interrupts for the Wifi module.
  */
void USART3_IRQHandler(void)
{
  BSP_SPWF_WIFI_IRQHandler();
}

/**
  * @brief  This function handles EXTI line 10 15 interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  if( (ST25DV_pwm != 1) && (HAL_GPIO_ReadPin(BUTTON_USER_GPIO_PORT, BUTTON_USER_PIN) == GPIO_PIN_SET) )
  {
    BSP_GPO_IRQHandler();
  }
  
  if( ST25DV_pwm == 1 )
  {
    if( (HAL_GPIO_ReadPin(BUTTON_USER_GPIO_PORT, BUTTON_USER_PIN) == GPIO_PIN_RESET) )
    {
      HAL_GPIO_EXTI_IRQHandler(BUTTON_USER_PIN);
    }
    else
    {
      HAL_GPIO_EXTI_IRQHandler(SCOPE_PWM1_PIN);
      HAL_GPIO_EXTI_IRQHandler(SCOPE_PWM2_PIN);
    }
  }
}

/**
  * @brief  This function handles callback from external line interrupt request
  * @param  GPIO_Pin
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if( GPIO_Pin == BNRG_SPI_EXTI_PIN )
  {		     
    HCI_Isr( );
  }
  
    /* User button */
  if( GPIO_Pin == BUTTON_USER_PIN )
  {  
    UserButtonStatus = 1;
  }
  
  /* Rising edge on PWM1 */
  if( GPIO_Pin == SCOPE_PWM1_PIN )
  { 
    PWM1_Callback( );
  }
  
  /* Rising edge on PWM2 */
  if( GPIO_Pin == SCOPE_PWM2_PIN )
  {
    PWM2_Callback( );
  }
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
