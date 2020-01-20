/**
  ******************************************************************************
  * @file    scope.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @ingroup ST25DV-W_Discovery_Demo
  * @brief   This file contains the scope displaying methods.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
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
#include "commonfunc.h"
#include "scope.h"
#include "st25dx_discovery_ts.h"
#include "Menu_config.h"
#include "oscillo1_bkgdtop_jpg.h"
#include "oscillo1_bkgdleft_jpg.h"
#include "oscillo1_bkgdright_jpg.h"
#include "oscillo1_bkgdbottom_jpg.h"
#include "oscillo1_offset_jpg.h"
#include "oscillo1_offset2_jpg.h"
#include "oscillo1_ch1_jpg.h"
#include "oscillo1_bckch1_jpg.h"
#include "oscillo1_ch2_jpg.h"
#include "oscillo1_bckch2_jpg.h"
#include "font8.c"
#include "font16.c"
#include <math.h>
#include <stdio.h>

/* Private defines -----------------------------------------------------------*/
#define SCOPE_CHANNEL1          HAL_TIM_ACTIVE_CHANNEL_3
#define SCOPE_CHANNEL2          HAL_TIM_ACTIVE_CHANNEL_4
#define SCOPE_ENABLE_CHANNEL1   (SCOPE_CHANNEL1 << 4)
#define SCOPE_ENABLE_CHANNEL2   (SCOPE_CHANNEL2 << 4)
#define SCOPE_FIELD_CHANNEL     (SCOPE_CHANNEL1 | SCOPE_CHANNEL2)
#define SCOPE_FIELD_EN_CHANNEL  (SCOPE_ENABLE_CHANNEL1 | SCOPE_ENABLE_CHANNEL2)
#define WAVEFORM1_COLOR         0xFFE0
#define WAVEFORM2_COLOR         0x37AC
#define WAVEFORM_BACK_COLOR     LCD_COLOR_BLACK
#define WAVEFORM1_AMPLITUDE     55
#define WAVEFORM1_LOW_LEVEL     115
#define WAVEFORM1_HIGH_LEVEL    (WAVEFORM1_LOW_LEVEL - WAVEFORM1_AMPLITUDE)
#define WAVEFORM2_AMPLITUDE     55
#define WAVEFORM2_LOW_LEVEL     195
#define WAVEFORM2_HIGH_LEVEL    (WAVEFORM2_LOW_LEVEL - WAVEFORM2_AMPLITUDE)
#define SCOPE_GRID_COLOR        LCD_COLOR_WHITE
#define SCOPE_GRID_BACK_COLOR   LCD_COLOR_BLACK
#define SCOPE_MEAS_COLOR        0xFFE0
#define SCOPE_MEAS2_COLOR       0x37AC
#define SCOPE_MEAS_BACK_COLOR   0x09CD
#define SCOPE_TIME_COLOR        LCD_COLOR_WHITE
#define SCOPE_TIME_BACK_COLOR   0x22B4
#define SCOPE_ERROR_COLOR       LCD_COLOR_BLACK
#define SCOPE_ERROR_BACK_COLOR  LCD_COLOR_LIGHTRED
#define SCREEN_WIDTH            320
#define SCREEN_HEIGHT           240
#define SCOPE_WIDTH             310
#define SCOPE_HEIGHT            190
#define SCOPE_X_START           5
#define SCOPE_Y_START           20
#define SCOPE_X_STOP            (SCREEN_WIDTH - SCOPE_X_START - 5)
#define SCOPE_Y_STOP            (SCREEN_HEIGHT - SCOPE_Y_START - 30)
#define SCOPE_MEAS_ARRAY_SIZE   100
#define SCOPE_MEAS_STORE_DATA   (SCOPE_MEAS_ARRAY_SIZE - 1)
#define SCOPE_MEAS_SIZE         (SCOPE_MEAS_STORE_DATA - 1)
#define SCOPE_TIMER_PERIOD      0xFFFFFFFF
#define SCOPE_TIMER_PRESC       5

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public variables ---------------------------------------------------------*/
TIM_HandleTypeDef      Tim2Handle;

/* Captured values flags */
uint8_t ComputeChannel = 0;
uint8_t Cnt_Level1 = 0xFF;
uint8_t Cnt_Level2 = 0xFF;
uint16_t Cnt_measure1 = 0;
uint16_t Cnt_measure2 = 0;

/* Computed Values arrays */
uint32_t adata1[3 * SCOPE_MEAS_ARRAY_SIZE];
uint32_t measFrequency1[SCOPE_MEAS_ARRAY_SIZE];
uint32_t measDutyCycle1[SCOPE_MEAS_ARRAY_SIZE];

uint32_t adata2[3 * SCOPE_MEAS_ARRAY_SIZE];
uint32_t measFrequency2[SCOPE_MEAS_ARRAY_SIZE];
uint32_t measDutyCycle2[SCOPE_MEAS_ARRAY_SIZE];
uint8_t range = 1;

/* Waveforms Data */
uint32_t WaveformData1[SCREEN_WIDTH];
uint32_t WaveformDataShadow1[SCREEN_WIDTH];

uint32_t WaveformData2[SCREEN_WIDTH];
uint32_t WaveformDataShadow2[SCREEN_WIDTH];

/* General variables */
uint8_t RefreshDisplay = 0;
uint8_t Channel = SCOPE_ENABLE_CHANNEL2 | SCOPE_ENABLE_CHANNEL1 | SCOPE_CHANNEL1;
uint8_t UserButtonStatus = 0;
uint8_t ScopeError = 0;
float MeasTickDelay = 0;

extern uint8_t ST25DV_pwm;

/* Public functions -----------------------------------------------------------*/
void TIM_IC_MspInit( TIM_HandleTypeDef *htim );
void TIM_IC_MspDeInit( TIM_HandleTypeDef *htim );
void Scope_Init( void );
void Scope_DeInit( void );
void Scope_Delay( uint32_t TickDelay );
void Scope_StartMeas( void );
void Scope_StopMeas( void );
void Scope_StartLevelDetect( void );
void Scope_Compute_Meas_Values( void );
void Scope_GenerateSquare( void );
void Scope_ClearWaveform( void );
void Scope_DrawWaveform( void );
void Scope_DrawGrid( uint8_t firstdraw );
void Scope_Error( char *msg );

/**
  * @brief Initializes the PWM1 pin MSP.
  */
void Scope_PWM1_MspInit( void )
{
  GPIO_InitTypeDef gpio_init;
  
  gpio_init.Pin   = SCOPE_PWM1_PIN;
  gpio_init.Mode  = GPIO_MODE_AF_PP;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Pull  = GPIO_NOPULL;
  gpio_init.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init( SCOPE_PWM1_PIN_PORT, &gpio_init );
}

/**
  * @brief Initializes the PWM2 pin MSP.
  */
void Scope_PWM2_MspInit( void )
{
  GPIO_InitTypeDef gpio_init;
  
  gpio_init.Pin   = SCOPE_PWM2_PIN;
  gpio_init.Mode  = GPIO_MODE_AF_PP;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Pull  = GPIO_NOPULL;
  gpio_init.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init( SCOPE_PWM1_PIN_PORT, &gpio_init );
}

/**
  * @brief Initializes the TIM MSP.
  */
