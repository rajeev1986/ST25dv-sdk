/**
 * @file    SDK_EVAL_EEPROM.h
 * @author  AMS - AAS Division
 * @version V1.0.1
 * @date    March 10, 2014
 * @brief   SDK EVAL eeprom management
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EEPROM_H_
#define __SDK_EEPROM_H_


/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

#ifdef __cplusplus
  "C" {
#endif


/**
 * @addtogroup SDK_EVAL_STM32L
 * @{
 */


/**
 * @defgroup SDK_EEPROM
 * @{
 */
   
/**
* @brief BlueNRG Eval Board (STEVAL-IDB002V1) & USB Dongle: 
 *       common SPI and GPIO lines x EEPROM
 */
#define EEPROM_V3_SPI_PERIPH_NB                  SPI2
#define EEPROM_V3_SPI_PERIPH_RCC                 RCC_APB1Periph_SPI2
  
/* Defines for MOSI pin */
#define EEPROM_V3_SPI_PERIPH_MOSI_PORT           GPIOB
#define EEPROM_V3_SPI_PERIPH_MOSI_PIN            GPIO_Pin_15
#define EEPROM_V3_SPI_PERIPH_MOSI_AF             GPIO_AF_SPI2
#define EEPROM_V3_SPI_PERIPH_MOSI_RCC            RCC_AHBPeriph_GPIOB
#define EEPROM_V3_SPI_PERIPH_MOSI_RCC_SOURCE     GPIO_PinSource15

/* Defines for MISO pin*/
#define EEPROM_V3_SPI_PERIPH_MISO_PORT           GPIOB
#define EEPROM_V3_SPI_PERIPH_MISO_PIN            GPIO_Pin_14
#define EEPROM_V3_SPI_PERIPH_MISO_AF             GPIO_AF_SPI2
#define EEPROM_V3_SPI_PERIPH_MISO_RCC            RCC_AHBPeriph_GPIOB
#define EEPROM_V3_SPI_PERIPH_MISO_RCC_SOURCE     GPIO_PinSource14

/* Defines for SCLK pin */
#define EEPROM_V3_SPI_PERIPH_SCLK_PORT           GPIOB
#define EEPROM_V3_SPI_PERIPH_SCLK_PIN            GPIO_Pin_13
#define EEPROM_V3_SPI_PERIPH_SCLK_AF             GPIO_AF_SPI2
#define EEPROM_V3_SPI_PERIPH_SCLK_RCC            RCC_AHBPeriph_GPIOB
#define EEPROM_V3_SPI_PERIPH_SCLK_RCC_SOURCE     GPIO_PinSource13

/* Defines for chip select pin x BlueNRG Eval Board (STEVAL-IDB002V1) */ 
#define EEPROM_V3_SPI_PERIPH_CS_PORT             GPIOC
#define EEPROM_V3_SPI_PERIPH_CS_PIN              GPIO_Pin_12
#define EEPROM_V3_SPI_PERIPH_CS_RCC              RCC_AHBPeriph_GPIOC
        
/* Defines for chip select pin x USB dongle */ 
#define EEPROM_VD1_SPI_PERIPH_CS_PORT            GPIOA
#define EEPROM_VD1_SPI_PERIPH_CS_PIN             GPIO_Pin_9
#define EEPROM_VD1_SPI_PERIPH_CS_RCC             RCC_AHBPeriph_GPIOA

/* EEPROM SPI commands */
#define EEPROM_CMD_WREN    0x06    // Write Enable
#define EEPROM_CMD_WRDI    0x04    // Write Disable
#define EEPROM_CMD_RDSR    0x05    // Read Status Register
#define EEPROM_CMD_WRSR    0x01    // Write Status Register
#define EEPROM_CMD_READ    0x03    // Read from Memory Array
#define EEPROM_CMD_WRITE   0x02    // Write to Memory Array   

/* EEPROM SPI status */
#define EEPROM_STATUS_SRWD    0x80       // Status Register Write Disable
#define EEPROM_STATUS_BP      0x0C       // Block Protect
#define EEPROM_STATUS_WEL     0x02       // Write Enable   
#define EEPROM_STATUS_WIP     0x01       // Write in Progress

/** @defgroup SDK_EVAL_EEPROM_Exported_Functions                   SDK EVAL EEPROM Exported Functions
 * @{
 */
int8_t EepromSpiInitialization(void);
void EepromCsPinInitialization(void);
uint8_t EepromStatus(void);
void EepromRead(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
void EepromWrite(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
uint8_t EepromResetSrwd(void);
uint8_t EepromSetSrwd(void);

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

