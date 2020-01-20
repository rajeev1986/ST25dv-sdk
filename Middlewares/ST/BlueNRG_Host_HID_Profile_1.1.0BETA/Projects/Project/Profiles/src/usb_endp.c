/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"
#include "usb_prop.h"

/* Extern variables ----------------------------------------------------------*/
extern LINE_CODING linecoding;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Interval between sending IN packets in frame number (1 frame = 1ms) */
#define VCOMPORT_IN_FRAME_INTERVAL             5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t USB_Rx_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
extern  uint8_t Print_Buffer[];
extern uint32_t Print_ptr_out;
extern uint32_t Print_length;
extern uint8_t  USB_Tx_State;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback (void)
{
  uint16_t USB_Tx_ptr;
  uint16_t USB_Tx_length;
  
  if (USB_Tx_State == 1)
  {
    if (Print_length == 0) 
    {
      USB_Tx_State = 0;
      USB_Tx_length = 0;
    }
    else 
    {
      if (Print_length > VIRTUAL_COM_PORT_DATA_SIZE){
        USB_Tx_ptr = Print_ptr_out;
        USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;
        
        Print_ptr_out += VIRTUAL_COM_PORT_DATA_SIZE;
        Print_length -= VIRTUAL_COM_PORT_DATA_SIZE;    
      }
      else 
      {
        USB_Tx_ptr = Print_ptr_out;
        USB_Tx_length = Print_length;
        
        Print_ptr_out += Print_length;
        Print_length = 0;
      }
      
#ifdef USE_STM3210C_EVAL
      USB_SIL_Write(EP1_IN, &Print_Buffer[USB_Tx_ptr], USB_Tx_length);  
#else
      UserToPMABufferCopy(&Print_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
#endif      
    }
    SetEPTxCount(ENDP1, USB_Tx_length);
    SetEPTxValid(ENDP1);
  }
}

/*******************************************************************************
* Function Name  : EP3_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP3_OUT_Callback(void)
{
  uint16_t USB_Rx_Cnt;
  
  /* Get the received data buffer and update the counter */
  USB_Rx_Cnt = USB_SIL_Read(EP3_OUT, USB_Rx_Buffer);
  
#if 0 // original code */
  for(int i = 0; i < USB_Rx_Cnt; i++){
      Read_Buffer_Push(USB_Rx_Buffer[i]);
  }
#else
  SdkEval_IO_Receive_Data(USB_Rx_Buffer, USB_Rx_Cnt);
#endif 
      
  
#ifndef STM32F10X_CL
  /* Enable the receive of data on EP3 */
  SetEPRxValid(ENDP3);
#endif /* STM32F10X_CL */
}


/*******************************************************************************
* Function Name  : SOF_Callback / INTR_SOFINTR_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
#ifdef STM32F10X_CL
void INTR_SOFINTR_Callback(void)
#else
void SOF_Callback(void)
#endif /* STM32F10X_CL */
{
  static uint32_t FrameCount = 0;
  
  if(bDeviceState == CONFIGURED)
  {
    if (FrameCount++ == VCOMPORT_IN_FRAME_INTERVAL)
    {
      /* Reset the frame counter */
      FrameCount = 0;
      
      /* Check the data to be sent through IN pipe */
      Handle_USBAsynchXfer();
    }
  }  
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

