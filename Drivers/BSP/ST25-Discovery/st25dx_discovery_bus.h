/**
  ******************************************************************************
  * @file    st25dx_discovery_bus.h 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   header file for the BSP BUS IO driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ST25DX_DISCOVERY_BUS_H
#define __ST25DX_DISCOVERY_BUS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "st25dx_discovery_conf.h"
#include "st25dx_discovery_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup ST25DX_DISCOVERY_BSP
  * @{
  */

/** @addtogroup ST25DX_DISCOVERY_LOW_LEVEL
  * @{
  */

/*############################### I2Cx #######################################*/
/* User can use this section to tailor I2C1 instance used and associated 
   resources */
/*********************** TSCR I2C ***********************/
#if defined(HAL_I2C_MODULE_ENABLED)
#define BUS_TS_I2C_ADDRESS                0x82

#define BUS_I2C1                          I2C1
#define BUS_I2C1_CLOCK_ENABLE()           __HAL_RCC_I2C1_CLK_ENABLE()
#define BUS_I2C1_CLOCK_DISABLE()          __HAL_RCC_I2C1_CLK_DISABLE()
#define BUS_I2C1_FORCE_RESET()            __HAL_RCC_I2C1_FORCE_RESET()
#define BUS_I2C1_RELEASE_RESET()          __HAL_RCC_I2C1_RELEASE_RESET()
#define BUS_I2C1_SDA_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C1_SCL_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C1_SDA_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUS_I2C1_SCL_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()

/* Definition for DISCO I2C1 Pins */
#define BUS_I2C1_SCL_GPIO_PIN             GPIO_PIN_6
#define BUS_I2C1_SCL_GPIO_PORT            GPIOB
#define BUS_I2C1_SCL_GPIO_AF              GPIO_AF4_I2C1
#define BUS_I2C1_SDA_GPIO_PIN             GPIO_PIN_7
#define BUS_I2C1_SDA_GPIO_PORT            GPIOB
#define BUS_I2C1_SDA_GPIO_AF              GPIO_AF4_I2C1

/* Definition for TS I2C's NVIC */
#define BUS_I2C1_EV_IRQn                  I2C1_EV_IRQn
#define BUS_I2C1_ER_IRQn                  I2C1_ER_IRQn

#ifndef BUS_I2C1_FREQUENCY
#define BUS_I2C1_FREQUENCY                100000U /* Frequency of I2C1 = 100 KHz*/
#endif

#ifndef BUS_I2C1_POLL_TIMEOUT
#define BUS_I2C1_POLL_TIMEOUT             0x1000U
#endif /* BUS_I2C1_POLL_TIMEOUT */

/*********************** NFCTAG I2C ***********************/
#define BUS_I2C2                          I2C2
#define BUS_I2C2_CLOCK_ENABLE()           __HAL_RCC_I2C2_CLK_ENABLE()
#define BUS_I2C2_CLOCK_DISABLE()          __HAL_RCC_I2C2_CLK_DISABLE()
#define BUS_I2C2_FORCE_RESET()            __HAL_RCC_I2C2_FORCE_RESET()
#define BUS_I2C2_RELEASE_RESET()          __HAL_RCC_I2C2_RELEASE_RESET()
#define BUS_I2C2_SDA_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C2_SCL_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C2_SDA_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUS_I2C2_SCL_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()

/* Definition for DISCO I2C2 Pins */
#define BUS_I2C2_SCL_GPIO_PIN             GPIO_PIN_10
#define BUS_I2C2_SCL_GPIO_PORT            GPIOB
#define BUS_I2C2_SCL_GPIO_AF              GPIO_AF4_I2C2
#define BUS_I2C2_SDA_GPIO_PIN             GPIO_PIN_11
#define BUS_I2C2_SDA_GPIO_PORT            GPIOB
#define BUS_I2C2_SDA_GPIO_AF              GPIO_AF4_I2C2

#ifndef BUS_I2C2_FREQUENCY
#define BUS_I2C2_FREQUENCY                400000U /* Frequency of I2C2 = 400 KHz*/
#endif /* BUS_I2C2_FREQUENCY */

#ifndef BUS_I2C2_POLL_TIMEOUT
#define BUS_I2C2_POLL_TIMEOUT             0x1000U
#endif /* BUS_I2C2_POLL_TIMEOUT */

