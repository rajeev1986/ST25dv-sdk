/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : hal.c
* Author             : AMS - HEA&RF BU
* Version            : V1.0.0
* Date               : 04-Oct-2013
* Description        : Implementation of some APIs to access BlueNRG interface.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <hal.h> 
#include "platform_config.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_tim.h"
#include "hw_config.h"
#include "low_power.h"
#include "misc.h"
#include <hci.h>
#include "gp_timer.h"

#include "SDK_EVAL_Spi_Driver.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Constants
 *****************************************************************************/

/******************************************************************************
 * Types
 *****************************************************************************/

/******************************************************************************
 * Local Function Prototypes
 *****************************************************************************/

/******************************************************************************
 * Global Functions
 *****************************************************************************/

void Hal_Write_Serial(const void* data1, const void* data2, uint16_t n_bytes1, uint16_t n_bytes2)
{
    struct timer t;
            
	Timer_Set(&t, CLOCK_SECOND/10);
    
    while(1){
      if(BlueNRG_SPI_Write((uint8_t *)data1,(uint8_t *)data2, n_bytes1, n_bytes2)==0) break;
      if(Timer_Expired(&t)){
          break;
      }
    }
}



/****************************************************************************/


