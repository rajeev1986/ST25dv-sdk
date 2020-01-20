/**
  ******************************************************************************
  * @file    bluenrg_interface.h 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file provides code for the BlueNRG Expansion Board driver
  *          based on STM32Cube HAL for STM32 Nucleo boards.
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
#include "bluenrg_interface.h"
#include "debug.h"
#include "ble_status.h"
#include "hci.h"

uint32_t Osal_Get_Cur_Time(void)
{
	return HAL_GetTick();
}

/************************ (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