/*********************** DPOT I2C ***********************/
#define BUS_I2C3                          I2C3
#define BUS_I2C3_CLOCK_ENABLE()           __HAL_RCC_I2C3_CLK_ENABLE()
#define BUS_I2C3_CLOCK_DISABLE()          __HAL_RCC_I2C3_CLK_DISABLE()
#define BUS_I2C3_FORCE_RESET()            __HAL_RCC_I2C3_FORCE_RESET()
#define BUS_I2C3_RELEASE_RESET()          __HAL_RCC_I2C3_RELEASE_RESET()
#define BUS_I2C3_SDA_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUS_I2C3_SCL_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_I2C3_SDA_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUS_I2C3_SCL_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOA_CLK_DISABLE()

/* Definition for DPOT DISCO I2Cx Pins */
#define BUS_I2C3_SCL_GPIO_PIN             GPIO_PIN_8
#define BUS_I2C3_SCL_GPIO_PORT            GPIOA
#define BUS_I2C3_SCL_GPIO_AF              GPIO_AF4_I2C3
#define BUS_I2C3_SDA_GPIO_PIN             GPIO_PIN_9
#define BUS_I2C3_SDA_GPIO_PORT            GPIOC
#define BUS_I2C3_SDA_GPIO_AF              GPIO_AF4_I2C3

/* Definition for DPOT I2C's NVIC */
#define BUS_I2C3_EV_IRQn                  I2C3_EV_IRQn
#define BUS_I2C3_ER_IRQn                  I2C3_ER_IRQn

#ifndef BUS_I2C3_FREQUENCY
#define BUS_I2C3_FREQUENCY                100000U /* Frequency of I2C3 = 100 KHz*/
#endif /* BUS_I2C2_FREQUENCY */

#ifndef BUS_I2C3_POLL_TIMEOUT
#define BUS_I2C3_POLL_TIMEOUT             0x64
#endif /* BUS_I2C2_POLL_TIMEOUT */

#endif /* HAL_I2C_MODULE_ENABLED */
/*############################### SPIx #######################################*/
#if defined(HAL_SPI_MODULE_ENABLED)
#define BUS_SPI1                          SPI1
#define BUS_SPI1_CLK_ENABLE()             __HAL_RCC_SPI1_CLK_ENABLE()
#define BUS_SPI1_CLK_DISABLE()            __HAL_RCC_SPI1_CLK_DISABLE()
#define BUS_SPI1_SCK_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_NSS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_SCK_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_SPI1_NSS_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_SPI1_MISO_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_MOSI_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_MISO_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_SPI1_MOSI_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_SPI1_SCK_GPIO_PIN             GPIO_PIN_5
#define BUS_SPI1_NSS_GPIO_PIN             GPIO_PIN_4
#define BUS_SPI1_MISO_GPIO_PIN            GPIO_PIN_6
#define BUS_SPI1_MOSI_GPIO_PIN            GPIO_PIN_7
#define BUS_SPI1_MISO_GPIO_PORT           GPIOA
#define BUS_SPI1_MOSI_GPIO_PORT           GPIOA
#define BUS_SPI1_SCK_GPIO_PORT            GPIOA
#define BUS_SPI1_NSS_GPIO_PORT            GPIOA
#define BUS_SPI1_SCK_GPIO_AF              GPIO_AF5_SPI1
#define BUS_SPI1_MISO_GPIO_AF             GPIO_AF5_SPI1
#define BUS_SPI1_MOSI_GPIO_AF             GPIO_AF5_SPI1

#ifndef BUS_SPI1_BAUDRATE
#define BUS_SPI1_BAUDRATE                 10000000U /* baud rate of SPI1 = 10 Mbps */
#endif

#ifndef BUS_SPI1_DATASIZE
#define BUS_SPI1_DATASIZE                 SPI_DATASIZE_8BIT /* default data size 8 bit */
#endif

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#ifndef BUS_SPI1_POLL_TIMEOUT
   #define BUS_SPI1_POLL_TIMEOUT          ((uint32_t)0x1000)
#endif /* BUS_SPI1_POLL_TIMEOUT */

