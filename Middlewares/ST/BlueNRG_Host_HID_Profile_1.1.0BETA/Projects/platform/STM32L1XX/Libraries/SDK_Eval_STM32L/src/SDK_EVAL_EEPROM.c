/**
 * @file    SDK_EVAL_EEPROM.c
 * @author  AMS - AAS Division
 * @version V1.0.0
 * @date    February 7, 2014
 * @brief   SDK EVAL eeprom management
 * @details
 *
 * This module exports API to manage the EEPROM.
 * The following example shows how to configure the shutdown pin
 * and the GPIO 3 as an EXTI input.
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

/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Config.h"


/**
* @addtogroup SDK_EVAL_STM32L
* @{
*/


/**
* @defgroup SDK_EEPROM              SDK EEPROM Management
* @{
*/


/** @defgroup SPI_Private_Functions
* @{
*/

#ifndef USER_DEFINED_PLATFORM

static SPI_TypeDef* s_EepromSpiPortVersion[SDK_PLATFORMS_NUMBER] = {EEPROM_V3_SPI_PERIPH_NB, EEPROM_V3_SPI_PERIPH_NB};
GPIO_TypeDef* s_vectpxEepromSpiCsPortVersion[SDK_PLATFORMS_NUMBER] = { EEPROM_V3_SPI_PERIPH_CS_PORT, EEPROM_VD1_SPI_PERIPH_CS_PORT };

static const uint32_t s_vectpxEepromSpiCsPinVersion[SDK_PLATFORMS_NUMBER] = { EEPROM_V3_SPI_PERIPH_CS_PIN, EEPROM_VD1_SPI_PERIPH_CS_PIN};
static const uint32_t s_vectpxEepromSpiCsClkVersion[SDK_PLATFORMS_NUMBER]= {EEPROM_V3_SPI_PERIPH_CS_RCC, EEPROM_VD1_SPI_PERIPH_CS_RCC};

GPIO_TypeDef* s_vectpxEepromSpiMosiPortVersion[SDK_PLATFORMS_NUMBER] = { EEPROM_V3_SPI_PERIPH_MOSI_PORT, EEPROM_V3_SPI_PERIPH_MOSI_PORT};

static const uint32_t s_vectpxEepromSpiMosiPinVersion[SDK_PLATFORMS_NUMBER] = { EEPROM_V3_SPI_PERIPH_MOSI_PIN, EEPROM_V3_SPI_PERIPH_MOSI_PIN};

/* SPI MOSI AF Pin Source list */
static uint16_t s_vectpxEepromSpiMosiPinSourceVersion[SDK_PLATFORMS_NUMBER] = {EEPROM_V3_SPI_PERIPH_MOSI_RCC_SOURCE, EEPROM_V3_SPI_PERIPH_MOSI_RCC_SOURCE};

/* SPI MOSI AF list */
static uint8_t s_vectpxEepromSpiMosiAFVersion[SDK_PLATFORMS_NUMBER] = {EEPROM_V3_SPI_PERIPH_MOSI_AF ,EEPROM_V3_SPI_PERIPH_MOSI_AF};

GPIO_TypeDef* s_vectpxEepromSpiMisoPortVersion[SDK_PLATFORMS_NUMBER] = { EEPROM_V3_SPI_PERIPH_MISO_PORT, EEPROM_V3_SPI_PERIPH_MISO_PORT};

static const uint32_t s_vectpxEepromSpiMisoPinVersion[SDK_PLATFORMS_NUMBER] = { EEPROM_V3_SPI_PERIPH_MISO_PIN, EEPROM_V3_SPI_PERIPH_MISO_PIN};

/* SPI MISO AF Pin Source list */
static uint16_t s_vectpxEepromSpiMisoPinSourceVersion[SDK_PLATFORMS_NUMBER] = {EEPROM_V3_SPI_PERIPH_MISO_RCC_SOURCE ,EEPROM_V3_SPI_PERIPH_MISO_RCC_SOURCE };

/* SPI MISO AF list */
static uint8_t s_vectpxEepromSpiMisoAFVersion[SDK_PLATFORMS_NUMBER] = {EEPROM_V3_SPI_PERIPH_MISO_AF,EEPROM_V3_SPI_PERIPH_MISO_AF};

