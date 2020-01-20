/**
* @file    SDK_EVAL_SPI_Driver.c
* @author  AAS,AMS
* @version V1.0.1
* @date    March 10, 2014
* @brief   This file provides all the low level SPI API to access to BlueNRG module
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
*
*/

/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Config.h"
#include "clock.h" 

/** @addtogroup SDK_EVAL_STM32L
* @{
*/

/** @addtogroup SDK_EVAL_Spi                    SDK EVAL Spi
* @brief SPI functions implementation.
* @details This file implements the BlueNRG Library SPI interface functions. 
* @{
*/


/** @defgroup SPI_Private_TypesDefinitions
* @{
*/
/* Added define for enabling BlueNRG SPI fix */
#ifdef CUT_3_1
#undef  ENABLE_SPI_FIX
#define ENABLE_SPI_FIX 0
#else
#ifndef ENABLE_SPI_FIX
#define ENABLE_SPI_FIX 1
#endif 
#endif

/**
* @}
*/

/** @defgroup SPI_Private_Variables
* @{
*/

#ifndef USER_DEFINED_PLATFORM

/* SPIs list */
static SPI_TypeDef* s_SpiPortVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_NB, SDK_EVAL_V3_SPI_PERIPH_NB};

/* SPI CS Ports list*/
GPIO_TypeDef* s_SpiCsPortVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_CS_PORT, SDK_EVAL_V3_SPI_PERIPH_CS_PORT};
/* SPI CS Pins list */
static const uint32_t s_SpiCsPinVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_CS_PIN, SDK_EVAL_V3_SPI_PERIPH_CS_PIN};

/* SPI Reset Ports list*/
GPIO_TypeDef* s_SpiRstPortVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SW_RST_GPIO_PORT, SDK_EVAL_V3_SW_RST_GPIO_PORT};
/* SPI Reset Pins list */
static const uint32_t s_SpiRstPinVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SW_RST_PIN, SDK_EVAL_V3_SW_RST_PIN};

/* SPI MOSI Ports list*/
GPIO_TypeDef* s_SpiMosiPortVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_MOSI_PORT, SDK_EVAL_V3_SPI_PERIPH_MOSI_PORT};
/* SPI MOSI Pins list */
static const uint32_t s_SpiMosiPinVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_MOSI_PIN, SDK_EVAL_V3_SPI_PERIPH_MOSI_PIN};

/* SPI MOSI AF Pin Source list */
static uint16_t s_SpiMosiPinSourceVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_MOSI_RCC_SOURCE, SDK_EVAL_V3_SPI_PERIPH_MOSI_RCC_SOURCE};

/* SPI MOSI AF list */
static uint8_t s_SpiMosiAFVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_MOSI_AF ,SDK_EVAL_V3_SPI_PERIPH_MOSI_AF};

/* SPI MISO Ports list*/
GPIO_TypeDef* s_SpiMisoPortVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_MISO_PORT, SDK_EVAL_V3_SPI_PERIPH_MISO_PORT};
/* SPI MISO Pins list */
static const uint32_t s_SpiMisoPinVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_MISO_PIN, SDK_EVAL_V3_SPI_PERIPH_MISO_PIN};

/* SPI MISO AF Pin Source list */
static uint16_t s_SpiMisoPinSourceVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_MISO_RCC_SOURCE,SDK_EVAL_V3_SPI_PERIPH_MISO_RCC_SOURCE};

/* SPI MISO AF list */
static uint8_t s_SpiMisoAFVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_MISO_AF,SDK_EVAL_V3_SPI_PERIPH_MISO_AF};


/* SPI SCLK Ports list*/
GPIO_TypeDef* s_SpiSclkPortVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_SCLK_PORT, SDK_EVAL_V3_SPI_PERIPH_SCLK_PORT};
/* SPI SCLK Pins list */
static const uint32_t s_SpiSclkPinVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_SCLK_PIN, SDK_EVAL_V3_SPI_PERIPH_SCLK_PIN};


/* SPI SCLK AF Pin Source list */
static uint16_t s_SpiSclkPinSourceVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_SCLK_RCC_SOURCE ,SDK_EVAL_V3_SPI_PERIPH_SCLK_RCC_SOURCE};