#define BUS_SPI2                          SPI2
#define BUS_SPI2_CLK_ENABLE()             __HAL_RCC_SPI2_CLK_ENABLE()
#define BUS_SPI2_CLK_DISABLE()            __HAL_RCC_SPI2_CLK_DISABLE()
#define BUS_SPI2_SCK_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_SPI2_NSS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_SPI2_SCK_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUS_SPI2_NSS_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUS_SPI2_MISO_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUS_SPI2_MOSI_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUS_SPI2_MISO_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUS_SPI2_MOSI_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUS_SPI2_SCK_GPIO_PIN             GPIO_PIN_13
#define BUS_SPI2_NSS_GPIO_PIN             GPIO_PIN_12
#define BUS_SPI2_MISO_GPIO_PIN            GPIO_PIN_2
#define BUS_SPI2_MOSI_GPIO_PIN            GPIO_PIN_3
#define BUS_SPI2_MISO_GPIO_PORT           GPIOC
#define BUS_SPI2_MOSI_GPIO_PORT           GPIOC
#define BUS_SPI2_SCK_GPIO_PORT            GPIOB
#define BUS_SPI2_NSS_GPIO_PORT            GPIOB
#define BUS_SPI2_SCK_GPIO_AF              GPIO_AF5_SPI2
#define BUS_SPI2_MISO_GPIO_AF             GPIO_AF5_SPI2
#define BUS_SPI2_MOSI_GPIO_AF             GPIO_AF5_SPI2

#ifndef BUS_SPI2_BAUDRATE
#define BUS_SPI2_BAUDRATE                 10000000U /* baud rate of SPI2 = 10 Mbps*/
#endif

#define BUS_SPI2_LOWBAUDRATE              6660000U /* baud rate of SPI2 = 6.66 Mbps*/
#define BUS_SPI2_HIGHBAUDRATE             10000000U /* baud rate of SPI2 = 10 Mbps*/

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#ifndef BUS_SPI2_POLL_TIMEOUT
   #define BUS_SPI2_POLL_TIMEOUT          ((uint32_t)0x1000)
#endif /* BUS_SPI2_POLL_TIMEOUT */

#define BUS_SPI3                          SPI3
#define BUS_SPI3_CLK_ENABLE()             __HAL_RCC_SPI3_CLK_ENABLE()
#define BUS_SPI3_CLK_DISABLE()            __HAL_RCC_SPI3_CLK_DISABLE()
#define BUS_SPI3_SCK_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUS_SPI3_NSS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()
#define BUS_SPI3_SCK_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUS_SPI3_NSS_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOD_CLK_DISABLE()
#define BUS_SPI3_MISO_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUS_SPI3_MOSI_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUS_SPI3_MISO_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUS_SPI3_MOSI_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUS_SPI3_SCK_GPIO_PIN             GPIO_PIN_10
#define BUS_SPI3_NSS_GPIO_PIN             GPIO_PIN_0
#define BUS_SPI3_MISO_GPIO_PIN            GPIO_PIN_11
#define BUS_SPI3_MOSI_GPIO_PIN            GPIO_PIN_12
#define BUS_SPI3_MISO_GPIO_PORT           GPIOC
#define BUS_SPI3_MOSI_GPIO_PORT           GPIOC
#define BUS_SPI3_SCK_GPIO_PORT            GPIOC
#define BUS_SPI3_NSS_GPIO_PORT            GPIOD
#define BUS_SPI3_SCK_GPIO_AF              GPIO_AF6_SPI3
#define BUS_SPI3_MISO_GPIO_AF             GPIO_AF6_SPI3
#define BUS_SPI3_MOSI_GPIO_AF             GPIO_AF6_SPI3

#define BUS_SPI3_IRQn                     EXTI2_IRQn

#ifndef BUS_SPI3_BAUDRATE
#define BUS_SPI3_BAUDRATE                 10000000U /* baud rate of SPI1 = 10 Mbps */
#endif

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#ifndef BUS_SPI3_POLL_TIMEOUT
#define BUS_SPI3_POLL_TIMEOUT             15U
#endif /* BUS_SPI3_POLL_TIMEOUT */

#endif /* HAL_SPI_MODULE_ENABLED */

