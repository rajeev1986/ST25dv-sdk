/**
  ******************************************************************************
  * @file    common.h 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   Header for main.c module
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
#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include <string.h>
#include "st25dx_discovery_lcd.h"
#include "st25dx_discovery_nfctag.h"

/* Exported types ------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

#undef FAIL
#define FAIL 0

#undef PASS
#define PASS !FAIL

/* Exported constants --------------------------------------------------------*/
#define MAX_NDEF_MEM                 ((uint32_t)NFCTAG_64K_SIZE)
#define NFC_DEVICE_MAX_NDEFMEMORY    ((uint32_t)MAX_NDEF_MEM)
	
#define RESULTOK                     0x00
#define ERRORCODE_GENERIC            1
/* Exported functions ------------------------------------------------------- */


#endif /* __COMMON_H */

/************************ (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
