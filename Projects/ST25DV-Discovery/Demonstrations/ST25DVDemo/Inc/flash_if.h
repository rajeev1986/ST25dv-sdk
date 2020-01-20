/**
  ******************************************************************************
  * @file    flash_if.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   Header file for flash_if.c
  ******************************************************************************
 * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
   
/** @addtogroup Flash_Interface
  * @{
  */
   
   
/* Exported types ------------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Exported constants --------------------------------------------------------*/
/* Define the flash memory start address */
#define USER_FLASH_STARTADDRESS    ((uint32_t)0x08000000) 

/* Define the address from where firmware will be stored. */
#define FIRMWARE_ADDRESS        0x080A0000      /* Upgraded firmware base address */

/* Last Page Address for firmware */
#define FIRMWARE_FLASH_LAST_PAGE_ADDRESS  0x080DFFFF - 4      /* Upgraded firmware last address */

/* Define the user application size */
#define FIRMWARE_FLASH_SIZE   (FIRMWARE_FLASH_LAST_PAGE_ADDRESS - FIRMWARE_ADDRESS + 1)     /* Upgraded firmware area size */

/* Define the address from where user data will be stored. */
#define USER_DATA_ADDRESS        0x080E0000      /* User data base address */

/* Last Page Address for data */
#define USER_DATA_FLASH_LAST_PAGE_ADDRESS  0x080FFFFF - 4   /* User data last address */

/* Define the user data size */
#define USER_DATA_FLASH_SIZE   (USER_DATA_FLASH_LAST_PAGE_ADDRESS - USER_DATA_ADDRESS + 1)    /* User data area size */

/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void FLASH_If_FlashLock( void );
void FLASH_If_FlashUnlock( void );
FlagStatus FLASH_If_ReadOutProtectionStatus( void );
uint32_t FLASH_If_EraseSectors( const uint32_t Address, const uint32_t LastAddress );
uint32_t FLASH_If_WriteByte( const uint32_t Address, const uint32_t LastAddress, const uint8_t Data );
uint32_t FLASH_If_Write( const uint32_t Address, const uint32_t LastAddress, const uint32_t Data );
uint32_t FLASH_If_WriteBuffer( const uint32_t Address, const uint32_t LastAddress, const uint8_t * const pData , const uint32_t Size );

/** @} */
#ifdef __cplusplus
}
#endif

#endif  /* __FLASH_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
