/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Virtual Com Port Demo main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#ifdef STM32L1XX_MD
#include "stm32l1xx.h"
#else
#include "stm32f10x.h"
#endif /* STM32L1XX_MD */


/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "hal_types.h"
#include "hci.h"
#include "bluenrg_aci.h"
#include "gp_timer.h"
#include "hal.h"
#include "osal.h"
#include "gatt_server.h"
#include "hci_const.h"
#include "gap.h"
#include "sm.h"
#include <stdio.h>

#include "SDK_EVAL_Config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef VECTOR_TABLE_BASE_ADDRESS 
/* default configuration: DFU upgrade is supported */
#define VECTOR_TABLE_BASE_ADDRESS            (0x3000)
#endif
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*  User Function where serial received data should be processed */
void processInputData(uint8_t * rx_data, uint16_t data_size);

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

void Host_Profile_Test_Application (void);

int main(void)
{
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, VECTOR_TABLE_BASE_ADDRESS);
  
  /* Identify BlueNRG platform */
  SdkEvalIdentification();
  
  RCC_Configuration();
  
  /* Init I/O ports */
  Init_GPIOs ();
  
  PWR_PVDCmd(DISABLE);
  
  /* Disable FLASH during Sleep  */
  FLASH_SLEEPPowerDownCmd(ENABLE);
  
  /* Enable Ultra low power mode */
  PWR_UltraLowPowerCmd(ENABLE);
  
  PWR_FastWakeUpCmd(DISABLE);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  Clock_Init();
  
  /* Delay needed only to be able to acces the JTAG interface after reset
  if it will be disabled later. */
  Clock_Wait(500);    
  
  /* Configure I/O communication channel:
  It requires the void IO_Receive_Data(uint8_t * rx_data, uint16_t data_size) function
  where user received data should be processed */
  SdkEval_IO_Config(processInputData);
  
  /* Delay for debug purpose, in order to see printed data at startup. */
  for(int i = 0; i < 100 ; i++){
    printf(".");
    Clock_Wait(40);
  }
  
  HCI_Init();
  
  /* Init SPI interface */
  SdkEvalSpiInit(SPI_MODE_EXTI);
  BlueNRG_RST(); 
  
  Hal_Init_Timer();
  
  aci_hal_set_tx_power_level(1,4);
  
  Host_Profile_Test_Application();  
}
#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