/* SPI SCLK AF list */
static uint8_t s_SpiSclkAFVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_SCLK_AF ,SDK_EVAL_V3_SPI_PERIPH_SCLK_AF};

/* SPI IRQ lists */
GPIO_TypeDef* s_SpiIrqPortVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_IRQ_PORT, SDK_EVAL_VD1_SPI_PERIPH_IRQ_PORT};

static const uint32_t s_SpiIrqPinVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_SPI_PERIPH_IRQ_PIN, SDK_EVAL_VD1_SPI_PERIPH_IRQ_PIN};

static const uint32_t s_SpiIrqClkVersion[SDK_PLATFORMS_NUMBER]= {SDK_EVAL_V3_SPI_PERIPH_IRQ_RCC, SDK_EVAL_VD1_SPI_PERIPH_IRQ_RCC};

static const uint32_t s_SpiIrqExtiLineVersion[SDK_PLATFORMS_NUMBER]= {SDK_EVAL_V3_EXTI_LINE, SDK_EVAL_VD1_EXTI_LINE};

static const uint8_t s_SpiIrqPortSourceVersion[SDK_PLATFORMS_NUMBER]= {SDK_EVAL_V3_EXTI_PORT_SOURCE, SDK_EVAL_VD1_EXTI_PORT_SOURCE};

static const uint8_t s_SpiIrqPinSourceVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_EXTI_PIN_SOURCE, SDK_EVAL_VD1_EXTI_PIN_SOURCE};

static const IRQn_Type s_SpiIrqIrqnVersion[SDK_PLATFORMS_NUMBER] = {SDK_EVAL_V3_EXTI_IRQn, SDK_EVAL_VD1_EXTI_IRQn}; 

/* SPI Port */
static SPI_TypeDef* s_SpiPort;


/* SPI CS Port */
static GPIO_TypeDef* s_SpiCsPort;
/* SPI CS Pin */
static uint32_t s_SpiCsPin;


/* SPI Reset Port */
static GPIO_TypeDef* s_SpiRstPort;
/* SPI Reset Pin */
static uint32_t s_SpiRstPin;

/* SPI MOSI Port */
static GPIO_TypeDef* s_SpiMosiPort;
/* SPI MOSI Pin */
static uint32_t s_SpiMosiPin;

/* SPI MOSI AF Pin Source */
static uint16_t s_SpiMosiPinSource;

/* SPI MOSI AF */
static uint8_t s_SpiMosiAF;

/* SPI MISO Port */
static GPIO_TypeDef* s_SpiMisoPort;
/* SPI MISO Pin */
static uint32_t s_SpiMisoPin;

/* SPI MISO AF Pin Source */
static uint16_t s_SpiMisoPinSource;

/* SPI MISO AF */
static uint8_t s_SpiMisoAF;

/* SPI SCLK Port */
static GPIO_TypeDef* s_SpiSclkPort;
/* SPI SCLK Pin */
static uint32_t s_SpiSclkPin;

/* SPI SCLK AF Pin Source */
static uint16_t s_SpiSclkPinSource;

/* SPI SCLK AF */
static uint8_t s_SpiSclkAF;

/* SPI IRQ pin, exti line, port source, pin source, irqn */
/* SPI clock */
static uint32_t s_SpiIrqClk; 
static GPIO_TypeDef* s_SpiIrqPort;
static uint32_t s_SpiIrqPin;
static uint32_t s_SpiIrqExtiLine;
static uint8_t s_SpiIrqPortSource;
static uint8_t s_SpiIrqPinSource;
static IRQn_Type s_SpiIrqIrqn;

 #endif /* #ifndef USER_DEFINED_PLATFORM */
 
/**
* @}
*/

/** @defgroup SPI_Functions
* @{
*/


/* SPI Platform Configuration Values: BlueNRG Development Kits Run-time support or simplified user defined platform support */

/* SPI Port*/
static SPI_TypeDef* Get_SpiPort()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI);
#else
  return(s_SpiPort);
#endif
}

/* SPI CS Port */
static GPIO_TypeDef* Get_SpiCsPort()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_CS_GPIO_PORT);
#else
  return(s_SpiCsPort);