void TIM_IC_MspInit( TIM_HandleTypeDef *htim )
{
  GPIO_InitTypeDef gpio_init;
  static DMA_HandleTypeDef  hdma_tim2_CC4;
  static DMA_HandleTypeDef  hdma_tim2_CC3;

  HAL_NVIC_DisableIRQ( BSP_GPO_EXTI );
  
  /* Disable I2c2 on pin PB10 PB11 */
  __HAL_RCC_I2C2_CLK_DISABLE( );
  gpio_init.Pin   = BSP_GPO_PIN;
  gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init.Speed = GPIO_SPEED_LOW;
  gpio_init.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init( BSP_GPO_PIN_PORT, &gpio_init );
  
  HAL_GPIO_WritePin(BSP_GPO_PIN_PORT, BSP_GPO_PIN, GPIO_PIN_SET);
  
  HAL_GPIO_EXTI_IRQHandler(BSP_GPO_PIN);
  
  SCOPE_PWM1_GPIO_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();
  
  /* Peripheral clock enable */
  SCOPE_PWM1_CLOCK_ENABLE();
  
  SCOPE_PWM1_FORCE_RESET();
  SCOPE_PWM1_RELEASE_RESET();
  
  Scope_PWM1_MspInit( );
  Scope_PWM2_MspInit( );
  
  /* Configure the DMA streams */
  /* Set the parameters to be configured */
  hdma_tim2_CC4.Instance = DMA1_Stream6; /* TIM2_CHAN4 */
   
  hdma_tim2_CC4.Init.Channel  = DMA_CHANNEL_3;
  hdma_tim2_CC4.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_tim2_CC4.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tim2_CC4.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tim2_CC4.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tim2_CC4.Init.MemDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tim2_CC4.Init.Mode = DMA_NORMAL;
  hdma_tim2_CC4.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_tim2_CC4.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        
  hdma_tim2_CC4.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
  hdma_tim2_CC4.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_tim2_CC4.Init.PeriphBurst = DMA_PBURST_SINGLE;
     
  /* Associate the initialized DMA handle to the the TIM2 handle */
  __HAL_LINKDMA(htim, hdma[TIM_DMA_ID_CC4], hdma_tim2_CC4);
  
  HAL_DMA_Init(htim->hdma[TIM_DMA_ID_CC4]);
   
  hdma_tim2_CC3.Instance = DMA1_Stream1; /* TIM2_CHAN3 */
   
  hdma_tim2_CC3.Init.Channel  = DMA_CHANNEL_3;
  hdma_tim2_CC3.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_tim2_CC3.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tim2_CC3.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tim2_CC3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tim2_CC3.Init.MemDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tim2_CC3.Init.Mode = DMA_NORMAL;
  hdma_tim2_CC3.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_tim2_CC3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        
  hdma_tim2_CC3.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
  hdma_tim2_CC3.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_tim2_CC3.Init.PeriphBurst = DMA_PBURST_SINGLE;
     
  /* Associate the initialized DMA handle to the the TIM2 handle */
  __HAL_LINKDMA(htim, hdma[TIM_DMA_ID_CC3], hdma_tim2_CC3);
  
  HAL_DMA_Init(htim->hdma[TIM_DMA_ID_CC3]);

  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 1, 0);   
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 2);   
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
}

/**
  * @brief De-Initializes the TIM MSP.
  */
void TIM_IC_MspDeInit( TIM_HandleTypeDef *htim )
{
  GPIO_InitTypeDef gpio_init;
  
  /* Disable the TIM2 global Interrupt */
  HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* Disable the DMA1 Stream6 global Interrupt */
  HAL_NVIC_DisableIRQ(DMA1_Stream6_IRQn);
  /* Disable the DMA1 Stream1 global Interrupt */
  HAL_NVIC_DisableIRQ(DMA1_Stream1_IRQn);
  
  HAL_DMA_DeInit( htim->hdma[TIM_DMA_ID_CC3] );
  HAL_DMA_DeInit( htim->hdma[TIM_DMA_ID_CC4] );
  
  gpio_init.Pin   = SCOPE_PWM1_PIN | SCOPE_PWM2_PIN;
  HAL_GPIO_DeInit(SCOPE_PWM1_PIN_PORT, gpio_init.Pin);
  
  /* Peripheral clock disable */
  SCOPE_PWM1_CLOCK_DISABLE( );
  
  BSP_I2C2_DeInit();
  BSP_I2C2_Init();
}

/** @brief  Pin, Clock, Tim configuration for oscilloscope demo.
  * @param  None
  * @return None
  */
void Scope_Init( void )
{
  uint32_t cnt;
  TIM_IC_InitTypeDef sConfig;
  
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  /* Set TIM2 instance */
  Tim2Handle.Instance = TIM2;
  
  /* Initialize TIM2 peripheral */
  Tim2Handle.Init.Period = SCOPE_TIMER_PERIOD;
  Tim2Handle.Init.Prescaler = SCOPE_TIMER_PRESC;
  Tim2Handle.Init.ClockDivision = 0;
  Tim2Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  Tim2Handle.Init.RepetitionCounter = 0;
  TIM_IC_MspInit( &Tim2Handle );
  if(HAL_TIM_IC_Init(&Tim2Handle) != HAL_OK)
  {
    /* Initialization Error */
    Scope_Error( "Timer Init Error!!!" );
    ScopeError = 1;
  }
  
  /* Initialize TIM2 Channels */
  sConfig.ICPrescaler = TIM_ICPSC_DIV1;
  sConfig.ICFilter = 0;
  sConfig.ICPolarity = TIM_ICPOLARITY_BOTHEDGE;
  sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
  
  /* Configure the Input Capture of channel both channel */
  if(HAL_TIM_IC_ConfigChannel(&Tim2Handle, &sConfig, SCOPE_CHAN1_TIMERCHAN) != HAL_OK)
  {
    /* Configuration Error */
    Scope_Error( "Timer Config Chan. 4 Error" );
    ScopeError = 1;
  }
  
  if(HAL_TIM_IC_ConfigChannel(&Tim2Handle, &sConfig, SCOPE_CHAN2_TIMERCHAN) != HAL_OK)
  {
    /* Configuration Error */
    Scope_Error( "Timer Config Chan. 3 Error" );
    ScopeError = 1;
  }
  
  /* Initialise variables */
  ComputeChannel = 0;
  Cnt_Level1 = 0xFF;
  Cnt_Level2 = 0xFF;
  Cnt_measure1 = 0;
  Cnt_measure2 = 0;
  
  for( cnt = 0; cnt < SCOPE_MEAS_ARRAY_SIZE; cnt++ )
  {
    measFrequency1[cnt] = 0;
    measDutyCycle1[cnt] = 0;
    measFrequency2[cnt] = 0;
    measDutyCycle2[cnt] = 0;
  }
  range = 1;
  
  RefreshDisplay = 0;
  Channel = SCOPE_ENABLE_CHANNEL2 | SCOPE_ENABLE_CHANNEL1 | SCOPE_CHANNEL1;
  ScopeError = 0;
  MeasTickDelay = 0;
}

/** @brief  Function to restore ST25DV environment.
  * @param  None.
  * @return None.
  */
void Scope_DeInit( void )
{
  TIM_IC_MspDeInit( &Tim2Handle );
  BSP_LCD_SetFont( &Font22);
  ST25DV_pwm = 0;
}

/** @brief  Function to wait a delay based on Cortex Tick.
  * @param  TickDelay Number of tick to wait (Tick Time = 1/Sysclock).
  * @return None
  */
