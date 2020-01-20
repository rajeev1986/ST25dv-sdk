/**
  ******************************************************************************
  * @file    st25dx_discovery_bluenrg.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file contains definitions for SPI communication
  *          BLE BlueNRG Moduke Board.
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
#ifndef ST25DX_DISCOVERY_BLUENRG_H
#define ST25DX_DISCOVERY_BLUENRG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "st25dx_discovery.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup ST25DX_DISCOVERY
  * @{
  */

/** @defgroup ST25DX_BLUENRG_BLE
  * @{
  */ 
  
/**
* @brief SPI communication details between Nucleo F4 and BlueNRG
*        Expansion Board.
*/
// SPI Reset Pin: PD.1
#define BNRG_SPI_RESET_PIN          GPIO_PIN_1
#define BNRG_SPI_RESET_MODE         GPIO_MODE_OUTPUT_PP
#define BNRG_SPI_RESET_PULL         GPIO_PULLUP
#define BNRG_SPI_RESET_SPEED        GPIO_SPEED_FREQ_LOW
#define BNRG_SPI_RESET_ALTERNATE    0
#define BNRG_SPI_RESET_PORT         GPIOD
#define BNRG_SPI_RESET_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

// IRQ: PD.2
#define BNRG_SPI_IRQ_PIN            GPIO_PIN_2
#define BNRG_SPI_IRQ_MODE           GPIO_MODE_IT_RISING
#define BNRG_SPI_IRQ_PULL           GPIO_NOPULL
#define BNRG_SPI_IRQ_SPEED          GPIO_SPEED_FREQ_VERY_HIGH
#define BNRG_SPI_IRQ_ALTERNATE      0
#define BNRG_SPI_IRQ_PORT           GPIOD
#define BNRG_SPI_IRQ_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()

// EXTI External Interrupt for SPI
// NOTE: if you change the IRQ pin remember to implement a corresponding handler
// function like EXTI0_IRQHandler() in the user project
#define BNRG_SPI_EXTI_IRQn          EXTI2_IRQn
#define BNRG_SPI_EXTI_IRQHandler    EXTI2_IRQHandler
#define BNRG_SPI_EXTI_PIN           BNRG_SPI_IRQ_PIN
#define BNRG_SPI_EXTI_PORT          BNRG_SPI_IRQ_PORT
#define RTC_WAKEUP_IRQHandler       RTC_WKUP_IRQHandler

#define BNRG_SPI_CS_LOW()           HAL_GPIO_WritePin(BUS_SPI3_NSS_GPIO_PORT, BUS_SPI3_NSS_GPIO_PIN, GPIO_PIN_RESET)
#define BNRG_SPI_CS_HIGH()          HAL_GPIO_WritePin(BUS_SPI3_NSS_GPIO_PORT, BUS_SPI3_NSS_GPIO_PIN, GPIO_PIN_SET)

void Enable_SPI_IRQ(void);
void Disable_SPI_IRQ(void);
void Clear_SPI_IRQ(void);
void Clear_SPI_EXTI_Flag(void);

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* ST25DX_DISCOVERY_BLUENRG_H */

    
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

