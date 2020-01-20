/**
  ******************************************************************************
  * @file    st25dx_discovery.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file provides set of firmware functions to manage Leds and
  *          push-button available on ST25DX-Discovery Kit from STMicroelectronics.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
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
#include "st25dx_discovery.h"
#include <stdio.h>

/** @defgroup BSP BSP
  * @{
  */ 

/** @addtogroup ST25DX_DISCOVERY_BSP ST25DX DISCOVERY
  * @brief    The Board support package for the ST25DX Discovery.
  * @details  Provides all the requires functions to abstract from the board HW.
  * @{
  */

/** @defgroup ST25DX_DISCOVERY_LOW_LEVEL ST25DX DISCOVERY LOW LEVEL
  * @brief This file provides set of firmware functions to manage Leds and push-button
  *        available on ST25DX Discovery Kit from STMicroelectronics.
  * @{
  */ 


/**
  * @brief ST25DX DISCOVERY BSP Driver version number V2.0.0
  * @{
  */
#define ST25DX_DISCOVERY_BSP_VERSION_X   (int32_t) (0x03) /*!< [31:24] main version */
#define ST25DX_DISCOVERY_BSP_VERSION_Y   (int32_t) (0x00) /*!< [23:16] sub1 version */
#define ST25DX_DISCOVERY_BSP_VERSION_Z   (int32_t) (0x00) /*!< [15:8]  sub2 version */
#define ST25DX_DISCOVERY_BSP_VERSION_RC  (int32_t) (0x00) /*!< [7:0]  release candidate */
#define ST25DX_DISCOVERY_BSP_VERSION        ((ST25DX_DISCOVERY_BSP_VERSION_X << 24)\
                                             |(ST25DX_DISCOVERY_BSP_VERSION_Y << 16)\
                                             |(ST25DX_DISCOVERY_BSP_VERSION_Z << 8)\
                                             |(ST25DX_DISCOVERY_BSP_VERSION_RC))
/**
  * @}
  */


/**
  * @brief List of GPIO ports used for the LEDs.
  */
GPIO_TypeDef* LED_PORT[LEDn] = {LED2_GPIO_PORT,
                                LED3_GPIO_PORT};

/**
  * @brief List of GPIO pins used for the LEDs.
  */
const uint32_t LED_PIN[LEDn] = {LED2_PIN,
                                LED3_PIN};

/**
  * @brief List of GPIO ports used for the Joystick & Button.
  */
GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {BUTTON_USER_GPIO_PORT,
                                      BUTTON_SELECT_GPIO_PORT,
                                      BUTTON_LEFT_GPIO_PORT,
                                      BUTTON_RIGHT_GPIO_PORT,
                                      BUTTON_UP_GPIO_PORT,
                                      BUTTON_DOWN_GPIO_PORT};

/**
  * @brief List of GPIO pins used for the Joystick & Button.
  */
const uint32_t BUTTON_PIN[BUTTONn] = {BUTTON_USER_PIN,
                                      BUTTON_SELECT_GPIO_PIN,
                                      BUTTON_LEFT_GPIO_PIN,
                                      BUTTON_RIGHT_GPIO_PIN,
                                      BUTTON_UP_GPIO_PIN,
                                      BUTTON_DOWN_GPIO_PIN};

/**
  * @brief List of interrupt lines used for the Joystick & Button.
  */
const uint32_t BUTTON_IRQn[BUTTONn] = {BUTTON_USER_EXTI_IRQn};

int32_t BSP_BUTTON_IT_PRIO[BUTTONn] = {BSP_BUTTON_USER_IT_PRIORITY,
                                       (uint32_t) 0x0F,
                                       (uint32_t) 0x0F,
                                       (uint32_t) 0x0F,
                                       (uint32_t) 0x0F,
                                       (uint32_t) 0x0F};


const uint32_t BUTTON_EXTI_LINE[BUTTONn] = {BUTTON_USER_EXTI_LINE};
EXTI_HandleTypeDef hpb_exti[BUTTONn];

#if USE_BSP_COM_FEATURE
UART_HandleTypeDef hcom_uart[COMn];

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
static uint32_t IsCOM1MspCbValid = 0;

