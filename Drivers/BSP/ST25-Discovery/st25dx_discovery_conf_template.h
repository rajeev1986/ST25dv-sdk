/**
 ******************************************************************************
 * @file    st25dx_discovery_conf.h
 * @author  MMY Application Team
 * @brief   This file contains definitions for the st25dv components bus interfaces
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

#ifndef ST25DX_DISCOVERY_CONF_H_
#define ST25DX_DISCOVERY_CONF_H_

#include "stm32f4xx_hal.h"
#include "st25dx_discovery_bus.h"
#include "st25dx_discovery_errno.h"

/** @defgroup ST25DX_DISCOVERY_Exported_Constants
  * @{
  */

#define BSP_BUTTON_USER_IT_PRIORITY         0x0FU

#define ST25DX_DISCOVERY_TS_I2C_ADDRESS     0x82

#define ST25DX_DISCOVERY_INIT_CLK_GPO_RFD() __HAL_RCC_GPIOE_CLK_ENABLE( );
#define ST25DX_DISCOVERY_INIT_CLK_LPD_RFD() __HAL_RCC_GPIOA_CLK_ENABLE( );

#define BSP_NFCTAG_BUS_HANDLE               (hbus_i2c2)
#define BSP_NFCTAG_INSTANCE                 0U

#define BSP_GPO_EXTI                        EXTI15_10_IRQn
#define BSP_GPO_PRIORITY                    0U
#define BSP_GPO_EXTI_LINE                   (EXTI_LINE_15)

#ifndef USE_BSP_COM_FEATURE
#define USE_BSP_COM_FEATURE                 1U
#endif

#define USE_COM_LOG                         1U

/**
  * @}
  */


#endif /* ST25DX_DISCOVERY_CONF_H_ */