void Scope_Delay( uint32_t TickDelay )
{
  uint32_t systickstart = 0;
  uint32_t systicktime = 0;
  uint32_t reload = 0x00FFFFFF;
  
  /* Prepare Systick */
  SysTick->CTRL = 0;
  SysTick->LOAD = 0x00FFFFFF;
  SysTick->VAL = 0;
  /* Start counter */
  SysTick->CTRL = 0x5;
  systickstart = SysTick->VAL;
  systicktime = reload - systickstart;
  /* Waiting loop */
  while( systicktime < TickDelay )
  {
    if( (SysTick->CTRL & 0x10000) == 0 )
    {
      systicktime = systickstart - SysTick->VAL;
    }
    else
    {
      /* if counter is reloading, avoid negatives values */
      systicktime = (reload - SysTick->VAL) + systickstart;
    }
  }
  /* Reconfigure Systick for oscilloscope proper operation */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
}

/** @brief  Scope start measurement for each PWM.
  * @param  None None
  * @return None
  */
void Scope_StartMeas( void )
{
  /* Check current Channel */
  if( (Channel & SCOPE_CHANNEL1) == SCOPE_CHANNEL1 )
  {
    /* Start measurement */
    if(HAL_TIM_IC_Start_DMA(&Tim2Handle, SCOPE_CHAN1_TIMERCHAN, adata1, (3 * SCOPE_MEAS_SIZE)) != HAL_OK )
    {
      /* Starting Error */
      Scope_Error( "Start Timer Chan. 4 Error" );
      ScopeError = 1;
    }
  }

  /* Check current Channel */
  if( (Channel & SCOPE_CHANNEL2) == SCOPE_CHANNEL2 )
  {
    /* Start measurement */
    if(HAL_TIM_IC_Start_DMA(&Tim2Handle, SCOPE_CHAN2_TIMERCHAN, adata2, (3 * SCOPE_MEAS_SIZE)) != HAL_OK )
    {
      /* Starting Error */
      Scope_Error( "Start Timer Chan. 3 Error" );
      ScopeError = 1;
    }
  }
}

/** @brief  Scope stop measurement for each PWM.
  * @param  None None
  * @return None
  */
void Scope_StopMeas( void )
{
  if( (Channel & SCOPE_CHANNEL1) == SCOPE_CHANNEL1 )
  {
    HAL_TIM_IC_Stop_DMA(&Tim2Handle, SCOPE_CHAN1_TIMERCHAN);
  }
  if( (Channel & SCOPE_CHANNEL2) == SCOPE_CHANNEL2 )
  {
    HAL_TIM_IC_Stop_DMA(&Tim2Handle, SCOPE_CHAN2_TIMERCHAN);
  }
}

/** @brief  Function to start interruption on rising edge and read PWM level at 49% of its period.
  * @param  None None
  * @return None
  */
void Scope_StartLevelDetect( void )
{  
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Stop current measurement if any */
  HAL_TIM_IC_Stop_DMA(&Tim2Handle, SCOPE_CHAN1_TIMERCHAN);
  HAL_TIM_IC_Stop_DMA(&Tim2Handle, SCOPE_CHAN2_TIMERCHAN);
  Cnt_Level1 = 0xFF;
  Cnt_Level2 = 0xFF;
  
  HAL_NVIC_DisableIRQ( EXTI15_10_IRQn );
  /* Check current channel */
  if( (Channel & SCOPE_CHANNEL1) == SCOPE_CHANNEL1 )
  {
    if( (measFrequency1[SCOPE_MEAS_STORE_DATA] != 0) && (measFrequency1[SCOPE_MEAS_STORE_DATA] != 100) )
    {
      /* Compute waiting time of 49% of period */
      MeasTickDelay = (measFrequency1[SCOPE_MEAS_STORE_DATA] * 100.0) / 49.0;
      MeasTickDelay = HAL_RCC_GetSysClockFreq( ) / MeasTickDelay;
      /* Deduct time to raise for interrupt ~5�s */
      MeasTickDelay = MeasTickDelay - ((5 * HAL_RCC_GetSysClockFreq( )) / 1000000);
  
      /* Configure pin for GPIO rising edge funtionnality */
      GPIO_InitStruct.Pin   = SCOPE_PWM1_PIN;
      GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Pull  = GPIO_NOPULL;
      HAL_GPIO_Init( SCOPE_PWM1_PIN_PORT, &GPIO_InitStruct );
    }
  }

  /* Check current channel */
  if( (Channel & SCOPE_CHANNEL2) == SCOPE_CHANNEL2 )
  {
    if( (measFrequency2[SCOPE_MEAS_STORE_DATA] != 0) && (measFrequency2[SCOPE_MEAS_STORE_DATA] != 100) )
    {
      /* Compute waiting time of 49% of period */
      MeasTickDelay = (measFrequency2[SCOPE_MEAS_STORE_DATA] * 100.0) / 49.0;
      MeasTickDelay = HAL_RCC_GetSysClockFreq( ) / MeasTickDelay;
      /* Deduct time to raise for interrupt ~5�s */
      MeasTickDelay = MeasTickDelay - ((5 * HAL_RCC_GetSysClockFreq( )) / 1000000);
  
      /* Configure pin for GPIO rising edge funtionnality */
      GPIO_InitStruct.Pin   = SCOPE_PWM2_PIN;
      GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Pull  = GPIO_NOPULL;
      HAL_GPIO_Init( SCOPE_PWM2_PIN_PORT, &GPIO_InitStruct );
    }
  }
  HAL_NVIC_EnableIRQ( EXTI15_10_IRQn );
}

/**
  * @brief  Input Capture callback in non blocking mode 
  * @param  htim: TIM IC handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  uint32_t i = 0;
  
  if( (htim->Channel == SCOPE_CHANNEL1) && ((Channel & SCOPE_ENABLE_CHANNEL1) == SCOPE_ENABLE_CHANNEL1) && \
        (ComputeChannel == 0) )
  {
    /* Check if data already computed */
    if( Cnt_measure1 == 0 )
    {
      /* Run through all data measured by packet of 3 */
      for( i = 0; i < (SCOPE_MEAS_SIZE * 3); i = i + 3)
      {
        /* Compute Duty Cycle, diffence between 2nd and first value */
        if( adata1[i + 1] > adata1[i] )
        {
          measDutyCycle1[Cnt_measure1] = adata1[i + 1] - adata1[i];
        }
        else
        {
          measDutyCycle1[Cnt_measure1] = (SCOPE_TIMER_PERIOD - adata1[i]) + adata1[i + 1];
        }
        /* Compute Frequency, diffence between 3rd and first value */
        if( adata1[i + 2] > adata1[i] )
        {
          measFrequency1[Cnt_measure1] = adata1[i + 2] - adata1[i];
        }
        else
        {
          measFrequency1[Cnt_measure1] = (SCOPE_TIMER_PERIOD - adata1[0]) + adata1[2];
        }
        Cnt_measure1++;
      }
    }
  }    
      
  if( (htim->Channel == SCOPE_CHANNEL2) && ((Channel & SCOPE_ENABLE_CHANNEL2) == SCOPE_ENABLE_CHANNEL2) && \
        (ComputeChannel == 0) )
  {
    /* Check if data already computed */
    if( Cnt_measure2 == 0 )
    {
      /* Run through all data measured by packet of 3 */
      for( i = 0; i < (SCOPE_MEAS_SIZE * 3); i = i + 3)
      {
        /* Compute Duty Cycle, diffence between 2nd and first value */
        if( adata2[i + 1] > adata2[i] )
        {
          measDutyCycle2[Cnt_measure2] = adata2[i + 1] - adata2[i];
        }
        else
        {
          measDutyCycle2[Cnt_measure2] = (SCOPE_TIMER_PERIOD - adata2[i]) + adata2[i + 1];
        }
        /* Compute Frequency, diffence between 3rd and first value */
        if( adata2[i + 2] > adata2[i] )
        {
          measFrequency2[Cnt_measure2] = adata2[i + 2] - adata2[i];
        }
        else
        {
          measFrequency2[Cnt_measure2] = (SCOPE_TIMER_PERIOD - adata2[i]) + adata2[i + 2];
        }  
        Cnt_measure2++;
      }
    }
  }
}