static uint32_t COM_IsCOMMspCbValid[COMn] = {IsCOM1MspCbValid};
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

static void USART6_MspInit(UART_HandleTypeDef *huart);
static void USART6_MspDeInit(UART_HandleTypeDef *huart);
__weak HAL_StatusTypeDef MX_USART6_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *Init);

USART_TypeDef* COM_USART[COMn] = {COM1_UART};
GPIO_TypeDef* COM_TX_PORT[COMn] = {COM1_TX_GPIO_PORT};
GPIO_TypeDef* COM_RX_PORT[COMn] = {COM1_RX_GPIO_PORT};
const uint16_t COM_TX_PIN[COMn] = {COM1_TX_PIN};
const uint16_t COM_RX_PIN[COMn] = {COM1_RX_PIN};
const uint16_t COM_TX_AF[COMn] = {COM1_TX_AF};
const uint16_t COM_RX_AF[COMn] = {COM1_RX_AF};
static void (*COM_MspInit[COMn])(UART_HandleTypeDef *) = {USART6_MspInit};
static void (*COM_MspDeInit[COMn])(UART_HandleTypeDef *) = {USART6_MspDeInit};
HAL_StatusTypeDef (*COM_MX_MspInit[COMn])(UART_HandleTypeDef *, MX_UART_InitTypeDef *) = {MX_USART6_Init};

#if defined(USE_BSP_COM_FEATURE) && (USE_COM_LOG > 0)
static COM_TypeDef COM_ActiveLogPort = COM1;
#endif /* (USE_COM_LOG) && (USE_COM_LOG > 0) */
#endif /* USE_BSP_COM_FEATURE */

#if defined(HAL_ADC_MODULE_ENABLED) && defined(USE_ST25DV)
/**
* @brief List of Analog to Digital converter IOs.
*/
const uint32_t ADC1_Channel_list[ST25DX_DISCOVERY_EH_CHANNELn] = ST25DX_DISCOVERY_EH_CHANNEL_LIST;
static ADC_HandleTypeDef hpot_adc1;

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
static uint32_t IsADC1MspCbValid = 0;
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

uint32_t ADC1InitVrefintCounter = 0;
uint32_t ADC1InitCounter = 0;
#endif /* HAL_ADC_MODULE_ENABLED && USE_ST25DV */

EXTI_HandleTypeDef hexti15 = {.Line=BSP_GPO_EXTI_LINE};

/**
  * @brief  This method returns the ST25 DISCOVERY BSP Driver revision
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
int32_t BSP_GetVersion(void)
{
  return (int32_t) ST25DX_DISCOVERY_BSP_VERSION;
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured.
  *   This parameter can be one of following parameters:
  *     @arg LED2
  */
int32_t BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef gpio_init;

  /* Enable the GPIO_LED Clock */
  switch (Led)
  {
    case LED_EH:
      LED2_GPIO_CLK_ENABLE();
      break;

    case LED_USER:
      LED3_GPIO_CLK_ENABLE();
      break;

    default:
      break;
  }

  /* Configure the GPIO_LED pin */
  gpio_init.Pin = LED_PIN[Led];
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(LED_PORT[Led], &gpio_init);

  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);

  return BSP_ERROR_NONE;
}

/**
  * @brief  DeInit LEDs.
  * @param  Led: LED to be de-init.
  *   This parameter can be one of the following values:
  *     @arg  LED2
  * @note Led DeInit does not disable the GPIO clock nor disable the Mfx
  */
int32_t BSP_LED_DeInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef gpio_init;

  /* DeInit the GPIO_LED pin */
  gpio_init.Pin = LED_PIN[Led];

  /* Turn off LED */
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
  HAL_GPIO_DeInit(LED_PORT[Led], gpio_init.Pin);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED2
  */
int32_t BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *     @arg LED2
  */
int32_t BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *     @arg LED2
  */
int32_t BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_KEY
  * @param  ButtonMode: Specifies Button mode.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability
  */
