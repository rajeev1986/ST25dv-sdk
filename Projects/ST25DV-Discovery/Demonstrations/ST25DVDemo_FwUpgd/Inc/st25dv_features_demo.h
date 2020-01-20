/**
  ******************************************************************************
  * @file    st25dv_features_demo.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file defines the api for the ST25DV features demo.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
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
#ifndef __ST25DV_FEATURES_DEMO_H
#define __ST25DV_FEATURES_DEMO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported function	--------------------------------------------------------*/
//void ST25DV_DEMO_Led2_Pwm(void);

/** @addtogroup ST25DV_Features_Demo
  * @ingroup ST25_Discovery_Demo
  * @{
  */
void ST25DV_DEMO_EnergyHarvesting(void);
void ST25DV_DEMO_GPO(void);
void ST25DV_DEMO_RF_Off(void);
void ST25DV_DEMO_RF_Sleep(void);
void ST25DV_DEMO_Low_Power_Down(void);
//void ST25DV_DEMO_Memory_Mapping_Password(void);


/**
  * @}
  */


#endif /* __ST25DV_FEATURES_DEMO_H */

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
