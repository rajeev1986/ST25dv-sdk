/**
  ******************************************************************************
  * @file    st25dx_discovery_dpot.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file contains all the functions prototypes for the
  *          st25dx_discovery_dpot.c driver.
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
#ifndef __ST25DX_DISCOVERY_DPOT_H
#define __ST25DX_DISCOVERY_DPOT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "st25dx_discovery.h"
#include "ad5161_Cube.h"
#include "st25dx_discovery_conf.h"
#include "st25dx_discovery_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup ST25DX_DISCOVERY_BSP
  * @{
  */

/** @addtogroup ST25DX_DISCOVERY_DPOT
  * @{
  */

#define BSP_DPOT_I2C_ADDR       AD5161_ADDR0

typedef enum
{
  DPOT_OK       = 0x00,
  DPOT_ERROR    = 0x01,
}DPOT_StatusTypeDef;

uint8_t BSP_DPOT_Init(void);
uint32_t BSP_DPOT_GetResistor( void );
void BSP_DPOT_SetResistor( uint32_t Val );
void BSP_DPOT_Reset( void );
void BSP_DPOT_Shutdown( void );
DPOT_ExtDrvTypeDef *BSP_DPOT_GetExtended_Drv( void );

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

#endif /* __ST25DX_DISCOVERY_DPOT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