int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef gpio_init;

  /* Enable the BUTTON clock: wakeup, tamper and key are on the same port */
  switch (Button)
  {
    case BUTTON_USER:
      BUTTON_USER_GPIO_CLK_ENABLE();
      break;

    case JOYSTICK_SELECT:
      BUTTON_SELECT_GPIO_CLK_ENABLE();
      break;

    case JOYSTICK_LEFT:
      BUTTON_LEFT_GPIO_CLK_ENABLE();
      break;

    case JOYSTICK_RIGHT:
      BUTTON_RIGHT_GPIO_CLK_ENABLE();
      break;

    case JOYSTICK_UP:
      BUTTON_UP_GPIO_CLK_ENABLE();
      break;

    case JOYSTICK_DOWN:
      BUTTON_DOWN_GPIO_CLK_ENABLE();
      break;

    default:
      break;
  }

  gpio_init.Pin = BUTTON_PIN[Button];
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

  if (ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    gpio_init.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init);
  }

  if (ButtonMode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    gpio_init.Mode = GPIO_MODE_IT_FALLING;
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init);

    HAL_EXTI_GetHandle(&hpb_exti[Button], BUTTON_EXTI_LINE[Button]);
    HAL_EXTI_RegisterCallback(&hpb_exti[Button], HAL_EXTI_RISING_CB_ID, (void (*)(void))BSP_PB_Callback);
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), BSP_BUTTON_IT_PRIO[Button], 0x00);
    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  Push Button DeInit.
  * @param  Button: Button to be configured.
  *   This parameter should be: BUTTON_KEY.
  * @note PB DeInit does not disable the GPIO clock.
  */
void BSP_PB_DeInit(Button_TypeDef Button)
{
    GPIO_InitTypeDef gpio_initstruct;

    gpio_initstruct.Pin = BUTTON_PIN[Button];
    HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
    HAL_GPIO_DeInit(BUTTON_PORT[Button], gpio_initstruct.Pin);
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_KEY.
  * @retval The Button GPIO pin value.
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return !HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

void BSP_PB_IRQHandler(Button_TypeDef Button)
{
  HAL_EXTI_IRQHandler(&hpb_exti [Button]);
}

__weak void BSP_PB_Callback(Button_TypeDef Button)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Button);

}

#if defined(USE_NFCTAG) && defined(USE_ST25DV)
/**
  * @brief  This function initialize the GPIO to manage the NFCTAG GPO pin
  * @param  None
  * @retval None
  */
int32_t BSP_GPO_Init(uint32_t it_mode)
{
  GPIO_InitTypeDef gpio_init;
  BSP_GPO_CLK_ENABLE();

  gpio_init.Pin   = BSP_GPO_PIN;
  gpio_init.Mode  = it_mode;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(BSP_GPO_PIN_PORT, &gpio_init);

  (void)HAL_EXTI_GetHandle(&hexti15, BSP_GPO_EXTI_LINE);
  (void)HAL_EXTI_RegisterCallback(&hexti15, HAL_EXTI_COMMON_CB_ID, BSP_GPO_Callback);

  /* Enable interruption */
  HAL_NVIC_SetPriority(BSP_GPO_EXTI, BSP_GPO_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(BSP_GPO_EXTI);

  return BSP_ERROR_NONE;
}

/**
  * @brief  DeInit GPO.
  * @param  None.
  * @return Status.
  * @note GPO DeInit does not disable the GPIO clock nor disable the Mfx
  */
int32_t BSP_GPO_DeInit(void)
{
  GPIO_InitTypeDef  gpio_init;

  /* Disable interrupt */
  HAL_NVIC_DisableIRQ(BSP_GPO_EXTI);

  /* DeInit the GPIO_GPO pin */
  gpio_init.Pin = BSP_GPO_PIN;
  HAL_GPIO_DeInit(BSP_GPO_PIN_PORT, gpio_init.Pin);

  return BSP_ERROR_NONE;

}

/**
  * @brief  This function get the GPO value through GPIO.
  * @param  None.
  * @retval GPIO pin status.
  */
int32_t BSP_GPO_ReadPin(void)
{
  return (int32_t)HAL_GPIO_ReadPin(BSP_GPO_PIN_PORT, BSP_GPO_PIN);
}

/**
  * @brief  BSP GPO callback.
  * @retval None.
  */
__weak void BSP_GPO_Callback(void)
{
  /* Prevent unused argument(s) compilation warning */

  /* This function should be implemented by the user application.
     It is called into this driver when an event on Button is triggered. */
}

/**
  * @brief  BSP GPO IRQHandler.
  * @retval None.
  */
void BSP_GPO_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hexti15);
}

