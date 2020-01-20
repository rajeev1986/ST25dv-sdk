/**
  ******************************************************************************
  * @file    st25dx_bluenrg_ble.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   
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
#ifndef __ST25DX_BLUENRG_BLE_H
#define __ST25DX_BLUENRG_BLE_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/ 
#include "stm32f4xx_hal.h"
#include "st25dx_discovery_bluenrg.h"
#include "st25dx_discovery.h"
#define SYSCLK_FREQ 84000000
#define SYSCLK_FREQ_SLEEP 32000

/** @addtogroup BSP
 *  @{
 */

/** @addtogroup ST25DX_DISCOVERY_BSP
 *  @{
 */
 
/** @addtogroup ST25DX_BLUENRG_BLE
 *  @{
 */


// FIXME: add prototypes for BlueNRG here
void BNRG_SPI_Init(void);
void BlueNRG_RST(void);
void BSP_BlueNRG_IRQHandler(void);
uint8_t BlueNRG_DataPresent(void);
void    BlueNRG_HW_Bootloader(void);
int32_t BlueNRG_SPI_Read_All(uint8_t *buffer,
                             uint8_t buff_size);
int32_t BlueNRG_SPI_Write(uint8_t* data1,
                          uint8_t* data2,
                          uint8_t Nb_bytes1,
                          uint8_t Nb_bytes2);
#ifdef OPTIMIZED_SPI
/* Optimized functions for throughput test */
/* Used by the server (L0 and F4, not L4) */
HAL_StatusTypeDef HAL_SPI_TransmitReceive_Opt(const uint8_t *pTxData, uint8_t *pRxData, uint8_t Size);
HAL_StatusTypeDef HAL_SPI_Transmit_Opt(const uint8_t *pTxData, uint8_t Size);
HAL_StatusTypeDef HAL_SPI_Receive_Opt(uint8_t *pRxData, uint8_t Size);
#endif /* OPTIMIZED_SPI */



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

#endif /* __ST25DX_BLUENRG_BLE_H */
    
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

