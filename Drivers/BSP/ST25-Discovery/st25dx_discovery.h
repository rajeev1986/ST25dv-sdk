/**
  ******************************************************************************
  * @file    st25dx_discovery.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file contains definitions for ST25Dx-Discovery Kit LEDs,
  *          push-buttons hardware resources.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ST25DX_DISCOVERY_H
#define ST25DX_DISCOVERY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "st25dx_discovery_conf.h"
#include "st25dx_discovery_errno.h"

#if defined(USE_NFCTAG) && defined(USE_ST25DV)
#include "st25dv.h"

#endif /* USE_NFCTAG && USE_ST25DV */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup ST25DX_DISCOVERY_BSP
  * @{
  */

/** @addtogroup ST25DX_DISCOVERY_LOW_LEVEL
  * @{
  */

/**
  * @brief
  */
typedef enum
{
  LED2 = 0,
  LED_EH = LED2,
  LED3 = 1,
  LED_USER = LED3
} Led_TypeDef;

typedef enum
{
  BUTTON_USER = 0,
  JOYSTICK_SELECT = 1,
  JOYSTICK_LEFT = 2,
  JOYSTICK_RIGHT = 3,
  JOYSTICK_UP = 4,
  JOYSTICK_DOWN = 5,
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef void (* BSP_EXTI_LineCallback) (void);

typedef enum
{
  COM1 = 0
} COM_TypeDef;

typedef enum
{
  COM_STOPBITS_1 = UART_STOPBITS_1,
  COM_STOPBITS_2 = UART_STOPBITS_2,
} COM_StopBitsTypeDef;

typedef enum
{
  COM_PARITY_NONE = UART_PARITY_NONE,
  COM_PARITY_EVEN = UART_PARITY_EVEN,
  COM_PARITY_ODD = UART_PARITY_ODD,
} COM_ParityTypeDef;

typedef enum
{
  COM_HWCONTROL_NONE = UART_HWCONTROL_NONE,
  COM_HWCONTROL_RTS = UART_HWCONTROL_RTS,
  COM_HWCONTROL_CTS = UART_HWCONTROL_CTS,
  COM_HWCONTROL_RTS_CTS = UART_HWCONTROL_RTS_CTS,
} COM_HwFlowCtlTypeDef;

typedef enum
{
  COM_WORDLENGTH_8B = UART_WORDLENGTH_8B,
  COM_WORDLENGTH_9B = UART_WORDLENGTH_9B,
} COM_WordLengthTypeDef;

typedef struct
{
  uint32_t BaudRate;
  COM_WordLengthTypeDef WordLength;
  COM_StopBitsTypeDef StopBits;
  COM_ParityTypeDef Parity;
  COM_HwFlowCtlTypeDef HwFlowCtl;
} COM_InitTypeDef;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
typedef struct
{
  void (* pMspInitCb)(UART_HandleTypeDef *);
  void (* pMspDeInitCb)(UART_HandleTypeDef *);
} BSP_COM_Cb_t;
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

/* LEDs */
#define LEDn                              2

#define LED2_PIN                          GPIO_PIN_4
#define LED2_GPIO_PORT                    GPIOC
#define LED2_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()

#define LED3_PIN                          GPIO_PIN_13
#define LED3_GPIO_PORT                    GPIOC
#define LED3_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()

/* BUTTONs */
#define BUTTONn                           6

#define BUTTON_USER_PIN                   GPIO_PIN_14
#define BUTTON_USER_GPIO_PORT             GPIOC
#define BUTTON_USER_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUTTON_USER_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUTTON_USER_EXTI_IRQn             EXTI15_10_IRQn
#define BUTTON_USER_EXTI_LINE             EXTI_LINE_14

#define BUTTON_SELECT_GPIO_PIN            GPIO_PIN_8
#define BUTTON_SELECT_GPIO_PORT           GPIOE
#define BUTTON_SELECT_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()
#define BUTTON_SELECT_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOE_CLK_DISABLE()

#define BUTTON_LEFT_GPIO_PIN              GPIO_PIN_9
#define BUTTON_LEFT_GPIO_PORT             GPIOE
#define BUTTON_LEFT_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define BUTTON_LEFT_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()

#define BUTTON_RIGHT_GPIO_PIN             GPIO_PIN_11
#define BUTTON_RIGHT_GPIO_PORT            GPIOE
#define BUTTON_RIGHT_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOE_CLK_ENABLE()
#define BUTTON_RIGHT_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOE_CLK_DISABLE()

#define BUTTON_UP_GPIO_PIN                GPIO_PIN_10
#define BUTTON_UP_GPIO_PORT               GPIOE
#define BUTTON_UP_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()
#define BUTTON_UP_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOE_CLK_DISABLE()

#define BUTTON_DOWN_GPIO_PIN              GPIO_PIN_12
#define BUTTON_DOWN_GPIO_PORT             GPIOE
#define BUTTON_DOWN_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define BUTTON_DOWN_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()

#define MX_UART_InitTypeDef               COM_InitTypeDef
#define COMn                              ((COM_TypeDef)1)
#define COM1_UART                         USART6
#define COM1_TX_GPIO_PORT                 GPIOC
#define COM1_TX_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define COM1_TX_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()
#define COM1_TX_PIN                       GPIO_PIN_6
#define COM1_TX_AF                        GPIO_AF8_USART6
#define COM1_RX_GPIO_PORT                 GPIOC
#define COM1_RX_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define COM1_RX_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()
#define COM1_RX_PIN                       GPIO_PIN_7
#define COM1_CLK_ENABLE()                 __HAL_RCC_USART6_CLK_ENABLE()
#define COM1_CLK_DISABLE()                __HAL_RCC_USART6_CLK_DISABLE()
#define COM1_RX_AF                        GPIO_AF8_USART6

/*################################ ADC1 ######################################*/
#if defined(HAL_ADC_MODULE_ENABLED)
/**
  * @brief  ADC Interface pins
  *         used to detect motion of Joystick available on Adafruit 1.8" TFT shield
  */
  typedef enum 
{
  ADC1_CHANNEL_8 = 0,
  ADC1_CHANNEL_9 = 1,
  ADC1_CHANNEL_15 = 2
} ST25DX_DISCOVERY_EH_ADC_CHANNEL_TypeDef;

#define ADC1_CLK_ENABLE()                 __HAL_RCC_ADC1_CLK_ENABLE()
#define ADC1_CLK_DISABLE()                __HAL_RCC_ADC1_CLK_DISABLE()
#define ADC1_CHANNEL15_GPIO_PIN           GPIO_PIN_5
#define ADC1_CHANNEL15_GPIO_PORT          GPIOC
#define ADC1_CHANNEL15_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()

#define ADC1_CHANNEL8_GPIO_PIN            GPIO_PIN_0
#define ADC1_CHANNEL8_GPIO_PORT           GPIOB
#define ADC1_CHANNEL8_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define ADC1_CHANNEL9_GPIO_PIN            GPIO_PIN_1
#define ADC1_CHANNEL9_GPIO_PORT           GPIOB
#define ADC1_CHANNEL9_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define ST25DX_DISCOVERY_VREFINT_CAL_ADDR ((uint16_t *)0x1FFF7A2AU)

#define ST25DX_DISCOVERY_EH_CHANNELn      3
#define ST25DX_DISCOVERY_EH_CHANNEL_LIST  {ADC_CHANNEL_15, ADC_CHANNEL_8, ADC_CHANNEL_9}

#endif /* HAL_ADC_MODULE_ENABLED */

/*################################ GPO NFCMEM #######################################*/
/** 
  * @brief  GPO Input pin
  */
#if defined(USE_NFCTAG)
#define BSP_GPO_PIN                       GPIO_PIN_15
#define BSP_GPO_PIN_PORT                  GPIOE
#define BSP_GPO_CLK_ENABLE()              __HAL_RCC_GPIOE_CLK_ENABLE()
#define BSP_GPO_CLK_DISABLE()             __HAL_RCC_GPIOE_CLK_DISABLE()

/*################################ SPARE NFCMEM #######################################*/
/** 
  * @brief  SPARE Control pin
  */
#define NFCMEM_SPARE_PIN                  GPIO_PIN_7
#define NFCMEM_SPARE_PIN_PORT             GPIOE
#define NFCMEM_SPARE_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define NFCMEM_SPARE_CLK_DISABLE()        __HAL_RCC_GPIOE_CLK_DISABLE()
#define NFCMEM_SPARE_EXTI                 EXTI9_5_IRQn
#define NFCMEM_SPARE_EXTIHandler          EXTI9_5_IRQHandler

#if defined(USE_ST25DV)
/**
  * @brief  ST25DV LPD wrapper
  */
#define NFCMEM_LPD_PIN                    NFCMEM_SPARE_PIN
#define NFCMEM_LPD_PIN_PORT               NFCMEM_SPARE_PIN_PORT
#define NFCMEM_LPD_CLK_ENABLE()           NFCMEM_SPARE_CLK_ENABLE()
#define NFCMEM_LPD_CLK_DISABLE()          NFCMEM_SPARE_CLK_DISABLE()
#define NFCMEM_LPD_EXTI                   NFCMEM_SPARE_EXTI
#define NFCMEM_LPD_EXTIHandler            NFCMEM_SPARE_EXTIHandler

#endif /* USE_ST25DV */

#endif /* USE_NFCTAG */

int32_t       BSP_GetVersion(void);
int32_t       BSP_LED_Init(Led_TypeDef Led);
int32_t       BSP_LED_DeInit(Led_TypeDef Led);
int32_t       BSP_LED_On(Led_TypeDef Led);
int32_t       BSP_LED_Off(Led_TypeDef Led);
int32_t       BSP_LED_Toggle(Led_TypeDef Led);
int32_t       BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
uint32_t      BSP_PB_GetState(Button_TypeDef Button);
void          BSP_PB_IRQHandler(Button_TypeDef Button);
void          BSP_PB_Callback(Button_TypeDef Button);
#if defined(USE_NFCTAG) && defined(USE_ST25DV)
int32_t       BSP_GPO_Init( uint32_t it_mode );
int32_t       BSP_GPO_DeInit(void);
int32_t       BSP_GPO_ReadPin(void);
void          BSP_GPO_Callback(void);
void          BSP_GPO_IRQHandler(void);
void          BSP_LPD_Init( void );
GPIO_PinState BSP_LPD_ReadPin( void );
void          BSP_LPD_WritePin( GPIO_PinState LpdPinState );

#endif /* USE_NFCTAG && USE_ST25DV */

#if defined(USE_BSP_COM_FEATURE)
HAL_StatusTypeDef MX_USART6_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *Init);
int32_t           BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init);
int32_t           BSP_COM_DeInit(COM_TypeDef COM);
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t           BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t           BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
#endif /* USE_BSP_COM_FEATURE */

#if defined(HAL_ADC_MODULE_ENABLED) && defined(USE_ST25DV)
uint8_t       BSP_ST25DV_EH_Init( uint32_t *vdda);
void          BSP_ST25DV_EH_Measure(uint16_t *values);
void          BSP_ST25DV_EH_DeInit(void);

#endif /* HAL_ADC_MODULE_ENABLED) && defined(USE_ST25DV */

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

#endif /* ST25DX_DISCOVERY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