/**
  * @brief  This function initialize the GPIO to manage the NFCTAG LPD pin
  * @param  None.
  * @retval None.
  */
void BSP_LPD_Init(void)
{
  GPIO_InitTypeDef gpio_initstruct;
  NFCMEM_LPD_CLK_ENABLE();

  gpio_initstruct.Pin   = NFCMEM_LPD_PIN;
  gpio_initstruct.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_initstruct.Speed = GPIO_SPEED_FREQ_LOW;
  gpio_initstruct.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(NFCMEM_LPD_PIN_PORT, &gpio_initstruct);

  HAL_GPIO_WritePin(NFCMEM_LPD_PIN_PORT, NFCMEM_LPD_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  This function get the GPIO value through GPIO.
  * @param  None.
  * @retval HAL GPIO pin status.
  */
GPIO_PinState BSP_LPD_ReadPin(void)
{
  return HAL_GPIO_ReadPin(NFCMEM_LPD_PIN_PORT, NFCMEM_LPD_PIN);
}

/**
  * @brief  This function get the GPIO value through GPIO.
  * @param  None.
  * @retval HAL GPIO pin status.
  */
void BSP_LPD_WritePin(GPIO_PinState LpdPinState)
{
  HAL_GPIO_WritePin(NFCMEM_LPD_PIN_PORT, NFCMEM_LPD_PIN, LpdPinState);
}
#endif /* USE_NFCTAG && USE_ST25DV */

#if defined(USE_BSP_COM_FEATURE)
/**
  * @brief Configures COM port.
  * @param huart USART handle.
  * @param Init Pointer to a UART_HandleTypeDef structure that contains the
  * configuration information for the specified USART peripheral.
  * @return HAL error code */
__weak HAL_StatusTypeDef MX_USART6_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *Init)
{
  /* USART configuration */
  huart->Init.BaudRate = Init->BaudRate;
  huart->Init.Mode = UART_MODE_TX_RX;
  huart->Init.Parity = (uint32_t)Init->Parity;
  huart->Init.WordLength = Init->WordLength;
  huart->Init.StopBits = (uint32_t)Init->StopBits;
  huart->Init.HwFlowCtl = (uint32_t)Init->HwFlowCtl;
  huart->Init.OverSampling = UART_OVERSAMPLING_8;

  return HAL_UART_Init(huart);
}

/**
  * @brief Initialize COM1 Msp part
  * @param huart UART handle
  * @retval None
  */
static void USART6_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef gpio_init;

  /* Enable GPIO clock */
  COM1_TX_GPIO_CLK_ENABLE();
  COM1_RX_GPIO_CLK_ENABLE();

  /* Configure USART Tx as alternate function */
  gpio_init.Pin = COM_TX_PIN[COM1];
  gpio_init.Mode = GPIO_MODE_AF_PP;
  gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio_init.Pull = GPIO_PULLUP;
  gpio_init.Alternate = COM1_TX_AF;
  HAL_GPIO_Init(COM_TX_PORT[COM1], &gpio_init);

  /* Configure USART Rx as alternate function */
  gpio_init.Pin = COM_RX_PIN[COM1];
  gpio_init.Mode = GPIO_MODE_AF_PP;
  gpio_init.Alternate = COM1_RX_AF;
  HAL_GPIO_Init(COM_RX_PORT[COM1], &gpio_init);

  /* Enable USART clock */
  COM1_CLK_ENABLE();
}

/**
  * @brief DeInitialize COM1 Msp part.
  * @param huart UART handle.
  * @retval None.
  */
static void USART6_MspDeInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef gpio_init;

  /* COM GPIO pin configuration */
  gpio_init.Pin = COM_TX_PIN[COM1];
  HAL_GPIO_DeInit(COM_TX_PORT[COM1], gpio_init.Pin);
  gpio_init.Pin = COM_RX_PIN[COM1];
  HAL_GPIO_DeInit(COM_RX_PORT[COM1], gpio_init.Pin);

  /* Disable USART clock */
  COM1_CLK_DISABLE();

}

