/**
 * @file    SDK_EVAL_Led.h
 * @author  AMS - AAS Division
 * @version V1.0.0
 * @date    Febrary 3, 2014
 * @brief   This file contains definitions for Software Development Kit eval board Leds.
 * @details
 *
 * In this module there are API for the management of the leds on the SDK Eval
 * motherboard.
 *
 * <b>Example:</b>
 * @code
 *
 *   SdkEvalLedInit(LED1);
 *
 *   ...
 *
 *   SdkEvalLedToggle(LED1);
 *
 *   ...
 *
 * @endcode
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
#ifndef __SDK_EVAL_LED_H
#define __SDK_EVAL_LED_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

#ifdef __cplusplus
 extern "C" {
#endif


/** @addtogroup SDK_EVAL_STM32L
 * @{
 */

/** @addtogroup SDK_EVAL_Led            SDK EVAL Led
 * @brief Management of Software Development Kit eval board Leds.
 * @details See the file <i>@ref SDK_EVAL_Led.h</i> for more details.
 * @{
 */

/** @defgroup SDK_EVAL_Led_Exported_Types               SDK EVAL Led Exported Types
 * @{
 */

/**
 * @brief  Enumeration of SDK EVAL LEDs
 */
typedef enum
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3,
  LED5 = 4

} SdkEvalLed;

/**
 * @}
 */


/** @defgroup SDK_EVAL_Led_Exported_Constants                           SDK EVAL Led Exported Constants
 * @{
 */

#define LEDn                             5

/**
 *  BlueNRG Eval Board (STEVAL-IDB002V1) DL1 led
 */
#define SDK_EVAL_V3_LED1_PIN                         GPIO_Pin_0
#define SDK_EVAL_V3_LED1_GPIO_PORT                   GPIOC
#define SDK_EVAL_V3_LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOC

/**
 *  BlueNRG Eval Board (STEVAL-IDB002V1) DL2 led
 */ 
#define SDK_EVAL_V3_LED2_PIN                         GPIO_Pin_1
#define SDK_EVAL_V3_LED2_GPIO_PORT                   GPIOC
#define SDK_EVAL_V3_LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOC
/**
 *  BlueNRG Eval Board (STEVAL-IDB002V1) DL3 led
 */ 
#define SDK_EVAL_V3_LED3_PIN                         GPIO_Pin_2
#define SDK_EVAL_V3_LED3_GPIO_PORT                   GPIOD
#define SDK_EVAL_V3_LED3_GPIO_CLK                    RCC_AHBPeriph_GPIOD

/**
 *  BlueNRG Eval Board (STEVAL-IDB002V1) DL4 led
 */ 
#define SDK_EVAL_V3_LED4_PIN                         GPIO_Pin_4
#define SDK_EVAL_V3_LED4_GPIO_PORT                   GPIOC
#define SDK_EVAL_V3_LED4_GPIO_CLK                    RCC_AHBPeriph_GPIOC

/**
 *  BlueNRG Eval Board (STEVAL-IDB002V1) DL5 led
 */ 
#define SDK_EVAL_V3_LED5_PIN                         GPIO_Pin_5
#define SDK_EVAL_V3_LED5_GPIO_PORT                   GPIOC
#define SDK_EVAL_V3_LED5_GPIO_CLK                    RCC_AHBPeriph_GPIOC


/**
 * @brief USB Dongle D2 led
 */
#define SDK_DONGLE_V1_LED1_PIN                       GPIO_Pin_0
#define SDK_DONGLE_V1_LED1_GPIO_PORT                 GPIOB
#define SDK_DONGLE_V1_LED1_GPIO_CLK                  RCC_AHBPeriph_GPIOB
#define SDK_DONGLE_LED2                              LED1     

/**
 * @brief USB Dongle D3 led
 */
#define SDK_DONGLE_V1_LED2_PIN                       GPIO_Pin_1
#define SDK_DONGLE_V1_LED2_GPIO_PORT                 GPIOB
#define SDK_DONGLE_V1_LED2_GPIO_CLK                  RCC_AHBPeriph_GPIOB
#define SDK_DONGLE_LED3                              LED5


/**
 * @}
 */

/**
 * @defgroup SDK_EVAL_Led_Exported_Macros                       SDK EVAL Led Exported Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_Led_Exported_Functions                   SDK EVAL Led Exported Functions
 * @{
 */

void SdkEvalLedInit(SdkEvalLed xLed);
void SdkEvalLedOn(SdkEvalLed xLed);
void SdkEvalLedOff(SdkEvalLed xLed);
void SdkEvalLedToggle(SdkEvalLed xLed);
FlagStatus SdkEvalLedGetState(SdkEvalLed xLed);

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
