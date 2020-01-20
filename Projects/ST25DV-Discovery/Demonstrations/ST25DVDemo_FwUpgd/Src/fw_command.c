/**
  ******************************************************************************
  * @file    fw_command.c
  * @author  MMY Application Team
  * @version $Revision:$
  * @date    $Date:$
  * @brief   This file provides all the flash programming command functions.
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
#include "fw_command.h"


/** @addtogroup ST25_Discovery_Demo
  * @{
  */

/** @defgroup Flash_Command Flash Command
  * @brief This module implements high level functions to write firmware or data to flash.
  * @details The module covers following functions:
  * - Erase Flash command.
  * - Write buffer to flash.
  * - Jump to firmware command.
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t              JumpAddress;
pFunction             Jump_To_Application;

extern void Error_Handler( void );

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Command To erase specific Flash memory area.
  * @param  Address Start address for erasing data.
  * @retval 0 Erase sectors done with success.
  * @retval 1 Erase error.
  */
uint32_t COMMAND_EraseFlash( const uint32_t Address )
{
  uint32_t ret = 0;
  FLASH_If_FlashUnlock( );
  /* Erase FLASH sectors to download image */
  if( Address == FIRMWARE_ADDRESS )
  {
    if( FLASH_If_EraseSectors( FIRMWARE_ADDRESS, FIRMWARE_FLASH_LAST_PAGE_ADDRESS ) != 0x00 )
    {
      /* Error Erase */
      ret = 1;
    }
  }
  else
  {
    if( FLASH_If_EraseSectors( USER_DATA_ADDRESS, USER_DATA_FLASH_LAST_PAGE_ADDRESS ) != 0x00 )
    {
      /* Error Erase */
      ret = 1;
    }
  }
  FLASH_If_FlashLock( );
  
  return ret;
}

/**
  * @brief  Writes buffer to Flash memory.
  * @param  StartAddress Start address for writing data.
  * @param  offset Offset of data to write.
  * @param  pData Buffer pointer to write.
  * @param  size Size of data to write.
  * @retval 0 Erase sectors done with success.
  * @retval 1 Erase error.
  */
uint32_t Command_WriteBufferToFlash( const uint32_t StartAddress, const uint32_t offset, const uint8_t * const pData, const uint32_t size )
{
  uint32_t ret = 0;
  if( StartAddress == FIRMWARE_ADDRESS )
  {
    ret = FLASH_If_WriteBuffer( (FIRMWARE_ADDRESS + offset), FIRMWARE_FLASH_LAST_PAGE_ADDRESS, pData, size );
  }
  else
  {
    ret = FLASH_If_WriteBuffer( (USER_DATA_ADDRESS + offset), USER_DATA_FLASH_LAST_PAGE_ADDRESS, pData, size );
  }
  
  return ret;
}

/**
  * @brief  Jump to user program.
  * @param  None No parameters.
  * @return None.
  */
void COMMAND_Jump( void )
{
  
  /* Jump to user application */
    JumpAddress = *(__IO uint32_t *)(FIRMWARE_ADDRESS + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP( *(__IO uint32_t *)FIRMWARE_ADDRESS );
    Jump_To_Application( );
}

/** @} */
/** @} */

/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