/**
  * @brief  This function initialize the COM1.
  * @param  COM COM port to initialise.
  * @param  COM_Init pointer to configuration structure.
  * @return status error.
  */
int32_t BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    hcom_uart[COM].Instance = COM_USART[COM];
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
    /* Init the UART Msp */
    COM_MspInit[COM](&hcom_uart[COM]);
#else
    if (COM_IsCOMMspCbValid[COM] == 0U)
    {
      if (BSP_COM_RegisterDefaultMspCallbacks(COM) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#endif

    if (COM_MX_MspInit[COM](&hcom_uart[COM], COM_Init) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief DeInit COM port.
  * @param COM COM port to be configured.
  * This parameter can be COM1.
  * @retval BSP status.
  */
int32_t BSP_COM_DeInit(COM_TypeDef COM)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
    COM_MspDeInit[COM](&hcom_uart[COM]);
#endif
    /* (USE_HAL_UART_REGISTER_CALLBACKS == 0) */
    if (HAL_UART_DeInit(&hcom_uart[COM]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

#if (USE_COM_LOG > 0)
#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
{
  (void)HAL_UART_Transmit(&hcom_uart[COM_ActiveLogPort], (uint8_t *) &ch, 1, 0xFFFF);
  
  return 0;
}
#endif /* USE_COM_LOG */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default BSP COM Msp Callbacks.
  * @retval BSP status.
  */
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_UART_RESET_HANDLE_STATE(&hcom_uart[COM]);

    /* Register default MspInit/MspDeInit Callback */
    if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPINIT_CB_ID, COM_MspInit[COM]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPDEINIT_CB_ID, COM_MspDeInit[COM]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      COM_IsCOMMspCbValid[COM] = 1U;
    }
  }
  /* BSP status */
  return ret;
}

/**
  * @brief BSP COM Msp Callback registering.
  * @param Callbacks     pointer to COM MspInit/MspDeInit callback functions.
  * @retval BSP status.
  */
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_UART_RESET_HANDLE_STATE(&hcom_uart[COM]);

    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPINIT_CB_ID, Callback->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPDEINIT_CB_ID, Callback->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      COM_IsCOMMspCbValid[COM] = 1U;
    }
  }
  /* BSP status */
  return ret;
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
#endif /* USE_BSP_COM_FEATURE */

/******************************* ADC driver ********************************/
#if defined(HAL_ADC_MODULE_ENABLED) && defined(USE_ST25DV)
/**
  * @brief Initialize POT2 Msp part
  * @param hadc ADC handle
  * @retval None */
static void ADC1_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef gpio_init;

  ADC1_CLK_ENABLE();
  ADC1_CHANNEL15_GPIO_CLK_ENABLE();
  ADC1_CHANNEL8_GPIO_CLK_ENABLE();
  ADC1_CHANNEL9_GPIO_CLK_ENABLE();

  /* ADC Channel GPIO pin configuration */
  gpio_init.Pin = ADC1_CHANNEL15_GPIO_PIN;
  gpio_init.Mode = GPIO_MODE_ANALOG;
  gpio_init.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADC1_CHANNEL15_GPIO_PORT, &gpio_init);
  
  /* ADC Channel GPIO pin configuration */
  gpio_init.Pin = ADC1_CHANNEL8_GPIO_PIN;
  gpio_init.Mode = GPIO_MODE_ANALOG;
  gpio_init.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADC1_CHANNEL8_GPIO_PORT, &gpio_init);
  
  /* ADC Channel GPIO pin configuration */
  gpio_init.Pin = ADC1_CHANNEL9_GPIO_PIN;
  gpio_init.Mode = GPIO_MODE_ANALOG;
  gpio_init.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADC1_CHANNEL9_GPIO_PORT, &gpio_init);
}

/**
  * @brief De-Initialize POT2 Msp part
  * @param hadc ADC handle
  * @retval None */