/*############################### USARTx #######################################*/
#if defined(HAL_USART_MODULE_ENABLED)
#define BUS_USART2                        USART2
#define BUS_USART2_CLK_ENABLE()           __HAL_RCC_USART2_CLK_ENABLE()
#define BUS_USART2_CLK_DISABLE()          __HAL_RCC_USART2_CLK_DISABLE()
#define BUS_USART2_TX_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_USART2_RX_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_USART2_RTS_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_USART2_CTS_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_USART2_TX_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_USART2_RX_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_USART2_RTS_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_USART2_CTS_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_USART2_TX_GPIO_PIN            GPIO_PIN_2
#define BUS_USART2_RX_GPIO_PIN            GPIO_PIN_3
#define BUS_USART2_RTS_GPIO_PIN           GPIO_PIN_1
#define BUS_USART2_CTS_GPIO_PIN           GPIO_PIN_0
#define BUS_USART2_TX_GPIO_PORT           GPIOA
#define BUS_USART2_RX_GPIO_PORT           GPIOA
#define BUS_USART2_RTS_GPIO_PORT          GPIOA
#define BUS_USART2_CTS_GPIO_PORT          GPIOA
#define BUS_USART2_TX_GPIO_AF             GPIO_AF7_USART2
#define BUS_USART2_RX_GPIO_AF             GPIO_AF7_USART2
#define BUS_USART2_RTS_GPIO_AF            GPIO_AF7_USART2
#define BUS_USART2_CTS_GPIO_AF            GPIO_AF7_USART2

#ifndef BUS_USART2_BAUDRATE
#define BUS_USART2_BAUDRATE               115200U
#endif

#ifndef BUS_USART2_POLL_TIMEOUT
#define BUS_USART2_POLL_TIMEOUT           ((uint32_t)0x1000)
#endif /* BUS_USART2_POLL_TIMEOUT */

#define BUS_USART3                        USART3
#define BUS_USART3_CLK_ENABLE()           __HAL_RCC_USART3_CLK_ENABLE()
#define BUS_USART3_CLK_DISABLE()          __HAL_RCC_USART3_CLK_DISABLE()
#define BUS_USART3_TX_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define BUS_USART3_RX_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define BUS_USART3_RTS_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_USART3_CTS_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOD_CLK_ENABLE()
#define BUS_USART3_TX_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOD_CLK_DISABLE()
#define BUS_USART3_RX_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOD_CLK_DISABLE()
#define BUS_USART3_RTS_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUS_USART3_CTS_GPIO_CLK_DISABLE() __HAL_RCC_GPIOD_CLK_DISABLE()
#define BUS_USART3_TX_GPIO_PIN            GPIO_PIN_8
#define BUS_USART3_RX_GPIO_PIN            GPIO_PIN_9
#define BUS_USART3_RTS_GPIO_PIN           GPIO_PIN_14
#define BUS_USART3_CTS_GPIO_PIN           GPIO_PIN_11
#define BUS_USART3_TX_GPIO_PORT           GPIOD
#define BUS_USART3_RX_GPIO_PORT           GPIOD
#define BUS_USART3_RTS_GPIO_PORT          GPIOB
#define BUS_USART3_CTS_GPIO_PORT          GPIOD
#define BUS_USART3_TX_GPIO_AF             GPIO_AF7_USART3
#define BUS_USART3_RX_GPIO_AF             GPIO_AF7_USART3
#define BUS_USART3_RTS_GPIO_AF            GPIO_AF7_USART3
#define BUS_USART3_CTS_GPIO_AF            GPIO_AF7_USART3

#define BUS_USART3_IRQn                   USART3_IRQn
#define BSP_USART3_PRIORITY               3U

#ifndef BUS_USART3_BAUDRATE
#define BUS_USART3_BAUDRATE               115200U
#endif

#ifndef BUS_USART3_POLL_TIMEOUT
#define BUS_USART3_POLL_TIMEOUT           ((uint32_t)0x1000)
#endif /* BUS_USART3_POLL_TIMEOUT */

#endif /* HAL_USART_MODULE_ENABLED */

extern I2C_HandleTypeDef hbus_i2c1;
extern I2C_HandleTypeDef hbus_i2c2;
extern I2C_HandleTypeDef hbus_i2c3;

extern SPI_HandleTypeDef hbus_spi2;

extern UART_HandleTypeDef hbus_usart3;

HAL_StatusTypeDef MX_I2C1_Init(I2C_HandleTypeDef* hi2c);
int32_t BSP_I2C1_Init(void);
int32_t BSP_I2C1_DeInit(void);
int32_t BSP_I2C1_IsReady(uint16_t DevAddr, uint32_t Trials);
int32_t BSP_I2C1_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
int32_t BSP_I2C1_RegisterDefaultMspCallbacks(void);
int32_t BSP_I2C1_RegisterMspCallbacks (BSP_I2C_Cb_t *Callbacks);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

