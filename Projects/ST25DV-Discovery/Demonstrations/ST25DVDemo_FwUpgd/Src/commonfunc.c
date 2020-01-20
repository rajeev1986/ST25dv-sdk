/**
  ******************************************************************************
  * @file    commonfunc.c 
  * @author  MMY Application Team
  * @version $Revision:$
  * @date    $Date:$
  * @ingroup ST25DV_Common_Functions
  * @brief   Common functions for the ST25DV management.
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

/* Includes ------------------------------------------------------------------*/
#include "commonfunc.h"
#include "Menu_core.h"


/** @addtogroup ST25_Discovery_Demo
  * @{
  */

/** @defgroup ST25DV_Common_Functions ST25DV common functions
  * @brief This module proposes a generic api used for the ST25DV.
  * @details The module covers password presentation, GPO init and management.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* GPO polling variable from stm32f4xx_it.c */
extern volatile uint8_t GPO_Activated;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Present password to the ST25DV to open or close an i2c session.
  * @param  passwd TRUE open session, FALSE close session.
  * @return ST25DV_I2CSSO_STATUS status.
  */
ST25DV_I2CSSO_STATUS PresentPasswd( const bool passwd )
{
  ST25DV_PASSWD mypass;
  ST25DV_I2CSSO_STATUS i2csso = ST25DV_SESSION_CLOSED;
  
  BSP_NFCTAG_ReadI2CSecuritySession_Dyn(BSP_NFCTAG_INSTANCE, &i2csso);
  if( (i2csso == ST25DV_SESSION_CLOSED) && (passwd == true) )
  {
    mypass.MsbPasswd = 0x0;
    mypass.LsbPasswd = 0x0;
    
    BSP_NFCTAG_PresentI2CPassword(BSP_NFCTAG_INSTANCE, mypass);
  }
  else if( (i2csso == ST25DV_SESSION_OPEN) && (passwd == false) )
  {
    mypass.MsbPasswd = 0x12345678;
    mypass.LsbPasswd = 0x13245678;

    BSP_NFCTAG_PresentI2CPassword(BSP_NFCTAG_INSTANCE, mypass);
  }

  BSP_NFCTAG_ReadI2CSecuritySession_Dyn(BSP_NFCTAG_INSTANCE, &i2csso);
  
  return i2csso;
}

/**
  * @brief  Enable & initialize the GPO interrupt.
  * @param  ITConfig Value of the interrupt register to configure.
  * @return NFCTAG_StatusTypeDef status.
  */
int32_t InitITGPOMode( const uint16_t ITConfig )
{
  int32_t ret = NFCTAG_OK;
  ST25DV_UID uid;
  uint8_t ProductRef;
  
  /* Init GPO Activation Flag */
  GPO_Activated = 0;

  /* set correct edge setting depending on GPO technology */
  ST25_RETRY( BSP_NFCTAG_ReadUID(BSP_NFCTAG_INSTANCE, &uid) );
  ProductRef = (uid.MsbUid & 0xFF00) >> 8;
  if( ST25DV_AM_I_OPEN_DRAIN(ProductRef) )
  {
    /* GPO Open drain */
    BSP_GPO_Init(GPIO_MODE_IT_FALLING);
  } 
  else 
  {
    /* GPO CMOS */
    BSP_GPO_Init(GPIO_MODE_IT_RISING);
  }
  
  /* Set GPO Configuration */
  ret = BSP_NFCTAG_ConfigIT(BSP_NFCTAG_INSTANCE, ITConfig);

  return ret;
}

/**
  * @brief  Disable the GPO interrupt.
  * @param  None No parameters.
  * @return None.
  */
void DeInitITGPOMode( void )
{
  /* Disable interruption */
  BSP_GPO_DeInit();
  ST25_RETRY(BSP_NFCTAG_ConfigIT(BSP_NFCTAG_INSTANCE, 0));

}

/**
  * @brief  Manage GPO interruption action.
  */
void BSP_GPO_Callback(void)
{
  GPO_Activated = 1;
}

