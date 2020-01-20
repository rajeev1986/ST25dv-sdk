/**
  ******************************************************************************
  * @file    flash_if.c
  * @author  MMY Application Team
  * @version $Revision:$
  * @date    $Date:$
  * @brief   This file provides all the flash layer functions.
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
#include "flash_if.h"


/** @addtogroup ST25_Discovery_Demo
  * @{
  */
  
/** @defgroup Flash_Interface Flash memory api
  * @brief   This module defines an API to access the internal flash memory.
  * @details The module covers following functions:
  * - Lock Flash modification.
  * - Unlock Flash modification.
  * - Read Flash protection status.
  * - Erase Flash sectors.
  * - Write Flash data.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t FirstSector = 0;
uint32_t NbOfSectors = 0;
uint32_t SectorError = 0;
uint32_t OB_RDP_LEVEL;
static FLASH_OBProgramInitTypeDef FLASH_OBProgramInitStruct;
static FLASH_EraseInitTypeDef FLASH_EraseInitStruct;

/* Private function prototypes -----------------------------------------------*/
static uint32_t FLASH_If_GetSectorNumber( const uint32_t Address );

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Locks the Flash to disable the flash control register access.
  * @param  None No parameters
  * @return None.
  */
void FLASH_If_FlashLock( void )
{
  HAL_FLASH_Lock( );
}

/**
  * @brief  Unlocks the Flash to enable the flash control register access.
  * @param  None No parameters.
  * @return None.
  */
void FLASH_If_FlashUnlock( void )
{
  HAL_FLASH_Unlock( );
}

/**
  * @brief  Gets Flash readout protection status.
  * @param  None No parameters.
  * @retval ReadOut protection status.
  */
FlagStatus FLASH_If_ReadOutProtectionStatus( void )
{
  FlagStatus readoutstatus = RESET;
  
  FLASH_OBProgramInitStruct.RDPLevel = OB_RDP_LEVEL;
  
  HAL_FLASHEx_OBGetConfig( &FLASH_OBProgramInitStruct );
  
  if( OB_RDP_LEVEL == SET )
  {
    readoutstatus = SET;
  }
  else
  {
    readoutstatus = RESET;
  }
  
  return readoutstatus;
}

/**
  * @brief  Erases the required FLASH Sectors computed with destination address.
  * @param  Address Start address for erasing data.
  * @param  LastAddress End address of flash area.
  * @retval 0 Erase sectors done with success.
  * @retval 1 Erase error.
  */
uint32_t FLASH_If_EraseSectors( const uint32_t Address, const uint32_t LastAddress )
{
  /* Erase the user Flash area
    (area defined by Address and LastAddress) ****/
  
  if( Address <= LastAddress )
  {
    /* Get the 1st sector to erase */
    FirstSector = FLASH_If_GetSectorNumber( Address );
    /* Get the number of sector to erase from 1st sector */
    NbOfSectors = FLASH_If_GetSectorNumber( LastAddress ) - FirstSector + 1;
    
    FLASH_EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    FLASH_EraseInitStruct.Sector = FirstSector;
    FLASH_EraseInitStruct.NbSectors = NbOfSectors;
    FLASH_EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    
    if( HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &SectorError) != HAL_OK )
      return (1);
  }
  else
  {
    return (1);
  }
  
  return (0);
}

/**
  * @brief  Writes a data in flash (data are 32-bit aligned).
  * @param  Address Start address for writing data buffer.
  * @param  LastAddress End address of flash area.
  * @param  Data Word data value to write.
  * @retval 1 Data successfully written to Flash memory.
  * @retval 0 Error occurred while writing data in Flash memory.
  */
uint32_t FLASH_If_Write( const uint32_t Address, const uint32_t LastAddress, const uint32_t Data )
{
  uint32_t ret = 0;
  /* Program the user Flash area word by word
    (area defined by Address and LastAddress) ***********/
  
  if( Address <= LastAddress )
  {
    if( HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, Data)!= HAL_OK )
      ret = 1;  
  }
  else
  {
    ret = 1;
  }
  
  return ret;
}

