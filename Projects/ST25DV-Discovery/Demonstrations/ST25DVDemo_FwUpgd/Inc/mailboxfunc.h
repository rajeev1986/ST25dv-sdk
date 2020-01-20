/**
  ******************************************************************************
  * @file    mailboxfunc.h 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   Header for mailboxfunc.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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
#ifndef __MAILBOXFUNC_H
#define __MAILBOXFUNC_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "stdbool.h"

/** @addtogroup ST25_Discovery_Demo
  * @{
  */

/** @addtogroup ST25DV_Mailbox_Functions
  * @ingroup ST25_Discovery_Demo
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/**
 * @brief  Mailbox global header information structure definition
 */
typedef struct
{
  uint16_t framesize;     /**< Size of current frame to transfer */
  uint8_t fctcode;        /**< Function code used to define the action of the requester */
  uint8_t cmdresp;        /**< Defines current frame command, answer, acknowledge */
  uint8_t error;          /**< Error code if an error is detected */
  uint8_t chaining;       /**< Defines if the frame is a simple frame (0) or chained frame (1) */
  uint8_t framelength;    /**< Informs the data length of the current frame */
  uint16_t totalchunk;    /**< Informs on the total number of chunk that will need to perform the transfer */
  uint16_t chunknb;       /**< Informs for a specific frame the current chunk number of that frame */
  uint32_t fulllength;    /**< Informs on the total length of data to transfer */
  uint8_t *pData;         /**< Pointer to buffer data to transfer */
} MB_HEADER_T;

typedef struct 
{
  uint8_t minute;
  uint8_t second;
  uint8_t hundredth;
} MB_STOPWATCH_T;


/* Exported constants --------------------------------------------------------*/
enum { MB_NOTCHAINED = 0, MB_CHAINED };

/* Exported macro ------------------------------------------------------------*/
#define MAXMAILBOXLENGTH      255
/* Payload=236 + Header=13 + flag=1 + crc16=2 = 252 (must be < 256) */
#define ALIGNED_FRAMESIZE     236

#define MB_FCTCODE            0
#define MB_CMDRESP            1
#define MB_ERROR              2
#define MB_CHAINING           3
#define MB_LENGTH             4
#define MB_DATA               5
#define MB_CH_FULLLENGTH      7
#define MB_CH_TOTALCHUNK      9
#define MB_CH_NBCHUNK         11
#define MB_CH_LENGTH          12
#define MB_CH_DATA            13

#define MB_MAXFUNCTION        0xFF
#define MB_R2HSIMPLETRANSFER  0x01
#define MB_R2HCHAINEDTRANFER  0x02
#define MB_R2HDATATRANSFER    0x03
#define MB_R2HFIRMWAREUPDATE  0x04
#define MB_H2RSIMPLETRANSFER  0x05
#define MB_H2RCHAINEDTRANSFER 0x06
#define MB_H2RIMAGEUPLOAD     0x07
#define MB_R2HPRESENTPASSWORD 0x08
#define MB_R2HIMAGEDOWNLOAD   0x09
#define MB_H2RDATATRANSFER    0x0A
#define MB_R2HSTOPWATCH       0x0B

#define MB_CANCELCOMMAND      0xF0
#define MB_RESETCOMMUNICATION 0xF1

#define MB_COMMAND            0
#define MB_RESPONSE           1
#define MB_ACKNOWLEDGE        2

#define MB_NOERROR            0
#define MB_DEFAULTERROR       1
#define MB_UNKNOWNFUNCTION    2
#define MB_BADREQUEST         3
#define MB_LENGTHERROR        4
#define MB_CHUNKERROR         5
#define MB_PROTOCOLERROR      6

#define MB_FWUPGDPWDTIMEOUT   60000  /* Timeout 1 min */

/* Exported functions ------------------------------------------------------- */
int32_t InitMailBoxMode( void );
int32_t DeInitMailBoxMode( void );
int32_t WriteMailBoxMsg( const uint8_t * const pData, const uint16_t NbBytes );
int32_t ReadCompleteMailBoxMsg( uint8_t * const pData, uint16_t * const plength );
int32_t ReadFragmentMailBoxMsg( uint8_t * const pData, const uint8_t Offset, const uint16_t NbBytes );
void MBDecodeHeader( const uint8_t * const pData, MB_HEADER_T * const mb_header );
void PrepareMBMsg( uint8_t * const pData, const MB_HEADER_T * const mb_header );
bool SendMBData( MB_HEADER_T * const mb_header, const uint8_t nbretry );

/**
  * @}
  */ 

/**
  * @}
  */ 

#endif /* __MAILBOXFUNC_H */

/************************ (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
