/**
  ******************************************************************************
  * @file    mailboxfunc.c 
  * @author  MMY Application Team
  * @version $Revision:$
  * @date    $Date:$
  * @ingroup ST25DV_Mailbox_Functions
  * @brief   Common Maibox functions used for Fast Transparent Mode protocol.
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
#include "mailboxfunc.h"

/** @addtogroup ST25_Discovery_Demo
  * @{
  */

/** @defgroup ST25DV_Mailbox_Functions ST25DV Mailbox functions
  * @brief This module proposes Common api for the ST25DV Mailbox.
  * @details The module covers following functions:
  * - Mailbox init.
  * - Read and Write to mailbox.
  * - Protocol header decode.
  * - Protocol message preparation.
  * - Protocol send message.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern volatile uint8_t GPO_Activated;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the Mailbox mode, Enables Mailbox and disables Mailbox Watchdog.
  * @param  None No parameters.
  * @return NFCTAG_StatusTypeDef status.
  */
int32_t InitMailBoxMode( void )
{
  int32_t ret = NFCTAG_OK;
  ST25DV_EN_STATUS MB_mode = ST25DV_DISABLE;
  
  /* If not activated, activate Mailbox */
  BSP_NFCTAG_ReadMBMode(BSP_NFCTAG_INSTANCE, &MB_mode);
  if( MB_mode == ST25DV_DISABLE )
  {
    ret = BSP_NFCTAG_WriteMBMode(BSP_NFCTAG_INSTANCE, ST25DV_ENABLE);
  }
  else
  {
    /* if already activated Clear MB content and flag */
    ret = BSP_NFCTAG_ResetMBEN_Dyn(BSP_NFCTAG_INSTANCE);
    ret |= BSP_NFCTAG_SetMBEN_Dyn(BSP_NFCTAG_INSTANCE);
  }
  
  /* Disable MB watchdog feature */
  ret |= BSP_NFCTAG_WriteMBWDG(BSP_NFCTAG_INSTANCE, 0);
  
  return ret;
}

/**
  * @brief  DeInitializes the Mailbox mode, disables mailbox mode.
  * @param  None No parameters.
  * @return NFCTAG_StatusTypeDef status.
  */
int32_t DeInitMailBoxMode( void )
{
  return BSP_NFCTAG_ResetMBEN_Dyn(BSP_NFCTAG_INSTANCE);
}

/**
  * @brief  Writes message in Mailbox.
  * @param  pData Pointer to the data to write.
  * @param  NbBytes Number of bytes to write.
  * @return NFCTAG_StatusTypeDef status.
  */
int32_t WriteMailBoxMsg( const uint8_t * const pData, const uint16_t NbBytes )
{
  int32_t ret = NFCTAG_OK;
  ST25DV_MB_CTRL_DYN_STATUS data = {0};
  
  /* Check if Mailbox is available */
  ret = BSP_NFCTAG_ReadMBCtrl_Dyn(BSP_NFCTAG_INSTANCE, &data);
  if( ret != NFCTAG_OK )
  {
    HAL_Delay(20);
    return ret;
  }
  
  /* If available, write data */
  if( (data.HostPutMsg == 0) && (data.RfPutMsg == 0) )
  {
    ret = BSP_NFCTAG_WriteMailboxData(BSP_NFCTAG_INSTANCE, pData, NbBytes);
  } 
  else 
  {
    HAL_Delay(20);
    return NFCTAG_BUSY;
  }
  
  return ret;
}

/**
  * @brief  Reads entire Mailbox Message from the tag.
  * @param  pData Pointer to the read data to store.
  * @param  pLength Number of bytes to read.
  * @return NFCTAG_StatusTypeDef status.
  */
int32_t ReadCompleteMailBoxMsg( uint8_t * const pData, uint16_t * const pLength )
{
  int32_t ret = NFCTAG_OK;
  uint16_t mblength = 0;
  
  /* Read length of message */
  ret = BSP_NFCTAG_ReadMBLength_Dyn(BSP_NFCTAG_INSTANCE, (uint8_t *)&mblength);
  if( ret != NFCTAG_OK )
  {
    return ret;
  }
  *pLength = mblength + 1;
  
  /* Read all data in Mailbox */
  return BSP_NFCTAG_ReadMailboxData(BSP_NFCTAG_INSTANCE, pData, 0, *pLength);
}

/**
  * @brief  Reads part of Mailbox Message from the tag.
  * @param  pData Pointer to store data.
  * @param  Offset Offset in Mailbox to start reading.
  * @param  NbBytes Number of bytes to read.
  * @return NFCTAG_StatusTypeDef status.
  */
int32_t ReadFragmentMailBoxMsg( uint8_t * const pData, const uint8_t Offset, const uint16_t NbBytes )
{
  return BSP_NFCTAG_ReadMailboxData(BSP_NFCTAG_INSTANCE, pData, Offset, NbBytes);
}

/**
  * @brief  Extracts global information from header in Fast transfer mode protocol.
  * @param  pData Pointer to the mailbox frame.
  * @param  mb_header Pointer to structure for storing global header info.
  * @return None.
  */
