/**
  ******************************************************************************
  * @file    jpeg_decode.h 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   Header for jpeg_decode.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
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
  
#ifndef __JPEG_DECODE_H__ 
#define __JPEG_DECODE_H__

#include "stdint.h"

/** @addtogroup Jpeg_Decode
  * @{
  */


uint32_t jpeg_decode(const char* jpeg, uint8_t (*callback)(uint8_t* , uint32_t));
void jpeg_getsize(const char* jpeg, uint32_t* Width, uint32_t* Height);
uint32_t jpeg_GetBufferSize( uint8_t *jpeg );

/** @brief Macro checking if a pointer points to the beginning of a JPEG picture. */
#define IS_JPEG(ptr) ((ptr[0] == 0xFF) && (ptr[1] == 0xD8))

/** @} */
#endif // __JPEG_DECODE_H__

/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