#endif
}

/* SPI CS Pin */
static uint32_t Get_SpiCsPin()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_CS_GPIO_PIN);
#else
  return(s_SpiCsPin);
#endif
}

/* SPI Reset Port */
static GPIO_TypeDef* Get_SpiRstPort()
{
#ifdef USER_DEFINED_PLATFORM
  return(SW_RST_GPIO_PORT);
#else
  return(s_SpiRstPort);
#endif
}

/* SPI Reset Pin */
static uint32_t Get_SpiRstPin()
{
#ifdef USER_DEFINED_PLATFORM
  return(SW_RST_GPIO_PIN);
#else
  return(s_SpiRstPin);
#endif
}

/* SPI MOSI Port */
static GPIO_TypeDef* Get_SpiMosiPort()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_MOSI_GPIO_PORT);
#else
  return(s_SpiMosiPort);
#endif
}

/* SPI MOSI Pin */
static uint32_t Get_SpiMosiPin()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_MOSI_GPIO_PIN);
#else
  return(s_SpiMosiPin);
#endif
}

/* SPI MOSI AF Pin Source */
static uint16_t Get_SpiMosiPinSource()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_MOSI_GPIO_SOURCE);
#else
  return(s_SpiMosiPinSource);
#endif
}

/* SPI MOSI AF */
static uint8_t Get_SpiMosiAF()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_MOSI_GPIO_AF);
#else
  return(s_SpiMosiAF);
#endif
}

/* SPI MISO Port */
static GPIO_TypeDef* Get_SpiMisoPort()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_MISO_GPIO_PORT);
#else
  return(s_SpiMisoPort);
#endif
}

/* SPI MISO Pin */
static uint32_t Get_SpiMisoPin()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_MISO_GPIO_PIN);
#else
  return(s_SpiMisoPin);
#endif
}

/* SPI MISO AF Pin Source */
static uint16_t Get_SpiMisoPinSource()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_MISO_GPIO_SOURCE);
#else
  return(s_SpiMisoPinSource);
#endif
}

/* SPI MISO AF */
static uint8_t Get_SpiMisoAF()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_MISO_GPIO_AF);
#else
  return(s_SpiMisoAF);
#endif
}

/* SPI SCLK Port */
static GPIO_TypeDef* Get_SpiSclkPort()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_SCLK_GPIO_PORT);
#else
  return(s_SpiSclkPort);
#endif
}

/* SPI SCLK Pin */
static uint32_t Get_SpiSclkPin()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_SCLK_GPIO_PIN);
#else
  return(s_SpiSclkPin);
#endif
}

/* SPI SCLK AF Pin Source */
static uint16_t Get_SpiSclkPinSource()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_SCLK_GPIO_SOURCE);
#else
  return(s_SpiSclkPinSource);
#endif
}

/* SPI SCLK AF */
static uint8_t Get_SpiSclkAF()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_SCLK_GPIO_AF);
#else
  return(s_SpiSclkAF);
#endif
}

/* SPI IRQ pin, exti line, port source, pin source, irqn */

/* SPI IRQ Port */
static GPIO_TypeDef* Get_SpiIrqPort()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_IRQ_GPIO_PORT);
#else
  return(s_SpiIrqPort);
#endif
}

/* SPI IRQ Pin */
static uint32_t Get_SpiIrqPin()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_IRQ_GPIO_PIN);
#else
  return(s_SpiIrqPin);
#endif
}
/* SPI IRQ Exti Line */
static uint32_t Get_SpiIrqExtiLine()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_IRQ_EXTI_LINE);
#else
  return(s_SpiIrqExtiLine);
#endif
}

/* SPI IRQ Port Source */
static uint8_t Get_SpiIrqPortSource()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_IRQ_EXTI_PORT_SOURCE); 
#else
  return(s_SpiIrqPortSource);
#endif
}

/* SPI IRQ Pin Source */
static uint8_t Get_SpiIrqPinSource()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_IRQ_EXTI_PIN_SOURCE);
#else
  return(s_SpiIrqPinSource);
#endif
}