/**
  * @brief  PWM callback function 
  * @param  None
  * @retval None
  */
void PWM1_Callback( void )
{
  /* Disable interrupt on pin */
  HAL_GPIO_DeInit( SCOPE_PWM1_PIN_PORT, SCOPE_PWM1_PIN );
  /* wait to reach 49% of period */
  Scope_Delay( MeasTickDelay );
  /* capture PWM level */
  Cnt_Level1 = HAL_GPIO_ReadPin( SCOPE_PWM1_PIN_PORT, SCOPE_PWM1_PIN );
  Scope_PWM1_MspInit( );
}

/**
  * @brief  PWM callback function 
  * @param  None
  * @retval None
  */
void PWM2_Callback( void )
{
  /* Disable interrupt on pin */
  HAL_GPIO_DeInit( SCOPE_PWM2_PIN_PORT, SCOPE_PWM2_PIN );
  /* wait to reach 49% of period */
  Scope_Delay( MeasTickDelay );
  /* capture PWM level */
  Cnt_Level2 = HAL_GPIO_ReadPin( SCOPE_PWM2_PIN_PORT, SCOPE_PWM2_PIN );
  Scope_PWM2_MspInit( );
}

/**
  * @brief  Compute Frequency and duty Cycle values from captured measure.
  * @param  None None
  * @retval None
  */
void Scope_Compute_Meas_Values( void )
{
  uint32_t i = 0, j = 0, maxdc = 0, maxdc_idx = 0, maxfr = 0, maxfr_idx = 0;
  uint32_t computedvalue = 0;
  uint32_t diff = 0;
  uint32_t freq[SCOPE_MEAS_ARRAY_SIZE];
  uint32_t dutycycle[SCOPE_MEAS_ARRAY_SIZE];
  uint32_t tickstart = 0;
  
  /* Check if all measurment have been done for channel 1 */
  if( Cnt_measure1 >= SCOPE_MEAS_SIZE )
  {
    /* Start computation on Channel 1 */
    ComputeChannel = 1;
    /* Initialize array */
    for( i = 0; i < SCOPE_MEAS_ARRAY_SIZE; i++ )
    {
      dutycycle[i] = 0;
      freq[i] = 0;
    }
    /* Count repeated values */
    for( i = 0; i < Cnt_measure1; i++ )
    {
      for( j = 0; j < Cnt_measure1; j++ )
      {
        if( (measDutyCycle1[i] < (measDutyCycle1[j] + (measDutyCycle1[j] * 0.01))) && \
              (measDutyCycle1[i] > (measDutyCycle1[j] - (measDutyCycle1[j] * 0.01))) )
        {
          dutycycle[i]++;
        }
        if( (measFrequency1[i] < (measFrequency1[j] + (measFrequency1[j] * 0.01))) && \
              (measFrequency1[i] > (measFrequency1[j] - (measFrequency1[j] * 0.01))) )
        {
          freq[i]++;
        }
      }
    }
    /* Get max repeated values */
    for( i = 0; i < Cnt_measure1; i++ )
    {
      if( dutycycle[i] > maxdc )
      {
        maxdc = dutycycle[i];
        maxdc_idx = i;
      }
      if( freq[i] > maxfr )
      {
        maxfr = freq[i];
        maxfr_idx = i;
      }
    }
    
    /* Compute and compare to previous frequency */
    computedvalue = round((2 * HAL_RCC_GetPCLK1Freq()) / (float)((SCOPE_TIMER_PRESC + 1) * measFrequency1[maxfr_idx]));
    if( computedvalue >= measFrequency1[SCOPE_MEAS_STORE_DATA] )
    {
      diff = computedvalue - measFrequency1[SCOPE_MEAS_STORE_DATA];
    }
    else
    {
      diff = measFrequency1[SCOPE_MEAS_STORE_DATA] - computedvalue;
    }
    /* Allow delta of 3% */
    if( diff > (measFrequency1[SCOPE_MEAS_STORE_DATA] * 0.03) )
    {
      /* Enable refresh for channel 1 and store new displayed value */
      RefreshDisplay |= SCOPE_CHANNEL1;
      measFrequency1[SCOPE_MEAS_STORE_DATA] = computedvalue;
    }
    
    Scope_StartLevelDetect( );

    /* Wait until measurement done, or timeout if PWM at 0% or 100% (no interruption) */
    tickstart = HAL_GetTick();
    while( (Cnt_Level1 == 0xFF) && ((HAL_GetTick() - tickstart) < 20) );
    Scope_PWM1_MspInit( );
    
    /* Compute and compare to Duty Cycle */
    computedvalue = round((measDutyCycle1[maxdc_idx] * 100) / (float)measFrequency1[maxfr_idx]);
    if( ((Cnt_Level1 == GPIO_PIN_RESET) && (computedvalue > 50)) || ((Cnt_Level1 == GPIO_PIN_SET) && \
          (computedvalue < 50)) )
    {
      computedvalue = 100 - computedvalue;
    }
    if( computedvalue != measDutyCycle1[SCOPE_MEAS_STORE_DATA] )
    {
      /* Enable refresh for channel 1 and store new displayed value */
      RefreshDisplay |= SCOPE_CHANNEL1;
      measDutyCycle1[SCOPE_MEAS_STORE_DATA] = computedvalue;
    }
    Cnt_measure1 = 0;
  }
  
  /* Check if all measurment have been done for channel 2 */
  if( Cnt_measure2 >= SCOPE_MEAS_SIZE )
  {
    /* Start computation on Channel 2 */
    ComputeChannel = 1;
    /* Initialize array */
    for( i = 0; i < SCOPE_MEAS_ARRAY_SIZE; i++ )
    {
      dutycycle[i] = 0;
      freq[i] = 0;
    }
    /* Count repeated values */
    for( i = 0; i < Cnt_measure2; i++ )
    {
      for( j = 0; j < Cnt_measure2; j++ )
      {
        if( measDutyCycle2[i] == measDutyCycle2[j] )
        {
          dutycycle[i]++;
        }
        if( measFrequency2[i] == measFrequency2[j] )
        {
          freq[i]++;
        }
      }
    }
    /* Get max repeated values */
    for( i = 0; i < Cnt_measure2; i++ )
    {
      if( dutycycle[i] > maxdc )
      {
        maxdc = dutycycle[i];
        maxdc_idx = i;
      }
      if( freq[i] > maxfr )
      {
        maxfr = freq[i];
        maxfr_idx = i;
      }
    }
    
    /* Compute and compare to previous frequency */
    computedvalue = round((2 * HAL_RCC_GetPCLK1Freq()) / (float)((SCOPE_TIMER_PRESC + 1) * measFrequency2[maxfr_idx]));
    if( computedvalue >= measFrequency2[SCOPE_MEAS_STORE_DATA] )
    {
      diff = computedvalue - measFrequency2[SCOPE_MEAS_STORE_DATA];
    }
    else
    {
      diff = measFrequency2[SCOPE_MEAS_STORE_DATA] - computedvalue;
    }
    /* Allow delta of 3% */
    if( diff > (measFrequency2[SCOPE_MEAS_STORE_DATA] * 0.03) )
    {
      /* Enable refresh for channel 2 and store new displayed value */
      RefreshDisplay |= SCOPE_CHANNEL2;
      measFrequency2[SCOPE_MEAS_STORE_DATA] = computedvalue;
    }
    
    Scope_StartLevelDetect( );

    /* Wait until measurement done, or timeout if PWM at 0% or 100% (no interruption) */
    tickstart = HAL_GetTick();
    while( ((Cnt_Level2 == 0xFF)) && ((HAL_GetTick() - tickstart) < 20) );
    Scope_PWM2_MspInit( );
    
    /* Compute and compare to previous Duty Cycle */
    computedvalue = round((measDutyCycle2[maxdc_idx] * 100) / (float)measFrequency2[maxfr_idx]);
    if( ((Cnt_Level2 == GPIO_PIN_RESET) && (computedvalue > 50)) || ((Cnt_Level2 == GPIO_PIN_SET) && \
          (computedvalue < 50)) )
    {
      computedvalue = 100 - computedvalue;
    }
    if( computedvalue != measDutyCycle2[SCOPE_MEAS_STORE_DATA] )
    {
      /* Enable refresh for channel 2 and store new displayed value */
      RefreshDisplay |= SCOPE_CHANNEL2;
      measDutyCycle2[SCOPE_MEAS_STORE_DATA] = computedvalue;
    }
    Cnt_measure2 = 0;
  }
  ComputeChannel = 0;
}