/**
  * @brief  Writes a data in flash (data are 8-bit aligned).
  * @param  Address Start address for writing data buffer.
  * @param  LastAddress End address of flash area.
  * @param  Data Byte data value to write.
  * @retval 0 Data successfully written to Flash memory.
  * @retval 1 Error occurred while writing data in Flash memory.
  */
uint32_t FLASH_If_WriteByte( const uint32_t Address, const uint32_t LastAddress, const uint8_t Data )
{
  uint32_t ret = 0;
  /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and APPLICATION_ADDRESS) ***********/
  
  if( Address <= LastAddress )
  {
    if( HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, Address, Data)!= HAL_OK )
      ret = 1;  
  }
  else
  {
    ret = 1;
  }
  
  return ret;
}

/**
  * @brief  Writes a data buffer in flash (manage data alignedment).
  * @param  Address Start address for writing data buffer.
  * @param  LastAddress End address of flash area.
  * @param  pData Pointer on data buffer.
  * @param  Size Size of the data.
  * @retval 0 Data successfully written to Flash memory.
  * @retval 1 Error occurred while writing data in Flash memory.
  */
uint32_t FLASH_If_WriteBuffer( const uint32_t Address, const uint32_t LastAddress, const uint8_t * const pData , const uint32_t Size )
{
  uint32_t cnt = 0;
  uint32_t ret = 0;
 
  if( Address <= LastAddress )
  {
    FLASH_If_FlashUnlock( );
    /* To write word in flash address must be 32-bit aligned */
    while( (((Address + cnt) % 4) != 0) && (ret == 0) )
    {
      ret |= FLASH_If_WriteByte( (Address + cnt), LastAddress, pData[cnt] );
      cnt++;
    }
    /* As the buffer contains more than 4 bytes, the flash is written word by word */
    while( ((Size - cnt) >= 4) && (ret == 0) )
    {
      ret |= FLASH_If_Write( (Address + cnt), LastAddress, *(uint32_t *)(pData + cnt) );
      cnt += 4;
    }
    /* End of buffer is less than 4 bytes write last bytes to flash */
    while( ((Size - cnt) != 0) && (ret == 0) )
    {
      ret |= FLASH_If_WriteByte( (Address + cnt), LastAddress, pData[cnt] );
      cnt++;
    }
    FLASH_If_FlashLock( ); 
  }
  else
  {
    ret = 1;
  }
  
  return ret;
}

/**
  * @brief  Returns the Flash sector Number of the address.
  * @param  None No parameters.
  * @return The Flash sector Number of the address.
  */
static uint32_t FLASH_If_GetSectorNumber( const uint32_t Address )
{
  uint32_t sector = 0;
  
  if( (Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0) )
  {
    sector = FLASH_SECTOR_0;  
  }
  else if( (Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1) )
  {
    sector = FLASH_SECTOR_1;  
  }
  else if( (Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2) )
  {
    sector = FLASH_SECTOR_2;  
  }
  else if( (Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3) )
  {
    sector = FLASH_SECTOR_3;  
  }
  else if( (Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4) )
  {
    sector = FLASH_SECTOR_4;  
  }
  else if( (Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5) )
  {
    sector = FLASH_SECTOR_5;  
  }
  else if( (Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6) )
  {
    sector = FLASH_SECTOR_6;  
  }
  else if( (Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7) )
  {
    sector = FLASH_SECTOR_7;  
  }
  else if( (Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8) )
  {
    sector = FLASH_SECTOR_8;  
  }
  else if( (Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9) )
  {
    sector = FLASH_SECTOR_9;  
  }
  else if( (Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10) )
  {
    sector = FLASH_SECTOR_10;  
  }
  else /*( (Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11) ) */
  {
    sector = FLASH_SECTOR_11;  
  }
  return sector;
}

/**
  * @} 
  */

/**
  * @} 
  */

/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
