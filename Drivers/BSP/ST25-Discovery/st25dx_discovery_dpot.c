/**
  ******************************************************************************
  * @file    st25dx_discovery_dpot.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file provides a set of functions needed to manage Digital
  *          potentiometer available with ad5161 device mounted on
  *          ST25-Discovery Kit.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
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

#include "st25dx_discovery_dpot.h"
#include "st25dx_discovery_bus.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup ST25DX_DISCOVERY_BSP
  * @{
  */

/** @defgroup ST25DX_DISCOVERY_DPOT ST25 DISCOVERY DIGITAL POTENTIOMETER
  * @{
  */

static DPOT_DrvTypeDef *DpotDrv;
/**
  * @brief  Configures all necessary hardware resources (GPIOs, clocks..).
  * @retval DPOT_OK: if all initializations are OK. Other value if error.
  */
uint8_t BSP_DPOT_Init(void)
{

  /* Initialize the DPOT driver structure */
  DpotDrv = &ad5161_dpot_drv;

  DpotDrv->Init(BSP_DPOT_I2C_ADDR);

  return DPOT_OK;
}

/**
  * @brief  Configures the DPOT_I2C interface.
  */
void DPOT_IO_Init(uint8_t Addr)
{
  BSP_I2C3_Init();
}

/**
  * @brief  DPOT Reads single data.
  * @param  Addr: I2C Address.
  * @param  Reg: Reg Address.
  * @retval The read data.
  */
uint8_t DPOT_IO_Read(uint8_t Addr)
{
  uint8_t data = 0;

  BSP_I2C3_Recv(Addr, &data, 1);

  return data;
}

/**
  * @brief  DPOT Writes buffer.
  * @param  Addr: I2C Address.
  * @param  pBuf: pointer to data buffer.
  * @param  Len: length of the data.
  * @return None.
  */
void DPOT_IO_WriteBuffer(uint8_t Addr, uint8_t *pBuf, uint16_t Len)
{
  BSP_I2C3_Send(Addr, pBuf, Len);
}

/**
  * @brief  Returns DPOT resistor value.
  * @retval Resistor value
  */
uint32_t BSP_DPOT_GetResistor(void)
{
  return DpotDrv->GetResistor(BSP_DPOT_I2C_ADDR);
}

/**
  * @brief  Set DPOT resistor value.
  * @param Resistor value
  * @retval None
  */
void BSP_DPOT_SetResistor(uint32_t Val)
{
  uint8_t resistor_value = 0;

  if (Val <= AD5161_WIPER_RESISTANCE)
  {
    resistor_value = 0x00;
  }
  else if (Val > 100000)
  {
    resistor_value = 0xFF;
  }
  else
  {
    resistor_value = ((Val - AD5161_WIPER_RESISTANCE) * AD5161_WIPER_PRECISION) / AD5161_RAB_RESISTANCE;
  }

  DpotDrv->SetResistor(BSP_DPOT_I2C_ADDR, resistor_value);
}

/**
  * @brief  Set DPOT in Reset state.
  * @retval None
  */
void BSP_DPOT_Reset(void)
{
  DpotDrv->Reset(BSP_DPOT_I2C_ADDR);
}

/**
  * @brief  Set DPOT in Reset state.
  * @retval None
  */
void BSP_DPOT_Shutdown(void)
{
  DpotDrv->Shutdown(BSP_DPOT_I2C_ADDR);
}

/**
  * @brief  Access to the extended features of the digital potentiometer.
  * @return Pointer on the Extended Component Structure for the nfctag.
  */
DPOT_ExtDrvTypeDef *BSP_DPOT_GetExtended_Drv(void)
{
  return (DPOT_ExtDrvTypeDef *)DpotDrv->pData;
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