void MBDecodeHeader( const uint8_t * const pData, MB_HEADER_T * const mb_header )
{
  mb_header->fctcode = pData[MB_FCTCODE];
  mb_header->cmdresp = pData[MB_CMDRESP];
  mb_header->error = pData[MB_ERROR];
  mb_header->chaining = pData[MB_CHAINING];
  if( mb_header->chaining == MB_NOTCHAINED )
  {
    /* If simple message header is 5-byte long */
    mb_header->framelength = pData[MB_LENGTH];
    mb_header->framesize = MB_DATA + mb_header->framelength;
  }
  else
  {
    /* If simple message header is 13-byte long */
    mb_header->fulllength = pData[MB_CH_FULLLENGTH - 3] & 0xFF;
    mb_header->fulllength = (mb_header->fulllength << 8) | pData[MB_CH_FULLLENGTH - 2];
    mb_header->fulllength = (mb_header->fulllength << 8) | pData[MB_CH_FULLLENGTH - 1];
    mb_header->fulllength = (mb_header->fulllength << 8) | pData[MB_CH_FULLLENGTH];
    mb_header->totalchunk = pData[MB_CH_TOTALCHUNK - 1] & 0xFF;
    mb_header->totalchunk = (mb_header->totalchunk << 8) | pData[MB_CH_TOTALCHUNK];
    mb_header->chunknb = pData[MB_CH_NBCHUNK - 1] & 0xFF;
    mb_header->chunknb = (mb_header->chunknb << 8) | pData[MB_CH_NBCHUNK];
    mb_header->framelength = pData[MB_CH_LENGTH];
    mb_header->framesize = MB_CH_LENGTH + mb_header->framelength;
  }
}

/**
  * @brief  Prepares header message to send to Mailbox.
  * @param  pData Pointer the the mailbox frame.
  * @param  mb_header Pointer to the header information structure.
  * @return None.
  */
void PrepareMBMsg( uint8_t * const pData, const MB_HEADER_T * const mb_header )
{
  pData[MB_FCTCODE] = mb_header->fctcode;
  pData[MB_CMDRESP] = mb_header->cmdresp;
  pData[MB_ERROR] = mb_header->error;
  pData[MB_CHAINING] = mb_header->chaining;
  if( mb_header->chaining == MB_NOTCHAINED )
  {
    pData[MB_LENGTH] = mb_header->framelength;
  }
  else
  {
    pData[MB_CH_FULLLENGTH - 3] = (mb_header->fulllength >> 24) & 0xFF;
    pData[MB_CH_FULLLENGTH - 2] = (mb_header->fulllength >> 16) & 0xFF;
    pData[MB_CH_FULLLENGTH - 1] = (mb_header->fulllength >> 8) & 0xFF;
    pData[MB_CH_FULLLENGTH] = mb_header->fulllength & 0xFF;
    pData[MB_CH_TOTALCHUNK - 1] = (mb_header->totalchunk >> 8) & 0xFF;
    pData[MB_CH_TOTALCHUNK] = mb_header->totalchunk & 0xFF;
    pData[MB_CH_NBCHUNK - 1] = (mb_header->chunknb >> 8) & 0xFF;
    pData[MB_CH_NBCHUNK] = mb_header->chunknb & 0xFF;
    pData[MB_CH_LENGTH] = mb_header->framelength;
  }
}

/**
  * @brief  Prepares and writes frame in Mailbox.
  * @param  mb_header Pointer to structure containing frame header info.
  * @param  nbretry Number of attempts.
  * @retval 1 Message was written to Mailbox.
  * @retval 0 Message was not written to Mailbox.
  */
bool SendMBData( MB_HEADER_T * const mb_header, const uint8_t nbretry )
{
  uint8_t st25dv_sendMB[256];
  uint8_t header_size = 0;
  uint32_t cnt = 0;
  int32_t ret;
  
  /* Write header to data frame */
  PrepareMBMsg( st25dv_sendMB, mb_header );

  /* Check type of frame chained or not */
  if( mb_header->chaining == MB_CHAINED )
  {
    header_size = MB_CH_DATA;
  }
  else
  {
    header_size = MB_DATA;
  }
  
  /* Copy data to buffer after header */
  for( cnt = 0; cnt < mb_header->framelength; cnt++ )
  {
    st25dv_sendMB[cnt + header_size] = *(mb_header->pData + cnt);
  }
  
  /* Compute total frame size */
  mb_header->framesize = header_size + mb_header->framelength;
 
  /* Write message to Mailbox with nbretry tentative*/
  cnt = nbretry;
  do
  {
    ret = WriteMailBoxMsg( st25dv_sendMB, mb_header->framesize );
    if( ret != NFCTAG_OK )
    {
      cnt--;
    }
  }while( (ret != NFCTAG_OK) && (cnt > 0) );

  if( cnt == 0 )
  {
    return FAIL;
  }
  
  return PASS;
}
/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
