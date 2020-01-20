/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

#include "stm32l1xx.h"  
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_conf.h"


#define STM32L15_USB_CONNECT                SYSCFG_USBPuCmd(ENABLE)
#define STM32L15_USB_DISCONNECT             SYSCFG_USBPuCmd(DISABLE)


#ifndef FALSE
#define FALSE 0
#define TRUE !FALSE
#endif

/* MACROs for SET, RESET or TOGGLE Output port */

#define GPIO_HIGH(a,b)          a->BSRRL = b
#define GPIO_LOW(a,b)           a->BSRRH = b
#define GPIO_TOGGLE(a,b)        a->ODR ^= b 

#define BUTTON_GPIO_PORT        GPIOA
#define USER_GPIO_PIN           GPIO_Pin_0


#define LD_PORT_1               GPIOC
#define LD_GREEN                GPIO_Pin_0
#define LD_ORANGE               GPIO_Pin_1
#define LD_BLUE                 GPIO_Pin_4
#define LD_YELLOW               GPIO_Pin_5

#define LD_PORT_2               GPIOD
#define LD_RED                  GPIO_Pin_2


#define SPI                           SPI2

#define SPI_CLK_APB1                  RCC_APB1Periph_SPI2

#define SPI_SCK_PIN                   GPIO_Pin_13                 /* PB.13 */
#define SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOB */
#define SPI_SCK_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define SPI_SCK_SOURCE                GPIO_PinSource13
#define SPI_SCK_AF                    GPIO_AF_SPI2
#define SPI_MISO_PIN                  GPIO_Pin_14                 /* PB.14 */
#define SPI_MISO_GPIO_PORT            GPIOB                       /* GPIOB */
#define SPI_MISO_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SPI_MISO_SOURCE               GPIO_PinSource14
#define SPI_MISO_AF                   GPIO_AF_SPI2
#define SPI_MOSI_PIN                  GPIO_Pin_15                 /* PB.15 */
#define SPI_MOSI_GPIO_PORT            GPIOB                       /* GPIOB */
#define SPI_MOSI_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SPI_MOSI_SOURCE               GPIO_PinSource15
#define SPI_MOSI_AF                   GPIO_AF_SPI2
#define SPI_CS_PIN                    GPIO_Pin_12                 /* PB.12 */
#define SPI_CS_GPIO_PORT              GPIOB                       /* GPIOB */
#define SPI_CS_GPIO_CLK               RCC_AHBPeriph_GPIOB


#ifdef USE_BLUENRG_USB_DONGLE

/* STEVAL-IDB003V1 platform: STM32L151CBT6 (40pins) + BlueNRG daughter board*/
#define SPI_IRQ_PIN                   GPIO_Pin_10                 /* PB.10 */
#define SPI_IRQ_EXTI_LINE             EXTI_Line10
#define SPI_IRQ_EXTI_PIN_SOURCE       EXTI_PinSource10
#define SPI_IRQ_GPIO_PORT             GPIOB                       /* GPIOB */
#define SPI_IRQ_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define SPI_IRQ_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOB

/* SW1 button */
#define	USER_BUTTON1_GPIO_PIN	      GPIO_Pin_2
#define	USER_BUTTON1_GPIO_PORT	      GPIOB

#else /* Default  platform: STEVAL-IDB002V1 */

#define SPI_IRQ_PIN                   GPIO_Pin_10                  /* PC.10 */
#define SPI_IRQ_EXTI_LINE             EXTI_Line10
#define SPI_IRQ_EXTI_PIN_SOURCE       EXTI_PinSource10
#define SPI_IRQ_GPIO_PORT             GPIOC                       /* GPIOC */
#define SPI_IRQ_GPIO_CLK              RCC_AHBPeriph_GPIOC
#define SPI_IRQ_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOC

/* Push_Button button */
#define	USER_BUTTON1_GPIO_PIN	      GPIO_Pin_6
#define	USER_BUTTON1_GPIO_PORT	      GPIOC

#endif /* end USE_BLUENRG_USB_DONGLE */

#define SPI_IRQ_EXTI_IRQn             EXTI15_10_IRQn
#define SPI_IRQ_IRQHandler            EXTI15_10_IRQHandler

#define SW_RST_PIN                    GPIO_Pin_13      /* PC.13 */
#define SW_RST_GPIO_PORT              GPIOC
#define SW_RST_GPIO_CLK               RCC_AHBPeriph_GPIOC

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