GPIO_TypeDef* s_vectpxEepromSpiSclkPortVersion[SDK_PLATFORMS_NUMBER] = { EEPROM_V3_SPI_PERIPH_SCLK_PORT, EEPROM_V3_SPI_PERIPH_SCLK_PORT};

static const uint32_t s_vectpxEepromSpiSclkPinVersion[SDK_PLATFORMS_NUMBER] = { EEPROM_V3_SPI_PERIPH_SCLK_PIN, EEPROM_V3_SPI_PERIPH_SCLK_PIN};

/* SPI SCLK AF Pin Source list */
static uint16_t s_vectpxEepromSpiSclkPinSourceVersion[SDK_PLATFORMS_NUMBER] = {EEPROM_V3_SPI_PERIPH_SCLK_RCC_SOURCE ,EEPROM_V3_SPI_PERIPH_SCLK_RCC_SOURCE};

/* SPI SCLK AF list */
static uint8_t s_vectpxEepromSpiSclkAFVersion[SDK_PLATFORMS_NUMBER] = {EEPROM_V3_SPI_PERIPH_SCLK_AF ,EEPROM_V3_SPI_PERIPH_SCLK_AF};

static SPI_TypeDef* s_EepromSpiPort;
static GPIO_TypeDef* s_vectpxEepromSpiCsPort;
static uint32_t s_vectnEepromSpiCsPin;
static uint32_t s_vectnEepromSpiCsClk;
static GPIO_TypeDef* s_vectpxEepromSpiMosiPort;
static uint32_t s_vectnEepromSpiMosiPin;
/* SPI MOSI AF Pin Source */
static uint16_t s_vectpxEepromSpiMosiPinSource;
/* SPI MOSI AF */
static uint8_t s_vectpxEepromSpiMosiAF;
static GPIO_TypeDef* s_vectpxEepromSpiMisoPort;
static uint32_t s_vectnEepromSpiMisoPin;
/* SPI MISO AF Pin Source */
static uint16_t s_vectpxEepromSpiMisoPinSource;
/* SPI MISO AF */
static uint8_t s_vectpxEepromSpiMisoAF;
static GPIO_TypeDef* s_vectpxEepromSpiSclkPort;
static uint32_t s_vectnEepromSpiSclkPin;
/* SPI SCLK AF Pin Source */
static uint16_t s_vectpxEepromSpiSclkPinSource;
/* SPI SCLK AF */
static uint8_t s_vectpxEepromSpiSclkAF;

#endif /* #ifndef USER_DEFINED_PLATFORM */

#define  EepromSPICSHigh(x,y)   do { GPIO_HIGH((x),(y));\
                                for(int i=0; i < 10; i++)__NOP();\
                              }while(0);

#define  EepromSPICSLow(x,y)    do { GPIO_LOW((x),(y));\
                                for(int i=0; i < 10; i++)__NOP();\
                              }while(0);

void EepromWriteEnable(void);


/* SPI Platform Configuration Values: BlueNRG Development Kits Run-time support or simplified user defined platform support */

/* SPI Port*/
static SPI_TypeDef* Get_EepromSpiPort()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_NB);
#else
  return(s_EepromSpiPort);
#endif
}

/* SPI CS Port */
static GPIO_TypeDef* Get_vectEepromSpiCsPort()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_CS_PORT);
#else
  return(s_vectpxEepromSpiCsPort);
#endif
}

/* SPI CS Pin */
static uint32_t Get_vectEepromSpiCsPin()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_CS_PIN);
#else
  return(s_vectnEepromSpiCsPin); 
#endif
}

/* SPI MOSI Port */
static GPIO_TypeDef* Get_vectEepromSpiMosiPort()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_MOSI_PORT);
#else
  return(s_vectpxEepromSpiMosiPort);
#endif
}

/* SPI MOSI Pin */
static uint32_t Get_vectEepromSpiMosiPin()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_MOSI_PIN);
#else
  return(s_vectnEepromSpiMosiPin);
#endif
}

/* SPI MOSI AF Pin Source */
static uint16_t Get_vectEepromSpiMosiPinSource()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_MOSI_RCC_SOURCE);
#else
  return(s_vectpxEepromSpiMosiPinSource);
