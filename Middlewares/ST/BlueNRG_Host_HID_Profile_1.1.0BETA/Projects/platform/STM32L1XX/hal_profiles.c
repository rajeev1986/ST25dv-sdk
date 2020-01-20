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
#include <timer.h>

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


void Hal_Init_Timer()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
	TIM_TimeBaseInitStruct.TIM_Prescaler = TIM_PRESCALER;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStruct.TIM_Pulse = 0xFFFF;
	TIM_OCInitStruct.TIM_OCPolarity = 0;
    
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
    
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);    
}

void Hal_Disable_Tim_Int()
{
    IRQn_Type irq = TIM2_IRQn;
    
    /* Disable the Selected IRQ Channels*/
    NVIC->ICER[irq >> 0x05] =
        (uint32_t)0x01 << (irq & (uint8_t)0x1F);      
}

void Hal_Enable_Tim_Int()
{
    IRQn_Type irq = TIM2_IRQn;
    
    /* Enable the Selected IRQ Channels*/
    NVIC->ISER[irq >> 0x05] =
        (uint32_t)0x01 << (irq & (uint8_t)0x1F);  
}

uint32_t Hal_Get_Timer_Value()
{    
    return TIM_GetCounter(TIM2);
}

void Hal_Start_Timer(uint32_t timeout)
{
    TIM_SetCompare1(TIM2, timeout);
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
}

void Hal_Stop_Timer()
{
    TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM2,TIM_FLAG_CC1)){
        TIM_ClearFlag(TIM2, TIM_FLAG_CC1);
        TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
        Timer_ExpiryCallback();
    }
}



/****************************************************************************/


