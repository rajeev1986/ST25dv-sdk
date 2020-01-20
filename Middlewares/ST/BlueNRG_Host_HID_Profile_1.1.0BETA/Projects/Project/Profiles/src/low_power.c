/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : low_power.c
* Author             : AMS - HEA&RF BU
* Version            : V1.0.0
* Date               : 04-Oct-2013
* Description        : Functions for low power management
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifdef SYSCLK_MSI 

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "gp_timer.h"
#include "hw_config.h"
#include "platform_config.h"
#include "low_power.h"

#include "SDK_EVAL_Config.h"

/* Variables used for saving GPIO configuration */
uint32_t GPIOA_MODER, GPIOB_MODER, GPIOC_MODER,GPIOD_MODER,GPIOE_MODER ,GPIOE_MODER,GPIOH_MODER;
uint32_t GPIOA_PUPDR, GPIOB_PUPDR , GPIOC_PUPDR, GPIOD_PUPDR,GPIOE_PUPDR,GPIOH_PUPDR;

#define RUN     0
#define SLEEP   1
#define STOP    2

/* Define to be set to the timer used (TIM2 is used on hal_profiles.c) */
#define PROFILE_USER_TIMER TIM2 

volatile uint8_t LP_mode = RUN;

/*******************************************************************************
* Function Name  : GPIO_LowPower_Config
* Description    : Configures GPIO to reach the lowest power consumption.
* Input          : None.
* Return         : None.
*******************************************************************************/
void GPIO_LowPower_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIOA_MODER = GPIOA->MODER;
    GPIOB_MODER = GPIOB->MODER;
    GPIOC_MODER = GPIOC->MODER;
    GPIOD_MODER = GPIOD->MODER;
    GPIOE_MODER = GPIOE->MODER;
    GPIOH_MODER = GPIOH->MODER;
    
    GPIOA_PUPDR = GPIOA->PUPDR;
    GPIOB_PUPDR = GPIOB->PUPDR;
    GPIOC_PUPDR = GPIOC->PUPDR;
    GPIOD_PUPDR = GPIOD->PUPDR;
    GPIOE_PUPDR = GPIOE->PUPDR;
    GPIOH_PUPDR = GPIOH->PUPDR;
    
    /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    GPIOD->MODER   = 0xFFFFFFFF;
    GPIOE->MODER   = 0xFFFFFFFF;
    GPIOH->MODER   = 0xFFFFFFFF;
    
    /* Configure unused GPIO port pins in Analog Input mode (floating input trigger OFF) */
    
    /* ******* STEVAL-IDB002V1 ************ */
    /*  GPIOA */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_All;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    if (SdkEvalGetVersion() == SDK_EVAL_VERSION_3) // BlueNRG Eval board
    {
      /* PB12(CS),PB13(SCLK),PB14(MISO), PB15(MOSI) are for BlueNRG SPI. */
      GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 |  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6 | \
          GPIO_Pin_7 | GPIO_Pin_8 |GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      
      /* [PC0, PC1 ( LED1,2 on Eval board), PC4,PC5(LED D4, D5)], [PC6 (Push Button)], PC10 is for BlueNRG Eval IRQ. PC13 is BlueNRG RESET */
      GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 |  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6 | \
          GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9 | GPIO_Pin_11 |  GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      
      /* [PD2 (LED3)]*/
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6|  \
          GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |  GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14 | GPIO_Pin_15;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    else if (SdkEvalGetVersion() == SDK_EVAL_VERSION_D1) //BlueNRG USB dongle
    {
      /* [PB0, PB1 (LED2,3 on USB dongle)], [PB2 (SW1 button)], PB10 is for BlueNRG USB dongle IRQ, PB12(CS),PB13(SCLK),PB14(MISO),PB15(MOSI) are for BlueNRG SPI. */
      GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6 | \
          GPIO_Pin_7 | GPIO_Pin_8 |GPIO_Pin_9 | GPIO_Pin_11;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      
      /* PC13 is BlueNRG RESET */
      GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6|  \
          GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |  GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      
      /* GPIOD */
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    else if (SdkEvalGetVersion() == USER_PLATFORM_VERSION)
    {
      /* TO BE DONE based on user platform configuration platform */
    }
    
#if 0 //TBR
    SdkEvalLedOff(LED1);
    SdkEvalLedOff(LED2);
    
    if (SdkEvalGetVersion() == SDK_EVAL_VERSION_3) // BlueNRG Eval
    {
      SdkEvalLedOff(LED3);
      SdkEvalLedOff(LED4);
      SdkEvalLedOff(LED5);
    }
#endif 
}

/*******************************************************************************
* Function Name  : Restore_GPIO_Config
* Description    : Restore GPIO settings as before GPIO_LowPower_Config.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Restore_GPIO_Config(void)
{
    GPIOA->MODER = GPIOA_MODER;
    GPIOB->MODER = GPIOB_MODER;
    GPIOC->MODER = GPIOC_MODER;
    GPIOD->MODER = GPIOD_MODER;
    GPIOE->MODER = GPIOE_MODER;
    GPIOH->MODER = GPIOH_MODER;
    
    GPIOA->PUPDR = GPIOA_PUPDR;
    GPIOB->PUPDR = GPIOB_PUPDR;
    GPIOC->PUPDR = GPIOC_PUPDR;
    GPIOD->PUPDR = GPIOD_PUPDR;
    GPIOE->PUPDR = GPIOE_PUPDR;
    GPIOH->PUPDR = GPIOH_PUPDR;
}

/*******************************************************************************
* Function Name  : ChangeMSIClock
* Description    : Change MSI frequency
* Input          : freq: frequency range,
*                  div2: if FALSE, HCLK will be SYSCLK, otherwise SYSCLK/2.
* Return         : None.
*******************************************************************************/
void ChangeMSIClock(uint32_t freq, bool div2)
{ 
    /* Enable the PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    
    /* To configure the MSI frequency */
    RCC_MSIRangeConfig(freq);
    
    if (div2)
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div2);
    }
    else {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
    }
}