#endif
}

/* SPI MOSI AF */
static uint8_t Get_vectEepromSpiMosiAF()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_MOSI_AF);
#else
  return(s_vectpxEepromSpiMosiAF);
#endif
}

/* SPI MISO Port */
static GPIO_TypeDef* Get_vectEepromSpiMisoPort()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_MISO_PORT);
#else
  return(s_vectpxEepromSpiMisoPort);
#endif
}

/* SPI MISO Pin */
static uint32_t Get_vectpxEepromSpiMisoPin()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_MISO_PIN);
#else
  return(s_vectnEepromSpiMisoPin);
#endif
}

/* SPI MISO AF Pin Source */
static uint16_t Get_vectEepromSpiMisoPinSource()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_MISO_RCC_SOURCE);
#else
  return(s_vectpxEepromSpiMisoPinSource);
#endif
}

/* SPI MISO AF */
static uint8_t Get_vectEepromSpiMisoAF()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_MISO_AF);
#else
  return(s_vectpxEepromSpiMisoAF);
#endif
}

/* SPI SCLK Port */
static GPIO_TypeDef* Get_vectEepromSpiSclkPort()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_SCLK_PORT);
#else
  return(s_vectpxEepromSpiSclkPort);
#endif
}

/* SPI SCLK Pin */
static uint32_t Get_vectEepromSpiSclkPin()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_SCLK_PIN);
#else
  return(s_vectnEepromSpiSclkPin);
#endif
}

/* SPI SCLK AF Pin Source */
static uint16_t Get_vectEepromSpiSclkPinSource()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_SCLK_RCC_SOURCE);
#else
  return(s_vectpxEepromSpiSclkPinSource);
#endif
}

/* SPI SCLK AF */
static uint8_t Get_vectEepromSpiSclkAF()
{
#ifdef USER_DEFINED_PLATFORM
  return(EEPROM_SPI_PERIPH_SCLK_AF);
#else
  return(s_vectpxEepromSpiSclkAF);
#endif
}


/**
* @brief  Basic SPI configuration according to the selected platform 
* @param  None
* @retval status
*/
static int8_t EepromSpi_Configure_Platform(void)
{
#ifdef USER_DEFINED_PLATFORM 

  /* SPI Periph clock enable */
#ifdef EEPROM_SPI_PERIPH_RCC_APB1
  RCC_APB1PeriphClockCmd(EEPROM_SPI_PERIPH_RCC_APB1, ENABLE);
#else
  RCC_APB2PeriphClockCmd(EEPROM_SPI_PERIPH_RCC_APB2, ENABLE);
#endif 
    
  /* Configure SPI Periph Clocks */
  RCC_AHBPeriphClockCmd(EEPROM_SPI_PERIPH_CS_RCC | EEPROM_SPI_PERIPH_MOSI_RCC_SOURCE |  
                        EEPROM_SPI_PERIPH_MISO_RCC_SOURCE| EEPROM_SPI_PERIPH_SCLK_RCC_SOURCE, ENABLE);

#else /* BlueNRG SPI Driver configuration for BlueNRG Development Kits platforms */

  s_EepromSpiPort = s_EepromSpiPortVersion[SdkEvalGetVersion()];
  s_vectpxEepromSpiCsPort = s_vectpxEepromSpiCsPortVersion[SdkEvalGetVersion()];
  s_vectnEepromSpiCsPin = s_vectpxEepromSpiCsPinVersion[SdkEvalGetVersion()];
  s_vectnEepromSpiCsClk = s_vectpxEepromSpiCsClkVersion[SdkEvalGetVersion()];

  s_vectpxEepromSpiMosiPort = s_vectpxEepromSpiMosiPortVersion[SdkEvalGetVersion()];
  s_vectnEepromSpiMosiPin = s_vectpxEepromSpiMosiPinVersion[SdkEvalGetVersion()];
  
  s_vectpxEepromSpiMosiPinSource = s_vectpxEepromSpiMosiPinSourceVersion[SdkEvalGetVersion()];
  s_vectpxEepromSpiMosiAF = s_vectpxEepromSpiMosiAFVersion[SdkEvalGetVersion()];
 
  s_vectpxEepromSpiMisoPort = s_vectpxEepromSpiMisoPortVersion[SdkEvalGetVersion()];
  s_vectnEepromSpiMisoPin = s_vectpxEepromSpiMisoPinVersion[SdkEvalGetVersion()];  
  
  s_vectpxEepromSpiMisoPinSource = s_vectpxEepromSpiMisoPinSourceVersion[SdkEvalGetVersion()];
  s_vectpxEepromSpiMisoAF = s_vectpxEepromSpiMisoAFVersion[SdkEvalGetVersion()];

  s_vectpxEepromSpiSclkPort = s_vectpxEepromSpiSclkPortVersion[SdkEvalGetVersion()];
  s_vectnEepromSpiSclkPin = s_vectpxEepromSpiSclkPinVersion[SdkEvalGetVersion()];
  
  s_vectpxEepromSpiSclkPinSource = s_vectpxEepromSpiSclkPinSourceVersion[SdkEvalGetVersion()];
  s_vectpxEepromSpiSclkAF = s_vectpxEepromSpiSclkAFVersion[SdkEvalGetVersion()];

  if(SdkEvalGetVersion() == SDK_EVAL_VERSION_3 || SdkEvalGetVersion() == SDK_EVAL_VERSION_D1)
  {      
    /* Enable SPI2 periph and SCLK, MOSI, MISO and CS GPIO clocks */
    RCC_APB1PeriphClockCmd(EEPROM_V3_SPI_PERIPH_RCC, ENABLE);  
    RCC_AHBPeriphClockCmd(EEPROM_V3_SPI_PERIPH_MOSI_RCC | EEPROM_V3_SPI_PERIPH_MISO_RCC | EEPROM_V3_SPI_PERIPH_SCLK_RCC | s_vectnEepromSpiCsClk, ENABLE);
  }
  else
  {
    /* Platform is not supported */
    return(-1); 
  }

#endif /* USER_DEFINED_PLATFORM */

  return(0);
}/* end EepromSpi_Configure_Platform() */

