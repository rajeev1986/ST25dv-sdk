/**
 ******************************************************************************
 * @file    st25dx_spwf_wifi.c
 * @author  MMY Application Team
 * @version $Revision$
 * @date    $Date$
 * @brief   HAL related functionality of ST25DX-discovery Wifi module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
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
#include "wifi_module.h"
#include "st25dx_spwf_wifi.h"
#include "ring_buffer.h"

/** @addtogroup BSP
* @{
*/ 

/** @addtogroup ST25DX_DISCOVERY_BSP
* @{
*/ 

/** @defgroup  ST25DX_DISCOVERY_WIFI ST25DX DISCOVERY WIFI
  * @brief Wi-Fi_driver modules
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Prescaler declaration */
uint32_t uwPrescalerValue = 0;

/* TIM handle declaration */
TIM_HandleTypeDef    TimHandle, PushTimHandle;

/* Private variables ---------------------------------------------------------*/
/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if (htim==&PushTimHandle)
  {
    /* TIMx Peripheral clock enable */
    TIMp_CLK_ENABLE();

    /*##-2- Configure the NVIC for TIMx ########################################*/
    /* Set the TIMx priority */
    HAL_NVIC_SetPriority(TIMp_IRQn, 3, 0);

    /* Enable the TIMx global Interrupt */
    HAL_NVIC_EnableIRQ(TIMp_IRQn);
  }
  else
  {
    /* TIMx Peripheral clock enable */
    TIMx_CLK_ENABLE();

    /*##-2- Configure the NVIC for TIMx ########################################*/
    /* Set the TIMx priority */
    HAL_NVIC_SetPriority(TIMx_IRQn, 3, 0);

    /* Enable the TIMx global Interrupt */
    HAL_NVIC_EnableIRQ(TIMx_IRQn);

  }
}

  /*##-1- Configure the TIM peripheral #######################################*/
  /* -----------------------------------------------------------------------
    In this example TIM3 input clock (TIM3CLK)  is set to APB1 clock (PCLK1) x2,
    since APB1 prescaler is set to 4 (0x100).
       TIM3CLK = PCLK1*2
       PCLK1   = HCLK/2
    => TIM3CLK = PCLK1*2 = (HCLK/2)*2 = HCLK = SystemCoreClock
    To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
    Prescaler = (TIM3CLK / TIM3 counter clock) - 1
    Prescaler = (SystemCoreClock /10 KHz) - 1

    Note:
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f1xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
  ----------------------------------------------------------------------- */
void Timer_Config()
{
  /* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
  uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;

  /* Set TIMx instance */
  TimHandle.Instance = TIMx;

  /* Initialize TIMx peripheral as follows:
       + Period = 10000 - 1
       + Prescaler = (SystemCoreClock/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period            = 100 - 1;
  TimHandle.Init.Prescaler         = uwPrescalerValue;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;

  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief Push_Timer_Config
  *        This function configures the Push Timer
  * @param None
  * @retval None
  */
void Push_Timer_Config()
{
  /* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
  uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;

  /* Set TIMx instance */
  PushTimHandle.Instance = TIMp;

  /* Initialize TIMx peripheral as follows:
       + Period = 10000 - 1
       + Prescaler = (SystemCoreClock/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  PushTimHandle.Init.Period            = 10 - 1;//10000
  PushTimHandle.Init.Prescaler         = uwPrescalerValue;
  PushTimHandle.Init.ClockDivision     = 0;
  PushTimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;

  if (HAL_TIM_Base_Init(&PushTimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
* @brief  USART_Configuration
* WB_WIFI_UART configured as follow:
*      - BaudRate = 115200 baud  
*      - Word Length = 8 Bits
*      - One Stop Bit
*      - No parity
*      - Hardware flow control enabled (RTS and CTS signals)
*      - Receive and transmit enabled
*
* @param  None
* @retval None
*/
void UART_Configuration(uint32_t baud_rate)
{
  BSP_USART3_Init();
}

/**
  * @brief  BSP SPWF WIFI IRQHandler
  * @retval None.
  */
void BSP_SPWF_WIFI_IRQHandler(void)
{
  HAL_UART_IRQHandler(&hbus_usart3);
}

#ifdef USART_PRINT_MSG
void USART_PRINT_MSG_Configuration(uint32_t baud_rate)
{
  COM_InitTypeDef uartcom;

  uartcom.BaudRate = baud_rate;
  uartcom.WordLength = UART_WORDLENGTH_8B;
  uartcom.StopBits = UART_STOPBITS_1;
  uartcom.Parity = UART_PARITY_NONE;
  uartcom.HwFlowCtl = UART_HWCONTROL_NONE;

  BSP_COM_Init(COM1, &uartcom);
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 


/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/