/*******************************************************************************
* Function Name  : User_Timer_Enter_Sleep
* Description    : Adjust timer prescaler when entering sleep mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void User_Timer_Enter_Sleep(void)
{
    uint16_t cnt;
    
    if(PROFILE_USER_TIMER->CR1 & TIM_CR1_CEN){
        TIM_ITConfig(PROFILE_USER_TIMER, TIM_IT_Update, DISABLE);
        PROFILE_USER_TIMER->PSC = TIM_PRESCALER_SLEEP; 
        cnt = PROFILE_USER_TIMER->CNT;
        PROFILE_USER_TIMER->EGR = TIM_PSCReloadMode_Immediate;
        PROFILE_USER_TIMER->CNT = cnt;
        TIM_ClearFlag(PROFILE_USER_TIMER, TIM_FLAG_Update);
        TIM_ITConfig(PROFILE_USER_TIMER, TIM_IT_Update, ENABLE);
    }
}

/*******************************************************************************
* Function Name  : User_Timer_Exit_Sleep
* Description    : Adjust timer prescaler when exiting sleep mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void User_Timer_Exit_Sleep(void)
{  
    uint16_t cnt;
    
    if(PROFILE_USER_TIMER->CR1 & TIM_CR1_CEN){
        TIM_ITConfig(PROFILE_USER_TIMER, TIM_IT_Update, DISABLE);
        PROFILE_USER_TIMER->PSC = TIM_PRESCALER; 
        cnt = PROFILE_USER_TIMER->CNT;
        PROFILE_USER_TIMER->EGR = TIM_PSCReloadMode_Immediate;
        PROFILE_USER_TIMER->CNT = cnt;
        TIM_ClearFlag(PROFILE_USER_TIMER, TIM_FLAG_Update);
        TIM_ITConfig(PROFILE_USER_TIMER, TIM_IT_Update, ENABLE);
    }    
}

/*******************************************************************************
* Function Name  : Enter_LP_Sleep_Mode
* Description    : Put the micro in low power sleep mode. This function assumes
*                   that the system clock is already MSI.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LP_Sleep_Mode()
{
    __disable_interrupt();
    
    LP_mode = SLEEP;
    
    GPIO_LowPower_Config();
    
    /* Disable PVD */
    PWR_PVDCmd(DISABLE);
    
    /* Stop the sys tick to avoid interrupts */
    SysTick->CTRL = 0;
    
    /* Switch in MSI 65KHz, HCLK 32kHz */
    ChangeMSIClock(RCC_MSIRange_0,TRUE);
    
    User_Timer_Enter_Sleep();
    
    /* Request Wait For Interrupt */    
    PWR_EnterSleepMode(PWR_Regulator_LowPower,PWR_SLEEPEntry_WFI);
    
    Exit_LP_Sleep_Mode();
    
    __enable_interrupt();
    
}

/*******************************************************************************
* Function Name  : Exit_LP_Sleep_Mode
* Description    : Exit from low power sleep mode.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Exit_LP_Sleep_Mode(void)
{        
    /* Switch in MSI 4MHz, HCLK 4MHz */
    ChangeMSIClock(RCC_MSIRange_6,FALSE);
    
    User_Timer_Exit_Sleep();
    
    Restore_GPIO_Config();
    
    Clock_Init();
    
    LP_mode = RUN;
}

#endif /* SYSCLK_MSI */
