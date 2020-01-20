  /**
  ******************************************************************************
  * @file    st25dx_spwf_wifi.h
  * @author  MMY Appli Team
  * @version &Revision$
  * @date    $Date$
  * @brief   Header file for HAL related functionality of WIFI module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ST25DX_SPWF_WIFI_H
#define __ST25DX_SPWF_WIFI_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "st25dx_discovery.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_rcc_ex.h"

/* Exported macro ------------------------------------------------------------*/
#define DEBUG_PRINT                              0
#define WiFi_ENABLE
#define FW_UPDATE_PACKET_SIZE                    252
#define MAX_PREFETCH_BUFFER                      20

/********** TimeOUT *******************/
#define WIFI_TIME_OUT                            40000   //4 sec
#define WIFI_HTTP_REQUEST_INTERVAL               60 //6sec

/********** Wi-Fi *******************/
#ifdef USART_PRINT_MSG
  #define WIFI_UART_MSG                           USART2
  #define USARTx_PRINT_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
  #define USARTx_PRINT_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
  #define USARTx_PRINT_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

  #define USARTx_PRINT_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
  #define USARTx_PRINT_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

  #define PRINTMSG_USARTx_TX_AF                  GPIO_AF7_USART2
  #define PRINTMSG_USARTx_RX_AF                  GPIO_AF7_USART2

  #define WiFi_USART_PRINT_TX_PIN                GPIO_PIN_2
  #define WiFi_USART_PRINT_TX_GPIO_PORT          GPIOA
  #define WiFi_USART_PRINT_RX_PIN                GPIO_PIN_3
  #define WiFi_USART_PRINT_RX_GPIO_PORT          GPIOA

  /* Definition for USARTx's NVIC */
  #define USARTx_PRINT_IRQn                      USART2_IRQn
  #define USARTx_PRINT_IRQHandler                USART2_IRQHandler

#endif //USART_PRINT_MSG


/* User can use this section to tailor USARTx/UARTx instance used and associated resources */
/* Definition for USARTx clock resources */
#define WB_WIFI_UART                             USART3
#define USARTx_CLK_ENABLE()                      __HAL_RCC_USART3_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOD_CLK_ENABLE() // not really for RX but for RTS
#define USARTx_TX_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_FORCE_RESET()                     __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()                   __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define WiFi_USART_TX_PIN                        GPIO_PIN_8
#define WiFi_USART_TX_GPIO_PORT                  GPIOD
#define WiFi_USART_RX_PIN                        GPIO_PIN_9
#define WiFi_USART_RX_GPIO_PORT                  GPIOD

#define WiFi_USART_RTS_PIN                       GPIO_PIN_14
#define WiFi_USART_RTS_GPIO_PORT                 GPIOB
#define WiFi_USART_CTS_PIN                       GPIO_PIN_11
#define WiFi_USART_CTS_GPIO_PORT                 GPIOD

#define WiFi_USARTx_TX_AF                        GPIO_AF7_USART3
#define WiFi_USARTx_RX_AF                        GPIO_AF7_USART3

#define  WiFi_RESET_GPIO_PIN                     GPIO_PIN_10
#define  WiFi_RESET_GPIO_PORT                    GPIOD

/* Not used on MB1283 */
#define  WiFi_WAKEUP_GPIO_PIN                    GPIO_PIN_8
#define  WiFi_WAKEUP_GPIO_PORT                   GPIOC /*PC8*/
#define RESET_WAKEUP_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOD_CLK_ENABLE()

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                              USART3_IRQn
#define USARTx_IRQHandler                        USART3_IRQHandler

#define USARTx_EXTI_IRQn                         EXTI15_10_IRQn
#define USARTx_EXTI_IRQHandler                   EXTI15_10_IRQHandler

#define TIMx                                     TIM3
#define TIMx_CLK_ENABLE()                        __HAL_RCC_TIM3_CLK_ENABLE()
#define TIMp                                     TIM2
#define TIMp_CLK_ENABLE()                        __HAL_RCC_TIM2_CLK_ENABLE()

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                                TIM3_IRQn
#define TIMx_IRQHandler                          TIM3_IRQHandler
#define TIMp_IRQn                                TIM2_IRQn
#define TIMp_IRQHandler                          TIM2_IRQHandler

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                             (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                             TXBUFFERSIZE

#define  WiFi_USART_BAUD_RATE                    115200

/* Exported functions ------------------------------------------------------- */
void Push_Timer_Config(void);
void BSP_SPWF_WIFI_IRQHandler(void);

#endif /*__ST25DX_SPWF_WIFI_H */
