/**
 * @file    SDK_EVAL_Led.c
 * @author  AMS - AAS Division
 * @version V1.0.0
 * @date    Febrary 3, 2014
 * @brief   This file provides all the low level API to manage SDK LEDs.
 * @details
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


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Config.h"

/** @addtogroup SDK_EVAL_STM32L
 * @{
 */


/** @addtogroup SDK_EVAL_Led
 * @{
 */

/** @defgroup SDK_EVAL_Led_Private_TypesDefinitions             SDK EVAL Led Private Types Definitions
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Led_Private_Defines                      SDK EVAL Led Private Defines
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Led_Private_Macros                       SDK EVAL Led Private Macros
 * @{
 */

/**
 * @}
 */

#ifndef USER_DEFINED_PLATFORM 

/** @defgroup SDK_EVAL_Led_Private_Variables                    SDK EVAL Led Private Variables
 * @{
 */
GPIO_TypeDef* vectpxGpioPortVersion[SDK_PLATFORMS_NUMBER][LEDn] = {
  {SDK_EVAL_V3_LED1_GPIO_PORT, SDK_EVAL_V3_LED2_GPIO_PORT, SDK_EVAL_V3_LED3_GPIO_PORT, SDK_EVAL_V3_LED4_GPIO_PORT, SDK_EVAL_V3_LED5_GPIO_PORT},
  {SDK_DONGLE_V1_LED1_GPIO_PORT, SDK_DONGLE_V1_LED2_GPIO_PORT, SDK_DONGLE_V1_LED1_GPIO_PORT, SDK_DONGLE_V1_LED1_GPIO_PORT, SDK_DONGLE_V1_LED2_GPIO_PORT}
 };
 
static const uint16_t s_vectnGpioPinVersion[SDK_PLATFORMS_NUMBER][LEDn] = {
  {SDK_EVAL_V3_LED1_PIN, SDK_EVAL_V3_LED2_PIN, SDK_EVAL_V3_LED3_PIN, SDK_EVAL_V3_LED4_PIN, SDK_EVAL_V3_LED5_PIN},
  {SDK_DONGLE_V1_LED1_PIN, SDK_DONGLE_V1_LED2_PIN, SDK_DONGLE_V1_LED1_PIN, SDK_DONGLE_V1_LED1_PIN, SDK_DONGLE_V1_LED2_PIN}
};

static const uint32_t s_vectlGpioClkVersion[SDK_PLATFORMS_NUMBER][LEDn] = {
  {SDK_EVAL_V3_LED1_GPIO_CLK, SDK_EVAL_V3_LED2_GPIO_CLK, SDK_EVAL_V3_LED3_GPIO_CLK, SDK_EVAL_V3_LED4_GPIO_CLK, SDK_EVAL_V3_LED5_GPIO_CLK},
  {SDK_DONGLE_V1_LED1_GPIO_CLK, SDK_DONGLE_V1_LED2_GPIO_CLK, SDK_DONGLE_V1_LED1_GPIO_CLK, SDK_DONGLE_V1_LED1_GPIO_CLK, SDK_DONGLE_V1_LED2_GPIO_CLK}
};


static GPIO_TypeDef** vectpxGpioPort;
static uint16_t* s_vectnGpioPin;
static uint32_t* s_vectlGpioClk;

#endif /* USER_DEFINED_PLATFORM */

/**
 * @}
 */


/**
 * @defgroup SDK_EVAL_Led_Private_FunctionPrototypes                    SDK EVAL Led Private Function Prototypes
 * @{
 */

/**
 * @}
 */


/**
 * @defgroup SDK_EVAL_Led_Private_Functions                             SDK EVAL Led Private Functions
 * @{
 */

/* LEDs GPIO Port */
static GPIO_TypeDef* Get_LedGpioPort(SdkEvalLed xLed)
{
#ifdef USER_DEFINED_PLATFORM
  GPIO_TypeDef* led_port;
  switch (xLed)
  {
    case(LED1):
      led_port = USER_LED1_GPIO_PORT;
      break;
    case(LED2):
      led_port = USER_LED2_GPIO_PORT;
      break;
    default:
      led_port = USER_LED1_GPIO_PORT;
      break;
  }
  return (led_port);
#else
  return (vectpxGpioPort[xLed]);
#endif
}

/* LEDs GPIO Pin */
static uint32_t Get_LedGpioPin(SdkEvalLed xLed)
{
#ifdef USER_DEFINED_PLATFORM
  uint32_t led_pin;
  switch (xLed)
  {
    case(LED1):
      led_pin = USER_LED1_GPIO_PIN;
      break;
    case(LED2):
      led_pin = USER_LED2_GPIO_PIN;
      break;
    default:
      led_pin = USER_LED1_GPIO_PIN;
      break;
  }
  return (led_pin);
#else
  return(s_vectnGpioPin[xLed]);
#endif
}