/**
  * @brief  Reads the GPO interrupt source.
  * @details This function reads the interrupt status register from the ST25DV to report which interrupt(s) occured.
  * @param gpo Pointer on IT_GPO_STATUS structure, to return the status of the GPO irq.
  * @return None.
  */
void ManageGPO( IT_GPO_STATUS * const gpo )
{
  uint8_t itstatus;
 
  if( GPO_Activated == 1 )
  {  
    GPO_Activated = 0;

    ST25_RETRY(BSP_NFCTAG_ReadITSTStatus_Dyn(BSP_NFCTAG_INSTANCE, &itstatus));
       
    if( (itstatus & ST25DV_ITSTS_DYN_RFUSERSTATE_MASK) == ST25DV_ITSTS_DYN_RFUSERSTATE_MASK )
    {
      gpo->Rfuser = 1;
    }
    if( (itstatus & ST25DV_ITSTS_DYN_RFACTIVITY_MASK) == ST25DV_ITSTS_DYN_RFACTIVITY_MASK )
    {
      gpo->RfActivity = 1;
    }   
    if( (itstatus & ST25DV_ITSTS_DYN_RFINTERRUPT_MASK) == ST25DV_ITSTS_DYN_RFINTERRUPT_MASK )
    {
      gpo->RfInterrupt = 1;
    }
  
    if( (itstatus & ST25DV_ITSTS_DYN_FIELDFALLING_MASK) == ST25DV_ITSTS_DYN_FIELDFALLING_MASK )
    {
      gpo->FieldOff = 1;
    }
    
    if( (itstatus & ST25DV_ITSTS_DYN_FIELDRISING_MASK) == ST25DV_ITSTS_DYN_FIELDRISING_MASK )
    {
      gpo->FieldOn = 1;
    }
    
    if( (itstatus & ST25DV_ITSTS_DYN_RFPUTMSG_MASK) == ST25DV_ITSTS_DYN_RFPUTMSG_MASK )
    {
      gpo->MsgInMailbox = 1;
    }
    
    if( (itstatus & ST25DV_ITSTS_DYN_RFGETMSG_MASK) == ST25DV_ITSTS_DYN_RFGETMSG_MASK )
    {
      gpo->MailboxMsgRead = 1;
    }
    
    if( (itstatus & ST25DV_ITSTS_DYN_RFWRITE_MASK) == ST25DV_ITSTS_DYN_RFWRITE_MASK )
    {
      gpo->WriteInEEPROM = 1;
    }
  }
}

/**
  * @brief Display an error message depending on the return value of a NFCTAG driver function.
  * @param status Return value from a NFCTAG driver function.
  */
void st25_error(int32_t status)
{
  uint32_t error_index = 5;
  char titles[6][20] = {
                          "",                   /* Success, so no message displayed! */
                          "ST25DV I2C Error",
                          "ST25DV I2C Busy",
                          "ST25DV I2C Timeout",
                          "ST25DV I2C NACK",
                          "ST25DV I2C Unknown"
                        };
  
  switch(status)
  {
    case NFCTAG_OK:
      error_index = 0;
      break;
    
    case NFCTAG_ERROR:
      error_index = 1;
      break;
    
    case NFCTAG_BUSY:
      error_index = 2;
      break;
    
    case NFCTAG_TIMEOUT:
      error_index = 3;
      break;
    
    case NFCTAG_NACK:
      error_index = 4;
      break;
    
    default:
      error_index = 5;
      break;
  }

  Menu_MsgStatus(titles[error_index], "RF may be busy.\nPlease move the reader...\nOr reconnect the board.", MSG_STATUS_ERROR);
  /* I2C peripheral may be in a bad shape... */
  if((status == NFCTAG_BUSY) || (status == NFCTAG_NACK))
  {
    BSP_NFCTAG_DeInit(BSP_NFCTAG_INSTANCE);
    BSP_NFCTAG_Init(BSP_NFCTAG_INSTANCE);
  }
  /* May have been NACKed because password presentation failed. */
  /* And if user has disconnected the board, password may be required as well. */
  if((status == NFCTAG_NACK) && (BSP_NFCTAG_isInitialized(BSP_NFCTAG_INSTANCE)))
  {
    PresentPasswd(true);
  }
}


/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
