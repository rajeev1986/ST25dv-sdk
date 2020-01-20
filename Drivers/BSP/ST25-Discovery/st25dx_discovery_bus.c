/**
  ******************************************************************************
  * @file    st25dx_discovery_bus.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file provides set of firmware functions to manage bus
  *          available on ST25DX-Discovery Kit from STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "st25dx_discovery_bus.h"

/** @defgroup BSP BSP
  * @{
  */

/** @addtogroup ST25DX_DISCOVERY_BSP ST25DX DISCOVERY
  * @brief    The Board support package for the ST25DX Discovery.
  * @details  Provides all the requires functions to abstract from the board HW.
  * @{
  */

/** @defgroup ST25DX_DISCOVERY_LOW_LEVEL ST25DX DISCOVERY LOW LEVEL
  * @brief This file provides set of firmware functions to manage Leds and push-button
  *        available on ST25DX Discovery Kit from STMicroelectronics.
  * @{
  */

/**
 * @brief BUS variables
 */
#ifdef HAL_SPI_MODULE_ENABLED
SPI_HandleTypeDef hbus_spi1;
SPI_HandleTypeDef hbus_spi2;
SPI_HandleTypeDef hbus_spi3;

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
static uint32_t IsSPI1MspCbValid = 0;
static uint32_t IsSPI2MspCbValid = 0;
static uint32_t IsSPI3MspCbValid = 0;
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

static uint32_t SPI1InitCounter = 0;
static uint32_t SPI2InitCounter = 0;
static uint32_t SPI3InitCounter = 0;
#endif /* HAL_SPI_MODULE_ENABLED */

#ifdef HAL_I2C_MODULE_ENABLED
I2C_HandleTypeDef hbus_i2c1;
I2C_HandleTypeDef hbus_i2c2;
I2C_HandleTypeDef hbus_i2c3;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
static uint32_t IsI2C1MspCbValid = 0;
static uint32_t IsI2C2MspCbValid = 0;
static uint32_t IsI2C3MspCbValid = 0;
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

static uint32_t I2C1InitCounter = 0;
static uint32_t I2C2InitCounter = 0;
static uint32_t I2C3InitCounter = 0;
#endif /* HAL_I2C_MODULE_ENABLED */

#ifdef HAL_UART_MODULE_ENABLED
UART_HandleTypeDef hbus_usart2;
UART_HandleTypeDef hbus_usart3;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
static uint32_t IsUSART2MspCbValid = 0;
static uint32_t IsUSART3MspCbValid = 0;
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

static uint32_t USART2InitCounter = 0;
static uint32_t USART3InitCounter = 0;
#endif /* HAL_UART_MODULE_ENABLED */

/******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/

/******************************* I2C Routines *********************************/
#if defined(HAL_I2C_MODULE_ENABLED)
/**
  * @brief  I2C1 Bus initialization.
  */
__weak HAL_StatusTypeDef MX_I2C1_Init(I2C_HandleTypeDef* hi2c)
{
  HAL_StatusTypeDef ret = HAL_OK;

  hi2c->Init.ClockSpeed = BUS_I2C1_FREQUENCY;
  hi2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c->Init.OwnAddress1 = 0;
  hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c->Init.OwnAddress2 = 0;
  hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(hi2c) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

/**
  * @brief  I2C1 MSP Initialization
  * @param  hi2c: I2C handle
  */
static void I2C1_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef gpio_init;

  BUS_I2C1_SDA_GPIO_CLK_ENABLE();
  BUS_I2C1_SCL_GPIO_CLK_ENABLE();

  /* Configure I2C Tx as alternate function */
  gpio_init.Pin       = BUS_I2C1_SCL_GPIO_PIN;
  gpio_init.Mode      = GPIO_MODE_AF_OD;
  gpio_init.Pull      = GPIO_NOPULL;
  gpio_init.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init.Alternate = BUS_I2C1_SCL_GPIO_AF;
  HAL_GPIO_Init(BUS_I2C1_SCL_GPIO_PORT, &gpio_init);

  /* Configure I2C Rx as alternate function */
  gpio_init.Pin = BUS_I2C1_SDA_GPIO_PIN;
  gpio_init.Mode      = GPIO_MODE_AF_OD;
  gpio_init.Alternate = BUS_I2C1_SDA_GPIO_AF;
  HAL_GPIO_Init(BUS_I2C1_SDA_GPIO_PORT, &gpio_init);

  /* Configure the Discovery I2Cx peripheral -------------------------------*/
  /* Enable I2C3 clock */
  BUS_I2C1_CLOCK_ENABLE();

  /* Force the I2C Peripheral Clock Reset */
  BUS_I2C1_FORCE_RESET();

  /* Release the I2C Peripheral Clock Reset */
  BUS_I2C1_RELEASE_RESET();

  /* Enable and set Discovery I2Cx Interrupt to the highest priority */
  HAL_NVIC_SetPriority(BUS_I2C1_EV_IRQn, 0x00, 0);
  HAL_NVIC_EnableIRQ(BUS_I2C1_EV_IRQn);

  /* Enable and set Discovery I2Cx Interrupt to the highest priority */
  HAL_NVIC_SetPriority(BUS_I2C1_ER_IRQn, 0x00, 0);
  HAL_NVIC_EnableIRQ(BUS_I2C1_ER_IRQn);
}

/**
  * @brief  I2C1 MSP Deinitialization
  * @param  hi2c: I2C handle
  */
static void I2C1_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef gpio_init;

  /* COM GPIO pin configuration */
  gpio_init.Pin = BUS_I2C1_SCL_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_I2C1_SCL_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_I2C1_SDA_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_I2C1_SDA_GPIO_PORT, gpio_init.Pin);

  /* Disable I2C1 clock */
  BUS_I2C1_CLOCK_DISABLE();
}

/**
  * @brief  Initialize I2C HAL
  * @retval BSP status
  */
