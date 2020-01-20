/**
  ******************************************************************************
  * @file    version.h 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   Header for firmware version number
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VERSION_H
#define __VERSION_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define TOSTRING(s) #s
#define STRINGIZE(s) TOSTRING(s)

#define MCU_FW_VERSION_MAJOR              1	
#define MCU_FW_VERSION_MINOR              1
#define MCU_FW_VERSION_REVISION           0
#define MCU_FW_VERSION                    STRINGIZE(MCU_FW_VERSION_MAJOR) "." \
                                          STRINGIZE(MCU_FW_VERSION_MINOR) "." \
                                          STRINGIZE(MCU_FW_VERSION_REVISION)
/* Exported functions ------------------------------------------------------- */

#endif /* __VERSION_H */

/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