/** @brief  Scope Demo main loop.
  * @param  None None
  * @return None
  */
void ST25DV_Scope_Start( void )
{
  uint8_t exit_scope = 0;
  uint32_t tickstart = 0;
  uint32_t timeout = 0;
  
  BSP_LCD_Clear( LCD_COLOR_BLACK );
  Scope_Init( );
  Scope_DrawGrid( 1 );
  
  while( exit_scope == 0 )
  {
    /* Start Frequency and Duty Cycle Measurement */
    Cnt_measure1 = 0;
    Cnt_measure2 = 0;
    
    Scope_StartMeas( );
    
    /* Wait until measurement done, or timeout if PWM at 0% or 100% (no interruption) */
    tickstart = HAL_GetTick();
    timeout = 0;
    while( ((Cnt_measure1 != SCOPE_MEAS_SIZE) && (Cnt_measure2 != SCOPE_MEAS_SIZE)) && (timeout < 300) )
    {
      timeout = (HAL_GetTick() - tickstart);
    }
    
    if( timeout < 300 )
    {
      /* Compute and Check if measurement changed and need to be displayed */
      Scope_Compute_Meas_Values( );
    }
    else
    {
      /* If there is no measure it due to signal with duty cycle = 100% or 0% */
      if( ((Channel & SCOPE_ENABLE_CHANNEL1) == SCOPE_ENABLE_CHANNEL1) && \
            ((Channel & SCOPE_CHANNEL1) == SCOPE_CHANNEL1) )
      {
        /* Stop current measurement as there is no interruption to stop measurement */
        Scope_StopMeas( );
        /* Read PWM Level for 0% or 100% detection */
        if( HAL_GPIO_ReadPin( SCOPE_PWM1_PIN_PORT, SCOPE_PWM1_PIN) == GPIO_PIN_RESET )
        {
          if( measDutyCycle1[SCOPE_MEAS_STORE_DATA] != 0 )
          {
            RefreshDisplay = SCOPE_CHANNEL1;
            measDutyCycle1[SCOPE_MEAS_STORE_DATA] = 0;
          }
        }
        else
        {
          if( measDutyCycle1[SCOPE_MEAS_STORE_DATA] != 100 )
          {
            RefreshDisplay = SCOPE_CHANNEL1;
            measDutyCycle1[SCOPE_MEAS_STORE_DATA] = 100;
          }
        }
      }
      
      if( ((Channel & SCOPE_ENABLE_CHANNEL2) == SCOPE_ENABLE_CHANNEL2) && \
            ((Channel & SCOPE_CHANNEL2) == SCOPE_CHANNEL2) )
      {
        /* Stop current measurement as there is no interruption to stop measurement */
        Scope_StopMeas( );
        /* Read PWM Level for 0% or 100% detection */
        if( HAL_GPIO_ReadPin( SCOPE_PWM2_PIN_PORT, SCOPE_PWM2_PIN) == GPIO_PIN_RESET )
        {
          if( measDutyCycle2[SCOPE_MEAS_STORE_DATA] != 0 )
          {
            RefreshDisplay = SCOPE_CHANNEL2;
            measDutyCycle2[SCOPE_MEAS_STORE_DATA] = 0;
          }
        }
        else
        {
          if( measDutyCycle2[SCOPE_MEAS_STORE_DATA] != 100 )
          {
            RefreshDisplay = SCOPE_CHANNEL2;
            measDutyCycle2[SCOPE_MEAS_STORE_DATA] = 100;
          }
        }
      }
    }
    
    /* Refresh if new values detected */
    if( RefreshDisplay > 0 )
    {
      Scope_GenerateSquare( );
      Scope_ClearWaveform( );
      Scope_DrawGrid( 0 );
      Scope_DrawWaveform( );
      RefreshDisplay = 0;
    }
    
    /* Button pressed management */
    if( UserButtonStatus == 1 )
    {
      RefreshDisplay = SCOPE_CHANNEL1 | SCOPE_CHANNEL2;
      Scope_ClearWaveform( );

      /* Switch between channels */
      switch( Channel & SCOPE_FIELD_EN_CHANNEL )
      {
        case SCOPE_ENABLE_CHANNEL1:
          Channel = SCOPE_ENABLE_CHANNEL2 | SCOPE_CHANNEL2;
          break;
        
        case SCOPE_ENABLE_CHANNEL2:
          Channel = SCOPE_ENABLE_CHANNEL1 | SCOPE_ENABLE_CHANNEL2 | SCOPE_CHANNEL1;
          break;
        
        case SCOPE_FIELD_EN_CHANNEL:
          Channel = SCOPE_ENABLE_CHANNEL1 | SCOPE_CHANNEL1;
          break;
        
        default:
          Channel = SCOPE_ENABLE_CHANNEL1 | SCOPE_ENABLE_CHANNEL2 | SCOPE_CHANNEL1;
      }
      Scope_GenerateSquare( );
      Scope_DrawGrid( 1 );
      Scope_DrawWaveform( );
      RefreshDisplay = 0;
      UserButtonStatus = 0;
    }
    
    /* Measurement cannot be done in the same time (DMA usage), 
       switching between channels for measurement is necessary */
    if( (Channel & SCOPE_FIELD_EN_CHANNEL) == SCOPE_FIELD_EN_CHANNEL )
    { /* Both channel activated, swap channel */
      if( (Channel & SCOPE_FIELD_CHANNEL) == SCOPE_CHANNEL1)
      {
        Channel &= (~SCOPE_FIELD_CHANNEL);
        Channel |= SCOPE_CHANNEL2;
      }
      else
      {
        Channel &= (~SCOPE_FIELD_CHANNEL);
        Channel |= SCOPE_CHANNEL1;
      }
    }
    
    HAL_Delay(10);
  }
}