int32_t BSP_I2C1_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;

  hbus_i2c1.Instance  = BUS_I2C1;

  if (I2C1InitCounter++ == 0)
  {
    if (HAL_I2C_GetState(&hbus_i2c1) == HAL_I2C_STATE_RESET)
    {
    #if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
      /* Init the I2C Msp */
      I2C1_MspInit(&hbus_i2c1);
    #else
      if (IsI2C1MspCbValid == 0U)
      {
        if (BSP_I2C1_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
        {
          return BSP_ERROR_MSP_FAILURE;
        }
      }
    #endif
      if (ret == BSP_ERROR_NONE)
      {
        /* Init the I2C */
        if (MX_I2C1_Init(&hbus_i2c1) != HAL_OK)
        {
          ret = BSP_ERROR_BUS_FAILURE;
        }
#if  defined(I2C_FLTR_ANOFF)
        else if (HAL_I2CEx_ConfigAnalogFilter(&hbus_i2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
        {
          ret = BSP_ERROR_BUS_FAILURE;
        }
#endif /* I2C_FLTR_ANOFF */
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
    }
  }
  return ret;
}

/**
  * @brief  DeInitialize I2C HAL.
  * @retval BSP status
  */
int32_t BSP_I2C1_DeInit(void)
{
  int32_t ret = BSP_ERROR_NONE;

  if (I2C1InitCounter > 0)
  {
    I2C1InitCounter = 0;
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
    /* DeInit the I2C */
    I2C1_MspDeInit(&hbus_i2c1);
#endif
    /* DeInit the I2C */
    if (HAL_I2C_DeInit(&hbus_i2c1) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Check whether the I2C bus is ready.
  * @param DevAddr : I2C device address
  * @param Trials : Check trials number
  *	@retval BSP status
  */
int32_t BSP_I2C1_IsReady(uint16_t DevAddr, uint32_t Trials)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_IsDeviceReady(&hbus_i2c1, DevAddr, Trials, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    ret = BSP_ERROR_BUSY;
  }
  
  return ret;
}

/**
  * @brief  Write a value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length
  * @retval BSP status
  */

int32_t BSP_I2C1_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Mem_Write(&hbus_i2c1, DevAddr, Reg, I2C_MEMADD_SIZE_8BIT, pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c1) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Read a register of the device through BUS
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to read
  * @param  pData  Pointer to data buffer to read
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t BSP_I2C1_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Mem_Read(&hbus_i2c1, DevAddr, Reg, I2C_MEMADD_SIZE_8BIT, pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c1) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Write a value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length
  * @retval BSP statu
  */
int32_t BSP_I2C1_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Mem_Write(&hbus_i2c1, DevAddr, Reg, I2C_MEMADD_SIZE_16BIT,
                              pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c1) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Read registers through a bus (16 bits)
  * @param  DevAddr: Device address on BUS
  * @param  Reg: The target register address to read
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t BSP_I2C1_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;
 
  if (HAL_I2C_Mem_Read(&hbus_i2c1, DevAddr, Reg, I2C_MEMADD_SIZE_16BIT, pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c1) != HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Send an amount width data through bus (Simplex)
  * @param  DevAddr: Device address on Bus.
  * @param  pData: Data pointer
  * @param  Length: Data length
  * @retval BSP status
  */
int32_t BSP_I2C1_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Master_Transmit(&hbus_i2c1, DevAddr, pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c1) != HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Receive an amount of data through a bus (Simplex)
  * @param  DevAddr: Device address on Bus.
  * @param  pData: Data pointer
  * @param  Length: Data length
  * @retval BSP status
  */
int32_t BSP_I2C1_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{	
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Master_Receive(&hbus_i2c1, DevAddr, pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c1) != HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default BSP I2C1 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_I2C1_RegisterDefaultMspCallbacks(void)
{

  __HAL_I2C_RESET_HANDLE_STATE(&hbus_i2c1);

  /* Register MspInit Callback */
  if (HAL_I2C_RegisterCallback(&hbus_i2c1, HAL_I2C_MSPINIT_CB_ID, I2C1_MspInit)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_I2C_RegisterCallback(&hbus_i2c1, HAL_I2C_MSPDEINIT_CB_ID, I2C1_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsI2C1MspCbValid = 1;

  return BSP_ERROR_NONE;
}

/**
  * @brief BSP I2C1 Bus Msp Callback registering
  * @param Callbacks     pointer to I2C1 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_I2C1_RegisterMspCallbacks(BSP_I2C_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_I2C_RESET_HANDLE_STATE(&hbus_i2c1);
 
   /* Register MspInit Callback */
  if (HAL_I2C_RegisterCallback(&hbus_i2c1, HAL_I2C_MSPINIT_CB_ID, Callbacks->pMspInitCb)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_I2C_RegisterCallback(&hbus_i2c1, HAL_I2C_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  IsI2C1MspCbValid = 1;

  return BSP_ERROR_NONE;
}
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

#if defined(USE_NFCTAG)
/**
  * @brief  I2C2 Bus initialization.
  */
__weak HAL_StatusTypeDef MX_I2C2_Init(I2C_HandleTypeDef* hi2c)
{
  HAL_StatusTypeDef ret = HAL_OK;

  hi2c->Init.ClockSpeed = BUS_I2C2_FREQUENCY;
  hi2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c->Init.OwnAddress1 = 0;
  hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c->Init.OwnAddress2 = 0;
  hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_Init(hi2c) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

/**
  * @brief  I2C2 MSP Initialization
  * @param  hi2c: I2C handle
  */
static void I2C2_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef gpio_init;

  /* Enable GPIO clock */
  BUS_I2C2_SDA_GPIO_CLK_ENABLE();
  BUS_I2C2_SCL_GPIO_CLK_ENABLE();

  /* Configure I2C SCL as alternate function */
  gpio_init.Pin = BUS_I2C2_SCL_GPIO_PIN;
  gpio_init.Mode = GPIO_MODE_AF_OD;
  gpio_init.Pull = GPIO_PULLUP;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = BUS_I2C2_SCL_GPIO_AF;
  HAL_GPIO_Init(BUS_I2C2_SCL_GPIO_PORT, &gpio_init);

  /* Configure I2C SDA as alternate function */
  gpio_init.Pin = BUS_I2C2_SDA_GPIO_PIN;
  gpio_init.Mode = GPIO_MODE_AF_OD;
  gpio_init.Alternate = BUS_I2C2_SDA_GPIO_AF;
  HAL_GPIO_Init(BUS_I2C2_SDA_GPIO_PORT, &gpio_init);

  /* Peripheral clock enable */
  BUS_I2C2_CLOCK_ENABLE();
}

/**
  * @brief  I2C2 MSP Deinitialization
  * @param  hi2c: I2C handle
  */
static void I2C2_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef gpio_init;

  /* COM GPIO pin configuration */
  gpio_init.Pin = BUS_I2C2_SCL_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_I2C2_SCL_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_I2C2_SDA_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_I2C2_SDA_GPIO_PORT, gpio_init.Pin);

  /* Disable I2C2 clock */
  BUS_I2C2_CLOCK_DISABLE();
}

/**
  * @brief  Initialize I2C2 HAL
  * @return BSP status
  */
int32_t BSP_I2C2_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;

  hbus_i2c2.Instance  = BUS_I2C2;

  if (I2C2InitCounter++ == 0)
  {
    if (HAL_I2C_GetState(&hbus_i2c2) == HAL_I2C_STATE_RESET)
    {
    #if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
      /* Init the I2C Msp */
      I2C2_MspInit(&hbus_i2c2);
    #else
      if (IsI2C2MspCbValid == 0U)
      {
        if (BSP_I2C2_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
        {
          return BSP_ERROR_MSP_FAILURE;
        }
      }
    #endif
      if (ret == BSP_ERROR_NONE)
      {
        /* Init the I2C */
        if (MX_I2C2_Init(&hbus_i2c2) != HAL_OK)
        {
          ret = BSP_ERROR_BUS_FAILURE;
        }
#if  defined(I2C_FLTR_ANOFF)
        else if (HAL_I2CEx_ConfigAnalogFilter(&hbus_i2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
        {
          ret = BSP_ERROR_BUS_FAILURE;
        }
#endif /* I2C_FLTR_ANOFF */
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
    }
  }
  return ret;
}

/**
  * @brief  DeInitialize I2C HAL.
  * @retval BSP status
  */
int32_t BSP_I2C2_DeInit(void)
{
  int32_t ret = BSP_ERROR_NONE;

  if (I2C2InitCounter > 0)
  {
    I2C2InitCounter = 0;
  #if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
    /* DeInit the I2C */
    I2C2_MspDeInit(&hbus_i2c2);
  #endif
    /* DeInit the I2C */
    if (HAL_I2C_DeInit(&hbus_i2c2) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Check whether the I2C bus is ready.
  * @param DevAddr : I2C device address
  * @param Trials : Check trials number
  *	@retval BSP status
  */
int32_t BSP_I2C2_IsReady(uint16_t DevAddr, uint32_t Trials)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_IsDeviceReady(&hbus_i2c2, DevAddr, Trials, BUS_I2C2_POLL_TIMEOUT) != HAL_OK)
  {
    ret = BSP_ERROR_BUSY;
  }
  
  return ret;
}

/**
  * @brief  Write a value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length
  * @retval BSP status
  */

int32_t BSP_I2C2_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Mem_Write(&hbus_i2c2, DevAddr,Reg, I2C_MEMADD_SIZE_8BIT, pData, Length, BUS_I2C2_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c2) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Read a register of the device through BUS
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to read
  * @param  pData  Pointer to data buffer to read
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t BSP_I2C2_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Mem_Read(&hbus_i2c2, DevAddr, Reg, I2C_MEMADD_SIZE_8BIT, pData, Length, BUS_I2C2_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c2) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Write a value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length
  * @retval BSP statu
  */
int32_t BSP_I2C2_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Mem_Write(&hbus_i2c2, DevAddr, Reg, I2C_MEMADD_SIZE_16BIT,
                              pData, Length, BUS_I2C2_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c2) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Read registers through a bus (16 bits)
  * @param  DevAddr: Device address on BUS
  * @param  Reg: The target register address to read
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t BSP_I2C2_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Mem_Read(&hbus_i2c2, DevAddr, Reg, I2C_MEMADD_SIZE_16BIT, pData, Length, BUS_I2C2_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c2) != HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Send an amount width data through bus (Simplex)
  * @param  DevAddr: Device address on Bus.
  * @param  pData: Data pointer
  * @param  Length: Data length
  * @retval BSP status
  */
int32_t BSP_I2C2_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Master_Transmit(&hbus_i2c2, DevAddr, pData, Length, BUS_I2C2_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c2) != HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Receive an amount of data through a bus (Simplex)
  * @param  DevAddr: Device address on Bus.
  * @param  pData: Data pointer
  * @param  Length: Data length
  * @retval BSP status
  */
int32_t BSP_I2C2_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Master_Receive(&hbus_i2c2, DevAddr, pData, Length, BUS_I2C2_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c2) != HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default BSP I2C2 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_I2C2_RegisterDefaultMspCallbacks(void)
{
  __HAL_I2C_RESET_HANDLE_STATE(&hbus_i2c2);

  /* Register MspInit Callback */
  if (HAL_I2C_RegisterCallback(&hbus_i2c2, HAL_I2C_MSPINIT_CB_ID, I2C2_MspInit)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_I2C_RegisterCallback(&hbus_i2c2, HAL_I2C_MSPDEINIT_CB_ID, I2C2_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsI2C2MspCbValid = 1;

  return BSP_ERROR_NONE;
}

/**
  * @brief BSP I2C2 Bus Msp Callback registering
  * @param Callbacks     pointer to I2C2 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_I2C2_RegisterMspCallbacks(BSP_I2C_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_I2C_RESET_HANDLE_STATE(&hbus_i2c2);  

   /* Register MspInit Callback */
  if (HAL_I2C_RegisterCallback(&hbus_i2c2, HAL_I2C_MSPINIT_CB_ID, Callbacks->pMspInitCb)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_I2C_RegisterCallback(&hbus_i2c2, HAL_I2C_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  IsI2C2MspCbValid = 1;

  return BSP_ERROR_NONE;
}
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
#endif /* USE_NFCTAG */

/**
  * @brief  I2C3 Bus initialization.
  */
__weak HAL_StatusTypeDef MX_I2C3_Init(I2C_HandleTypeDef* hi2c)
{
  HAL_StatusTypeDef ret = HAL_OK;

  hi2c->Init.ClockSpeed = BUS_I2C3_FREQUENCY;
  hi2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c->Init.OwnAddress1 = 0;
  hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c->Init.OwnAddress2 = 0;
  hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_Init(hi2c) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

/**
  * @brief  I2C3 MSP Initialization
  * @param  hi2c: I2C handle
  */
static void I2C3_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef  gpio_init;

  /* Configure the GPIOs ---------------------------------------------------*/
  /* Enable GPIO clock */
  BUS_I2C3_SCL_GPIO_CLK_ENABLE();
  BUS_I2C3_SDA_GPIO_CLK_ENABLE();

  /* Configure I2C SCL as alternate function */
  gpio_init.Pin       = BUS_I2C3_SCL_GPIO_PIN;
  gpio_init.Mode      = GPIO_MODE_AF_OD;
  gpio_init.Pull      = GPIO_NOPULL;
  gpio_init.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init.Alternate = BUS_I2C3_SCL_GPIO_AF;
  HAL_GPIO_Init(BUS_I2C3_SCL_GPIO_PORT, &gpio_init);

  /* Configure I2C SDA as alternate function */
  gpio_init.Pin = BUS_I2C3_SDA_GPIO_PIN;
  gpio_init.Alternate = BUS_I2C3_SCL_GPIO_AF;
  HAL_GPIO_Init(BUS_I2C3_SDA_GPIO_PORT, &gpio_init);

  /* Configure the Discovery I2Cx peripheral -------------------------------*/
  /* Enable I2C3 clock */
  BUS_I2C3_CLOCK_ENABLE();

  /* Force the I2C Peripheral Clock Reset */
  BUS_I2C3_FORCE_RESET();

  /* Release the I2C Peripheral Clock Reset */
  BUS_I2C3_RELEASE_RESET();

  /* Enable and set Discovery I2Cx Interrupt to the highest priority */
  HAL_NVIC_SetPriority(BUS_I2C3_EV_IRQn, 0x00, 0);
  HAL_NVIC_EnableIRQ(BUS_I2C3_EV_IRQn);

  /* Enable and set Discovery I2Cx Interrupt to the highest priority */
  HAL_NVIC_SetPriority(BUS_I2C3_ER_IRQn, 0x00, 0);
  HAL_NVIC_EnableIRQ(BUS_I2C3_ER_IRQn);
}

/**
  * @brief  I2C3 MSP Deinitialization
  * @param  hi2c: I2C handle
  */
static void I2C3_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef gpio_init;

  /* COM GPIO pin configuration */
  gpio_init.Pin = BUS_I2C3_SCL_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_I2C3_SCL_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_I2C3_SDA_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_I2C3_SDA_GPIO_PORT, gpio_init.Pin);

  /* Disable I2C3 clock */
  BUS_I2C3_CLOCK_DISABLE();
}

/**
  * @brief  Initialize I2C3 HAL
  * @return BSP status
  */
int32_t BSP_I2C3_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;

  hbus_i2c3.Instance  = BUS_I2C3;

  if (I2C3InitCounter++ == 0)
  {
    if (HAL_I2C_GetState(&hbus_i2c3) == HAL_I2C_STATE_RESET)
    {
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
      /* Init the I2C Msp */
      I2C3_MspInit(&hbus_i2c3);
#else
      if (IsI2C3MspCbValid == 0U)
      {
        if (BSP_I2C3_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
        {
          return BSP_ERROR_MSP_FAILURE;
        }
      }
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      if (ret == BSP_ERROR_NONE)
      {
        /* Init the I2C */
        if (MX_I2C3_Init(&hbus_i2c3) != HAL_OK)
        {
          ret = BSP_ERROR_BUS_FAILURE;
        }
#if  defined(I2C_FLTR_ANOFF)
        else if (HAL_I2CEx_ConfigAnalogFilter(&hbus_i2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
        {
          ret = BSP_ERROR_BUS_FAILURE;
        }
#endif /* I2C_FLTR_ANOFF */
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
    }
  }
  return ret;
}

/**
  * @brief  DeInitialize I2C HAL.
  * @retval BSP status
  */
int32_t BSP_I2C3_DeInit(void)
{
  int32_t ret = BSP_ERROR_NONE;

  if (I2C3InitCounter > 0)
  {
    I2C3InitCounter = 0;
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
    /* DeInit the I2C */
    I2C3_MspDeInit(&hbus_i2c3);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    /* DeInit the I2C */
    if (HAL_I2C_DeInit(&hbus_i2c3) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Check whether the I2C bus is ready.
  * @param DevAddr : I2C device address
  * @param Trials : Check trials number
  *	@retval BSP status
  */
int32_t BSP_I2C3_IsReady(uint16_t DevAddr, uint32_t Trials)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_IsDeviceReady(&hbus_i2c3, DevAddr, Trials, BUS_I2C3_POLL_TIMEOUT) != HAL_OK)
  {
    ret = BSP_ERROR_BUSY;
  }

  return ret;
}

/**
  * @brief  Write a value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length
  * @retval BSP status
  */

int32_t BSP_I2C3_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Mem_Write(&hbus_i2c3, DevAddr, Reg, I2C_MEMADD_SIZE_8BIT, pData, Length, BUS_I2C3_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c3) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Read a register of the device through BUS
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to read
  * @param  pData  Pointer to data buffer to read
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t BSP_I2C3_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Mem_Read(&hbus_i2c3, DevAddr, Reg, I2C_MEMADD_SIZE_8BIT, pData, Length, BUS_I2C3_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c3) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**

  * @brief  Write a value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length
  * @retval BSP statu
  */
int32_t BSP_I2C3_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Mem_Write(&hbus_i2c3, DevAddr, Reg, I2C_MEMADD_SIZE_16BIT,
                              pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c3) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Read registers through a bus (16 bits)
  * @param  DevAddr: Device address on BUS
  * @param  Reg: The target register address to read
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t BSP_I2C3_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Mem_Read(&hbus_i2c3, DevAddr, Reg, I2C_MEMADD_SIZE_16BIT, pData, Length, BUS_I2C3_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c3) != HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Send an amount width data through bus (Simplex)
  * @param  DevAddr: Device address on Bus.
  * @param  pData: Data pointer
  * @param  Length: Data length
  * @retval BSP status
  */
int32_t BSP_I2C3_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Master_Transmit(&hbus_i2c3, DevAddr, pData, Length, BUS_I2C3_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c3) != HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Receive an amount of data through a bus (Simplex)
  * @param  DevAddr: Device address on Bus.
  * @param  pData: Data pointer
  * @param  Length: Data length
  * @retval BSP status
  */
int32_t BSP_I2C3_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_I2C_Master_Receive(&hbus_i2c3, DevAddr, pData, Length, BUS_I2C3_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hbus_i2c3) != HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default BSP I2C3 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_I2C3_RegisterDefaultMspCallbacks(void)
{
  __HAL_I2C_RESET_HANDLE_STATE(&hbus_i2c3);

  /* Register MspInit Callback */
  if (HAL_I2C_RegisterCallback(&hbus_i2c3, HAL_I2C_MSPINIT_CB_ID, I2C3_MspInit)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_I2C_RegisterCallback(&hbus_i2c3, HAL_I2C_MSPDEINIT_CB_ID, I2C3_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsI2C3MspCbValid = 1;

  return BSP_ERROR_NONE;
}

/**
  * @brief BSP I2C3 Bus Msp Callback registering
  * @param Callbacks     pointer to I2C3 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_I2C3_RegisterMspCallbacks(BSP_I2C_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_I2C_RESET_HANDLE_STATE(&hbus_i2c3);

   /* Register MspInit Callback */
  if (HAL_I2C_RegisterCallback(&hbus_i2c3, HAL_I2C_MSPINIT_CB_ID, Callbacks->pMspInitCb)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_I2C_RegisterCallback(&hbus_i2c3, HAL_I2C_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  IsI2C3MspCbValid = 1;

  return BSP_ERROR_NONE;
}
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
#endif /* HAL_I2C_MODULE_ENABLED */

/******************************* SPI Routines *********************************/
#if defined(HAL_SPI_MODULE_ENABLED)
/**
  * @brief  Compute SPI prescaler value according to clock parameter.
  */
static uint32_t SPI_GetPrescaler(uint32_t clock_src_hz, uint32_t baudrate_mbps)
{
  uint32_t divisor = 0;
  uint32_t spi_clk = clock_src_hz;
  uint32_t presc = 0;
  static const uint32_t baudrate[]= { 
                                      SPI_BAUDRATEPRESCALER_2,
                                      SPI_BAUDRATEPRESCALER_4,
                                      SPI_BAUDRATEPRESCALER_8,
                                      SPI_BAUDRATEPRESCALER_16,
                                      SPI_BAUDRATEPRESCALER_32,
                                      SPI_BAUDRATEPRESCALER_64,
                                      SPI_BAUDRATEPRESCALER_128,
                                      SPI_BAUDRATEPRESCALER_256,
                                    };

  while(spi_clk > baudrate_mbps)
  {
    presc = baudrate[divisor];

    if (++divisor > 7)
    {
      break;
    }

    spi_clk = (spi_clk >> 1);
  }

  return presc;
}

/**
  * @brief  SPI1 Bus initialization.
  */
__weak HAL_StatusTypeDef MX_SPI1_Init(SPI_HandleTypeDef *hspi, uint32_t baudrate_presc, uint32_t DataSize)
{
  HAL_StatusTypeDef ret = HAL_OK;

  hspi->Init.BaudRatePrescaler = baudrate_presc;
  hspi->Init.Direction         = SPI_DIRECTION_1LINE;
  hspi->Init.CLKPhase          = SPI_PHASE_1EDGE;
  hspi->Init.CLKPolarity       = SPI_POLARITY_LOW;
  hspi->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  hspi->Init.CRCPolynomial     = 7;
  hspi->Init.DataSize          = DataSize;
  hspi->Init.FirstBit          = SPI_FIRSTBIT_MSB;
  hspi->Init.NSS               = SPI_NSS_SOFT;
  hspi->Init.TIMode            = SPI_TIMODE_DISABLE;
  hspi->Init.Mode              = SPI_MODE_MASTER;

  if (HAL_SPI_Init(hspi) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

/**
  * @brief  Initializes SPI MSP.
  */
static void SPI1_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  gpio_init;

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
  BUS_SPI2_SCK_GPIO_CLK_ENABLE();
  BUS_SPI2_NSS_GPIO_CLK_ENABLE();
  BUS_SPI2_MISO_GPIO_CLK_ENABLE();
  BUS_SPI2_MOSI_GPIO_CLK_ENABLE();

  /* Configure SPI NSS */
  gpio_init.Pin   = BUS_SPI1_NSS_GPIO_PIN;
  gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pull  = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(BUS_SPI1_NSS_GPIO_PORT, &gpio_init);

  /* Configure SPI SCK */
  gpio_init.Pin       = BUS_SPI1_SCK_GPIO_PIN;
  gpio_init.Mode      = GPIO_MODE_AF_PP;
  gpio_init.Pull      = GPIO_NOPULL;
  gpio_init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = BUS_SPI1_SCK_GPIO_AF;
  HAL_GPIO_Init(BUS_SPI1_SCK_GPIO_PORT, &gpio_init);

  /* Configure SPI MOSI */
  gpio_init.Pin       = BUS_SPI1_MOSI_GPIO_PIN;
  gpio_init.Alternate = BUS_SPI1_MOSI_GPIO_AF;
  gpio_init.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(BUS_SPI1_MOSI_GPIO_PORT, &gpio_init);

  /* Configure SPI MISO */
  gpio_init.Pin       = BUS_SPI1_MISO_GPIO_PIN;
  gpio_init.Alternate = BUS_SPI1_MISO_GPIO_AF;
  gpio_init.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(BUS_SPI1_MISO_GPIO_PORT, &gpio_init);

  /*** Configure the SPI peripheral ***/
  /* Enable SPI clock */
  BUS_SPI1_CLK_ENABLE();
}

/**
  * @brief  Deinitializes SPI MSP.
  */
static void SPI1_MspDeInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef gpio_init;

  /* COM GPIO pin configuration */
  gpio_init.Pin = BUS_SPI1_NSS_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_SPI1_NSS_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_SPI1_SCK_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_SPI1_SCK_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_SPI1_MOSI_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_SPI1_MOSI_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_SPI1_MISO_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_SPI1_MISO_GPIO_PORT, gpio_init.Pin);

  /* Disable SPI1 clock */
  BUS_SPI1_CLK_DISABLE();
}

/**
  * @brief  Initialize SPI1 HAL
  * @return BSP status
  */
int32_t BSP_SPI1_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;
  hbus_spi1.Instance = BUS_SPI1;
  if (SPI1InitCounter++ == 0)
  {
    if (HAL_SPI_GetState(&hbus_spi1) == HAL_SPI_STATE_RESET)
    {
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
      /* Init the BUS Msp */
      SPI1_MspInit(&hbus_spi1);
#else 
      if (IsSPI1MspCbValid == 0U)
      {
        if (BSP_SPI1_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_MSP_FAILURE;
        }
      }
#endif
      if (MX_SPI1_Init(&hbus_spi1, SPI_GetPrescaler(HAL_RCC_GetPCLK2Freq(),
                                  BUS_SPI1_BAUDRATE), BUS_SPI1_DATASIZE) != HAL_OK)
      {
        ret = BSP_ERROR_BUS_FAILURE;
      }
    }
  }
  return ret;
}

/**
  * @brief  DeInitialize SPI1 HAL.
  * @retval BSP status
  */
int32_t BSP_SPI1_DeInit(void)
{
  int32_t ret = BSP_ERROR_NONE;

  if (SPI1InitCounter > 0)
  {
    SPI1InitCounter = 0;
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
    /* DeInit the SPI */
    SPI1_MspDeInit(&hbus_spi1);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    /* DeInit the SPI */
    if (HAL_SPI_DeInit(&hbus_spi1) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  SPI Write a data to device.
  * @param  pData: pointer to data to send.
  * @param  Length: size of data to send.
  * @retval positive Length of data sent.
  * @retval negative error returned.
  */
int32_t BSP_SPI1_Send(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

  if (HAL_SPI_Transmit(&hbus_spi1, pData, Length, BUS_SPI1_POLL_TIMEOUT) == HAL_OK)
  {
    ret = Length;
  }

  return ret;
}

/**
  * @brief  Reads data from device.
  * @param  pdata: pointer to data received.
  * @param  Length: length of data received.
  * @retval positive Length of data received.
  * @retval negative error returned.
  */
int32_t BSP_SPI1_Recv(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

  if (HAL_SPI_Receive(&hbus_spi1, pData, Length, BUS_SPI1_POLL_TIMEOUT) == HAL_OK)
  {
    return Length;
  }

  return ret;
}

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default BSP SPI1 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_SPI1_RegisterDefaultMspCallbacks(void)
{
  __HAL_BSP_RESET_HANDLE_STATE(&hbus_spi1);

  /* Register MspInit Callback */
  if (HAL_SPI_RegisterCallback(&hbus_spi1, HAL_SPI_MSPINIT_CB_ID, SPI1_MspInit)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_SPI_RegisterCallback(&hbus_spi1, HAL_SPI_MSPDEINIT_CB_ID, SPI1_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsSPI1MspCbValid = 1;

  return BSP_ERROR_NONE;
}

/**
  * @brief BSP SPI1 Bus Msp Callback registering
  * @param Callbacks     pointer to SPI1 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_SPI1_RegisterMspCallbacks(BSP_SPI_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_SPI_RESET_HANDLE_STATE(&hbus_spi1);

   /* Register MspInit Callback */
  if (HAL_SPI_RegisterCallback(&hbus_spi1, HAL_SPI_MSPINIT_CB_ID, Callbacks->pMspInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_SPI_RegisterCallback(&hbus_spi1, HAL_SPI_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  IsSPI1MspCbValid = 1;

  return BSP_ERROR_NONE;
}
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

#if defined (USE_LCD)
/**
  * @brief  SPI2 Bus initialization.
  */
__weak HAL_StatusTypeDef MX_SPI2_Init(SPI_HandleTypeDef *hspi, uint32_t baudrate_presc, uint32_t DataSize)
{
  HAL_StatusTypeDef ret = HAL_OK;

  hspi->Init.BaudRatePrescaler = baudrate_presc;
  hspi->Init.Direction         = SPI_DIRECTION_1LINE;
  hspi->Init.CLKPhase          = SPI_PHASE_1EDGE;
  hspi->Init.CLKPolarity       = SPI_POLARITY_LOW;
  hspi->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  hspi->Init.CRCPolynomial     = 7;
  hspi->Init.DataSize          = DataSize;
  hspi->Init.FirstBit          = SPI_FIRSTBIT_MSB;
  hspi->Init.NSS               = SPI_NSS_SOFT;
  hspi->Init.TIMode            = SPI_TIMODE_DISABLE;
  hspi->Init.Mode              = SPI_MODE_MASTER;

  if (HAL_SPI_Init(hspi) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

/**
  * @brief  Initializes SPI MSP.
  */
static void SPI2_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  gpio_init;

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
  BUS_SPI2_SCK_GPIO_CLK_ENABLE();
  BUS_SPI2_NSS_GPIO_CLK_ENABLE();
  BUS_SPI2_MISO_GPIO_CLK_ENABLE();
  BUS_SPI2_MOSI_GPIO_CLK_ENABLE();

  /* Configure SPI NSS */
  gpio_init.Pin   = BUS_SPI2_NSS_GPIO_PIN;
  gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pull  = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(BUS_SPI2_NSS_GPIO_PORT, &gpio_init);

  /* Configure SPI SCK */
  gpio_init.Pin       = BUS_SPI2_SCK_GPIO_PIN;
  gpio_init.Mode      = GPIO_MODE_AF_PP;
  gpio_init.Pull      = GPIO_NOPULL;
  gpio_init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = BUS_SPI2_SCK_GPIO_AF;
  HAL_GPIO_Init(BUS_SPI2_SCK_GPIO_PORT, &gpio_init);

  /* Configure SPI MOSI */
  gpio_init.Pin       = BUS_SPI2_MOSI_GPIO_PIN;
  gpio_init.Alternate = BUS_SPI2_MOSI_GPIO_AF;
  gpio_init.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(BUS_SPI2_MOSI_GPIO_PORT, &gpio_init);

  /* Configure SPI MISO */
  gpio_init.Pin       = BUS_SPI2_MISO_GPIO_PIN;
  gpio_init.Alternate = BUS_SPI2_MISO_GPIO_AF;
  gpio_init.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(BUS_SPI2_MISO_GPIO_PORT, &gpio_init);

  /*** Configure the SPI peripheral ***/
  /* Enable SPI clock */
  BUS_SPI2_CLK_ENABLE();
}

/**
  * @brief  Deinitializes SPI MSP.
  */
static void SPI2_MspDeInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef gpio_init;

  /* COM GPIO pin configuration */
  gpio_init.Pin = BUS_SPI2_NSS_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_SPI2_NSS_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_SPI2_SCK_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_SPI2_SCK_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_SPI2_MOSI_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_SPI2_MOSI_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_SPI2_MISO_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_SPI2_MISO_GPIO_PORT, gpio_init.Pin);

  /* Disable SPI2 clock */
  BUS_SPI2_CLK_DISABLE();
}

/**
  * @brief  Initialize SPI2 HAL
  * @return BSP status
  */
int32_t BSP_SPI2_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;
  hbus_spi2.Instance = BUS_SPI2;
  if (SPI2InitCounter++ == 0)
  {
    if (HAL_SPI_GetState(&hbus_spi2) == HAL_SPI_STATE_RESET)
    {
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
      /* Init the BUS Msp */
      SPI2_MspInit(&hbus_spi2);
#else 
      if (IsSPI2MspCbValid == 0U)
      {
        if (BSP_SPI2_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_MSP_FAILURE;
        }
      }
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      if (MX_SPI2_Init(&hbus_spi2, SPI_GetPrescaler(HAL_RCC_GetPCLK1Freq(), 
                                    BUS_SPI2_BAUDRATE), SPI_DATASIZE_8BIT) != HAL_OK)
      {
        ret = BSP_ERROR_BUS_FAILURE;
      }
    }
  }
  return ret;
}

/**
  * @brief  DeInitialize SPI HAL.
  * @retval BSP status
  */
int32_t BSP_SPI2_DeInit(void)
{
  int32_t ret = BSP_ERROR_NONE;

  if (SPI2InitCounter > 0)
  {
    SPI2InitCounter = 0;
  #if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
    /* DeInit the SPI */
    SPI2_MspDeInit(&hbus_spi2);
  #endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    /* DeInit the SPI */
    if (HAL_SPI_DeInit(&hbus_spi2) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Change SPI2 datasize configuration.
  */
void BSP_SPI2_SetDataWidth(uint32_t w16)
{
  if (w16 != hbus_spi2.Init.DataSize)
  {
    if (w16 == SPI_DATASIZE_16BIT)
    {
      MX_SPI2_Init(&hbus_spi2, hbus_spi2.Init.BaudRatePrescaler, SPI_DATASIZE_16BIT);
    }
    else
    {
      MX_SPI2_Init(&hbus_spi2, hbus_spi2.Init.BaudRatePrescaler, SPI_DATASIZE_8BIT);
    }
  }
}

/**
  * @brief  Initializes SPI HAL read access (lower freq).
  */
void BSP_SPI2_LowFreq(void)
{
  /* SPI Config */
  MX_SPI2_Init(&hbus_spi2, SPI_GetPrescaler(HAL_RCC_GetPCLK1Freq(), BUS_SPI2_LOWBAUDRATE), hbus_spi2.Init.DataSize);
}

/**
  * @brief  Initializes SPI HAL write access (higher freq).
  */
void BSP_SPI2_HighFreq(void)
{
  MX_SPI2_Init(&hbus_spi2, SPI_GetPrescaler(HAL_RCC_GetPCLK1Freq(), BUS_SPI2_HIGHBAUDRATE), hbus_spi2.Init.DataSize);
}

/**
  * @brief  SPI Write a data to device.
  * @param  pData: pointer to data to send.
  * @param  Length: size of data to send.
  * @retval positive Length of data sent.
  * @retval negative error returned.
  */
int32_t BSP_SPI2_Send(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

  if (HAL_SPI_Transmit(&hbus_spi2, pData, Length, BUS_SPI2_POLL_TIMEOUT) == HAL_OK)
  {
    ret = Length;
  }

  return ret;
}

/**
  * @brief  Reads data from device.
  * @param  pdata: pointer to data received.
  * @param  Length: length of data received.
  * @retval positive Length of data received.
  * @retval negative error returned.
  */
int32_t BSP_SPI2_Recv(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

  if (HAL_SPI_Receive(&hbus_spi2, pData, Length, BUS_SPI2_POLL_TIMEOUT) == HAL_OK)
  {
    return Length;
  }

  return ret;
}

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default BSP SPI2 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_SPI2_RegisterDefaultMspCallbacks(void)
{
  __HAL_BSP_RESET_HANDLE_STATE(&hbus_spi2);

  /* Register MspInit Callback */
  if (HAL_SPI_RegisterCallback(&hbus_spi2, HAL_SPI_MSPINIT_CB_ID, SPI2_MspInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_SPI_RegisterCallback(&hbus_spi2, HAL_SPI_MSPDEINIT_CB_ID, SPI2_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsSPI2MspCbValid = 1;

  return BSP_ERROR_NONE;
}

/**
  * @brief BSP SPI2 Bus Msp Callback registering
  * @param Callbacks     pointer to SPI2 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_SPI2_RegisterMspCallbacks(BSP_SPI_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_SPI_RESET_HANDLE_STATE(&hbus_spi2);

   /* Register MspInit Callback */
  if (HAL_SPI_RegisterCallback(&hbus_spi2, HAL_SPI_MSPINIT_CB_ID, Callbacks->pMspInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_SPI_RegisterCallback(&hbus_spi2, HAL_SPI_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  IsSPI2MspCbValid = 1;

  return BSP_ERROR_NONE;
}
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
#endif /* USE_LCD */

/**
  * @brief  SPI3 Bus initialization.
  */
__weak HAL_StatusTypeDef MX_SPI3_Init(SPI_HandleTypeDef *hspi, uint32_t baudrate_presc, uint32_t DataSize)
{
  HAL_StatusTypeDef ret = HAL_OK;

  hspi->Init.BaudRatePrescaler = baudrate_presc;
  hspi->Init.Direction         = SPI_DIRECTION_2LINES;
  hspi->Init.CLKPhase          = SPI_PHASE_1EDGE;
  hspi->Init.CLKPolarity       = SPI_POLARITY_LOW;
  hspi->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  hspi->Init.CRCPolynomial     = 7;
  hspi->Init.DataSize          = DataSize;
  hspi->Init.FirstBit          = SPI_FIRSTBIT_MSB;
  hspi->Init.NSS               = SPI_NSS_SOFT;
  hspi->Init.TIMode            = SPI_TIMODE_DISABLE;
  hspi->Init.Mode              = SPI_MODE_MASTER;

  if (HAL_SPI_Init(hspi) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

/**
  * @brief  Initializes SPI3 MSP.
  */
static void SPI3_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  gpio_init;

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
  BUS_SPI3_SCK_GPIO_CLK_ENABLE();
  BUS_SPI3_NSS_GPIO_CLK_ENABLE();
  BUS_SPI3_MISO_GPIO_CLK_ENABLE();
  BUS_SPI3_MOSI_GPIO_CLK_ENABLE();

  /* Configure SPI NSS */
  gpio_init.Pin   = BUS_SPI3_NSS_GPIO_PIN;
  gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pull  = GPIO_PULLUP;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(BUS_SPI3_NSS_GPIO_PORT, &gpio_init);

  /* Configure SPI SCK */
  gpio_init.Pin       = BUS_SPI3_SCK_GPIO_PIN;
  gpio_init.Mode      = GPIO_MODE_AF_PP;
  gpio_init.Pull      = GPIO_PULLDOWN;
  gpio_init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = BUS_SPI3_SCK_GPIO_AF;
  HAL_GPIO_Init(BUS_SPI3_SCK_GPIO_PORT, &gpio_init);

  /* Configure SPI MOSI */
  gpio_init.Pin       = BUS_SPI3_MOSI_GPIO_PIN;
  gpio_init.Pull      = GPIO_NOPULL;
  gpio_init.Alternate = BUS_SPI3_MOSI_GPIO_AF;
  HAL_GPIO_Init(BUS_SPI3_MOSI_GPIO_PORT, &gpio_init);

  /* Configure SPI MISO */
  gpio_init.Pin       = BUS_SPI3_MISO_GPIO_PIN;
  gpio_init.Alternate = BUS_SPI3_MISO_GPIO_AF;
  gpio_init.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(BUS_SPI3_MISO_GPIO_PORT, &gpio_init);

  /*** Configure the SPI peripheral ***/
  /* Enable SPI clock */
  BUS_SPI3_CLK_ENABLE();

  /* Configure the NVIC for SPI */
  HAL_NVIC_SetPriority(BUS_SPI3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(BUS_SPI3_IRQn);
}

/**
  * @brief  Deinitializes SPI3 MSP.
  */
static void SPI3_MspDeInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef gpio_init;

  /* COM GPIO pin configuration */
  gpio_init.Pin = BUS_SPI3_NSS_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_SPI3_NSS_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_SPI3_SCK_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_SPI3_SCK_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_SPI3_MOSI_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_SPI3_MOSI_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_SPI3_MISO_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_SPI3_MISO_GPIO_PORT, gpio_init.Pin);

  /* Disable SPI3 clock */
  BUS_SPI3_CLK_DISABLE();
}

/**
  * @brief  Initialize SPI3 HAL
  * @return BSP status
  */
int32_t BSP_SPI3_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;

  hbus_spi3.Instance = BUS_SPI3;

  if (SPI3InitCounter++ == 0)
  {
    if (HAL_SPI_GetState(&hbus_spi3) == HAL_SPI_STATE_RESET)
    {
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
      /* Init the BUS Msp */
      SPI3_MspInit(&hbus_spi3);
#else 
      if (IsSPI3MspCbValid == 0U)
      {
        if (BSP_SPI3_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_MSP_FAILURE;
        }
      }
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      if (MX_SPI3_Init(&hbus_spi3, SPI_GetPrescaler(HAL_RCC_GetPCLK1Freq(),
                                    BUS_SPI3_BAUDRATE), SPI_DATASIZE_8BIT) != HAL_OK)
      {
        ret = BSP_ERROR_BUS_FAILURE;
      }
    }
  }
  return ret;
}

/**
  * @brief  DeInitialize SPI1 HAL.
  * @retval BSP status
  */
int32_t BSP_SPI3_DeInit(void)
{
  int32_t ret = BSP_ERROR_NONE;

  if (SPI3InitCounter > 0)
  {
    SPI3InitCounter = 0;
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
    /* DeInit the SPI */
    SPI3_MspDeInit(&hbus_spi3);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    /* DeInit the SPI */
    if (HAL_SPI_DeInit(&hbus_spi3) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  SPI Write a data to device.
  * @param  pData: pointer to data to send.
  * @param  Length: size of data to send.
  * @retval positive Length of data sent.
  * @retval negative error returned.
  */
int32_t BSP_SPI3_Send(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

  if (HAL_SPI_Transmit(&hbus_spi3, pData, Length, BUS_SPI3_POLL_TIMEOUT) == HAL_OK)
  {
    ret = Length;
  }

  return ret;
}

/**
  * @brief  Reads data from device.
  * @param  pdata: pointer to data received.
  * @param  Length: length of data received.
  * @retval positive Length of data received.
  * @retval negative error returned.
  */
int32_t BSP_SPI3_Recv(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

  if (HAL_SPI_Receive(&hbus_spi3, pData, Length, BUS_SPI3_POLL_TIMEOUT) == HAL_OK)
  {
    return Length;
  }

  return ret;
}

/**
  * @brief  Writes and Reads data from device.
  * @param  pTxdata: pointer to data to send.
  * @param  pRxdata: pointer to data received.
  * @param  Length: length of data received.
  * @retval positive Length of data received.
  * @retval negative error returned.
  */
int32_t BSP_SPI3_SendRecv(uint8_t *pTxdata, uint8_t *pRxdata, uint16_t Length)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

  if (HAL_SPI_TransmitReceive(&hbus_spi3, pTxdata, pRxdata, Length, BUS_SPI3_POLL_TIMEOUT) == HAL_OK)
  {
    return Length;
  }

  return ret;
}

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default BSP SPI3 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_SPI3_RegisterDefaultMspCallbacks(void)
{
  __HAL_BSP_RESET_HANDLE_STATE(&hbus_spi3);

  /* Register MspInit Callback */
  if (HAL_SPI_RegisterCallback(&hbus_spi3, HAL_SPI_MSPINIT_CB_ID, SPI3_MspInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_SPI_RegisterCallback(&hbus_spi3, HAL_SPI_MSPDEINIT_CB_ID, SPI3_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsSPI3MspCbValid = 1;

  return BSP_ERROR_NONE;
}

/**
  * @brief BSP SPI3 Bus Msp Callback registering
  * @param Callbacks     pointer to SPI3 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_SPI3_RegisterMspCallbacks(BSP_SPI_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_SPI_RESET_HANDLE_STATE(&hbus_spi3);

  /* Register MspInit Callback */
  if (HAL_SPI_RegisterCallback(&hbus_spi3, HAL_SPI_MSPINIT_CB_ID, Callbacks->pMspInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_SPI_RegisterCallback(&hbus_spi3, HAL_SPI_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  IsSPI3MspCbValid = 1;

  return BSP_ERROR_NONE;
}
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
#endif /* HAL_SPI_MODULE_ENABLED */

/******************************* USART Routines *********************************/
#if defined(HAL_UART_MODULE_ENABLED)
/**
  * @brief  USART2 Bus initialization.
  */
__weak HAL_StatusTypeDef MX_USART2_Init(UART_HandleTypeDef *huart, uint32_t baud_rate)
{
  HAL_StatusTypeDef ret = HAL_OK;

  huart->Init.BaudRate        = baud_rate;
  huart->Init.WordLength      = UART_WORDLENGTH_8B;
  huart->Init.StopBits        = UART_STOPBITS_1;
  huart->Init.Parity          = UART_PARITY_NONE ;
  huart->Init.HwFlowCtl       = UART_HWCONTROL_NONE;
  huart->Init.Mode            = UART_MODE_TX_RX;
  huart->Init.OverSampling    = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(huart) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

/**
  * @brief  Initializes UART MSP.
  */
static void USART2_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  gpio_init;

  /* USART2 GPIO Configuration */
  gpio_init.Pin = BUS_USART2_TX_GPIO_PIN;
  gpio_init.Mode = GPIO_MODE_AF_PP;
  gpio_init.Pull = GPIO_PULLUP;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = BUS_USART2_TX_GPIO_AF;
  HAL_GPIO_Init(BUS_USART2_TX_GPIO_PORT, &gpio_init);

  gpio_init.Pin = BUS_USART2_RX_GPIO_PIN;
  gpio_init.Alternate = BUS_USART2_RX_GPIO_AF;
  HAL_GPIO_Init(BUS_USART2_RX_GPIO_PORT, &gpio_init);

  gpio_init.Pin = BUS_USART2_RTS_GPIO_PIN;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Alternate = BUS_USART2_RTS_GPIO_AF;
  HAL_GPIO_Init(BUS_USART2_RTS_GPIO_PORT, &gpio_init);

  gpio_init.Pin = BUS_USART2_CTS_GPIO_PIN;
  gpio_init.Alternate = BUS_USART2_CTS_GPIO_AF;
  HAL_GPIO_Init(BUS_USART2_CTS_GPIO_PORT, &gpio_init);

  /* Peripheral clock enable */
  BUS_USART2_CLK_ENABLE();
}

/**
  * @brief  Deinitializes UART MSP.
  */
static void USART2_MspDeInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef gpio_init;

  /* COM GPIO pin configuration */
  gpio_init.Pin = BUS_USART2_TX_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_USART2_TX_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_USART2_RX_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_USART2_RX_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_USART2_RTS_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_USART2_RTS_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_USART2_CTS_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_USART2_CTS_GPIO_PORT, gpio_init.Pin);

  /* Disable SPI2 clock */
  BUS_USART2_CLK_DISABLE();
}

/**
  * @brief  Initialize USART2 HAL
  * @return BSP status
  */
int32_t BSP_USART2_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;
  hbus_usart2.Instance = BUS_USART2;
  if (USART2InitCounter++ == 0)
  {
    if (HAL_UART_GetState(&hbus_usart2) == HAL_UART_STATE_RESET)
    {
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
      /* Init the BUS Msp */
      USART2_MspInit(&hbus_usart2);
#else
      if (IsUSART2MspCbValid == 0U)
      {
        if (BSP_USART2_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_MSP_FAILURE;
        }
      }
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
      if (MX_USART2_Init(&hbus_usart2, BUS_USART2_BAUDRATE) != HAL_OK)
      {
        ret = BSP_ERROR_BUS_FAILURE;
      }
    }
  }
  return ret;
}

/**
  * @brief  DeInitialize UART HAL.
  * @retval BSP status
  */
int32_t BSP_USART2_DeInit(void)
{
  int32_t ret = BSP_ERROR_NONE;

  if (USART2InitCounter > 0)
  {
    USART2InitCounter = 0;
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
    /* DeInit the UART */
    USART2_MspDeInit(&hbus_usart2);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
    /* DeInit the UART */ 
    if (HAL_UART_DeInit(&hbus_usart2) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  UART Write a data to device.
  * @param  pData: pointer to data to send.
  * @param  Length: size of data to send.
  * @retval positive Length of data sent.
  * @retval negative error returned.
  */
int32_t BSP_USART2_Send(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

  if (HAL_UART_Transmit(&hbus_usart2, pData, Length, BUS_USART2_POLL_TIMEOUT) == HAL_OK)
  {
    ret = Length;
  }

  return ret;
}

/**
  * @brief  Reads data from device.
  * @param  pdata: pointer to data received.
  * @param  Length: length of data received.
  * @retval positive Length of data received.
  * @retval negative error returned.
  */
int32_t BSP_USART2_Recv(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

  if (HAL_UART_Receive(&hbus_usart2, pData, Length, BUS_USART2_POLL_TIMEOUT) == HAL_OK)
  {
    return Length;
  }

  return ret;
}

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default BSP USART2 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_USART2_RegisterDefaultMspCallbacks(void)
{
  __HAL_BSP_RESET_HANDLE_STATE(&hbus_usart2);

  /* Register MspInit Callback */
  if (HAL_UART_RegisterCallback(&hbus_usart2, HAL_UART_MSPINIT_CB_ID, USART2_MspInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_UART_RegisterCallback(&hbus_usart2, HAL_UART_MSPDEINIT_CB_ID, USART2_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsUSART2MspCbValid = 1;

  return BSP_ERROR_NONE;
}

/**
  * @brief BSP USART2 Bus Msp Callback registering
  * @param Callbacks     pointer to USART2 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_USART2_RegisterMspCallbacks(BSP_SPI_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_UART_RESET_HANDLE_STATE(&hbus_usart2);

  /* Register MspInit Callback */
  if (HAL_UART_RegisterCallback(&hbus_usart2, HAL_UART_MSPINIT_CB_ID, Callbacks->pMspInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_UART_RegisterCallback(&hbus_usart2, HAL_UART_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  IsUSART2MspCbValid = 1;

  return BSP_ERROR_NONE;
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

/**
  * @brief  USART3 Bus initialization.
  */
__weak HAL_StatusTypeDef MX_USART3_Init(UART_HandleTypeDef *huart, uint32_t baud_rate)
{
  HAL_StatusTypeDef ret = HAL_OK;

  huart->Init.BaudRate        = baud_rate;
  huart->Init.WordLength      = UART_WORDLENGTH_8B;
  huart->Init.StopBits        = UART_STOPBITS_1;
  huart->Init.Parity          = UART_PARITY_NONE ;
  huart->Init.HwFlowCtl       = UART_HWCONTROL_RTS;
  huart->Init.Mode            = UART_MODE_TX_RX;
  huart->Init.OverSampling    = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(huart) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

/**
  * @brief  Initializes UART MSP.
  */
static void USART3_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  gpio_init;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  BUS_USART3_TX_GPIO_CLK_ENABLE();
  BUS_USART3_RX_GPIO_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  gpio_init.Pin       = BUS_USART3_TX_GPIO_PIN;
  gpio_init.Mode      = GPIO_MODE_AF_PP;
  gpio_init.Pull      = GPIO_NOPULL;
  gpio_init.Alternate = BUS_USART3_TX_GPIO_AF;
  gpio_init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

  HAL_GPIO_Init(BUS_USART3_TX_GPIO_PORT, &gpio_init);

  /* UART RX GPIO pin configuration */
  gpio_init.Pin       = BUS_USART3_RX_GPIO_PIN;
  gpio_init.Pull      = GPIO_NOPULL;
  gpio_init.Alternate = BUS_USART3_RX_GPIO_AF;
  gpio_init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

  HAL_GPIO_Init(BUS_USART3_RX_GPIO_PORT, &gpio_init);

  /* UART RTS GPIO pin configuration  */
  gpio_init.Pin = BUS_USART3_RTS_GPIO_PIN;
  gpio_init.Pull     = GPIO_PULLUP;
  gpio_init.Alternate = BUS_USART3_RTS_GPIO_AF;

  HAL_GPIO_Init(BUS_USART3_RTS_GPIO_PORT, &gpio_init);

  /* UART CTS GPIO pin configuration */
  gpio_init.Pin = BUS_USART3_CTS_GPIO_PIN;
  gpio_init.Mode = GPIO_MODE_AF_PP;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = BUS_USART3_CTS_GPIO_AF;
  HAL_GPIO_Init(BUS_USART3_CTS_GPIO_PORT, &gpio_init);

  /* Enable USARTx clock */
  BUS_USART3_CLK_ENABLE();
//  __HAL_RCC_USART3_FORCE_RESET();
//  __HAL_RCC_USART3_RELEASE_RESET();

  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART */
  HAL_NVIC_SetPriority(BUS_USART3_IRQn, BSP_USART3_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(BUS_USART3_IRQn);
}

/**
  * @brief  Deinitializes UART MSP.
  */
static void USART3_MspDeInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef gpio_init;

  /* COM GPIO pin configuration */
  gpio_init.Pin = BUS_USART3_TX_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_USART3_TX_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_USART3_RX_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_USART3_RX_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_USART3_RTS_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_USART3_RTS_GPIO_PORT, gpio_init.Pin);

  gpio_init.Pin = BUS_USART3_CTS_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_USART3_CTS_GPIO_PORT, gpio_init.Pin);

  HAL_NVIC_DisableIRQ(BUS_USART3_IRQn);

  /* Disable SPI2 clock */
  BUS_USART3_CLK_DISABLE();
}

/**
  * @brief  Initialize USART3 HAL
  * @return BSP status
  */
int32_t BSP_USART3_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;
  hbus_usart3.Instance = BUS_USART3;
  if (USART3InitCounter++ == 0)
  {
    if (HAL_UART_GetState(&hbus_usart3) == HAL_UART_STATE_RESET)
    {
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
      /* Init the BUS Msp */
      USART3_MspInit(&hbus_usart3);
#else
      if (IsUSART3MspCbValid == 0U)
      {
        if (BSP_USART3_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_MSP_FAILURE;
        }
      }
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
      if (MX_USART3_Init(&hbus_usart3, BUS_USART3_BAUDRATE) != HAL_OK)
      {
        ret = BSP_ERROR_BUS_FAILURE;
      }
    }
  }
  return ret;
}

/**
  * @brief  DeInitialize UART HAL.
  * @retval BSP status
  */
int32_t BSP_USART3_DeInit(void)
{
  int32_t ret = BSP_ERROR_NONE;

  if (USART3InitCounter > 0)
  {
    USART3InitCounter = 0;
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
    /* DeInit the UART */
    USART3_MspDeInit(&hbus_usart3);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
    /* DeInit the UART */
    if (HAL_UART_DeInit(&hbus_usart3) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  UART Write a data to device.
  * @param  pData: pointer to data to send.
  * @param  Length: size of data to send.
  * @retval positive Length of data sent.
  * @retval negative error returned.
  */
int32_t BSP_USART3_Send(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

  if (HAL_UART_Transmit(&hbus_usart3, pData, Length, BUS_USART3_POLL_TIMEOUT) == HAL_OK)
  {
    ret = Length;
  }

  return ret;
}

/**
  * @brief  Reads data from device.
  * @param  pdata: pointer to data received.
  * @param  Length: length of data received.
  * @retval positive Length of data received.
  * @retval negative error returned.
  */
int32_t BSP_USART3_Recv(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

  if (HAL_UART_Receive(&hbus_usart3, pData, Length, BUS_USART3_POLL_TIMEOUT) == HAL_OK)
  {
    return Length;
  }

  return ret;
}

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default BSP USART3 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_USART3_RegisterDefaultMspCallbacks(void)
{
  __HAL_BSP_RESET_HANDLE_STATE(&hbus_usart3);

  /* Register MspInit Callback */
  if (HAL_UART_RegisterCallback(&hbus_usart3, HAL_UART_MSPINIT_CB_ID, USART3_MspInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_UART_RegisterCallback(&hbus_usart3, HAL_UART_MSPDEINIT_CB_ID, USART3_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsUSART3MspCbValid = 1;

  return BSP_ERROR_NONE;
}

/**
  * @brief BSP USART3 Bus Msp Callback registering
  * @param Callbacks     pointer to USART3 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_USART3_RegisterMspCallbacks(BSP_SPI_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_UART_RESET_HANDLE_STATE(&hbus_usart3);

  /* Register MspInit Callback */
  if (HAL_UART_RegisterCallback(&hbus_usart3, HAL_UART_MSPINIT_CB_ID, Callbacks->pMspInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_UART_RegisterCallback(&hbus_usart3, HAL_UART_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  IsUSART3MspCbValid = 1;

  return BSP_ERROR_NONE;
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
#endif /* HAL_UART_MODULE_ENABLED */

/**
  * @brief  Return system tick in ms
  * @retval Current HAL time base time stamp
  */
int32_t BSP_GetTick(void)
{
  return (int32_t)HAL_GetTick();
}

/**
  * @}
  */

/**
  * @}
  */    

/**
  * @}
  */ 
    
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