/* SPI IRQ line */
static IRQn_Type Get_SpiIrqIrqn()
{
#ifdef USER_DEFINED_PLATFORM
  return(SPI_IRQ_EXTI_IRQn);
#else
  return(s_SpiIrqIrqn);
#endif
}


/**
* @brief  Basic SPI configuration according to the selected platform 
* @param  None
* @retval status
*/
static int8_t SPI_Configure_Platform(void)
{
#ifdef USER_DEFINED_PLATFORM 

  /* SPI Periph clock enable */
#ifdef SPI_CLK_APB1
  RCC_APB1PeriphClockCmd(SPI_CLK_APB1, ENABLE);
#else
  RCC_APB2PeriphClockCmd(SPI_CLK_APB2, ENABLE); 
#endif 
    
  /* Configure SPI Periph Clocks */
  RCC_AHBPeriphClockCmd(SPI_CS_GPIO_CLK | SPI_MOSI_GPIO_CLK | SPI_MISO_GPIO_CLK |
                        SPI_SCLK_GPIO_CLK | SW_RST_GPIO_CLK | SPI_IRQ_GPIO_CLK, ENABLE);

#else /* BlueNRG SPI Driver configuration for BlueNRG Development Kits platforms */

  s_SpiPort = s_SpiPortVersion[SdkEvalGetVersion()];
  
  s_SpiCsPort = s_SpiCsPortVersion[SdkEvalGetVersion()];
  s_SpiCsPin = s_SpiCsPinVersion[SdkEvalGetVersion()];
  
  s_SpiRstPort = s_SpiRstPortVersion[SdkEvalGetVersion()];
  s_SpiRstPin = s_SpiRstPinVersion[SdkEvalGetVersion()];

  s_SpiMosiPort = s_SpiMosiPortVersion[SdkEvalGetVersion()];
  s_SpiMosiPin = s_SpiMosiPinVersion[SdkEvalGetVersion()];

  s_SpiMosiPinSource = s_SpiMosiPinSourceVersion[SdkEvalGetVersion()];
  s_SpiMosiAF = s_SpiMosiAFVersion[SdkEvalGetVersion()];

  s_SpiMisoPort = s_SpiMisoPortVersion[SdkEvalGetVersion()];
  s_SpiMisoPin = s_SpiMisoPinVersion[SdkEvalGetVersion()];

  s_SpiMisoPinSource = s_SpiMisoPinSourceVersion[SdkEvalGetVersion()];
  s_SpiMisoAF = s_SpiMisoAFVersion[SdkEvalGetVersion()];

  s_SpiSclkPort = s_SpiSclkPortVersion[SdkEvalGetVersion()];
  s_SpiSclkPin = s_SpiSclkPinVersion[SdkEvalGetVersion()];

  s_SpiSclkPinSource = s_SpiSclkPinSourceVersion[SdkEvalGetVersion()];
  s_SpiSclkAF = s_SpiSclkAFVersion[SdkEvalGetVersion()];

  s_SpiIrqClk = s_SpiIrqClkVersion[SdkEvalGetVersion()];
  
  s_SpiIrqPort = s_SpiIrqPortVersion[SdkEvalGetVersion()];
  s_SpiIrqPin = s_SpiIrqPinVersion[SdkEvalGetVersion()];
  
  s_SpiIrqExtiLine = s_SpiIrqExtiLineVersion[SdkEvalGetVersion()];
  
  s_SpiIrqPortSource = s_SpiIrqPortSourceVersion[SdkEvalGetVersion()];
  s_SpiIrqPinSource = s_SpiIrqPinSourceVersion[SdkEvalGetVersion()];
  
  s_SpiIrqIrqn = s_SpiIrqIrqnVersion[SdkEvalGetVersion()]; 
  
  if((SdkEvalGetVersion() == SDK_EVAL_VERSION_3) || (SdkEvalGetVersion() == SDK_EVAL_VERSION_D1))
  {
    /* Enable SPI2 periph and SCLK, MOSI, MISO and CS GPIO clocks */
    RCC_APB1PeriphClockCmd(SDK_EVAL_V3_SPI_PERIPH_RCC, ENABLE);     
    
    RCC_AHBPeriphClockCmd(SDK_EVAL_V3_SPI_PERIPH_CS_RCC | SDK_EVAL_V3_SPI_PERIPH_MOSI_RCC | SDK_EVAL_V3_SPI_PERIPH_MISO_RCC |
                          SDK_EVAL_V3_SPI_PERIPH_SCLK_RCC | SDK_EVAL_V3_SW_RST_GPIO_RCC | s_SpiIrqClk, ENABLE);
  }
  else
  {
    /* Platform is not supported */
    return(-1); 
  }  

#endif /* USER_DEFINED_PLATFORM */

  return(0);
}/* end SPI_Configure_Platform() */