static void ADC1_MspDeInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef gpio_init;

  /* POT GPIO pin configuration */
  gpio_init.Pin = ADC1_CHANNEL15_GPIO_PIN;
  HAL_GPIO_DeInit(ADC1_CHANNEL15_GPIO_PORT, gpio_init.Pin);
  
  /* POT GPIO pin configuration */
  gpio_init.Pin = ADC1_CHANNEL8_GPIO_PIN;
  HAL_GPIO_DeInit(ADC1_CHANNEL8_GPIO_PORT, gpio_init.Pin);
  
  /* POT GPIO pin configuration */
  gpio_init.Pin = ADC1_CHANNEL9_GPIO_PIN;
  HAL_GPIO_DeInit(ADC1_CHANNEL9_GPIO_PORT, gpio_init.Pin);

  /* Disable ADC clock */
  ADC1_CLK_DISABLE();
}

/**
  * @brief Initialize ADC1 Msp part
  * @param hadc ADC handle
  * @retval None */
static void ADC1_MspInitVrefInt(ADC_HandleTypeDef *hadc)
{
  ADC1_CLK_ENABLE();
}

/**
  * @brief DeInitialize ADC1 Msp part
  * @param hadc ADC handle
  * @retval None */
static void ADC1_MspDeInitVrefInt(ADC_HandleTypeDef *hadc)
{
  ADC1_CLK_DISABLE();
}

/**
  * @brief  Initializes ADC HAL.
  */
__weak HAL_StatusTypeDef MX_ADC1_InitVrefint(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef ret = HAL_OK;
  ADC_ChannelConfTypeDef sConfig;

  /* ADC configuration */
  hadc->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc->Init.Resolution = ADC_RESOLUTION_12B;
  hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc->Init.ScanConvMode = ENABLE;
  hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc->Init.ContinuousConvMode = DISABLE;
  hadc->Init.NbrOfConversion = 1;
  hadc->Init.DiscontinuousConvMode = DISABLE;
  hadc->Init.NbrOfDiscConversion = 0;
  hadc->Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
  hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc->Init.DMAContinuousRequests = DISABLE;

  if (HAL_ADC_Init(hadc) != HAL_OK)
  {
    ret = HAL_ERROR;
  }
  else
  {
    sConfig.Channel = ADC_CHANNEL_VREFINT;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    sConfig.Offset = 0;

    HAL_ADC_ConfigChannel(hadc, &sConfig);
  }
  return ret;
}

/**
  * @brief  Initializes ADC HAL.
  */
__weak HAL_StatusTypeDef MX_ADC1_Init(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef ret = HAL_OK;
  ADC_ChannelConfTypeDef sConfig;

  /* ADC configuration */
  hadc->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc->Init.Resolution = ADC_RESOLUTION_12B;
  hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc->Init.ScanConvMode = ENABLE;
  hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc->Init.ContinuousConvMode = DISABLE;
  hadc->Init.NbrOfConversion = ST25DX_DISCOVERY_EH_CHANNELn;
  hadc->Init.DiscontinuousConvMode = DISABLE;
  hadc->Init.NbrOfDiscConversion = 0;
  hadc->Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
  hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc->Init.DMAContinuousRequests = DISABLE;

  if (HAL_ADC_Init(hadc) != HAL_OK)
  {
    ret = HAL_ERROR;
  }
  else
  {
    /* Configure the selected ADC regular channels */
    for (int i = 0; i < ST25DX_DISCOVERY_EH_CHANNELn; i++)
    {
      sConfig.Channel = ADC1_Channel_list[i];
      sConfig.Rank = i;
      sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;

      HAL_ADC_ConfigChannel(hadc, &sConfig);
    }
  }
  return ret;
}

/**
  * @brief  Initializes ADC1 peripheral for vref measure.
  */
