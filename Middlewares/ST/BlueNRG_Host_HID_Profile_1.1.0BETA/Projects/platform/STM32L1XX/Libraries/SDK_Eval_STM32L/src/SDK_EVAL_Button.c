/**
 * @file    SDK_EVAL_Button.c
 * @author  AMS - AAS Division
 * @version V1.0.0
 * @date    Febrary 3, 2014
 * @brief   This file provides all the low level API to manage SDK buttons.
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


/** @addtogroup SDK_EVAL_Button
 * @{
 */

/** @defgroup SDK_EVAL_Button_Private_TypesDefinitions          SDK EVAL Button Private Types Definitions
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Button_Private_Defines                   SDK EVAL Button Private Defines
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Button_Private_Macros                    SDK EVAL Button Private Macros
 * @{
 */

/**
 * @}
 */


#ifndef USER_DEFINED_PLATFORM 

/** @defgroup SDK_EVAL_Button_Private_Variables                         SDK EVAL Button Private Variables
 * @{
 */

/**
 * NOTE:on BlueNRG Eval Board Second user button is the reset button, not a user button: 
 *      reuse the Joystick Right push-button only to keep a  consistent array structures.
 * Arrays logic:
 *      Second User Button if available, Joystick right, Joystick left, Joystick up, Joystick down, Joystick sel, First User Button. 
 */
   
GPIO_TypeDef* s_vectpxButtonPortVersion[SDK_PLATFORMS_NUMBER][BUTTONn] = {
    {RIGHT_BUTTON_GPIO_PORT, RIGHT_BUTTON_GPIO_PORT, LEFT_BUTTON_V3_GPIO_PORT, UP_BUTTON_V3_GPIO_PORT, DOWN_BUTTON_GPIO_PORT, SEL_BUTTON_V3_GPIO_PORT, SCM_PS_BUTTON_GPIO_PORT},
    {KEY_BUTTON_VD1_GPIO_PORT, KEY_BUTTON_VD1_GPIO_PORT, KEY_BUTTON_VD1_GPIO_PORT, KEY_BUTTON_VD1_GPIO_PORT, KEY_BUTTON_VD1_GPIO_PORT, KEY_BUTTON_VD1_GPIO_PORT, SCM_PS_BUTTON_VD1_GPIO_PORT}   
};

static const uint16_t s_vectnButtonPinVersion[SDK_PLATFORMS_NUMBER][BUTTONn] = {
    {RIGHT_BUTTON_PIN, RIGHT_BUTTON_PIN, LEFT_BUTTON_V3_PIN, UP_BUTTON_V3_PIN, DOWN_BUTTON_PIN, SEL_BUTTON_V3_PIN, SCM_PS_BUTTON_PIN},
    {KEY_BUTTON_VD1_PIN, KEY_BUTTON_VD1_PIN, KEY_BUTTON_VD1_PIN, KEY_BUTTON_VD1_PIN, KEY_BUTTON_VD1_PIN, KEY_BUTTON_VD1_PIN, SCM_PS_BUTTON_VD1_PIN}
};

static const uint32_t s_vectlButtonClkVersion[SDK_PLATFORMS_NUMBER][BUTTONn] = {
    {RIGHT_BUTTON_GPIO_CLK, RIGHT_BUTTON_GPIO_CLK, LEFT_BUTTON_V3_GPIO_CLK, UP_BUTTON_V3_GPIO_CLK, DOWN_BUTTON_GPIO_CLK, SEL_BUTTON_V3_GPIO_CLK, SCM_PS_BUTTON_GPIO_CLK},
    {KEY_BUTTON_VD1_GPIO_CLK, KEY_BUTTON_VD1_GPIO_CLK, KEY_BUTTON_VD1_GPIO_CLK, KEY_BUTTON_VD1_GPIO_CLK, KEY_BUTTON_VD1_GPIO_CLK, KEY_BUTTON_VD1_GPIO_CLK, SCM_PS_BUTTON_VD1_GPIO_CLK}
};