/**
* @brief  Return SPI IRQ pin value
* @param  None
* @retval SPI IRQ pin value
*/
uint8_t SdkEvalSPI_Irq_Pin(void) 
{
  return (GPIO_ReadInputDataBit(Get_SpiIrqPort(), Get_SpiIrqPin()));
}/* end SdkEvalSPI_Irq_Pin() */

/**
* @brief  Initializes the SPI 
* @param  xSpiMode: SPI mode - GPIO (polling) or EXTI (IRQ)
* @retval status
*/
int8_t SdkEvalSpiInit(SdkEvalSpiMode xSpiMode) 
{
  int8_t retValue; 
  SPI_InitTypeDef SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure SPI initialization parameters depending on the selected 
     platform: BlueNRG EVal Kits or User Defined platforms */
  retValue = SPI_Configure_Platform();
  if (retValue == -1)
    /* BlueNRG Development Kit platfrom is not supported */
    return (retValue);

  /* Configure the AF for MOSI, MISO and SCLK GPIO pins */
  GPIO_PinAFConfig(Get_SpiMosiPort(), Get_SpiMosiPinSource(), Get_SpiMosiAF());
  GPIO_PinAFConfig(Get_SpiMisoPort(), Get_SpiMisoPinSource(), Get_SpiMisoAF());
  GPIO_PinAFConfig(Get_SpiSclkPort(), Get_SpiSclkPinSource(), Get_SpiSclkAF());

  /*!< Configure BlueNRG reset line */
  GPIO_WriteBit(Get_SpiRstPort(),Get_SpiRstPin(), Bit_SET);
  GPIO_InitStructure.GPIO_Pin = Get_SpiRstPin();
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(Get_SpiRstPort(), &GPIO_InitStructure);
  BlueNRG_Power_Down(); //Added to avoid spurious interrupt from the BlueNRG

  /*!< Configure SPI pins: SCLK */
  GPIO_InitStructure.GPIO_Pin = Get_SpiSclkPin();
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(Get_SpiSclkPort(), &GPIO_InitStructure);

  /*!< Configure SPI pins: MISO */
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Pin = Get_SpiMisoPin();
  GPIO_Init(Get_SpiMisoPort(), &GPIO_InitStructure);

  /*!< Configure SPI pins: MOSI */
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = Get_SpiMosiPin();
  GPIO_Init(Get_SpiMosiPort(), &GPIO_InitStructure);

  /*!< Configure SPI pins: CS pin */
  GPIO_InitStructure.GPIO_Pin = Get_SpiCsPin();
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    
  GPIO_WriteBit(Get_SpiCsPort(), Get_SpiCsPin(), Bit_SET);
    
  GPIO_Init(Get_SpiCsPort(), &GPIO_InitStructure);
  
  /*!< Configure SPI pins: IRQ pin */
  // Configuration for BlueNRG IRQ pin (5-wire SPI interface)
  GPIO_InitStructure.GPIO_Pin = Get_SpiIrqPin();
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(Get_SpiIrqPort(), &GPIO_InitStructure);
  
  /*!< SPI Config */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
#if SYSCLK_FREQ > 20000000
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
#else
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
#endif

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(Get_SpiPort(), &SPI_InitStructure);
  
  SPI_Cmd(Get_SpiPort(), ENABLE); /*!< SPI enable */
  
  if (xSpiMode == SPI_MODE_EXTI)
  {

    SYSCFG_EXTILineConfig(Get_SpiIrqPortSource(), Get_SpiIrqPinSource());
 
    EXTI_InitStructure.EXTI_Line = Get_SpiIrqExtiLine();
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;    
    EXTI_Init(&EXTI_InitStructure);
  
    EXTI_ClearFlag(Get_SpiIrqExtiLine());
  
    NVIC_InitStructure.NVIC_IRQChannel = Get_SpiIrqIrqn();
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  }
  return(0);
}/* end SdkEvalSpiInit() */

