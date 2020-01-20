/**
  ******************************************************************************
  * @file    stm32f4xx_hal_msp.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @ingroup ST25Discovery_MSP
  * @brief   This file provides code for the MSP Initialization 
  *          and De-initialization.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
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

/**
  * @brief Initializes the Global MSP (do nothing...).
  */
void HAL_MspInit(void)
{
  /** Empty function. */
}

/**
  * @brief  Initializes the low level hardware: GPIO, CLOCK, NVIC for CRC.
  * @param  hcrc: pointer to a CRC_HandleTypeDef structure that contains
  *         the configuration information for CRC module.
  * @retval None
  */
void HAL_CRC_MspInit( CRC_HandleTypeDef *hcrc )
{
  __HAL_RCC_CRC_CLK_ENABLE( );
}

/**
  * @brief  DeInitializes the low level hardware: GPIO, CLOCK, NVIC for CRC.
  * @param  hcrc: pointer to a CRC_HandleTypeDef structure that contains
  *         the configuration information for CRC module.
  * @retval None
  */
void HAL_CRC_MspDeInit( CRC_HandleTypeDef *hcrc )
{
  __HAL_RCC_CRC_CLK_DISABLE( );
}

/**
  * @}
  */

/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
