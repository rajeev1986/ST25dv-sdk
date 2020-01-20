/**
  ******************************************************************************
  * @file    main.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @ingroup ST25_Discovery_Demo
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
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
#include "main.h"
#include "Menu_config.h"
#include "Menu_definition.h"
#include "st25dx_discovery_conf.h"
#include "st25dx_discovery_lcd.h"
#include "st25dx_discovery_ts.h"
#include "st25dx_discovery_nfctag.h"
#include "st25dx_discovery_dpot.h"
#include "commonfunc.h"
//#include "st_logo_jpg.h"
#include "../../../Utilities/Fonts/font24.c"
#include "../../../Utilities/Fonts/font22.c"

/** @mainpage
  * ST25DV-Discovery firmware documentation
  * ==================================================================================
  * This document describes the ST25 Discovery Firmware for the ST25DV demo.
  * @page Introduction
  * @section Firmware_objectives Firmware objectives
  * The purpose of this firmware is to run demos on the ST25DV dynamic NFC tag.
  * @subsection ST25DV_Dyn_Tag The ST25DV dynamic NFC tag
  * The ST25DV is a dynamic NFC tag, thus providing 2 different interfaces:
  * - The NFC RF interface, compliant with standards:
  *   - NFC Forum Type 5 Tag
  *   - ISO15693
  * - An i2c interface, controlled by a MCU (executing this firmware).
  *
  * This tag also supports a Fast Transfer mode to enable faster communication between the MCU & the RF world.
  * @section About_the_hardware About the hardware
  * This Firmware runs on the ST25DV-Discovery Kit which includes 2 different boards:
  * - The ST25 Discovery Motherboard: embedding a STM32F415 MCU, on which this firmware is intended to run.
  * - The ST25DV Ant boards: having the ST25DV tag and a NFC Antenna.  
  * @subsection ST25_Discovery_board ST25 Discovery board
  * The ST25 Discovery board is built around a STM32F415 MCU (running this firmware).
  *
  * The board embeds also:
  * - a LCD display with a touchscreen, to display and control the demo.
  * - 2 USB connectors, one for the ST-Link, the second being available for any user application.
  * - a connector for a Tag Antenna daughter board (such as one of the ST25DV Antenna boards)
  * - And as optionnal modules:
  *   - a ST Bluetooth Low Energy module.
  *   - a ST Wifi module.
  * @subsection ST25DV_Ant_board ST25DV Antenna boards
  * These boards are built around the ST25DV dynamic NFC tag and different NFC antennas.
  * @section Demo_overview Demo overview
  * The firmware implements 3 demo categories:
  * - The Fast Transfer Mode demos, with different usecases benefiting from the faster communication between the MCU & a Reader:
  *   - FW upgrade.
  *   - Picture download/upload.
  *   - Data transfers (from or to the reader).
  * - The NDEF mesages demos:
  *   - NFC well-known types: URL, Phone number, SMS, Email,...
  *   - vCard
  *   - Android Application Record
  *   - Proprietary record (MyAPP)
  *   - Pairing with Out Of Band records for Bluetooth LE & Wifi
  * - Demo of other features of the ST25DV:
  *   - RF interrupt through a dedicated GPO
  *   - Energy Harvesting from the RF to power an additional device
  *   - Different states: RF off, RF sleep and Low power down
  *   - Memory mappping and password protection
  *
  * The firmware implements also a user interface consisting in:
  * - a menu to select the demos
  * - several screens to display demo instructions and status.
  * @section Cube_Methodology STM32Cube methodology
  * This firmware has been designed to specifically runs on the STM32F415 MCU embedded on the ST25 Discovery board.
  * However, thanks to the STM32Cube methodology, the structure of the firmware enables both an easy full porting to an other STM32 MCU and an easy reuse of parts of the demo.
  * Two important elements have been particularly designed to make their reuse as straightforward as possible:
  * - The ST25DV driver, which implements the functions to control the ST25DV from its i2c interface,  is completely independant from the MCU.\n
  *   This driver can thus be easily reused in any other project based on the STM32Cube methodology, but can also be reused on any other HW solution interacting with a ST25DV.
  * - The NDEF library, which implements the standard NDEF protocol, is provided as a Cube Middleware fully independant from the HW.
  *
  * Here is a high level view of how the firmware is organized.
  * @image latex db_schema.eps  "ST25DV demo firmware overview."
  * @section Documentation_Structure Firmware documentation structure
  * This document has been generated from the Doxygen comments embedded in the firmware source code.\n
  * The structure of this document follows the firmware structure, and uses the standard doxygen implementation based on Modules.\n
  * @subsection Main_Modules Main Demo modules
  * - @ref ST25_Discovery_Demo : @copybrief  ST25_Discovery_Demo
  * - @ref Fast_Transfer_Mode : @copybrief Fast_Transfer_Mode
  * - @ref NDEF_DEMO : @copybrief NDEF_DEMO
  * - @ref ST25DV_Features_Demo : @copybrief ST25DV_Features_Demo
  * @subsection FTM_Modules Fast transfer mode demo related modules
  * - @ref ST25DV_Mailbox_Functions : @copybrief ST25DV_Mailbox_Functions
  * - @ref Flash_Command : @copybrief Flash_Command
  * - @ref Flash_Interface : @copybrief Flash_Interface
  * @subsection ST25DV_Management ST25DV management related module
  * - @ref ST25DV_Common_Functions : @copybrief ST25DV_Common_Functions
  * @subsection BoardSupportPackageDriver ST25DV Board Support Package and Driver
  * - @ref BSP : @copybrief BSP
  * - @ref ST25_DISCOVERY_NFCTAG : @copybrief ST25_DISCOVERY_NFCTAG
  * - @ref ST25DV : @copybrief ST25DV
  * @subsection Menu_Of_The_Demo Menu of the demo
  * - @ref ST25DV_Menu_Definition : @copybrief ST25DV_Menu_Definition
  * - @ref Menu_Demo_Interface : @copybrief Menu_Demo_Interface
  * - @ref Jpeg_Decode : @copybrief Jpeg_Decode
  * @subsection MCU_Support_Modules MCU support modules
  * - @ref ST25Discovery_IT : @copybrief ST25Discovery_IT
  * - @ref ST25Discovery_MSP : @copybrief ST25Discovery_MSP
  *
  * @note Doxygen non-default settings to re-generate this document:
  * - OPTIMIZE_OUTPUT_FOR_C=YES
  * - INLINE_SIMPLE_STRUCT=YES
  * - MAX_INITIALIZER_LINES=1
  * - HIDE_UNDOC_MEMBERS=1
  *
  * @section Middlewares_used Middlewares used in this firmware
  *   This firmware relies on several Middlewares, either provided by ST or a third party.
  *   These Middlewares are HW-independant softwares implementing a generic feature.
  *   They will not be detailed in this documentation.\n
  *   Herebelow is the list of the Middlewares used by this firmware.
  *   @subsection LibNDEF LibNDEF
  *     This library provides functions to read & write NDEF messages to a tag.\n
  *     It supports a variety of NDEF records, such as:
  *     - URI record (includes URLs)
  *     - SMS record
  *     - Email record
  *     - vCard record
  *     - Geo location record
  *     - Bluetooth OOB record
  *     - Wifi OOB record
  *     - Android Application Record (AAR)
  *
  *     The library also defines a NFC-Forum Type5 Tag wrapper to comply with the NFC-Forum Type5 Tag specification.
  *   @subsection LibJPEG LibJPEG
  *     This library implements the JPEG codec. This firmware only includes the JPEG decoding part of the library.
  *   @subsection STM32_BlueNRG STM32 BlueNRG
  *     This Middleware provides the communication stack for the ST BlueNRG module (Bluetooth Low Energy).\n
  *     A HID profile is used on top of it, to remotely control a mouse pointer on a paired device.
  *   @subsection STM32_SPWF01SA STM32 SPWF01SA
  *     This Middleware provides the communication stack for the ST SPWF01 Wifi module.\n
  *     It is used to set the Wifi Module as a mini Access Point, to receive connections from remote devices.
  *   @subsection Menu_Demo Menu Demo
  *     This Middleware implements functions to display an icon-and-text-based menu.\n
  *     It also manages inputs from a touchscreen, a joystick and a button, to interact with the user.  
  *
  * @section ST25DV_BSP ST25DV Board Support Package
  *   The Board Support Package software (BSP) is defined by the Cube methodology as the abstraction layer for the board specific features.\n
  *   It implements all the functions required to access:
  *     - the components on the board.
  *     - the MCU peripherals requiring a board specific configuration.
  *
  *   The different parts of the BSP are described below.
  *   @subsection IOBus IOBus
  *     This part of the BSP implements the low level functions interfacing between the components drivers and the MCU peripherals (by caling the Cube HAL).\n
  *     The BSP IOBus functions are not detailed in this document.
  *   @subsection High_Level_API High level APIs
  *     This parts of the BSP provides high level functions called by the application or Middlewares to access the component drivers.\n
  *     This layer acts as a bridge between the application and the component drivers.\n
  *     In this documentation, only the NFCTAG BSP is detailed: @ref ST25_DISCOVERY_NFCTAG.
  *
  * @section ST25DV_Components Components
  *   The ST25DV Discovery kit embeds different components requiring specific softwares to be correctly driven.
  *   Unless it is specified differently, these drivers are not detailed in this document.
  *   @subsection ST25DV_DRIVER ST25DV driver
  *     This driver provides all the required functions to access the ST25DV NFC dynamic tag.
  *     This driver is covered by this document in the module @ref ST25DV.
  *   @subsection ILI9341_DRIVER ILI9341 driver
  *     This driver implements functions to access the LCD display of the ST25-Discovery.
  *   @subsection STMPE811_DRIVER STMPE811 driver
  *     This driver implements functions to access the Touchscreen of the ST25-Discovery.
  *   @subsection AD5161_DRIVER AD5161 driver
  *     This driver implements functions to access the Digital Potentiometer of the ST25-Discovery.
  */


