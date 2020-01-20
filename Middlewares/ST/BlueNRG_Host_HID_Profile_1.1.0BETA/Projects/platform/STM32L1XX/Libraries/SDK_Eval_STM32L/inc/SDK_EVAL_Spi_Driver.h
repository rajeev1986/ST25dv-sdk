/**
 * @file    SDK_EVAL_Spi_Driver.h
 * @author  AMS - AAS Division
 * @version V1.0.1
 * @date    March, 10 2014
 * @brief   This file contains definitions for Software Development Kit eval SPI devices
 * @details
 *
 * In this module there are API for the management of the SPI devices on the SDK Eval
 * motherboards.
 *
 * <b>Example:</b>
 * @code
 *
 *   SdkEvalSpiInit();
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
#ifndef __SDK_EVAL_DRIVER_H
#define __SDK_EVAL_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

#ifdef __cplusplus
 extern "C" {
#endif


/** @addtogroup SDK_EVAL_STM32L
* @{
*/


/** @addtogroup SDK_EVAL_Spi                    SDK EVAL Spi
* @brief SPI functions implementation.
* @details This file implements the BlueNRG Library SPI interface functions. 
* @{
*/

   /** @addtogroup SDK_EVAL_SPI_Exported_Types          SDK EVAL Spi Exported Types
 * @{
 */

/**
 * @brief  SPI Mode for SDK EVAL enumeration
 */
typedef enum
{
  SPI_MODE_GPIO = 0,
  SPI_MODE_EXTI = 1

} SdkEvalSpiMode;

/**
* @}
*/

/** @defgroup SPI_Private_Defines
* @{
*/


#define CS_TO_SCLK_DELAY  0x0001
#define CLK_TO_CS_DELAY   0x0001


/**
* @}
*/


/** @defgroup SDK_EVAL_Spi_Peripheral_Gpio
* @{
*/


/**
* @brief BlueNRG Eval Board (STEVAL-IDB002V1) & USB Dongle: 
 *       common SPI and GPIO lines
 */
#define SDK_EVAL_V3_SPI_PERIPH_NB              SPI2
#define SDK_EVAL_V3_SPI_PERIPH_RCC             RCC_APB1Periph_SPI2

/* Defines for MOSI pin */
#define SDK_EVAL_V3_SPI_PERIPH_MOSI_PORT       GPIOB
#define SDK_EVAL_V3_SPI_PERIPH_MOSI_PIN        GPIO_Pin_15 
#define SDK_EVAL_V3_SPI_PERIPH_MOSI_AF         GPIO_AF_SPI2
#define SDK_EVAL_V3_SPI_PERIPH_MOSI_RCC        RCC_AHBPeriph_GPIOB
#define SDK_EVAL_V3_SPI_PERIPH_MOSI_RCC_SOURCE GPIO_PinSource15

/* Defines for MISO pin*/
#define SDK_EVAL_V3_SPI_PERIPH_MISO_PORT       GPIOB
#define SDK_EVAL_V3_SPI_PERIPH_MISO_PIN        GPIO_Pin_14 
#define SDK_EVAL_V3_SPI_PERIPH_MISO_AF         GPIO_AF_SPI2
#define SDK_EVAL_V3_SPI_PERIPH_MISO_RCC        RCC_AHBPeriph_GPIOB
#define SDK_EVAL_V3_SPI_PERIPH_MISO_RCC_SOURCE GPIO_PinSource14

/* Defines for SCLK pin */
#define SDK_EVAL_V3_SPI_PERIPH_SCLK_PORT       GPIOB
#define SDK_EVAL_V3_SPI_PERIPH_SCLK_PIN        GPIO_Pin_13
#define SDK_EVAL_V3_SPI_PERIPH_SCLK_AF         GPIO_AF_SPI2
#define SDK_EVAL_V3_SPI_PERIPH_SCLK_RCC        RCC_AHBPeriph_GPIOB
#define SDK_EVAL_V3_SPI_PERIPH_SCLK_RCC_SOURCE GPIO_PinSource13

