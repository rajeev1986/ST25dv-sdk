/**
  ******************************************************************************
  * @file    ndef_demo.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file defines the api for the NDEF demo.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
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
#ifndef __NDEF_DEMO_H
#define __NDEF_DEMO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"
#include "lib_NDEF_URI.h"
#include "lib_NDEF_SMS.h"
#include "lib_NDEF_Email.h"
#include "lib_NDEF_Vcard.h"
#include "lib_NDEF_Geo.h"
#include "lib_NDEF_MyApp.h"
#include "lib_NDEF_AAR.h"
#include "lib_NDEF_Handover.h"
#include "lib_NDEF_Bluetooth.h"
#include "lib_NDEF_Wifi.h"

/** @addtogroup NDEF_DEMO
  * @ingroup ST25_Discovery_Demo
  * @{
  */


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported function	--------------------------------------------------------*/
void NDEF_DEMO_Init_Tag(void);
void NDEF_DEMO_Write_URI_URL(void);
void NDEF_DEMO_Write_URI_Tel(void);
void NDEF_DEMO_Read_URI(void);
void NDEF_DEMO_Read_SMS(void);
void NDEF_DEMO_Write_SMS(void);
void NDEF_DEMO_Read_Email(void);
void NDEF_DEMO_Write_Email(void);
void NDEF_DEMO_Read_Vcard(void);
void NDEF_DEMO_Write_Vcard(void);
void NDEF_DEMO_Write_Picture_Vcard(void);
void NDEF_DEMO_Read_Geo(void);
void NDEF_DEMO_Write_Geo(void);
void NDEF_DEMO_Read_MyAPP(void);
void NDEF_DEMO_Write_AAR(void);
void NDEF_DEMO_MultiRecord_With_AAR(void);
void NDEF_DEMO_BLE_ChangeDeviceAddr(void);
void NDEF_DEMO_Write_BLE_OOB(void);
void NDEF_DEMO_Read_Bluetooth_OOB(void);
void NDEF_DEMO_Write_Wifi_OOB(void);
void NDEF_DEMO_Read_Wifi_OOB(void);
void NDEF_DEMO_Write_empty_NDEF( void );
void NDEF_DEMO_Erase_CCFile( void );
void NDEF_DEMO_Clear_Eeprom( void );

/**
  * @}
  */




#endif /* __NDEF_DEMO_H */

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
