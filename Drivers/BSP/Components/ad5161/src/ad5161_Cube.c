/**
  ******************************************************************************
  * @file    ad5161.c 
  * @author  MMY Application Team
  * @version $Revision:$
  * @date    $Date: 2016-05-27 14:58:41 +0200 (Fri, 27 May 2016) $
  * @brief   This file provides a set of functions needed to manage the AD5161
  *          Digital Potentiometer devices.
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
#include "ad5161_Cube.h"

/* Global variables ----------------------------------------------------------*/
/* Public variables ---------------------------------------------------------*/
DPOT_DrvTypeDef ad5161_dpot_drv = 
{	
  ad5161_Init,
  0,
  ad5161_Reset,
  ad5161_GetResistor,
  ad5161_SetResistor,
  ad5161_Shutdown,
  0
};

DPOT_ExtDrvTypeDef ad5112_dpot_Extdrv =
{
  0
};
/* Private variables ---------------------------------------------------------*/
/* Public functions ---------------------------------------------------------*/


/**
 * @brief   This function init the HW for AD5161 
 * @param   Addr	I2C address of the AD5161 device
 * @retval	None
 */
void ad5161_Init( uint8_t Addr )
{
  DPOT_IO_Init( Addr );
}

/**
 * @brief   This function read the resistor value from AD5161 
 * @param   Addr	I2C address of the AD5161 device
 * @retval	None
 */
uint8_t ad5161_GetResistor( uint8_t Addr )
{
  return DPOT_IO_Read( Addr );
}

/**
 * @brief   This function write the resistor value to AD5161 (It disable Reset/Shutdown modes) 
 * @param   Addr	I2C address of the AD5161 device
 * @param   Data	Resistor value to be written
 * @retval	None
 */
void ad5161_SetResistor( uint8_t Addr, uint8_t Data )
{
  uint8_t buffer[2] = { AD5161_NA, 0 };
  buffer[1] = Data;
  DPOT_IO_WriteBuffer( Addr, buffer, 2 );
}

/**
 * @brief   This function set the AD5161 in Reset mode 
 * @param   Addr	I2C address of the AD5161 device
 * @retval	None
 */
void ad5161_Reset( uint8_t Addr )
{
  uint8_t buffer[2] = { AD5161_RS, 0 };
  DPOT_IO_WriteBuffer( Addr, buffer, 2 );
}

/**
 * @brief   This function set the AD5161 in Shutdown mode 
 * @param   Addr	I2C address of the AD5161 device
 * @retval	None
 */
void ad5161_Shutdown( uint8_t Addr )
{
  uint8_t buffer[2] = { AD5161_SD, 0 };
  DPOT_IO_WriteBuffer( Addr, buffer, 2 );
}