/** @brief  Compute waveform from measured freq and Duty Cycle Scope Demo.
  * @param  None None
  * @return None
  */
void Scope_GenerateSquare( void )
{
  uint32_t period1 = 0;
  uint32_t period2 = 0;
  uint32_t i = 0;
  uint32_t wavepos1 = 0;
  uint32_t dutycyclepos1 = 0;
  uint32_t wavepos2 = 0;
  uint32_t dutycyclepos2 = 0;
  
  /* compute period, unit in �s */
  if( measFrequency1[SCOPE_MEAS_STORE_DATA] > 0 )
  {
    period1 = 1000000 / measFrequency1[SCOPE_MEAS_STORE_DATA];
  }
  else
  { /* Avoid division by 0 */
    period1 = 0;
  }
  
  if( measFrequency2[SCOPE_MEAS_STORE_DATA] > 0 )
  {
    period2 = 1000000 / measFrequency2[SCOPE_MEAS_STORE_DATA];
  }
  else
  { /* Avoid division by 0 */
    period2 = 0;
  }
  
  /* Compute range to adapt display only for channel 1 if 2 channels */
  if( ((period1 <= 128) && ((Channel & SCOPE_ENABLE_CHANNEL1) == SCOPE_ENABLE_CHANNEL1)) || ((period2 <= 128) && \
        ((Channel & SCOPE_ENABLE_CHANNEL1) == 0x00)) )
  { /* 1px = 1�s */
    if( range != 1 )
    {
      RefreshDisplay = SCOPE_CHANNEL1 | SCOPE_CHANNEL2;
    }
    range = 1;
  }
  else if( ((period1 <= 500) && ((Channel & SCOPE_ENABLE_CHANNEL1) == SCOPE_ENABLE_CHANNEL1)) || ((period2 <= 500) && \
              ((Channel & SCOPE_ENABLE_CHANNEL1) == 0x00)) )
  { /* 1px = 4�s */
    if( range != 4 )
    {
      RefreshDisplay = SCOPE_CHANNEL1 | SCOPE_CHANNEL2;
    }
    range = 4;
  }
  else
  { /* 1px = 16�s */
    if( range != 16 )
    {
      RefreshDisplay = SCOPE_CHANNEL1 | SCOPE_CHANNEL2;
    }
    range = 16;
  }
  
  for( i = 0; i < SCREEN_WIDTH; i++)
  {
    /* Compute position on period signal */
    wavepos1 = i % (period1 / range);
    wavepos2 = i % (period2 / range);
    /* Compute position on duty cycle signal */
    dutycyclepos1 = (period1 * (100 - measDutyCycle1[SCOPE_MEAS_STORE_DATA])) / (float)(100 * range);
    if( dutycyclepos1 == 0 )
    {
      dutycyclepos1 = 1;
    }
    dutycyclepos2 = (period2 * (100 - measDutyCycle2[SCOPE_MEAS_STORE_DATA])) / (float)(100 * range);
    if( dutycyclepos2 == 0 )
    {
      dutycyclepos2 = 1;
    }
    
    /* Position point in waveform 1 */
    if( measDutyCycle1[SCOPE_MEAS_STORE_DATA] == 0 )
    {
      WaveformData1[i] = WAVEFORM1_LOW_LEVEL;
    }
    else if( measDutyCycle1[SCOPE_MEAS_STORE_DATA] == 100 )
    {
      WaveformData1[i] = WAVEFORM1_HIGH_LEVEL;
    }
    else
    {
      if( wavepos1 < dutycyclepos1 )
      { /* Waveform 1 low level */
        WaveformData1[i] = WAVEFORM1_LOW_LEVEL;
      }
      else
      { /* Waveform 1 high level */
        WaveformData1[i] = WAVEFORM1_HIGH_LEVEL;
      }
    }
    
    /* Position point in waveform 2 */
    if( measDutyCycle2[SCOPE_MEAS_STORE_DATA] == 0 )
    {
      WaveformData2[i] = WAVEFORM2_LOW_LEVEL;
    }
    else if( measDutyCycle2[SCOPE_MEAS_STORE_DATA] == 100 )
    {
      WaveformData2[i] = WAVEFORM2_HIGH_LEVEL;
    }
    else
    {
      if( wavepos2 < dutycyclepos2 )
      { /* Waveform 2 low level */
        WaveformData2[i] = WAVEFORM2_LOW_LEVEL;
      }
      else
      { /* Waveform 2 high level */
        WaveformData2[i] = WAVEFORM2_HIGH_LEVEL;
      }
    }
  }
}

/** @brief  Clear waveform on LCD.
  * @param  None None
  * @return None
  */
void Scope_ClearWaveform( void )
{
  uint32_t i = 0;
  
  BSP_LCD_SetTextColor(WAVEFORM_BACK_COLOR);
  for( i = (SCOPE_X_START + 1); i < (SCOPE_WIDTH + SCOPE_X_START); i++)
  {
    if( i == (SCREEN_WIDTH - ((SCOPE_WIDTH / 2) + SCOPE_X_START)) )
    {
      BSP_LCD_SetTextColor(SCOPE_GRID_COLOR);
    }
    else
    {
      BSP_LCD_SetTextColor(WAVEFORM_BACK_COLOR);
    }
    if( ((Channel & SCOPE_ENABLE_CHANNEL1) == SCOPE_ENABLE_CHANNEL1) && \
          ((RefreshDisplay & SCOPE_CHANNEL1) == SCOPE_CHANNEL1) )
    {
      /* Channel 1 */
      if( (WaveformDataShadow1[i] == ((SCOPE_HEIGHT / 2) + SCOPE_Y_START)) || \
            (i == (SCREEN_WIDTH - ((SCOPE_WIDTH / 2) + SCOPE_X_START))) )
      { /* Clear waveform point on horizontal grid */
        BSP_LCD_DrawPixel( SCREEN_WIDTH - i, WaveformDataShadow1[i], SCOPE_GRID_COLOR);
      }
      else if( (WaveformDataShadow1[i] == WAVEFORM1_HIGH_LEVEL) || (WaveformDataShadow1[i] == WAVEFORM1_LOW_LEVEL) )
      { /* Clear waveform point */
        BSP_LCD_DrawPixel( SCREEN_WIDTH - i, WaveformDataShadow1[i], WAVEFORM_BACK_COLOR);
      }
      if( (i > 0) && (WaveformDataShadow1[i-1] > WaveformDataShadow1[i]) && (WaveformDataShadow1[i] > 0) )
      { /* Clear vertical waveform line */
        BSP_LCD_DrawVLine( SCREEN_WIDTH - i, WaveformDataShadow1[i], WAVEFORM1_AMPLITUDE);
      }
      else if( (i > 0) && (WaveformDataShadow1[i-1] < WaveformDataShadow1[i]) && (WaveformDataShadow1[i] > 0) )
      { /* Clear vertical waveform line */
        BSP_LCD_DrawVLine( SCREEN_WIDTH - i, WaveformDataShadow1[i] - WAVEFORM1_AMPLITUDE, WAVEFORM1_AMPLITUDE);
      }
    }
    
    if( ((Channel & SCOPE_ENABLE_CHANNEL2) == SCOPE_ENABLE_CHANNEL2) && \
          ((RefreshDisplay & SCOPE_CHANNEL2) == SCOPE_CHANNEL2) )
    {
      /* Channel 2 */
      if( (WaveformDataShadow2[i] == ((SCOPE_HEIGHT / 2) + SCOPE_Y_START)) || \
            (i == (SCREEN_WIDTH - ((SCOPE_WIDTH / 2) + SCOPE_X_START))) )
      {
        /* Clear waveform point on horizontal grid */
        BSP_LCD_DrawPixel( SCREEN_WIDTH - i, WaveformDataShadow2[i], SCOPE_GRID_COLOR);
      }
      else if( (WaveformDataShadow2[i] == WAVEFORM2_HIGH_LEVEL) || (WaveformDataShadow2[i] == WAVEFORM2_LOW_LEVEL) )
      {
        /* Clear waveform point */
        BSP_LCD_DrawPixel( SCREEN_WIDTH - i, WaveformDataShadow2[i], WAVEFORM_BACK_COLOR);
      }
      if( (i > 0) && (WaveformDataShadow2[i-1] > WaveformDataShadow2[i]) && (WaveformDataShadow2[i] > 0) )
      { /* Clear vertical waveform line */
        BSP_LCD_DrawVLine( SCREEN_WIDTH - i, WaveformDataShadow2[i], WAVEFORM2_AMPLITUDE);
      }
      else if( (i > 0) && (WaveformDataShadow2[i-1] < WaveformDataShadow2[i]) && (WaveformDataShadow2[i] > 0) )
      { /* Clear vertical waveform line */
        BSP_LCD_DrawVLine( SCREEN_WIDTH - i, WaveformDataShadow2[i] - WAVEFORM2_AMPLITUDE, WAVEFORM2_AMPLITUDE);
      }
    }
  }
}