int32_t BSP_ADC1_InitVrefint(void)
{
  int32_t ret = BSP_ERROR_NONE;

  hpot_adc1.Instance = ADC1;
  
  if (ADC1InitVrefintCounter++ == 0)
  {
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 0)
    /* Init the ADC Msp */
    ADC1_MspInitVrefInt(&hpot_adc1);
#else
    if (IsADC1MspCbValid == 0U)
    {
      if (BSP_ADC1_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#endif
    if (MX_ADC1_InitVrefint(&hpot_adc1) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  DeInitialize ADC HAL.
  * @retval BSP status
  */
int32_t BSP_ADC1_DeInitVrefint(void)
{
  int32_t ret = BSP_ERROR_NONE;

  if (ADC1InitVrefintCounter > 0)
  {
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 0)
    /* DeInit the ADC */
    ADC1_MspDeInitVrefInt(&hpot_adc1);
#endif
    ADC1InitVrefintCounter = 0;
    /* DeInit the ADC */
    if (HAL_ADC_DeInit(&hpot_adc1) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Initializes ADC1 peripheral for measures.
  */
int32_t BSP_ADC1_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;

  hpot_adc1.Instance = ADC1;
  
  if (ADC1InitCounter++ == 0)
  {
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 0)
    /* Init the ADC Msp */
    ADC1_MspInit(&hpot_adc1);
#else
    if (IsADC1MspCbValid == 0U)
    {
      if (BSP_ADC1_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#endif
    if (MX_ADC1_Init(&hpot_adc1) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  DeInitialize ADC HAL.
  * @retval BSP status
  */
int32_t BSP_ADC1_DeInit(void)
{
  int32_t ret = BSP_ERROR_NONE;

  if (ADC1InitCounter > 0)
  {
    ADC1InitCounter = 0;
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 0)
    /* DeInit the ADC */
    ADC1_MspDeInit(&hpot_adc1);
#endif
    /* DeInit the I2C */
    if (HAL_ADC_DeInit(&hpot_adc1) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default BSP ADC1 Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_ADC1_RegisterDefaultMspCallbacks(void)
{

  __HAL_ADC_RESET_HANDLE_STATE(&hpot_adc1);

  /* Register MspInit Callback */
  if (HAL_ADC_RegisterCallback(&hpot_adc1, HAL_ADC_MSPINIT_CB_ID, ADC1_MspInit)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_ADC_RegisterCallback(&hpot_adc1, HAL_ADC_MSPDEINIT_CB_ID, ADC1_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsADC1MspCbValid = 1;

  return BSP_ERROR_NONE;
}

/**
  * @brief BSP ADC1 Bus Msp Callback registering
  * @param Callbacks     pointer to ADC1 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_ADC1_RegisterMspCallbacks(BSP_ADC_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_ADC_RESET_HANDLE_STATE(&hpot_adc1);
 
   /* Register MspInit Callback */
  if (HAL_ADC_RegisterCallback(&hpot_adc1, HAL_ADC_MSPINIT_CB_ID, Callbacks->pMspInitCb)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_ADC_RegisterCallback(&hpot_adc1, HAL_ADC_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  IsADC1MspCbValid = 1;

  return BSP_ERROR_NONE;
}
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

/******************************* LINK ST25DV EH ********************************/
/**
  * @brief  Configures ADC for Energy Harvesting measurement from ST25DV.
  * @retval (0=> success, 1=> fail)
  */
uint8_t BSP_ST25DV_EH_Init(uint32_t *vdda)
{
  uint8_t status = HAL_OK;
  uint16_t vref_int = 0;
  
  BSP_ADC1_InitVrefint();

  BSP_ST25DV_EH_Measure(&vref_int);

  *vdda = (3300 * (*ST25DX_DISCOVERY_VREFINT_CAL_ADDR)) / vref_int;

  BSP_ADC1_DeInitVrefint();
  
  BSP_ADC1_Init();

  /* Return EH meas initialization status */
  return status;
}

/**
 * @brief   This function starts the ADC and poll for conversions, stops when the sequence of conversion is over.
 * @param   values	Table to save converted values.
 * @retval  None.
 */
void BSP_ST25DV_EH_Measure(uint16_t *values)
{
    int i;

    HAL_ADC_Start(&hpot_adc1);
    for (i = 0; i < hpot_adc1.Init.NbrOfConversion; i++)
    {
      HAL_ADC_PollForConversion(&hpot_adc1, 10);
      values[i] =  HAL_ADC_GetValue(&hpot_adc1);
    }
    HAL_ADC_Stop(&hpot_adc1);
}

/**
  * @brief  Deconfigures ADC for Energy Harvesting measurement from ST25DV.
  * @retval None.
  */
void BSP_ST25DV_EH_DeInit(void)
{
  BSP_ADC1_DeInit();
}

#endif /* HAL_ADC_MODULE_ENABLED && USE_ST25DV */

/**
  * @}
  */

/**
  * @}
  */    

/**
  * @}
  */ 
    
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