/**
* @brief  Initializes the SPI for the EEPROM.
*         SPI, MISO, MOSI and SCLK are the same used for the BlueNRG.
*         This function can be replaced by EepromCsPinInitialization() if
*         SdkEvalSpiInit() is called.
* @param  None
* @retval Status 
*/
int8_t EepromSpiInitialization(void)
{ 
  int8_t retValue; 
  SPI_InitTypeDef SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure Eeprom SPI initialization parameters depending on the selected 
     platform: BlueNRG EVal Kits or User Defined platforms */
  retValue = EepromSpi_Configure_Platform();
  if (retValue == -1)
    /* BlueNRG Development Kit platfrom is not supported */
    return (retValue);
  
  /* Configure the AF for MOSI, MISO and SCLK GPIO pins */
  GPIO_PinAFConfig(Get_vectEepromSpiMosiPort(), Get_vectEepromSpiMosiPinSource(), Get_vectEepromSpiMosiAF());
  GPIO_PinAFConfig(Get_vectEepromSpiMisoPort(), Get_vectEepromSpiMisoPinSource(), Get_vectEepromSpiMisoAF());
  GPIO_PinAFConfig(Get_vectEepromSpiSclkPort(), Get_vectEepromSpiSclkPinSource(), Get_vectEepromSpiSclkAF());
  
  /* Configure SPI pins:SCLK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    
  GPIO_InitStructure.GPIO_Pin = Get_vectEepromSpiSclkPin();
  GPIO_Init(Get_vectEepromSpiSclkPort(), &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = Get_vectpxEepromSpiMisoPin();
  GPIO_Init(Get_vectEepromSpiMisoPort(), &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = Get_vectEepromSpiMosiPin();
  GPIO_Init(Get_vectEepromSpiMosiPort(), &GPIO_InitStructure);

  /* Configure SPI pin: CS */
  GPIO_InitStructure.GPIO_Pin = Get_vectEepromSpiCsPin();
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  
  GPIO_WriteBit(Get_vectEepromSpiCsPort(), Get_vectEepromSpiCsPin(), Bit_SET); 
  GPIO_Init(Get_vectEepromSpiCsPort(), &GPIO_InitStructure);
  
  /* Configure SPI peripheral */
  SPI_DeInit(Get_EepromSpiPort());
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(Get_EepromSpiPort(), &SPI_InitStructure);
  
  SPI_Cmd(Get_EepromSpiPort(), ENABLE);
  
  EepromSPICSHigh(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin()); /* TBR */
  
  return(0);
  
}/* end EepromSpiInitialization() */