/** @brief  Draw waveform on LCD.
  * @param  None None
  * @return None
  */
void Scope_DrawWaveform( void )
{
  uint32_t i = 0;
  
  for( i = (SCOPE_X_START + 1); i < (SCOPE_WIDTH + SCOPE_X_START); i++)
  {
    if( ((Channel & SCOPE_ENABLE_CHANNEL1) == SCOPE_ENABLE_CHANNEL1) && \
          ((RefreshDisplay & SCOPE_CHANNEL1) == SCOPE_CHANNEL1) )
    {
      /* Channel 1 */
      BSP_LCD_DrawPixel( SCREEN_WIDTH - i, WaveformData1[i], WAVEFORM1_COLOR);
      BSP_LCD_SetTextColor(WAVEFORM1_COLOR);
      if( (i > 0) && (WaveformData1[i-1] > WaveformData1[i]) && (WaveformData1[i] > 0) )
      { /* Draw vertical waveform line */
        BSP_LCD_DrawVLine( SCREEN_WIDTH - i, WaveformData1[i], WAVEFORM1_AMPLITUDE);
      }
      else if( (i > 0) && (WaveformData1[i-1] < WaveformData1[i]) && (WaveformData1[i] > 0) )
      { /* Draw vertical waveform line */
        BSP_LCD_DrawVLine( SCREEN_WIDTH - i, WaveformData1[i] - WAVEFORM1_AMPLITUDE, WAVEFORM1_AMPLITUDE);
      }
      
      /* Store Waveform data for clearing */
      WaveformDataShadow1[i] = WaveformData1[i];
    }

    if( ((Channel & SCOPE_ENABLE_CHANNEL2) == SCOPE_ENABLE_CHANNEL2) && \
          ((RefreshDisplay & SCOPE_CHANNEL2) == SCOPE_CHANNEL2) )
    {
      /* Channel 2 */
      BSP_LCD_DrawPixel( SCREEN_WIDTH - i, WaveformData2[i], WAVEFORM2_COLOR);
      BSP_LCD_SetTextColor(WAVEFORM2_COLOR);
      if( (i > 0) && (WaveformData2[i-1] > WaveformData2[i]) && (WaveformData2[i] > 0) )
      { /* Draw vertical waveform line */
        BSP_LCD_DrawVLine( SCREEN_WIDTH - i, WaveformData2[i], WAVEFORM2_AMPLITUDE);
      }
      else if( (i > 0) && (WaveformData2[i-1] < WaveformData2[i]) && (WaveformData2[i] > 0) )
      { /* Draw vertical waveform line */
        BSP_LCD_DrawVLine( SCREEN_WIDTH - i, WaveformData2[i] - WAVEFORM2_AMPLITUDE, WAVEFORM2_AMPLITUDE);
      }
      
      /* Store Waveform data for clearing */
      WaveformDataShadow2[i] = WaveformData2[i];
    }
  }
}

/** @brief  Draw oscilloscope grid on LCD.
  * @param  firstdraw 1 to draw only the first time 
  * @return None
  */