/**
  * @brief  Set GPIO pin in Hi-Z state.
  * @param  GPIOx: GPIO port
  *         GPIO_Pin: GPIo pin        
  * @retval None
  */
static void GPIO_SetHiZ(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Configure GPIO pin as output open drain HI-Z state */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOx, &GPIO_InitStructure);
  GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET);
}/* end GPIO_SetHiZ() */

#if ENABLE_SPI_FIX
/**
  * @brief  Utility function for delay
  * @param  None 
  * @retval None
  * NOTE: TODO: implement with clock-independent function.
  */
static void us150Delay()
{
#if SYSCLK_FREQ == 4000000
for(volatile int i = 0; i < 35; i++)__NOP();
#elif SYSCLK_FREQ == 32000000
for(volatile int i = 0; i < 420; i++)__NOP();
#else
#error Implement delay function.
#endif    
}
#endif /* end ENABLE_SPI_FIX */

/**
  * @brief  Disable SPI
  * @param  None
  * @retval None
  */
void SdkEvalSpiDisable(void)
{
  EXTI_InitTypeDef  EXTI_InitStructure;
  
#ifdef USER_DEFINED_PLATFORM 

  /* SPI Periph clock enable */
#ifdef SPI_CLK_APB1
  RCC_APB1PeriphClockCmd(SPI_CLK_APB1, DISABLE);
#else
  RCC_APB2PeriphClockCmd(SPI_CLK_APB2, DISABLE);
#endif 
    
#else /* BlueNRG SPI Driver configuration for BlueNRG Development Kits platforms */

  if((SdkEvalGetVersion() == SDK_EVAL_VERSION_3) || (SdkEvalGetVersion() == SDK_EVAL_VERSION_D1))
  {
    /*!< SPI Periph clock disable */
    RCC_APB1PeriphClockCmd(SDK_EVAL_V3_SPI_PERIPH_RCC, DISABLE);  
  }
  
#endif

  SPI_Cmd(Get_SpiPort(), DISABLE); /*!< SPI disable */ 
  
  /* Configure pins as output open drain HI-Z state */
  GPIO_SetHiZ(Get_SpiSclkPort(), Get_SpiSclkPin());
  GPIO_SetHiZ(Get_SpiMosiPort(), Get_SpiMosiPin());
  GPIO_SetHiZ(Get_SpiMisoPort(), Get_SpiMisoPin());
  GPIO_SetHiZ(Get_SpiCsPort(), Get_SpiCsPin());
  
  /* TO be done: only when EXTI_MODE is done at init time ? */
  EXTI_InitStructure.EXTI_Line = Get_SpiIrqExtiLine();
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);
}/* end SdkEvalSpiDisable() */

/**
  * @brief  Reset the BlueNRG
  * @param  None
  * @retval None
  */
void BlueNRG_RST(void)
{   
  GPIO_WriteBit(Get_SpiRstPort(), Get_SpiRstPin(), Bit_RESET);
  /* 5 ms delay */
  Clock_Wait(5);
  GPIO_WriteBit(Get_SpiRstPort(), Get_SpiRstPin(), Bit_SET);
  Clock_Wait(5);  
}/* end BlueNRG_RST() */

/**
  * @brief  Power down the BlueNRG (reset).
  * @param  None
  * @retval None
  */
void BlueNRG_Power_Down(void)
{
  GPIO_WriteBit(Get_SpiRstPort(), Get_SpiRstPin(), Bit_RESET);  

}/* end BlueNRG_Power_Down() */

/**
  * @brief  Power up the BlueNRG (release reset).
  * @param  None
  * @retval None
  */
void BlueNRG_Power_Up(void)
{
  GPIO_WriteBit(Get_SpiRstPort(), Get_SpiRstPin(), Bit_SET);
  Clock_Wait(5);
}/* end BlueNRG_Power_Up() */