static const uint16_t s_vectnButtonExtiLineVersion[SDK_PLATFORMS_NUMBER][BUTTONn] = {
    {RIGHT_BUTTON_EXTI_LINE, RIGHT_BUTTON_EXTI_LINE, LEFT_BUTTON_V3_EXTI_LINE, UP_BUTTON_V3_EXTI_LINE, DOWN_BUTTON_EXTI_LINE, SEL_BUTTON_V3_EXTI_LINE, SCM_PS_BUTTON_EXTI_LINE},
    {KEY_BUTTON_VD1_EXTI_LINE, KEY_BUTTON_VD1_EXTI_LINE, KEY_BUTTON_VD1_EXTI_LINE, KEY_BUTTON_VD1_EXTI_LINE, KEY_BUTTON_VD1_EXTI_LINE, KEY_BUTTON_VD1_EXTI_LINE, SCM_PS_BUTTON_VD1_EXTI_LINE},
};

static const uint16_t s_vectnButtonPortSourceVersion[SDK_PLATFORMS_NUMBER][BUTTONn] = {
    {RIGHT_BUTTON_EXTI_PORT_SOURCE, RIGHT_BUTTON_EXTI_PORT_SOURCE, LEFT_BUTTON_V3_EXTI_PORT_SOURCE, UP_BUTTON_V3_EXTI_PORT_SOURCE, DOWN_BUTTON_EXTI_PORT_SOURCE, SEL_BUTTON_V3_EXTI_PORT_SOURCE, SCM_PS_BUTTON_EXTI_PORT_SOURCE},
    {KEY_BUTTON_VD1_EXTI_PORT_SOURCE, KEY_BUTTON_VD1_EXTI_PORT_SOURCE, KEY_BUTTON_VD1_EXTI_PORT_SOURCE, KEY_BUTTON_VD1_EXTI_PORT_SOURCE, KEY_BUTTON_VD1_EXTI_PORT_SOURCE, KEY_BUTTON_VD1_EXTI_PORT_SOURCE, SCM_PS_BUTTON_VD1_EXTI_PORT_SOURCE}
};

static const uint16_t s_vectnButtonPinSourceVersion[SDK_PLATFORMS_NUMBER][BUTTONn] = {
    {RIGHT_BUTTON_EXTI_PIN_SOURCE, RIGHT_BUTTON_EXTI_PIN_SOURCE, LEFT_BUTTON_V3_EXTI_PIN_SOURCE, UP_BUTTON_V3_EXTI_PIN_SOURCE, DOWN_BUTTON_EXTI_PIN_SOURCE, SEL_BUTTON_V3_EXTI_PIN_SOURCE, SCM_PS_BUTTON_EXTI_PIN_SOURCE},
    {KEY_BUTTON_VD1_EXTI_PIN_SOURCE, KEY_BUTTON_VD1_EXTI_PIN_SOURCE, KEY_BUTTON_VD1_EXTI_PIN_SOURCE, KEY_BUTTON_VD1_EXTI_PIN_SOURCE, KEY_BUTTON_VD1_EXTI_PIN_SOURCE, KEY_BUTTON_VD1_EXTI_PIN_SOURCE, SCM_PS_BUTTON_VD1_EXTI_PIN_SOURCE}
};

static const IRQn_Type s_vectnButtonIrqnVersion[SDK_PLATFORMS_NUMBER][BUTTONn] = {
    {RIGHT_BUTTON_EXTI_IRQn, RIGHT_BUTTON_EXTI_IRQn, LEFT_BUTTON_V3_EXTI_IRQn, UP_BUTTON_V3_EXTI_IRQn, DOWN_BUTTON_EXTI_IRQn, SEL_BUTTON_V3_EXTI_IRQn, SCM_PS_BUTTON_EXTI_IRQn},
    {KEY_BUTTON_VD1_EXTI_IRQn, KEY_BUTTON_VD1_EXTI_IRQn, KEY_BUTTON_VD1_EXTI_IRQn, KEY_BUTTON_VD1_EXTI_IRQn, KEY_BUTTON_VD1_EXTI_IRQn, KEY_BUTTON_VD1_EXTI_IRQn, SCM_PS_BUTTON_VD1_EXTI_IRQn}
};


static GPIO_TypeDef** s_vectpxButtonPort;
static uint16_t* s_vectnButtonPin;
static uint32_t* s_vectlButtonClk;
static uint16_t* s_vectnButtonExtiLine;
static uint16_t* s_vectnButtonPortSource;
static uint16_t* s_vectnButtonPinSource;
static IRQn_Type* s_vectnButtonIrqn;