/* LEDs GPIO CLK */
static uint32_t Get_LedGpioClk(SdkEvalLed xLed)
{
#ifdef USER_DEFINED_PLATFORM
  uint32_t led_clk;
  switch (xLed)
  {
    case(LED1):
      led_clk = USER_LED1_GPIO_CLK;
      break;
    case(LED2):
      led_clk = USER_LED1_GPIO_CLK;
      break;
    default:
      led_clk = USER_LED1_GPIO_CLK;
      break;
  }
  return (led_clk);
#else
  return(s_vectlGpioClk[xLed]);
#endif
}


/**
 * @brief  Configures LED GPIO.
 * @param  xLed Specifies the Led to be configured.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 *         @arg LED3
 *         @arg LED4
 *         @arg LED5
 * @retval None.
 */
void SdkEvalLedInit(SdkEvalLed xLed)
{
#ifndef USER_DEFINED_PLATFORM
  vectpxGpioPort = vectpxGpioPortVersion[SdkEvalGetVersion()];
  s_vectnGpioPin = (uint16_t *)s_vectnGpioPinVersion[SdkEvalGetVersion()];
  s_vectlGpioClk = (uint32_t *)s_vectlGpioClkVersion[SdkEvalGetVersion()];
#endif
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_AHBPeriphClockCmd(Get_LedGpioClk(xLed), ENABLE);
  
  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = Get_LedGpioPin(xLed);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(Get_LedGpioPort(xLed), &GPIO_InitStructure);

  /* To be modified based on USER_PLATFORM */
  if (SdkEvalGetVersion() == SDK_EVAL_VERSION_3) 
    (Get_LedGpioPort(xLed))-> BSRRL = Get_LedGpioPin(xLed);
  else if(SdkEvalGetVersion() == SDK_EVAL_VERSION_D1)
    (Get_LedGpioPort(xLed))-> BSRRH = Get_LedGpioPin(xLed);
  else 
    (Get_LedGpioPort(xLed))-> BSRRL = Get_LedGpioPin(xLed);
}

/**
 * @brief  Turns selected LED On.
 * @param  xLed Specifies the Led to be set on.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 *         @arg LED3
 *         @arg LED4
 *         @arg LED5
 * @retval None.
 */
void SdkEvalLedOn(SdkEvalLed xLed)
{
  /* To be modified based on USER_PLATFORM */
  if (SdkEvalGetVersion() == SDK_EVAL_VERSION_3) 
    Get_LedGpioPort(xLed)-> BSRRH = Get_LedGpioPin(xLed);
  else if(SdkEvalGetVersion() == SDK_EVAL_VERSION_D1)
    Get_LedGpioPort(xLed)-> BSRRL = Get_LedGpioPin(xLed);
  else
    Get_LedGpioPort(xLed)-> BSRRH = Get_LedGpioPin(xLed);
}

/**
 * @brief  Turns selected LED Off.
 * @param  xLed Specifies the Led to be set off.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 *         @arg LED3
 *         @arg LED4
 *         @arg LED5
 * @retval None.
 */
void SdkEvalLedOff(SdkEvalLed xLed)
{
  /* To be modified based on USER_PLATFORM */
  if (SdkEvalGetVersion() == SDK_EVAL_VERSION_3) 
    Get_LedGpioPort(xLed)-> BSRRL = Get_LedGpioPin(xLed);
  else if(SdkEvalGetVersion() == SDK_EVAL_VERSION_D1)
    Get_LedGpioPort(xLed)-> BSRRH = Get_LedGpioPin(xLed);
  else
    Get_LedGpioPort(xLed)-> BSRRL = Get_LedGpioPin(xLed);
}

/**
 * @brief  Toggles the selected LED.
 * @param  xLed Specifies the Led to be toggled.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 *         @arg LED3
 *         @arg LED4
 *         @arg LED5
 * @retval None.
 */
void SdkEvalLedToggle(SdkEvalLed xLed)
{
  Get_LedGpioPort(xLed)->ODR ^= Get_LedGpioPin(xLed);
}

/**
 * @brief  Returns the status of a specified led.
 * @param  xLed Specifies the Led to be read.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 *         @arg LED3
 *         @arg LED4
 *         @arg LED5
 * @retval FlagStatus return the status of the LED. This parameter can be:
 *         SET or RESET.
 */
FlagStatus SdkEvalLedGetState(SdkEvalLed xLed)
{
  if(Get_LedGpioPort(xLed)->IDR & Get_LedGpioPin(xLed))
    return RESET;
  else
    return SET;
}


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */



/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/
