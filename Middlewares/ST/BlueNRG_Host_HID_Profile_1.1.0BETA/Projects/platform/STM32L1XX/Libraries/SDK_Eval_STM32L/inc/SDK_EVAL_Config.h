/**
 * @file    SDK_EVAL_Config.h
 * @author  AMS - AAS Division
 * @version V1.0.1
 * @date    March 10, 2014
 * @brief   This file contains SDK EVAL configuration and useful defines.
 * @details
 *
 * This file is used to include all or a part of the SDK Eval
 * libraries into the application program which will be used.
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_CONFIG_H
#define __SDK_EVAL_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "SDK_Eval_Clock.h"

/* BlueNRG Development platform: STEVAL-IDB002V1 */
#define STEVAL_IDB002V1    1
/* BlueNRG USB dongle : STEVAL-IDB003V1  */
#define STEVAL_IDB003V1    2
/* User defined platform */
#define USER_EVAL_PLATFORM 3 

/* 
 * All BlueNRG DK EWARM workspaces are configured for generating a single binary image 
 * which is valid for the BlueNRG Development Kits platforms: STEVAL-IDB002V1, STEVAL-IDB003V1 
 * (runtime platform recognition). 
 *
 * STEVAL-IDB002V1, STEVAL-IDB003V1 platforms can be also supported at compile time, by adding, 
 * respectively, only one of the following define on EWARM workspace preprocessor options:
 *
 *  a) USER_DEFINED_PLATFORM=STEVAL_IDB002V1 (it supports only STEVAL-IDB002V1 kit).
 *  b) USER_DEFINED_PLATFORM=STEVAL_IDB003V1 (it supports only STEVAL_IDB003V1 kit).
 * 
 */
/* User defined platforms targeting BlueNRG Development kits: STEVAL-IDB002V1 or STEVAL-IDB003V1*/
#if USER_DEFINED_PLATFORM == STEVAL_IDB002V1
#include "USER_Platform_Configuration_STEVAL_IDB002V1.h"
#define USER_PLATFORM_VERSION    (STEVAL_IDB002V1 - 1)
#elif USER_DEFINED_PLATFORM == STEVAL_IDB003V1
#include "USER_Platform_Configuration_STEVAL_IDB003V1.h"
#define USER_PLATFORM_VERSION    (STEVAL_IDB003V1 - 1)
/* 
 * An user platform can be simply supported, at compile time, by following these steps: 
 *  
 * 1) Create a file  "USER_Platform_Configuration.h" with specific user 
 *    platform configuration (USER_Platform_Configuration_STEVAL_IDB002V1.h or
 *    USER_Platform_Configuration_STEVAL_IDB003V1.h can be used as reference).
 * 2) Place the "USER_Platform_Configuration.h" on the 
 *    STM32L\platform\STM32L1XX\Libraries\SDK_Eval_STM32L\inc folder. 
 * 3) On the selected EWARM workspace preprocessor options, add this define: 
 *    USER_DEFINED_PLATFORM=USER_EVAL_PLATFORM
 */
#elif USER_DEFINED_PLATFORM == USER_EVAL_PLATFORM
/* User define platform header file: to be created from user based on his specific platform */
#include "USER_Platform_Configuration.h" 
#endif 

#include "SDK_EVAL_Io.h"
#include "SDK_EVAL_Spi_Driver.h"
#include "SDK_EVAL_Button.h"
#include "SDK_EVAL_Led.h"
#include "SDK_EVAL_EEPROM.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SDK_EVAL_Config         SDK EVAL Config
 * @brief SDK EVAL configuration.
 * @details See the file <i>@ref SDK_EVAL_Config.h</i> for more details.
 * @{
 */

/** @addtogroup SDK_EVAL_Config_Exported_Types          SDK EVAL Config Exported Types
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_Config_Exported_Constants        SDK EVAL Config Exported Constants
 * @{
 */

/* Number of supported BlueNRG Development Kit platforms */
#define SDK_PLATFORMS_NUMBER     (2) 
   
/* BlueNRG Development platform: STEVAL-IDB002V3 (array index starting from 0) */
#define SDK_EVAL_VERSION_3       (STEVAL_IDB002V1 - 1)  
/* BlueNRG USB Dongle: STEVAL-IDB003V1 (array index starting from 0) */ 
#define SDK_EVAL_VERSION_D1      (STEVAL_IDB003V1 - 1)    
      
/* User Defined Platform */
#ifndef USER_PLATFORM_VERSION
#define USER_PLATFORM_VERSION    (SDK_PLATFORMS_NUMBER)
#endif 
   
/**
 * @}
 */


/** @defgroup SDK_EVAL_Config_Exported_Macros           SDK EVAL Config Exported Macros
 * @{
 */

#define GPIO_HIGH(a,b)          a->BSRRL = b
#define GPIO_LOW(a,b)           a->BSRRH = b
#define GPIO_TOGGLE(a,b)        a->ODR ^= b 

/**
 * @}
 */


/** @defgroup SDK_EVAL_Config_Exported_Functions        SDK EVAL Config Exported Functions
 * @{
 */

void SdkEvalIdentification(void);
uint8_t SdkEvalGetVersion(void);
   
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

#endif

/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/