/**
* @brief  Initialization of the CSn pin of the EEPROM.
*         SPI, MISO, MOSI and SCLK are the same used for the BlueNRG.
*         This function can be replaced by EepromCsPinInitialization() if
*         SdkEvalSpiInit() is called.
* @param  None
* @retval None
*/
void EepromCsPinInitialization(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
#ifdef USER_DEFINED_PLATFORM 
   
  /* Configure SPI Periph Clock x CS line*/
  RCC_AHBPeriphClockCmd(EEPROM_SPI_PERIPH_CS_RCC, ENABLE);

#else /* BlueNRG SPI Driver configuration for BlueNRG Development Kits platforms */
  
  s_EepromSpiPort = s_EepromSpiPortVersion[SdkEvalGetVersion()];
  s_vectpxEepromSpiCsPort = s_vectpxEepromSpiCsPortVersion[SdkEvalGetVersion()];
  s_vectnEepromSpiCsPin = s_vectpxEepromSpiCsPinVersion[SdkEvalGetVersion()];
  s_vectnEepromSpiCsClk = s_vectpxEepromSpiCsClkVersion[SdkEvalGetVersion()];

  /* Configure SPI Periph Clock x CS line*/
  RCC_AHBPeriphClockCmd(s_vectnEepromSpiCsClk, ENABLE);
  
#endif /* USER_DEFINED_PLATFORM */
  
  /* Configure SPI pin: CS */
  GPIO_InitStructure.GPIO_Pin = Get_vectEepromSpiCsPin();
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_WriteBit(Get_vectEepromSpiCsPort(), Get_vectEepromSpiCsPin(), Bit_SET); 
  GPIO_Init(Get_vectEepromSpiCsPort(), &GPIO_InitStructure);
     
  /* Put the SPI chip select high to end the transaction */
  //EepromSPICSHigh(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin()); /* Is it needed ?TBR */
}/* end EepromCsPinInitialization() */

void EepromWriteEnable(void);

/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
uint8_t EepromStatus(void)
{
  uint8_t status;
  uint8_t cmd[] = {EEPROM_CMD_RDSR, 0xFF};
  
  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
  /* Send command */
  for(uint8_t k=0;k<2;k++)
  {
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
    SPI_SendData(Get_EepromSpiPort(), cmd[k]);
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_RXNE) == RESET);
    status = SPI_ReceiveData(Get_EepromSpiPort());
  }
  
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
  
  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
  return status;
}/* end EepromStatus() */


/**
* @brief  Set the ERSR status bit.
* @param  None
* @retval Status
*/
uint8_t EepromSetSrwd(void)
{
  uint8_t status;
  uint8_t cmd[] = {EEPROM_CMD_WRSR, EEPROM_STATUS_SRWD};
  
  EepromWriteEnable();
  
  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
  /* Send command */
  for(uint8_t k=0;k<2;k++)
  {
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
    SPI_SendData(Get_EepromSpiPort(), cmd[k]);
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_RXNE) == RESET);
    status = SPI_ReceiveData(Get_EepromSpiPort());
  }
  
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
  
  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
  return status;
}/* end EepromSetSrwd() */

/**
* @brief  Reset the ERSR status bit.
* @param  None
* @retval Status
*/
uint8_t EepromResetSrwd(void)
{
  uint8_t status;
  uint8_t cmd[] = {EEPROM_CMD_WRSR, EEPROM_STATUS_SRWD};
  
  EepromWriteEnable();
  
  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
  /* Send command */
  for(uint8_t k=0;k<2;k++)
  {
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
    SPI_SendData(Get_EepromSpiPort(), cmd[k]);
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_RXNE) == RESET);
    status = SPI_ReceiveData(Get_EepromSpiPort());
  }
  
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
  
  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
  return status;
}/* end EepromResetSrwd() */