HAL_StatusTypeDef MX_I2C2_Init(I2C_HandleTypeDef* hi2c);
int32_t BSP_I2C2_Init(void);
int32_t BSP_I2C2_DeInit(void);
int32_t BSP_I2C2_IsReady(uint16_t DevAddr, uint32_t Trials);
int32_t BSP_I2C2_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
int32_t BSP_I2C2_RegisterDefaultMspCallbacks(void);
int32_t BSP_I2C2_RegisterMspCallbacks(BSP_I2C_Cb_t *Callbacks);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

HAL_StatusTypeDef MX_I2C3_Init(I2C_HandleTypeDef* hi2c);
int32_t BSP_I2C3_Init(void);
int32_t BSP_I2C3_DeInit(void);
int32_t BSP_I2C3_IsReady(uint16_t DevAddr, uint32_t Trials);
int32_t BSP_I2C3_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
int32_t BSP_I2C3_RegisterDefaultMspCallbacks(void);
int32_t BSP_I2C3_RegisterMspCallbacks(BSP_I2C_Cb_t *Callbacks);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

HAL_StatusTypeDef MX_SPI1_Init(SPI_HandleTypeDef *hspi, uint32_t baudrate_presc, uint32_t DataSize);
int32_t BSP_SPI1_Init(void);
int32_t BSP_SPI1_Send(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI1_Recv(uint8_t *pData, uint16_t Length);
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
int32_t BSP_SPI1_RegisterDefaultMspCallbacks(void);
int32_t BSP_SPI1_RegisterMspCallbacks(BSP_SPI_Cb_t *Callbacks);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

HAL_StatusTypeDef MX_SPI2_Init(SPI_HandleTypeDef *hspi, uint32_t baudrate_presc, uint32_t DataSize);
int32_t BSP_SPI2_Init(void);
void BSP_SPI2_SetDataWidth(uint32_t w16);
void BSP_SPI2_LowFreq(void);
void BSP_SPI2_HighFreq(void);
int32_t BSP_SPI2_Send(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI2_Recv(uint8_t *pData, uint16_t Length);
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
int32_t BSP_SPI2_RegisterDefaultMspCallbacks(void);
int32_t BSP_SPI2_RegisterMspCallbacks(BSP_SPI_Cb_t *Callbacks);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

HAL_StatusTypeDef MX_SPI3_Init(SPI_HandleTypeDef *hspi, uint32_t baudrate_presc, uint32_t DataSize);
int32_t BSP_SPI3_Init(void);
int32_t BSP_SPI3_Send(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI3_Recv(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI3_SendRecv(uint8_t *pTxdata, uint8_t *pRxdata, uint16_t Length);
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
int32_t BSP_SPI3_RegisterDefaultMspCallbacks(void);
int32_t BSP_SPI3_RegisterMspCallbacks(BSP_SPI_Cb_t *Callbacks);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

HAL_StatusTypeDef MX_USART2_Init(UART_HandleTypeDef *huart, uint32_t baud_rate);
int32_t BSP_USART2_Init(void);
int32_t BSP_USART2_DeInit(void);
int32_t BSP_USART2_Send(uint8_t *pData, uint16_t Length);
int32_t BSP_USART2_Recv(uint8_t *pData, uint16_t Length);
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_USART2_RegisterDefaultMspCallbacks(void);
int32_t BSP_USART2_RegisterMspCallbacks(BSP_SPI_Cb_t *Callbacks);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

HAL_StatusTypeDef MX_USART3_Init(UART_HandleTypeDef *huart, uint32_t baud_rate);
int32_t BSP_USART3_Init(void);
int32_t BSP_USART3_DeInit(void);
int32_t BSP_USART3_Send(uint8_t *pData, uint16_t Length);
int32_t BSP_USART3_Recv(uint8_t *pData, uint16_t Length);
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_USART3_RegisterDefaultMspCallbacks(void);
int32_t BSP_USART3_RegisterMspCallbacks(BSP_SPI_Cb_t *Callbacks);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

int32_t BSP_GetTick(void);
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

#endif /* ST25DX_DISCOVERY_BUS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