/**
  * @brief  Reports if the BlueNRG has data for the host micro.
  * @param  None
  * @retval TRUE if data are present, FALSE otherwise
  */
uint8_t BlueNRG_DataPresent(void)
{
  if (GPIO_ReadInputDataBit(Get_SpiIrqPort(), Get_SpiIrqPin()) == SET)
      return TRUE;
  else  
      return FALSE;
} /* end BlueNRG_DataPresent() */

/**
* @brief  Disable SPI IRQ
* @param  None
* @retval None
*/
void Disable_SPI_IRQ(void)
{  
  NVIC_DisableIRQ(Get_SpiIrqIrqn());
}/* end Disable_SPI_IRQ() */

/**
* @brief  Enable SPI IRQ
* @param  None
* @retval None
*/
void Enable_SPI_IRQ(void)
{  
  NVIC_EnableIRQ(Get_SpiIrqIrqn());
}/* end Enable_SPI_IRQ() */

/**
* @brief  Clear Pending SPI IRQ
* @param  None
* @retval None
*/
void Clear_SPI_IRQ(void)
{
    NVIC_ClearPendingIRQ(Get_SpiIrqIrqn());    
}/* end Clear_SPI_IRQ() */

/**
* @brief  Clear Exti line for SPI IRQ
* @param  None
* @retval None
*/
void Clear_SPI_EXTI_Flag()
{
  EXTI_ClearFlag(Get_SpiIrqExtiLine());
}/* end Clear_SPI_EXTI_Flag() */

/**
  * @brief  Activate internal bootloader using pin.
  * @param  None
  * @retval None
  */
void BlueNRG_HW_Bootloader(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    Disable_SPI_IRQ();
    
    GPIO_InitStructure.GPIO_Pin = Get_SpiIrqPin();
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(Get_SpiIrqPort(), &GPIO_InitStructure);
    GPIO_WriteBit(Get_SpiIrqPort(), Get_SpiIrqPin(), Bit_SET);
      
    GPIO_WriteBit(Get_SpiRstPort(), Get_SpiRstPin(), Bit_RESET);
    /* 5 ms delay */
    Clock_Wait(5);
    GPIO_WriteBit(Get_SpiRstPort(), Get_SpiRstPin(), Bit_SET);
    Clock_Wait(5);
    
    /* Restore port configuration. */    
    GPIO_InitStructure.GPIO_Pin = Get_SpiIrqPin();
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(Get_SpiIrqPort(), &GPIO_InitStructure);
    
    Enable_SPI_IRQ();
}

/**
* @brief  Read from BlueNRG SPI buffer and store data into local buffer 
* @param  buffer:    buffer where data from SPI are stored
*         buff_size: buffer size
* @retval number of read bytes
*/
int32_t BlueNRG_SPI_Read_All(uint8_t *buffer, uint8_t buff_size)
{
    uint16_t byte_count;
    uint8_t i = 0;
    uint8_t len = 0;
    
    
    uint8_t header_master[5] = {0x0b, 0x00, 0x00, 0x00, 0x00};
    uint8_t header_slave[5];
    
    __disable_irq();
    
    GPIO_WriteBit(Get_SpiCsPort(), Get_SpiCsPin(), Bit_RESET);
    
    /* Read the header */
    for (i = 0; i < 5; i++)
    { 
        SPI_I2S_SendData(Get_SpiPort(), header_master[i]); 
        while(SPI_I2S_GetFlagStatus(Get_SpiPort(), SPI_I2S_FLAG_TXE) == RESET);
        while(SPI_I2S_GetFlagStatus(Get_SpiPort(), SPI_I2S_FLAG_RXNE) == RESET);
        header_slave[i] = SPI_I2S_ReceiveData(Get_SpiPort());
    }
    
    if (header_slave[0] == 0x02) {
        // device is ready
        
        byte_count = (header_slave[4]<<8)|header_slave[3];
        
        if (byte_count > 0) {
            
            // avoid to read more data that size of the buffer
            if (byte_count > buff_size)
                byte_count = buff_size;
            
            for (len = 0; len < byte_count; len++)
            { 
                SPI_I2S_SendData(Get_SpiPort(), 0xFF); 
                while(SPI_I2S_GetFlagStatus(Get_SpiPort(), SPI_I2S_FLAG_TXE) == RESET);
                while(SPI_I2S_GetFlagStatus(Get_SpiPort(), SPI_I2S_FLAG_RXNE) == RESET);
                buffer[len] = SPI_I2S_ReceiveData(Get_SpiPort());
            }
        }    
    }
    
    // Release CS line.
    GPIO_WriteBit(Get_SpiCsPort(), Get_SpiCsPin(), Bit_SET);
    
    __enable_irq();
    
    return len;
    
}/* end BlueNRG_SPI_Read_All() */