/**
* @brief  Set the internal WEL flag to allow write operation.
* @param  None
* @retval None
*/
void EepromWriteEnable(void)
{
  
  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
  /* Send command */
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
  SPI_SendData(Get_EepromSpiPort(), EEPROM_CMD_WREN);
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_RXNE) == RESET);
  SPI_ReceiveData(Get_EepromSpiPort());
  
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
  
  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
}/* end EepromWriteEnable() */

/**
* @brief  Wait polling the SPI until the internal WIP flag is RESET.
*         The flag is SET when a write operation is running.
* @param  None
* @retval None
*/
void EepromWaitEndWriteOperation(void)
{
  uint8_t cmd = EEPROM_CMD_RDSR;
  uint8_t dummy = 0xFF;
  uint8_t status;
  
  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
  /* Send command */
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
  SPI_SendData(Get_EepromSpiPort(), cmd);
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_RXNE) == RESET);
  SPI_ReceiveData(Get_EepromSpiPort());
  
  /* Polling on status register */
  do{
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
    SPI_SendData(Get_EepromSpiPort(), dummy);
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_RXNE) == RESET);
    status = SPI_ReceiveData(Get_EepromSpiPort());
  }while(status&EEPROM_STATUS_WIP);
  
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
  
  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
}/* end EepromWaitEndWriteOperation() */


/**
* @brief  Read a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
* @param  None
* @retval None
*/
void EepromRead(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t cmd[3];
  cmd[0] = EEPROM_CMD_READ;

  for(uint8_t k=0; k<2; k++) {
    cmd[k+1] = (uint8_t)(nAddress>>((1-k)*8));
  }  
  
  /* Wait the end of a previous write operation */
  EepromWaitEndWriteOperation();
  
  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
  /* Write the header bytes and read the status bytes */
  for(uint8_t i=0; i<3; i++) {
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
    SPI_SendData(Get_EepromSpiPort(), cmd[i]);
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_RXNE) == RESET);
    SPI_ReceiveData(Get_EepromSpiPort());
  }
  
  /* Read the registers according to the number of bytes */
  for(int index=0; index<cNbBytes; index++)
  {
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
    SPI_SendData(Get_EepromSpiPort(), 0xFF);
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_RXNE) == RESET);
    *pcBuffer = SPI_ReceiveData(Get_EepromSpiPort());
    pcBuffer++;
  }
  
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
  
  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
}/* end EepromRead() */


/**
* @brief  Write a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
*         It is allowed to write only a page for each operation. If the bytes
*         exceed the single page location, the other bytes are written at the 
*         beginning.
* @param  None
* @retval None
*/
void EepromWrite(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t cmd = EEPROM_CMD_WRITE;
  uint8_t address[2];
  
  /* Wait the end of a previous write operation */
  EepromWaitEndWriteOperation();
  
  /* SET the WREN flag */
  EepromWriteEnable();
  
  for(uint8_t k=0; k<2; k++) {
    address[k] = (uint8_t)(nAddress>>((1-k)*8));
  }
  
  
  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
  /* Write the header bytes and read the status bytes */
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
  SPI_SendData(Get_EepromSpiPort(), cmd);
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_RXNE) == RESET);
  SPI_ReceiveData(Get_EepromSpiPort());
  
  for(int i=0; i<2; i++)
  {
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
    SPI_SendData(Get_EepromSpiPort(), address[i]);
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_RXNE) == RESET);
    SPI_ReceiveData(Get_EepromSpiPort());
  }
  
  /* Writes the registers according to the number of bytes */
  for(int index=0; index<cNbBytes; index++)
  {
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
    SPI_SendData(Get_EepromSpiPort(), pcBuffer[index]);   
    while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_RXNE) == RESET);
    SPI_ReceiveData(Get_EepromSpiPort());
  }
  
  while (SPI_GetFlagStatus(Get_EepromSpiPort(), SPI_FLAG_TXE) == RESET);
  
  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh(Get_vectEepromSpiCsPort(),Get_vectEepromSpiCsPin());
  
}/* end EepromWrite() */



/**
* @}
*/

/**
* @}
*/


/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/