/* Private defines -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* Public variables ---------------------------------------------------------*/


/** @defgroup ST25_Discovery_Demo ST25 Discovery Demo
  * @brief    This module is the main module for all the ST25Discovery board demos.
  * @details  Demos are divided in 3 different sub-modules:
  *           - Fast transfer mode demos
  *           - ST25DV features demos
  *           - NDEF demos
  * @{
  */

/* Public functions -----------------------------------------------------------*/
/** @brief Demo entry point. */
int main(void)

{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

	/* Configure the GPIOs */
  MX_GPIO_Init();
	
	HAL_Delay(50);
  
	BSP_TS_Init(240,320);
  BSP_LCD_Init();
  BSP_LCD_SetFont( &Font22 );
  
  BSP_DPOT_Init();
  BSP_DPOT_SetResistor(0xff);

  SplashScreen( );
  
    ST25_RETRY(BSP_NFCTAG_Init(BSP_NFCTAG_INSTANCE));

  Menu_Start();
	
  while(1);
}

/** @brief System Clock Configuration.
  * @details   The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}



/** @brief Configure the GPIO pins.
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10|GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC14 PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PE7 PE8 PE9 PE10 
                           PE11 PE12 PE14 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10 
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PD10 PD0 PD1 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/**
  * @brief  Display splash screen.
  */
void SplashScreen( void )
{
  BSP_LCD_Clear( LCD_COLOR_BLUEST );
  Menu_SetStyle( CLEAR_PLAIN );
  
  Menu_DisplayString( Line1, "ST25DV Demonstration" );
  Menu_DisplayString( Line2, "    Version "MCU_FW_VERSION"   " );
  Menu_DisplayString( Line4, " STMicroelectronics " );
  Menu_DisplayString( Line5, "   www.st.com/st25  " );
  Menu_DisplayString( Line7, "      MMY Team      " );
  Menu_DisplayString( Line8, "   COPYRIGHT 2019   " );
  /* Delay reduced, as Wifi module detection takes 2s */
  HAL_Delay( 1000 );
}

/**
  * @brief Display "_about_" screen.
  */
void MenuAbout( void )
{
  char *msg = "\n\nST25DV Demonstration\n\n\nVersion "MCU_FW_VERSION"\n\n\f\nSTMicroelectronics\n\nwww.st.com/st25\n\nMMY Team\nCOPYRIGHT 2019\n\n";
  Menu_MsgStatus( "Credits :", msg, MSG_ABOUT);
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 


/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