#endif /* USER_DEFINED_PLATFORM */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Button_Private_FunctionPrototypes                        SDK EVAL Button Private Function Prototypes
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_Button_Private_Functions                                 SDK EVAL Button Private Functions
 * @{
 */

/* BUTTON IRQ pin, exti line, port source, pin source, irqn */

/* Buttons IRQ Port */
static GPIO_TypeDef* Get_ButtonGpioPort(SdkEvalButton xButton)
{
#ifdef USER_DEFINED_PLATFORM
  GPIO_TypeDef* button_port;
  switch (xButton)
  {
    case(BUTTON_1):
      button_port = USER_BUTTON1_GPIO_PORT;
      break;
    case(BUTTON_2):
      button_port = USER_BUTTON2_GPIO_PORT;
      break;
    default:
      button_port = USER_BUTTON1_GPIO_PORT;
      break;
  }
  return (button_port);
#else
  return(s_vectpxButtonPort[xButton]);
#endif
}

/* Buttons IRQ Pin */
static uint32_t Get_ButtonGpioPin(SdkEvalButton xButton)
{
#ifdef USER_DEFINED_PLATFORM
  uint32_t button_pin;
  switch (xButton)
  {
    case(BUTTON_1):
      button_pin = USER_BUTTON1_GPIO_PIN;
      break;
    case(BUTTON_2):
      button_pin = USER_BUTTON2_GPIO_PIN;
      break;
    default:
      button_pin = USER_BUTTON1_GPIO_PIN;
      break;
  }
  return (button_pin);
#else
  return(s_vectnButtonPin[xButton]);
#endif
}

/* Buttons GPIO CLK */
static uint32_t Get_ButtonGpioClk(SdkEvalButton xButton)
{
#ifdef USER_DEFINED_PLATFORM
  uint32_t button_clk;
  switch (xButton)
  {
    case(BUTTON_1):
      button_clk = USER_BUTTON1_GPIO_CLK;
      break;
    case(BUTTON_2):
      button_clk = USER_BUTTON2_GPIO_CLK;
      break;
    default:
      button_clk = USER_BUTTON1_GPIO_CLK;
      break;
  }
  return (button_clk);
#else
  return(s_vectlButtonClk[xButton]);
#endif
}

/* Buttons IRQ Exti Line */
static uint32_t Get_ButtonIrqExtiLine(SdkEvalButton xButton)
{
#ifdef USER_DEFINED_PLATFORM
  uint32_t button_exti_line;
  switch (xButton)
  {
    case(BUTTON_1):
      button_exti_line = USER_BUTTON1_EXTI_LINE;
      break;
    case(BUTTON_2):
      button_exti_line = USER_BUTTON2_EXTI_LINE;
      break;
    default:
      button_exti_line = USER_BUTTON1_EXTI_LINE;
      break;
  }
  return (button_exti_line);
#else
  return(s_vectnButtonExtiLine[xButton]);
#endif
}

/* Button IRQ Port Source */
static uint8_t Get_ButtonIrqPortSource(SdkEvalButton xButton)
{
#ifdef USER_DEFINED_PLATFORM
  uint8_t button_port_source;
  switch (xButton)
  {
    case(BUTTON_1):
      button_port_source = USER_BUTTON1_EXTI_PORT_SOURCE;
      break;
    case(BUTTON_2):
      button_port_source = USER_BUTTON2_EXTI_PORT_SOURCE;
      break;
    default:
      button_port_source = USER_BUTTON1_EXTI_PORT_SOURCE;
      break;
  }
  return (button_port_source);
#else
  return(s_vectnButtonPortSource[xButton]);
#endif
}

/* Button IRQ Pin Source */
static uint8_t Get_ButtonIrqPinSource(SdkEvalButton xButton)
{
#ifdef USER_DEFINED_PLATFORM
  uint8_t button_pin_source;
  switch (xButton)
  {
    case(BUTTON_1):
      button_pin_source = USER_BUTTON1_EXTI_PIN_SOURCE;
      break;
    case(BUTTON_2):
      button_pin_source = USER_BUTTON2_EXTI_PIN_SOURCE;
      break;
    default:
      button_pin_source = USER_BUTTON1_EXTI_PIN_SOURCE;
      break;
  }
  return (button_pin_source);
#else
  return(s_vectnButtonPinSource[xButton]);
#endif
}

/* Button IRQ line */
static IRQn_Type Get_ButtonIrqIrqn(SdkEvalButton xButton)
{
#ifdef USER_DEFINED_PLATFORM
  IRQn_Type button_irq;
  switch (xButton)
  {
    case(BUTTON_1):
      button_irq = USER_BUTTON1_EXTI_IRQn;
      break;
    case(BUTTON_2):
      button_irq = USER_BUTTON2_EXTI_IRQn;
      break;
    default:
      button_irq = USER_BUTTON1_EXTI_IRQn;
      break;
  }
  return (button_irq);
#else
  return(s_vectnButtonIrqn[xButton]);
#endif
}



/**
 * @brief  Configures Button GPIO and EXTI Line.
 * @param  xButton Specifies the Button to be configured.
 *         This parameter can be one of following parameters:
 *         @arg BUTTON_WAKEUP: Wakeup Push Button
 *         @arg BUTTON_TAMPER: Tamper Push Button
 *         @arg BUTTON_KEY: Key Push Button
 *         @arg BUTTON_RIGHT: Joystick Right Push Button
 *         @arg BUTTON_LEFT: Joystick Left Push Button
 *         @arg BUTTON_UP: Joystick Up Push Button
 *         @arg BUTTON_DOWN: Joystick Down Push Button
 *         @arg BUTTON_SEL: Joystick Sel Push Button
 * @param  xButtonMode Specifies Button mode.
 *         This parameter can be one of following parameters:
 *         @arg BUTTON_MODE_GPIO: Button will be used as simple IO
 *         @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
 *         generation capability
 * @retval None.
 */
void SdkEvalPushButtonInit(SdkEvalButton xButton, SdkEvalButtonMode xButtonMode)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
#ifndef USER_DEFINED_PLATFORM 
  s_vectpxButtonPort = s_vectpxButtonPortVersion[SdkEvalGetVersion()];
  s_vectnButtonPin = (uint16_t *)s_vectnButtonPinVersion[SdkEvalGetVersion()];
  s_vectlButtonClk = (uint32_t *)s_vectlButtonClkVersion[SdkEvalGetVersion()];
  s_vectnButtonExtiLine = (uint16_t *)s_vectnButtonExtiLineVersion[SdkEvalGetVersion()];
  s_vectnButtonPortSource = (uint16_t *)s_vectnButtonPortSourceVersion[SdkEvalGetVersion()];
  s_vectnButtonPinSource = (uint16_t *)s_vectnButtonPinSourceVersion[SdkEvalGetVersion()];
  s_vectnButtonIrqn = (IRQn_Type *)s_vectnButtonIrqnVersion[SdkEvalGetVersion()];
#endif /* end USER_DEFINED_PLATFORM */
  
  /* Enables the BUTTON Clock */
  RCC_AHBPeriphClockCmd(Get_ButtonGpioClk(xButton), ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configures Button pin as input */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = Get_ButtonGpioPin(xButton);
  GPIO_Init(Get_ButtonGpioPort(xButton), &GPIO_InitStructure);

  if (xButtonMode == BUTTON_MODE_EXTI)
  {
    /* Connects Button EXTI Line to Button GPIO Pin */
    SYSCFG_EXTILineConfig(Get_ButtonIrqPortSource(xButton), Get_ButtonIrqPinSource(xButton));

    /* Configures Button EXTI line */
    EXTI_InitStructure.EXTI_Line = Get_ButtonIrqExtiLine(xButton);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enables and sets Button EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = Get_ButtonIrqIrqn(xButton);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BUTTON_IRQ_PREEMPTION_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = BUTTON_IRQ_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    EXTI_ClearITPendingBit(Get_ButtonIrqExtiLine(xButton));

    NVIC_Init(&NVIC_InitStructure);
  }
}

/**
 * @brief  Returns the selected Button state.
 * @param  xButton Specifies the Button to be checked.
 *         This parameter can be one of following parameters:
 *         @arg BUTTON_WAKEUP: Wakeup Push Button
 *         @arg BUTTON_TAMPER: Tamper Push Button
 *         @arg BUTTON_KEY: Key Push Button
 *         @arg BUTTON_RIGHT: Joystick Right Push Button
 *         @arg BUTTON_LEFT: Joystick Left Push Button
 *         @arg BUTTON_UP: Joystick Up Push Button
 *         @arg BUTTON_DOWN: Joystick Down Push Button
 *         @arg BUTTON_SEL: Joystick Sel Push Button
 * @retval FlagStatus The Button GPIO pin value.
 */
FlagStatus SdkEvalPushButtonGetState(SdkEvalButton xButton)
{
  if(GPIO_ReadInputDataBit(Get_ButtonGpioPort(xButton), Get_ButtonGpioPin(xButton)))
    return SET;
  else
    return RESET;
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
