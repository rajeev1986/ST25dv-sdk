/**
  ******************************************************************************
  * @file    mailbox.c 
  * @author  MMY Application Team
  * @version $Revision:$
  * @date    $Date:$
  * @ingroup Fast_Transfer_Mode
  * @brief   Mailbox functions to illustrate the Fast Transfer Mode feature.
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
#include "mailbox.h"
#include "fw_command.h"
#include "Menu_config.h"
#include "ndef_demo.h"
#include "st25dx_discovery_ts.h"
#include "jpeg_decode.h"
#include "pictures_48x48_jpg.h"
#include "home_48x48_jpg.h"
//#include "CustomSizeH2R_jpg.h"
#include "bridgebynight_jpg.h"
#include "mountainview_jpg.h"
#include "running_jpg.h"
#include "stwireless_jpg.h"
#include "datatransfer_48x48_jpg.h"
#include "st25_80x80_jpg.h"
#include "font22_spec_char.c"
#include <stdio.h>

/** @addtogroup ST25_Discovery_Demo
  * @{
  */

/** @defgroup Fast_Transfer_Mode Fast transfer mode demo
  * @brief This module provides functions to manage the protocol for the Fast Transfer Mode demos.
  * @details The usecases demonstrated are:
  * - Data transfer.
  * - Firmware upgrade.
  * - Download pictures.
  * - Upload pictures.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static bool ExitFunction = false;
static uint32_t fwpassword = 0xDF8A8A2B;
uint32_t number = 0;
/** AAR definition for ST Android app */
static const sAARInfo w_aar = {.PackageName = "com.st.st25nfc"};

/* Imported variables --------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void Clear_FTM_Screen( void );
static int KeyboardCallback(char input);
static uint32_t Get_Custom_Transfer_Length(void);
static void FTM_StopWatch(uint8_t *buff, MB_STOPWATCH_T *prev_stopwatch, uint8_t first_latency);

/* Public functions ---------------------------------------------------------*/
/**
  * @brief Manage data exchange protocol with reader through Mailbox.
  * @param None No parameters.
  * @return None.
  */