/**
* @brief  Write data from local buffer to SPI
* @param  data1:    first data buffer to be written
*         data2:    second data buffer to be written
*         Nb_bytes1: size of first data buffer to be written
*         Nb_bytes2: size of second data buffer to be written
* @retval number of read bytes
*/
int32_t BlueNRG_SPI_Write(uint8_t* data1, uint8_t* data2, uint8_t Nb_bytes1, uint8_t Nb_bytes2)
{  
    uint32_t i;
    int32_t result = 0;
    
    unsigned char header_master[5] = {0x0a, 0x00, 0x00, 0x00, 0x00};
    unsigned char header_slave[5]  = {0xaa, 0x00, 0x00, 0x00, 0x00};
    
    Disable_SPI_IRQ();
    
#if ENABLE_SPI_FIX
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    /* Pull IRQ high */
    GPIO_InitStructure.GPIO_Pin = Get_SpiIrqPin();
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(Get_SpiIrqPort(), &GPIO_InitStructure);
    GPIO_WriteBit(Get_SpiIrqPort(), Get_SpiIrqPin(), Bit_SET);
    /* Assert CS line after at least 112us */
    us150Delay();    
#endif
    
    GPIO_WriteBit(Get_SpiCsPort(), Get_SpiCsPin(), Bit_RESET);
    
    for (i = 0; i < 5; i++)
    { 
        SPI_I2S_SendData(Get_SpiPort(), header_master[i]); 
        while(SPI_I2S_GetFlagStatus(Get_SpiPort(), SPI_I2S_FLAG_TXE) == RESET);
        while(SPI_I2S_GetFlagStatus(Get_SpiPort(), SPI_I2S_FLAG_RXNE) == RESET);
        header_slave[i] = SPI_I2S_ReceiveData(Get_SpiPort());
    }
    
#if ENABLE_SPI_FIX
    /* IRQ input again */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(Get_SpiIrqPort(), &GPIO_InitStructure);
#endif
    
    if (header_slave[0] == 0x02) {
        // SPI is ready
        if (header_slave[1] >= (Nb_bytes1+Nb_bytes2)) {
            //  Buffer is big enough
            for (i = 0; i < Nb_bytes1; i++) { 
                SPI_I2S_SendData(Get_SpiPort(), *(data1 + i)); 
                while(SPI_I2S_GetFlagStatus(Get_SpiPort(), SPI_I2S_FLAG_TXE) == RESET);
                while(SPI_I2S_GetFlagStatus(Get_SpiPort(), SPI_I2S_FLAG_RXNE) == RESET);
                SPI_I2S_ReceiveData(Get_SpiPort());
            }
            for (i = 0; i < Nb_bytes2; i++) { 
                SPI_I2S_SendData(Get_SpiPort(), *(data2 + i)); 
                while(SPI_I2S_GetFlagStatus(Get_SpiPort(), SPI_I2S_FLAG_TXE) == RESET);
                while(SPI_I2S_GetFlagStatus(Get_SpiPort(), SPI_I2S_FLAG_RXNE) == RESET);
                SPI_I2S_ReceiveData(Get_SpiPort());
            }
        } else {
            // Buffer is too small
            result = -2;
        }
    } else {
        // SPI is not ready
        result = -1;
    }
    
    // Release CS line.
    GPIO_WriteBit(Get_SpiCsPort(), Get_SpiCsPin(), Bit_SET);
    
    Enable_SPI_IRQ();
    
    return result;
}/* end BlueNRG_SPI_Write() */

/**
* @}
*/


/**
* @}
*/


/**
* @}
*/



/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/
