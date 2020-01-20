/**
  ******************************************************************************
  * @file    bluenrg_interface.h
  * @author  CL
  * @version $Revision$
  * @date    $Date$
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
  *
  * Licensed under MMY-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BLUENRG_INTERFACE_H_
#define __BLUENRG_INTERFACE_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32_bluenrg_ble.h"
#include "hal_types.h"

void Hal_Write_Serial(const void* data1, const void* data2, int32_t n_bytes1,
                      int32_t n_bytes2);



#endif /*__BLUENRG_INTERFACE_H_ */

/************************ (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