void FTMManagement( void )
{
  bool allowfwupg = false;
  uint8_t st25dv_receiveMB[256];
  uint8_t requestcmd = 0;
  uint8_t currentcmd = 0;
  char displaystring[220] = "";
  const char *picture[] = { cMountainView, cBridgeByNight, cRunning, cStWireless, cSt25 };
  const uint32_t picture_sizes[] = {sizeof(cMountainView),
                                    sizeof(cBridgeByNight),
                                    sizeof(cRunning),
                                    sizeof(cStWireless),
                                    sizeof(cSt25)};
  uint16_t mb_readlength = 0;
  uint32_t mb_cmdmngt[MB_MAXFUNCTION];
  uint32_t cnt = 0;
  uint32_t compute_crc = 0;
  uint32_t received_data = 0;
  uint32_t last_chunck = 0;
  uint32_t fwpasswordtimeout = 0;
  uint32_t jpegbuffersize = 0;
  uint32_t time_elapse_ms = 0;
  uint32_t dx = 0;
  uint32_t dy = 0;
  uint32_t iwidth = 0;
  uint32_t iheight = 0;
  float CurrentPercSize = 0;
  int32_t ret = NFCTAG_OK;
  IT_GPO_STATUS gpo_status = {0};
  MB_HEADER_T mb_header[MB_MAXFUNCTION];
  CRC_HandleTypeDef hcrc;
  TS_StateTypeDef tstap;
  
  MB_STOPWATCH_T stopwatch = {0};
  uint8_t stopwatch_first_frame = 1;
  
  dx = Menu_GetDisplayWidth( );
  dy = Menu_GetDisplayHeight( );
  
  Menu_DisplayClear( );
  Menu_DisplayCenterString( Line4, "Starting Demo..." );
  
  /* Writes the ST25 app AAR */
  NDEF_DEMO_Init_Tag();
  NDEF_ClearNDEF();
  NDEF_AddAAR (&w_aar);

  /* Init end function variable */
  ExitFunction = false;
  
  /* Erase Flash for Data save */
  if( COMMAND_EraseFlash( USER_DATA_ADDRESS ) )
  {
    Menu_MsgStatus( "FTM Failure!", "Flash cannot be erased!", MSG_STATUS_ERROR );
    ExitFunction = true;
  }
  else
  {
    /* Display FTM menu */
    Clear_FTM_Screen( );
    
    /* Present password to change ST25DV configuration */
    PresentPasswd( true );
    
    /* Disable Energy Harvesting */
    ret = BSP_NFCTAG_ResetEHENMode_Dyn(BSP_NFCTAG_INSTANCE);

    /* Initialize Mailbox for FTM */
    ret |= InitMailBoxMode( );
    if( ret != NFCTAG_OK )
    {
      Menu_MsgStatus( "FTM Failure!", "Device cannot be configured!", MSG_STATUS_ERROR );
      ExitFunction = true;
    }
    else
    {
      /* Initialise GPO interruption */
      ret = BSP_NFCTAG_WriteITPulse(BSP_NFCTAG_INSTANCE, ST25DV_302_US);
      ret |= InitITGPOMode( ST25DV_GPO_RFPUTMSG_MASK | ST25DV_GPO_RFGETMSG_MASK | ST25DV_GPO_ENABLE_MASK );
      if( ret != NFCTAG_OK )
      {
        Menu_MsgStatus( "FTM Failure!", "Device cannot be configured!", MSG_STATUS_ERROR );
        ExitFunction = true;
      }
      else
      {
        /* initialize CRC Driver */
        hcrc.Instance = CRC;
        hcrc.Lock = HAL_UNLOCKED;
        hcrc.State = HAL_CRC_STATE_RESET;
        HAL_CRC_Init( &hcrc );
        
        /* Init array to store information for each function code */
        for( cnt = 0; cnt < MB_MAXFUNCTION; cnt++ )
        {
          mb_cmdmngt[cnt] = 0;
        }
      }
    }
  }
  
  /* Start loop until ESC key is pressed */
  while( ExitFunction == false )
  {
    /* Check if GPO IT has raised */
    ManageGPO( &gpo_status );
    
    if( gpo_status.MsgInMailbox == 1 )
    {
      /* Msg in mailbox from reader reported */
      gpo_status.MsgInMailbox = 0;
         
      /* Read Mailbox */
      do
      {
        ret = ReadCompleteMailBoxMsg( st25dv_receiveMB, &mb_readlength );
      }while( ret != NFCTAG_OK );     
    
      /* Check function code received */
      switch( st25dv_receiveMB[MB_FCTCODE] )
      {
        case MB_R2HDATATRANSFER:
        case MB_R2HFIRMWAREUPDATE:
        case MB_R2HIMAGEDOWNLOAD:
          
          /* Store command value */
          currentcmd = st25dv_receiveMB[MB_FCTCODE];
          if( (currentcmd == MB_R2HFIRMWAREUPDATE) && (allowfwupg == false) )
          {
            Menu_MsgStatus( "Fw Upgrade Demo", "Need password for firmware upgrade!", MSG_STATUS_ERROR );
            /* Display FTM menu */
            InitMailBoxMode( );
            Clear_FTM_Screen( );
            break;
          }
          if( mb_cmdmngt[currentcmd] == 0 )
          {
            /* Transfer start */
            
            /*Display info depending on command received */
            Menu_MsgStatus( "Fast Transfer Demo", "", MSG_INFO);
            Menu_DisplayCenterString( Line10, "<--");
            Menu_SetStyle( PLAIN );
            
            if( currentcmd == MB_R2HDATATRANSFER )
            {
              Menu_DisplayCenterString( Line2, "Data transf. started" );
              allowfwupg = false;
            }
            else if( currentcmd == MB_R2HFIRMWAREUPDATE )
            {
              Menu_DisplayCenterString( Line2, "Fw upgrade started" );
            }
            else
            {
              Menu_DisplayCenterString( Line2, "Img download started" );
              allowfwupg = false;
            }
            
            /* Decode header from frame */
            MBDecodeHeader( st25dv_receiveMB, &mb_header[currentcmd] );

          if( ( (currentcmd == MB_R2HFIRMWAREUPDATE) && (mb_header[currentcmd].fulllength > FIRMWARE_FLASH_SIZE) ) ||
            ( ((currentcmd == MB_R2HDATATRANSFER) || (currentcmd == MB_R2HIMAGEDOWNLOAD )) && (mb_header[currentcmd].fulllength > USER_DATA_FLASH_SIZE) ))
          {
            Menu_MsgStatus( "FTM Demo", "Transfer size is bigger than receive buffer!", MSG_STATUS_ERROR );
           /* Display FTM menu */
            InitMailBoxMode( );
            Clear_FTM_Screen( );
            currentcmd = 0;
            break;
          }
            /* Prepare screen before download */
            sprintf( displaystring, "File size: %lu B", mb_header[currentcmd].chaining == MB_CHAINED ? mb_header[currentcmd].fulllength : mb_header[currentcmd].framelength );
            Menu_DisplayCenterString( Line3, displaystring );
            Menu_DisplayClearLine( Line4 );
            Menu_DisplayClearLine( Line5 );
            CurrentPercSize = 0;
            Menu_DrawProgressBarLine( Line5, CurrentPercSize );
            //Menu_DisplayPicture( 250, 168, cHomeSmall );
            time_elapse_ms = HAL_GetTick( );
            
            if( mb_header[currentcmd].chaining == MB_CHAINED )
            {
              /* Process chained frame */
              /* Store last chunk value */
              last_chunck = mb_header[currentcmd].chunknb;
              
              if( currentcmd == MB_R2HFIRMWAREUPDATE )
              {
                /* Store data in FLASH */
                Command_WriteBufferToFlash( FIRMWARE_ADDRESS, mb_cmdmngt[currentcmd], &st25dv_receiveMB[MB_CH_DATA], mb_header[currentcmd].framelength );
              }
              else
              {
                /* Store data in FLASH */
                Command_WriteBufferToFlash( USER_DATA_ADDRESS, mb_cmdmngt[currentcmd], &st25dv_receiveMB[MB_CH_DATA], mb_header[currentcmd].framelength );
              }
            }
            else
            {
              /* Only 1 frame communication */
              if( currentcmd == MB_R2HFIRMWAREUPDATE )
              {
                Command_WriteBufferToFlash( FIRMWARE_ADDRESS, mb_cmdmngt[currentcmd], &st25dv_receiveMB[MB_DATA], mb_header[currentcmd].framelength );
                
                /* Compute CRC on received data */
                compute_crc = HAL_CRC_Calculate( &hcrc, (uint32_t *)FIRMWARE_ADDRESS, (uint32_t)(mb_header[currentcmd].framelength/4) );
              }
              else
              {
                Command_WriteBufferToFlash( USER_DATA_ADDRESS, mb_cmdmngt[currentcmd], &st25dv_receiveMB[MB_DATA], mb_header[currentcmd].framelength );
                
                /* Compute CRC on received data */
                compute_crc = HAL_CRC_Calculate( &hcrc, (uint32_t *)USER_DATA_ADDRESS, (uint32_t)(mb_header[currentcmd].framelength/4) );
              }

              compute_crc = ((compute_crc << 24) & 0xFF000000) | ((compute_crc << 8) & 0x00FF0000) | ((compute_crc >> 8) & 0x0000FF00) | ((compute_crc >> 24) & 0x000000FF);

              mb_header[currentcmd].fctcode = currentcmd;

              /* Prepare crc to send to reader */
              mb_header[currentcmd].cmdresp = MB_RESPONSE;
              mb_header[currentcmd].error = MB_NOERROR;
              mb_header[currentcmd].chaining = MB_NOTCHAINED;
              mb_header[currentcmd].framelength = sizeof(compute_crc);
              mb_header[currentcmd].pData = (uint8_t *)&compute_crc;
              /* Send CRC to reader */
              SendMBData( &mb_header[currentcmd], 255 );
            }
            /* Store frame length for next steps */
            mb_cmdmngt[currentcmd] += mb_header[currentcmd].framelength;
            
            /* Update progress bar */
            if( mb_header[currentcmd].chaining == MB_CHAINED )
            {
              CurrentPercSize = ((mb_cmdmngt[currentcmd] * 100) / (float)(mb_header[currentcmd].fulllength));
            }
            else
            {
              CurrentPercSize = 100;
            }

            /* Complete Bar from NbPixelPrevious to NbPixelCurrent with step of 1px */
            Menu_DrawProgressBarLine( Line5, CurrentPercSize );
          }
          else
          {
            /* received following chunks */
            if( (mb_header[currentcmd].chunknb < mb_header[currentcmd].totalchunk) && (mb_header[currentcmd].chaining == MB_CHAINED) )
            {
              /* Decode header from frame */
              MBDecodeHeader( st25dv_receiveMB, &mb_header[currentcmd] );
              
              if( mb_header[currentcmd].chunknb > (last_chunck + 1) )
              {
                Menu_DisplayCenterString( Line7, "Missing chunk..." );
              }
              
              
              /************* Store Data **************/
              if( currentcmd == MB_R2HFIRMWAREUPDATE )
              { 
                /* Store data in FLASH */
                if( mb_header[currentcmd].chunknb == mb_header[currentcmd].totalchunk )
                {
                  Command_WriteBufferToFlash( FIRMWARE_ADDRESS, mb_cmdmngt[currentcmd], &st25dv_receiveMB[MB_CH_DATA], mb_header[currentcmd].framelength );
                }
                else
                {
                  Command_WriteBufferToFlash( FIRMWARE_ADDRESS, ((mb_header[currentcmd].chunknb - 1) * mb_header[currentcmd].framelength), &st25dv_receiveMB[MB_CH_DATA], mb_header[currentcmd].framelength );
                }
              }
              else
              {
                /* Store data in FLASH */
                if( mb_header[currentcmd].chunknb == mb_header[currentcmd].totalchunk )
                {
                  Command_WriteBufferToFlash( USER_DATA_ADDRESS, mb_cmdmngt[currentcmd], &st25dv_receiveMB[MB_CH_DATA], mb_header[currentcmd].framelength );
                }
                else
                {
                  Command_WriteBufferToFlash( USER_DATA_ADDRESS, ((mb_header[currentcmd].chunknb - 1) * mb_header[currentcmd].framelength), &st25dv_receiveMB[MB_CH_DATA], mb_header[currentcmd].framelength );
                }
              }
              /* Store frame length + size of data received */
              if( mb_header[currentcmd].chunknb == (last_chunck + 1) )
              {
                mb_cmdmngt[currentcmd] += mb_header[currentcmd].framelength;
              }
              /***************************************/
              
              
              /********* Update progress bar *********/
              CurrentPercSize = ((mb_cmdmngt[currentcmd] * 100) / (float)(mb_header[currentcmd].fulllength));

              /* Complete Bar from NbPixelPrevious to NbPixelCurrent with step of 1px */
              Menu_DrawProgressBarLine( Line5, CurrentPercSize );
              /***************************************/
              
              /* Update last chunk info */
              last_chunck = mb_header[currentcmd].chunknb;
              
              /******** Last chunk received *********/
              if( mb_header[currentcmd].chunknb == mb_header[currentcmd].totalchunk )
              {
                /* Check received data size */
                if( mb_cmdmngt[currentcmd] != mb_header[currentcmd].fulllength )
                {
                  Menu_DisplayCenterString( Line7, "Size error..." );
                }

                /*********** Compute CRC ************/
                if( currentcmd == MB_R2HFIRMWAREUPDATE )
                {
                  /* Compute CRC of received data */
                  compute_crc = HAL_CRC_Calculate( &hcrc, (uint32_t *)FIRMWARE_ADDRESS, (uint32_t)(mb_header[currentcmd].fulllength / 4) );
                }
                else
                {
                  /* Compute CRC of received data */
                  compute_crc = HAL_CRC_Calculate( &hcrc, (uint32_t *)USER_DATA_ADDRESS, (uint32_t)(mb_header[currentcmd].fulllength / 4) );
                }
                compute_crc = ((compute_crc << 24) & 0xFF000000) | ((compute_crc << 8) & 0x00FF0000) | ((compute_crc >> 8) & 0x0000FF00) | ((compute_crc >> 24) & 0x000000FF);
                /***************************************/
                
                /* Prepare CRC frame to send to reader */
                mb_header[currentcmd].fctcode = currentcmd;
                mb_header[currentcmd].cmdresp = MB_RESPONSE;
                mb_header[currentcmd].error = MB_NOERROR;
                mb_header[currentcmd].chaining = MB_NOTCHAINED;
                mb_header[currentcmd].framelength = sizeof(compute_crc);
                mb_header[currentcmd].pData = (uint8_t *)&compute_crc;
                /* Send CRC to reader */
                SendMBData( &mb_header[currentcmd], 255 );
              }
              /***************************************/
            }
            else
            {
              /******** Acknowledge received *********/
              /* Decode header from frame */
              MBDecodeHeader( st25dv_receiveMB, &mb_header[currentcmd] );
              if( (mb_header[currentcmd].cmdresp == MB_ACKNOWLEDGE) && (mb_header[currentcmd].error != MB_NOERROR) )
              {
                sprintf( displaystring, "CRC : 0x%lX\n\n\nFile size: %lu B", compute_crc, mb_cmdmngt[currentcmd] );
                Menu_MsgStatus( "Transfer failed!", displaystring, MSG_STATUS_ERROR );
                Clear_FTM_Screen( );
                /* Erase Flash for Data save */
                if( currentcmd == MB_R2HFIRMWAREUPDATE )
                {
                  COMMAND_EraseFlash( FIRMWARE_ADDRESS );
                }
                else if( currentcmd == MB_R2HIMAGEDOWNLOAD )
                {
                  COMMAND_EraseFlash( USER_DATA_ADDRESS );
                }
              }
              else
              {
                time_elapse_ms = (HAL_GetTick( ) - time_elapse_ms);
                sprintf( displaystring, "CRC : 0x%lX\n\n\nDuration: %lu ms", compute_crc, time_elapse_ms );
                Menu_MsgStatus( "File transfer done!", displaystring, MSG_STATUS_SUCCESS );  
                Menu_DisplayClear();
                
                /* End of communication */
                if( currentcmd == MB_R2HFIRMWAREUPDATE )
                {
                  allowfwupg = false;
                  /* Jump to new Firmware */
                  COMMAND_Jump();
                }
                else if( currentcmd == MB_R2HIMAGEDOWNLOAD )
                {
                  /* Display Image */
                  Menu_DisplayPicture( 0, 0, (char *)USER_DATA_ADDRESS );
                  
                  /* Erase Flash for Data save */
                  COMMAND_EraseFlash( USER_DATA_ADDRESS );
                  
                  BSP_TS_GetState( &tstap );
                  while( tstap.TouchDetected == 0 )
                  {
                    HAL_Delay( 50 );
                    BSP_TS_GetState( &tstap );
                  }
                  Clear_FTM_Screen( );
                }
                else
                {
                  /* Erase Flash for Data save */
                  COMMAND_EraseFlash( USER_DATA_ADDRESS );
                  Clear_FTM_Screen( );
                }
              }
              /***************************************/
              mb_cmdmngt[currentcmd] = 0;
              currentcmd = 0;
            }
          }
          break;
          
        case MB_H2RDATATRANSFER:
        case MB_H2RIMAGEUPLOAD:
            
            /* Store command value */
            currentcmd = st25dv_receiveMB[MB_FCTCODE];
            if( mb_cmdmngt[currentcmd] != 0 )
            {
              received_data = st25dv_receiveMB[MB_DATA];
              received_data = (received_data << 8) | (st25dv_receiveMB[MB_DATA+1]);
              received_data = (received_data << 8) | (st25dv_receiveMB[MB_DATA+2]);
              received_data = (received_data << 8) | (st25dv_receiveMB[MB_DATA+3]);
              /* Prepare acknowledge answer */
              mb_header[currentcmd].fctcode = currentcmd;
              mb_header[currentcmd].cmdresp = MB_ACKNOWLEDGE;
              if( compute_crc != received_data )
              {
                mb_header[currentcmd].error = MB_DEFAULTERROR;
              }
              else
              {
                mb_header[currentcmd].error = MB_NOERROR;
              }
              
              mb_header[currentcmd].chaining = MB_NOTCHAINED;
              mb_header[currentcmd].framelength = 0;
              SendMBData( &mb_header[currentcmd], 255 );
              sprintf( displaystring, "CRC : 0x%lX", compute_crc );
              if( compute_crc != received_data )
              {
                Menu_MsgStatus( "Transfer failed!!!", displaystring, MSG_STATUS_ERROR );
              }
              else
              {
                time_elapse_ms = (HAL_GetTick( ) - time_elapse_ms);
                sprintf( displaystring, "CRC : 0x%lX\n\n\nDuration: %lu ms", compute_crc, time_elapse_ms );
                Menu_MsgStatus( "File transfer done!", displaystring, MSG_STATUS_SUCCESS );
              }
              mb_cmdmngt[currentcmd] = 0;
              requestcmd = 0;
              currentcmd = 0;

              Clear_FTM_Screen( );
            }
            break;
          
        case MB_R2HPRESENTPASSWORD:
          
          /* Decode header from frame */
          MBDecodeHeader( st25dv_receiveMB, &mb_header[MB_R2HPRESENTPASSWORD] );
          /* Extract password */
          received_data = st25dv_receiveMB[MB_DATA];
          received_data = (received_data << 8) | (st25dv_receiveMB[MB_DATA + 1]);
          received_data = (received_data << 8) | (st25dv_receiveMB[MB_DATA + 2]);
          received_data = (received_data << 8) | (st25dv_receiveMB[MB_DATA + 3]);

          /* Compute CRC of received data */
          received_data = HAL_CRC_Calculate( &hcrc, &received_data, 1 );
        
          /* Check firmware upgrade password */
          if( fwpassword == received_data )
          {
            allowfwupg = true;
            fwpasswordtimeout = HAL_GetTick( );
          }
          else
          {
            allowfwupg = false;
          }

          /* Answer Ok if password is good */
          mb_header[currentcmd].fctcode = MB_R2HPRESENTPASSWORD;

          /* Prepare crc to send to reader */
          mb_header[currentcmd].cmdresp = MB_RESPONSE;
          if( allowfwupg == false )
          {
            mb_header[currentcmd].error = MB_BADREQUEST;
          }
          else
          {
            mb_header[currentcmd].error = MB_NOERROR;
          }
          mb_header[currentcmd].chaining = MB_NOTCHAINED;
          mb_header[currentcmd].framelength = 0x0;
          /* Send Answer to reader */
          SendMBData( &mb_header[currentcmd], 255 );
          
          if( fwpassword == received_data )
          {
            Menu_DisplayCenterString( Line4, "Init Fw Flash..." );
            Menu_SetStyle( PLAIN );
            Menu_DisplayClearLine( Line5 );
            /* Erase Flash for Data save */
            COMMAND_EraseFlash( FIRMWARE_ADDRESS );
            
            BSP_LCD_SetBackColor( LCD_COLOR_WHITE );
            BSP_LCD_SetTextColor( LCD_COLOR_GREEN );
            Menu_DisplayCenterString( Line4, "Password OK!" );
            Menu_DisplayCenterString( Line5, "Fw upgrade granted!!" );
            Menu_SetStyle( PLAIN );
          }
          else
          {
            BSP_LCD_SetBackColor( LCD_COLOR_WHITE );
            BSP_LCD_SetTextColor( LCD_COLOR_RED );
            Menu_DisplayCenterString( Line4, "Wrong Password" );
            Menu_DisplayCenterString( Line5, "Fw upgrade denied!!!" );
            Menu_SetStyle( PLAIN );
            HAL_Delay( 3000 );
            Clear_FTM_Screen( );
          }
          
          break;
        
        case MB_CANCELCOMMAND:
          
          /* Decode header from frame */
          MBDecodeHeader( st25dv_receiveMB, &mb_header[MB_CANCELCOMMAND] );
          switch( st25dv_receiveMB[MB_DATA] )
          {
            case MB_R2HFIRMWAREUPDATE:
              
              mb_cmdmngt[MB_R2HFIRMWAREUPDATE] = 0;
              allowfwupg = false;
              fwpasswordtimeout = 0;
              compute_crc = 0;
              last_chunck = 0;
              break;
            
            case MB_H2RSIMPLETRANSFER:
              
              mb_cmdmngt[MB_H2RSIMPLETRANSFER] = 0;
              compute_crc = 0;
              last_chunck = 0;
              break;
            
            default:
              
              break;
          }
          break;
        
        case MB_RESETCOMMUNICATION:
          
          allowfwupg = false;
          requestcmd = 0;
          currentcmd = 0;
          mb_readlength = 0;
          compute_crc = 0;
          received_data = 0;
          last_chunck = 0;
          fwpasswordtimeout = 0;
        
          /* Erase Flash for Data save */
          COMMAND_EraseFlash( USER_DATA_ADDRESS );
          
          /* Init array to store information for each function code */
          for( cnt = 0; cnt < MB_MAXFUNCTION; cnt++ )
          {
            mb_cmdmngt[cnt] = 0;
          }
          break;
        case MB_R2HSTOPWATCH:
          /* Detect if this is the start of the demo */
          if(currentcmd != MB_R2HSTOPWATCH)
          {
            stopwatch_first_frame = 1;
            currentcmd = MB_R2HSTOPWATCH;
          }
          /* Display the demo screen ~90ms */
          if(stopwatch_first_frame)
          {
            Menu_MsgStatus( "Fast Transfer Demo", "", MSG_INFO);
            Menu_DisplayCenterString( Line10, "<--" );
            Menu_SetStyle( PLAIN );
            BSP_LCD_SetFont(&Font36);
            strcpy(displaystring,"00:00:00");
            Menu_DisplayCenterString(Line2,displaystring);
            BSP_LCD_SetFont(&Font22);
            strcpy(displaystring,"  T btw Frames:##ms");
            Menu_DisplayString(Line6,displaystring);
            Menu_DisplaySpecChar(Line6, 1, (char *)Font22_DeltaChar);
            strcpy(displaystring,"        Max   :##ms");
            Menu_DisplayString(Line7,displaystring);
            allowfwupg = false;
          }
          /* Update the demo screen ~ 12ms */
          FTM_StopWatch(st25dv_receiveMB,&stopwatch,stopwatch_first_frame);
          stopwatch_first_frame = false;
          break;

        default:
          
          break;
      }    

    }
    
    if( gpo_status.MailboxMsgRead == 1 )
    {
      /* Msg read by reader reported */
      gpo_status.MailboxMsgRead = 0;
      /* Check each function to send next frame if needed */
      for( cnt = 0; cnt < MB_MAXFUNCTION; cnt++ )
      {
        if( mb_cmdmngt[cnt] != 0)
        {
          switch(cnt)
          {
            case MB_R2HDATATRANSFER:
              
              break;
            
            case MB_R2HFIRMWAREUPDATE:
              
              break;
            
            case MB_R2HIMAGEDOWNLOAD:
              
              break;
              
            case MB_H2RDATATRANSFER:
            case MB_H2RIMAGEUPLOAD:
              
              currentcmd = cnt;
              if( mb_cmdmngt[currentcmd] != mb_header[currentcmd].fulllength )
              {
                if( mb_header[currentcmd].chunknb == 1 )
                {
                  time_elapse_ms = HAL_GetTick( );
                }
                mb_header[currentcmd].chunknb++;
                
                /* point Data to next part in buffer */
                /* Unless it's the 32kByte case where same data is written each time */
                if( currentcmd == MB_H2RIMAGEUPLOAD )
                {
                  mb_header[MB_H2RIMAGEUPLOAD].pData += mb_header[MB_H2RIMAGEUPLOAD].framelength;
                }
                /* Check if last chunk with less payload or normal payload size */
                if( (mb_header[currentcmd].fulllength - mb_cmdmngt[currentcmd]) > ALIGNED_FRAMESIZE )
                {
                  mb_header[currentcmd].framelength = ALIGNED_FRAMESIZE;
                  /* Compute CRC with following frames */
                  compute_crc = HAL_CRC_Accumulate( &hcrc, (uint32_t *)mb_header[currentcmd].pData, (mb_header[currentcmd].framelength / 4) );
                }
                else
                {
                  /* Last chunk to send */
                  mb_header[currentcmd].framelength = mb_header[currentcmd].fulllength - mb_cmdmngt[currentcmd];

                  compute_crc = HAL_CRC_Accumulate( &hcrc, (uint32_t *)mb_header[currentcmd].pData, (mb_header[currentcmd].framelength / 4) );
                }
                
                /* update datasize info */
                mb_cmdmngt[currentcmd] += mb_header[currentcmd].framelength;
                
                /********* Update progress bar *********/
                CurrentPercSize = ((mb_cmdmngt[currentcmd] * 100) / mb_header[currentcmd].fulllength);

                /* Complete Bar with CurrentPercSize */
                Menu_DrawProgressBarLine( Line5, CurrentPercSize );
                /***************************************/
                while (SendMBData( &mb_header[currentcmd], 255 ) != PASS);
                
                if( mb_cmdmngt[currentcmd] == mb_header[currentcmd].fulllength )
                {
                  Menu_DisplayCenterString( Line4, "Waiting for CRC..." );
                }
              }
              break;
              
            case MB_R2HPRESENTPASSWORD:
            
              break;
            
            default:

              break;
          }
        }
      }
    }
    
    /* Check if User request to send a command to reader */
    switch( requestcmd )
    {
      case MB_H2RDATATRANSFER:
      case MB_H2RIMAGEUPLOAD:
        
        currentcmd = requestcmd;
        if( mb_cmdmngt[currentcmd] == 0 )
        {
          __HAL_CRC_DR_RESET( &hcrc );
          InitMailBoxMode( );
          /* Prepare and Send command to reader */
          mb_header[currentcmd].fctcode = currentcmd;
          mb_header[currentcmd].cmdresp = MB_COMMAND;
          mb_header[currentcmd].error = MB_NOERROR;
          mb_header[currentcmd].chaining = MB_CHAINED;
          mb_header[currentcmd].fulllength += mb_header[currentcmd].fulllength % 4;
          mb_header[currentcmd].totalchunk = (uint16_t)(mb_header[currentcmd].fulllength / ALIGNED_FRAMESIZE);
          if( (mb_header[currentcmd].fulllength % ALIGNED_FRAMESIZE) != 0 )
          {
            mb_header[currentcmd].totalchunk++;
          }
          mb_header[currentcmd].chunknb = 1;
          mb_header[currentcmd].framelength = ALIGNED_FRAMESIZE;
          
          /* Prepare screen before download */
          if(mb_header[currentcmd].fulllength < 10000)
          {
            sprintf( displaystring, "File size: %lu B", mb_header[currentcmd].fulllength );
          } 
          else 
          {
            sprintf( displaystring, "File size: %lu kB", mb_header[currentcmd].fulllength/1000 );
          }
          Menu_DisplayCenterString( Line3, displaystring );
          Menu_DisplayClearLine( Line4 );
          Menu_DisplayClearLine( Line5 );
          CurrentPercSize = 0;
          Menu_DrawProgressBarLine( Line5, CurrentPercSize );
//          Menu_GetPictureDim( cHomeSmall, &iwidth, &iheight );
//          Menu_DisplayPicture( (dx - iwidth - 10), (dy - iheight - 30), cHomeSmall );
          
          
          /* Compute CRC with first frame */
          compute_crc = HAL_CRC_Accumulate( &hcrc, (uint32_t *)mb_header[currentcmd].pData, (mb_header[currentcmd].framelength / 4) );
          SendMBData( &mb_header[currentcmd], 255 );
          mb_cmdmngt[currentcmd] = mb_header[currentcmd].framelength;
        }
        break;
      
      default:
        break;
    }
    
    /* Check password timeout */
    if( (allowfwupg == true) && (mb_cmdmngt[MB_R2HFIRMWAREUPDATE] == 0) )
    {
      if( (HAL_GetTick( ) - fwpasswordtimeout) > MB_FWUPGDPWDTIMEOUT )
      {
        fwpasswordtimeout = 0;
        allowfwupg = false;
        Clear_FTM_Screen( );
      }
    }

      BSP_TS_GetState( &tstap );
      if( tstap.TouchDetected != 0 )
      {
        /* First picture touched */
        Menu_GetPictureDim( cPictures, &iwidth, &iheight );
        if( (tstap.X < (10 + iwidth)) && (tstap.Y > (dy - iheight - 30)) )
        {
          jpegbuffersize = Menu_SelectImage( picture, 5, picture_sizes );
          
          if( jpegbuffersize != 0xFF )
          {

            mb_header[MB_H2RIMAGEUPLOAD].pData = (uint8_t *)picture[jpegbuffersize];

            jpegbuffersize = jpeg_GetBufferSize( mb_header[MB_H2RIMAGEUPLOAD].pData );
            
            if( (jpegbuffersize % 4) != 0 )
            {
              mb_header[MB_H2RIMAGEUPLOAD].fulllength = jpegbuffersize + (4 - (jpegbuffersize % 4));
            }
            else
            {
              mb_header[MB_H2RIMAGEUPLOAD].fulllength = jpegbuffersize;
            }

            requestcmd = MB_H2RIMAGEUPLOAD;
            allowfwupg = false;
            Menu_MsgStatus( "Fast Transfer Demo", "", MSG_INFO);
            Menu_DisplayCenterString( Line10, "<--" );
            Menu_SetStyle( PLAIN );
            Menu_DisplayCenterString( Line2 , "Img upload started" );
          }
          else
          {
            Clear_FTM_Screen( );
          }
        }
        
        /* Second picture touched */
        if( tstap.X > (10 + iwidth + 20))
        {
          Menu_GetPictureDim( cDataTransfer, &iwidth, &iheight );
          if( (tstap.X < (2 * (10 + iwidth))) && (tstap.Y > (dy - iheight - 30)) )
          {
            mb_header[MB_H2RDATATRANSFER].fulllength = Get_Custom_Transfer_Length() * 1000;
            mb_header[MB_H2RDATATRANSFER].pData = (uint8_t *)picture;
            if(mb_header[MB_H2RDATATRANSFER].fulllength != 0)
            {
              requestcmd = MB_H2RDATATRANSFER;
              allowfwupg = false;
              Menu_MsgStatus( "Fast Transfer Demo", "", MSG_INFO);
              Menu_SetStyle( HEADER );
              Menu_DisplayCenterString( Line10 , "<--" );
              Menu_SetStyle( PLAIN );
              Menu_DisplayCenterString( Line2 , "Data transf. started" );
            }
            else
            {
              Clear_FTM_Screen( );
            }
          }
        }
      
        /* Back touched */
        if( currentcmd > 0)
        {
          if( tstap.Y > (Menu_GetDisplayHeight() - Menu_GetFontHeight()) )
          {
            ExitFunction = true;
          }
        }
        else
        {
          Menu_GetPictureDim( cHomeSmall, &iwidth, &iheight );
          if( (tstap.X > (dx - iwidth - 10)) && (tstap.Y > (dy - iheight - 30)) )
          {
            ExitFunction = true;
          }
        }
      }
  }
    /* De-activate any protection */
    NDEF_DEMO_Init_Tag();
 
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Clears display and shows FTM interface.
  * @param  None No parameters.
  * @return None.
  */
static void Clear_FTM_Screen( void )
{
  uint32_t dx = 0;
  uint32_t dy = 0;
  uint32_t iwidth = 0;
  uint32_t iheight = 0;
  
  dx = Menu_GetDisplayWidth( );
  dy = Menu_GetDisplayHeight( );

  /* Clear Screen and display FTM screen */
  Menu_MsgStatus( "Fast Transfer Demo", "Ready to start\ndemo!!", MSG_INFO);
  
  Menu_GetPictureDim( cHomeSmall, &iwidth, &iheight );
  Menu_DisplayPicture( (dx - iwidth - 10), (dy - iheight - 30), cHomeSmall );
  Menu_DisplayPicture( 10, (dy - iheight - 30), cPictures );
  Menu_GetPictureDim( cPictures, &iwidth, &iheight );
  Menu_DisplayPicture( (10 + iwidth + 10), (dy - iheight - 30), cDataTransfer );
}

/**
  * @brief  Callbaclk function definition for keyboard menu.
  * @param  input caracter touched on screen.
  * @retval 0 Keep the keyboard open.
  * @retval 1 Close keyboard.
  */
static int KeyboardCallback( char input )
{
  char msg[20];

  if( (input >= '0') && (input <= '9') && (number < 100) )
  {
    number = number * 10 + input - '0';
  } 
  else if( input == '\b' )
  {
    if( number > 0 )
    {
      number = number / 10;
    } 
    else 
    {
      /* click on back when there is no number, abandon */
      number = 0;
      /* 1: close the keyboard */
      return 1;
    }
  }
  else if( input == 0x1B )
  {
    /* click on back */
    number = 0;
    /* 1: close the keyboard */
    return 1;
  }
  else if( input == '\r' )
  {
    /* 1: close the keyboard */
    return 1;
  }
    
  sprintf( msg,"Size = %03lu kB",number );
  Menu_SetStyle( CLEAR_PLAIN );
  Menu_DisplayCenterString( Line0, msg );

  /* 0: keep the keyboard open */
  return 0;
}

/**
  * @brief  Clears display and shows FTM interface.
  * @param  None No parameters.
  * @return None.
  */
static uint32_t Get_Custom_Transfer_Length(void)
{
  char keyboard[] = { '1','2','3',
                      '4','5','6',
                      '7','8','9',
                      '\b','0','\r'};
  
  char *spec_char[] = { (char *)Font22_BackspaceChar, (char *)Font22_ReturnChar };
                      
  Menu_MsgStatus( "Enter the size in kB", "", MSG_INFO);
  Menu_DisplayCenterString(Line10, "<--");
  Menu_SetStyle(PLAIN); 
  number = 0;
  Menu_DisplayKeyboard(keyboard, spec_char, 4, 3, 3, 2, &KeyboardCallback);
                      
  return number;
}

/** @brief  This function receives Time frames from the reader, and displays it.
  *         It also compute the latency between 2 frames.
  * @param  buff            Pointer on MailBox buffer.
  * @param  prev_stopwatch  Pointer used to return the stopwatch structure with received time.
  * @param  first_frame     Indicate that this is the first frame and that latency must not be computed.
*/
static void FTM_StopWatch(uint8_t *buff, MB_STOPWATCH_T *prev_stopwatch, uint8_t first_frame)
{
  /* Used to compute average latency */
  static uint32_t sum_latency = 0;
  /* Delay to update the latency */
  static uint8_t latency_cnt = 0;
  /* Store the maximum computed latency */
  static uint32_t max_latency = 0;
  /* Record time of previous frame, to compute latency */
  static uint32_t frame_tick;
  /* Current latency: time between 2 received frames */
  uint32_t latency = 0;
  
  /* Decode the time frame, duration ~3ms */
  MB_STOPWATCH_T *stopwatch = (MB_STOPWATCH_T*)&buff[MB_DATA];
  BSP_LCD_SetFont(&Font36);    
  /* Only modify required portion of the display - to avoid useless latency */
  if(prev_stopwatch->minute != stopwatch->minute)
  {
    /* Assume here that minutes are always lower than 100 */
    BSP_LCD_DisplayChar(Line2, 2, stopwatch->minute/10 + '0');
    BSP_LCD_DisplayChar(Line2, 3, stopwatch->minute%10 + '0');
  }
  if(prev_stopwatch->second != stopwatch->second)
  {
    BSP_LCD_DisplayChar(Line2, 5, stopwatch->second/10 + '0');
    BSP_LCD_DisplayChar(Line2, 6, stopwatch->second%10 + '0');  
  }
  if(prev_stopwatch->hundredth != stopwatch->hundredth)
  {
    BSP_LCD_DisplayChar(Line2, 8, stopwatch->hundredth/10 + '0');
    BSP_LCD_DisplayChar(Line2, 9, stopwatch->hundredth%10 + '0');  
  }
  BSP_LCD_SetFont(&Font22);
  /* Compute max & average latency (i.e. time between 2 frames), duration ~7ms */
  uint32_t current_tick = HAL_GetTick();
  latency = current_tick - frame_tick;
  frame_tick = current_tick;
  if(!first_frame)
  {
    sum_latency += latency;

    /* Compute max latency */
    if((latency > max_latency))
    {
      max_latency = latency;
    }
      
    /* Refresh latency display every 20 frames */
    if(latency_cnt++ >= 20)
    {
      sum_latency /=21;
      /* only display relevant values */
      if(sum_latency < 100)
      {
        BSP_LCD_DisplayChar(Line6, 15, sum_latency/10 + '0');
        BSP_LCD_DisplayChar(Line6, 16, sum_latency%10 + '0');  
      } else {
        BSP_LCD_DisplayChar(Line6, 15, '#');
        BSP_LCD_DisplayChar(Line6, 16, '#');            
      }
      if(max_latency < 100)
      {
        BSP_LCD_DisplayChar(Line7, 15, max_latency/10 + '0');
        BSP_LCD_DisplayChar(Line7, 16, max_latency%10 + '0');  
      } else {
        BSP_LCD_DisplayChar(Line7, 15, '#');
        BSP_LCD_DisplayChar(Line7, 16, '#');            
      }
      latency_cnt = 0;
      sum_latency = 0;
      max_latency = 0;
    }
  }
  /* Save current stopwatch value to compare with next iteration */
  memcpy(prev_stopwatch,stopwatch,sizeof(MB_STOPWATCH_T));
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