/* Defines for chip select pin */
#define SDK_EVAL_V3_SPI_PERIPH_CS_PORT         GPIOB
#define SDK_EVAL_V3_SPI_PERIPH_CS_PIN          GPIO_Pin_12
#define SDK_EVAL_V3_SPI_PERIPH_CS_RCC          RCC_AHBPeriph_GPIOB

/* BlueNRG reset pin */
#define SDK_EVAL_V3_SW_RST_GPIO_PORT           GPIOC
#define SDK_EVAL_V3_SW_RST_PIN                 GPIO_Pin_13      /* PC.13 */ 
#define SDK_EVAL_V3_SW_RST_GPIO_RCC            RCC_AHBPeriph_GPIOC


/* Defines for SPI IRQ pin */ 

/**
 * @brief BlueNRG Eval Board (STEVAL-IDB002V1) SPI IRQ line: EXTI15_10_IRQHandler() 
 */
#define SDK_EVAL_V3_SPI_PERIPH_IRQ_PORT        GPIOC
#define SDK_EVAL_V3_SPI_PERIPH_IRQ_PIN         GPIO_Pin_10
#define SDK_EVAL_V3_SPI_PERIPH_IRQ_RCC         RCC_AHBPeriph_GPIOC

#define	SDK_EVAL_V3_EXTI_LINE	               EXTI_Line10
#define	SDK_EVAL_V3_EXTI_PORT_SOURCE           EXTI_PortSourceGPIOC
#define	SDK_EVAL_V3_EXTI_PIN_SOURCE            EXTI_PinSource10
#define	SDK_EVAL_V3_EXTI_IRQn	               EXTI15_10_IRQn

/**
 * @brief USB Dongle SPI IRQ line: EXTI15_10_IRQHandler() 
 */
#define SDK_EVAL_VD1_SPI_PERIPH_IRQ_PORT       GPIOB
#define SDK_EVAL_VD1_SPI_PERIPH_IRQ_PIN        GPIO_Pin_10
#define SDK_EVAL_VD1_SPI_PERIPH_IRQ_RCC        RCC_AHBPeriph_GPIOB

#define	SDK_EVAL_VD1_EXTI_LINE	               EXTI_Line10
#define	SDK_EVAL_VD1_EXTI_PORT_SOURCE	       EXTI_PortSourceGPIOB
#define	SDK_EVAL_VD1_EXTI_PIN_SOURCE	       EXTI_PinSource10
#define	SDK_EVAL_VD1_EXTI_IRQn	               EXTI15_10_IRQn

/* BlueNRG Eval Board (STEVAL-IDB002V1) & USB Dongle STEVAL-IDB003V1 IRQ EXTI handler */
#ifndef SPI_IRQ_IRQHandler
#define	SPI_IRQ_IRQHandler	               EXTI15_10_IRQHandler 
#endif 

/* end EVAL_V3 */

/**
* @}
*/

/**
* @}
*/


/**
* @brief  Initializes the SPI for BlueNRG module
* @param  xSpiMode : Spi mode (Polling or IRQ)
* @retval Status
*/
int8_t SdkEvalSpiInit(SdkEvalSpiMode xSpiMode);
uint8_t SdkEvalSPI_Irq_Pin(void);

void Clear_SPI_EXTI_Flag();

void BlueNRG_RST(void);
void BlueNRG_HW_Bootloader(void);
int32_t BlueNRG_SPI_Read_All(uint8_t *buffer, uint8_t buff_size);
int32_t BlueNRG_SPI_Write(uint8_t* data1, uint8_t* data2, uint8_t Nb_bytes1, uint8_t Nb_bytes2);
void BlueNRG_Power_Down(void);
void BlueNRG_Power_Up(void);
uint8_t BlueNRG_DataPresent(void);

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