void Scope_DrawGrid( uint8_t firstdraw )
{
  uint16_t x = 0, y = 0;
  char displaystring[220] = "";
  
  /* Display error management */
  if( ScopeError == 1)
  {
    ScopeError = 2;
  }
  else if( ScopeError == 2 )
  {
    /* If error prviously displayed, clear it */
    firstdraw = 1;
    ScopeError = 0;
    BSP_LCD_SetBackColor( WAVEFORM_BACK_COLOR );
    BSP_LCD_SetTextColor( LCD_COLOR_BLACK );
    BSP_LCD_DisplayStringAt( 0, LINE(Line6), (uint8_t *)displaystring, CENTER_MODE);
    RefreshDisplay = SCOPE_CHANNEL1 | SCOPE_CHANNEL2;
  }
  
  if( firstdraw > 0 )
  {
    /* Display Background image */
    BSP_LCD_DisplayPicture( 0, 0, cOscillo1_Bkgdtop );
    BSP_LCD_DisplayPicture( 0, 20, cOscillo1_Bkgdleft );
    BSP_LCD_DisplayPicture( 315, 20, cOscillo1_Bkgdright );
    BSP_LCD_DisplayPicture( 0, 210, cOscillo1_Bkgdbottom );
    /* Display Channel logo */
    if( (Channel & SCOPE_ENABLE_CHANNEL1) == SCOPE_ENABLE_CHANNEL1 )
    {
      BSP_LCD_DisplayPicture( 6, 4, cOscillo1_ch1 );
    }
    if( (Channel & SCOPE_ENABLE_CHANNEL2) == SCOPE_ENABLE_CHANNEL2 )
    {
      BSP_LCD_DisplayPicture( 40, 4, cOscillo1_ch2 );
    }
  }
  
  BSP_LCD_SetFont( &Font8);
  BSP_LCD_SetBackColor( SCOPE_TIME_BACK_COLOR );
  BSP_LCD_SetTextColor( SCOPE_TIME_COLOR );
  
  if( ((Channel & SCOPE_ENABLE_CHANNEL1) == SCOPE_ENABLE_CHANNEL1) && \
        ((RefreshDisplay & SCOPE_CHANNEL1) == SCOPE_CHANNEL1) )
  {
    if( (measFrequency1[SCOPE_MEAS_STORE_DATA] < 400) || (measFrequency1[SCOPE_MEAS_STORE_DATA] > 45000) \
        || (measDutyCycle1[SCOPE_MEAS_STORE_DATA] > 100) )
    {
      /* Display Time range */
      BSP_LCD_DisplayStringAt( 160, 7, (uint8_t *)"   ", LEFT_MODE);
      
      /* Display Frequency value */
      BSP_LCD_SetFont( &Font16 );
      BSP_LCD_SetBackColor( SCOPE_MEAS_BACK_COLOR );
      BSP_LCD_SetTextColor( SCOPE_MEAS_COLOR );
      BSP_LCD_DisplayStringAt( 24, 217, (uint8_t *)"     ", LEFT_MODE);
      
      /* Display Duty Cycle value */  
      BSP_LCD_DisplayStringAt( 105, 217, (uint8_t *)"     ", LEFT_MODE);
    }
    else
    {
      /* Display Time range */
      sprintf( displaystring, "Time %3d us", (range * 40) );
      BSP_LCD_DisplayStringAt( 138, 7, (uint8_t *)displaystring, LEFT_MODE);
      
      /* Display Frequency value */
      BSP_LCD_SetFont( &Font16 );
      BSP_LCD_SetBackColor( SCOPE_MEAS_BACK_COLOR );
      BSP_LCD_SetTextColor( SCOPE_MEAS_COLOR );
      if( (measDutyCycle1[SCOPE_MEAS_STORE_DATA] == 0) || (measDutyCycle1[SCOPE_MEAS_STORE_DATA] == 100) )
      {
        BSP_LCD_DisplayStringAt( 24, 217, (uint8_t *)"     ", LEFT_MODE);
      }
      else
      {
        if( measFrequency1[SCOPE_MEAS_STORE_DATA] >= 1000 )
        {
          sprintf( displaystring, "%2luKHz", (measFrequency1[SCOPE_MEAS_STORE_DATA] / 1000));
          BSP_LCD_DisplayStringAt( 24, 217, (uint8_t *)displaystring, LEFT_MODE);
        }
        else
        {
          sprintf( displaystring, "%3ludHz", measFrequency1[SCOPE_MEAS_STORE_DATA]);
          BSP_LCD_DisplayStringAt( 24, 217, (uint8_t *)displaystring, LEFT_MODE);
        }
      }
      
      /* Display Duty Cycle value */
      BSP_LCD_DisplayStringAt( 105, 217, (uint8_t *)"    ", LEFT_MODE);
      sprintf( displaystring, "%2lu%%", measDutyCycle1[SCOPE_MEAS_STORE_DATA]);
      BSP_LCD_DisplayStringAt( 105, 217, (uint8_t *)displaystring, LEFT_MODE);
    }
  }
  
  if( ((Channel & SCOPE_ENABLE_CHANNEL2) == SCOPE_ENABLE_CHANNEL2) && \
        ((RefreshDisplay & SCOPE_CHANNEL2) == SCOPE_CHANNEL2) )
  {
    if( (measFrequency2[SCOPE_MEAS_STORE_DATA] < 400) || (measFrequency2[SCOPE_MEAS_STORE_DATA] > 45000) \
        || (measDutyCycle2[SCOPE_MEAS_STORE_DATA] > 100) )
    {
      if( (Channel & SCOPE_ENABLE_CHANNEL1) == 0x00 )
      {
        /* Display Time range */
        BSP_LCD_DisplayStringAt( 160, 7, (uint8_t *)"   ", LEFT_MODE);
      }
      
      /* Display Frequency value */
      BSP_LCD_SetFont( &Font16 );
      BSP_LCD_SetBackColor( SCOPE_MEAS_BACK_COLOR );
      BSP_LCD_SetTextColor( SCOPE_MEAS2_COLOR );
      BSP_LCD_DisplayStringAt( 182, 217, (uint8_t *)"     ", LEFT_MODE);
      
      /* Display Duty Cycle value */  
      BSP_LCD_DisplayStringAt( 260, 217, (uint8_t *)"     ", LEFT_MODE);
    }
    else
    {
      if( (Channel & SCOPE_ENABLE_CHANNEL1) == 0x00 )
      {
        /* Display Time range */
        sprintf( displaystring, "Time %3d us", (range * 40) );
        BSP_LCD_DisplayStringAt( 138, 7, (uint8_t *)displaystring, LEFT_MODE);
      }
      
      /* Display Frequency value */
      BSP_LCD_SetFont( &Font16 );
      BSP_LCD_SetBackColor( SCOPE_MEAS_BACK_COLOR );
      BSP_LCD_SetTextColor( SCOPE_MEAS2_COLOR );
      if( (measDutyCycle2[SCOPE_MEAS_STORE_DATA] == 0) || (measDutyCycle2[SCOPE_MEAS_STORE_DATA] == 100) )
      {
        BSP_LCD_DisplayStringAt( 182, 217, (uint8_t *)"     ", LEFT_MODE);
      }
      else
      {
        if( measFrequency2[SCOPE_MEAS_STORE_DATA] >= 1000 )
        {
          sprintf( displaystring, "%2luKHz", (measFrequency2[SCOPE_MEAS_STORE_DATA] / 1000));
          BSP_LCD_DisplayStringAt( 182, 217, (uint8_t *)displaystring, LEFT_MODE);
        }
        else
        {
          sprintf( displaystring, "%3luHz", measFrequency2[SCOPE_MEAS_STORE_DATA]);
          BSP_LCD_DisplayStringAt( 182, 217, (uint8_t *)displaystring, LEFT_MODE);
        }
      }
      
      /* Display Duty Cycle value */
      BSP_LCD_DisplayStringAt( 260, 217, (uint8_t *)"    ", LEFT_MODE);
      sprintf( displaystring, "%2lu%%", measDutyCycle2[SCOPE_MEAS_STORE_DATA]);
      BSP_LCD_DisplayStringAt( 260, 217, (uint8_t *)displaystring, LEFT_MODE);
    }
  }
  
  /* Display Grid */
  BSP_LCD_SetBackColor( SCOPE_GRID_BACK_COLOR );
  BSP_LCD_SetTextColor( SCOPE_GRID_COLOR );
  BSP_LCD_DrawRect( SCOPE_X_START, SCOPE_Y_START, SCOPE_WIDTH, SCOPE_HEIGHT );
  if( firstdraw > 0 )
  {
    BSP_LCD_DrawHLine( SCOPE_X_START, ((SCOPE_HEIGHT / 2) + SCOPE_Y_START), SCOPE_WIDTH);
    BSP_LCD_DrawVLine( ((SCOPE_WIDTH / 2) + SCOPE_X_START), SCOPE_Y_START, SCOPE_HEIGHT);
  }
  for( y = 35; y < 210; y = y + 40)
  {
    for( x = 40; x < 315; x = x + 40)
    {
      BSP_LCD_DrawPixel( x, y, SCOPE_GRID_COLOR);
    }
  }
  
  /* Display Channel base waveform */
  if( (Channel & SCOPE_ENABLE_CHANNEL1) == SCOPE_ENABLE_CHANNEL1 )
  {
    BSP_LCD_DisplayPicture( 0, (WAVEFORM1_LOW_LEVEL - 3), cOscillo1_offset );
  }
  if( (Channel & SCOPE_ENABLE_CHANNEL2) == SCOPE_ENABLE_CHANNEL2 )
  {
    BSP_LCD_DisplayPicture( 0, (WAVEFORM2_LOW_LEVEL - 3), cOscillo1_offset2 );
  }

}

/** @brief  Display error message.
  * @param  msg pointer to message to display 
  * @return None
  */
void Scope_Error( char *msg )
{
  BSP_LCD_SetBackColor( SCOPE_ERROR_BACK_COLOR );
  BSP_LCD_SetTextColor( SCOPE_ERROR_COLOR );
  BSP_LCD_DisplayStringAt( 0, LINE(Line6), (uint8_t *)msg, CENTER_MODE);
  HAL_Delay( 3000 );
}

/************************ (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
