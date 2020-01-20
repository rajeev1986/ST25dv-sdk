/**
 * @file    SDK_EVAL_Io.c
 * @author  AMS - AAS Division
 * @version V1.0.0
 * @date    April,2 2014
 * @brief   This file provides all the low level API to manage SDK I/O ports.
 * @details
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
 */


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Config.h"
#ifdef ENABLE_USB
#include "hw_config.h"
#endif 

/** @addtogroup SDK_EVAL_STM32L
 * @{
 */


/** @addtogroup SDK_EVAL_Io
 * @{
 */


/** @defgroup SDK_EVAL_Io_Private_TypesDefinitions             SDK EVAL IO Private Types Definitions
 * @{
 */

/**
 * @brief  IO_Receive_Data User callback 
 */
typedef struct _IO_ReceiveCallBack
{
  /**
   * callback to notify the applicatio with the received I/O data
   */ 
  IO_RECEIVE_DATA_USER_CALLBACK_TYPE IO_user_application;
} IO_ReceiveCallBack;
/**
 * @}
 */


/** @defgroup SDK_EVAL_Io_Private_Defines                      SDK EVAL IO Private Defines
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Io_Private_Macros                       SDK EVAL IO Private Macros
 * @{
 */

/* IO User Callback used to handle received data from I/O channel*/
static IO_ReceiveCallBack IO_Receive_Data;

/**
 * @}
 */

/** @defgroup SDK_EVAL_Io_Private_Variables                    SDK EVAL IO Private Variables
 * @{
 */

#ifdef ENABLE_USART
USART_TypeDef* vectpxComUsart[COMn] = {EVAL_COM1};

GPIO_TypeDef* vectpxComTxPort[COMn] = {EVAL_COM1_TX_GPIO_PORT};

GPIO_TypeDef* vectpxComRxPort[COMn] = {EVAL_COM1_RX_GPIO_PORT};

static const uint32_t s_vectlComUsartClk[COMn] = {EVAL_COM1_CLK};

static const uint32_t s_vectlComTxPortClk[COMn] = {EVAL_COM1_TX_GPIO_CLK};

static const uint32_t s_vectlComRxPortClk[COMn] = {EVAL_COM1_RX_GPIO_CLK};

static const uint16_t s_vectnComTxPin[COMn] = {EVAL_COM1_TX_PIN};

static const uint16_t s_vectnComRxPin[COMn] = {EVAL_COM1_RX_PIN};

static const uint16_t s_vectnComTxPinSource[COMn] = {EVAL_COM1_TX_SOURCE};

static const uint16_t s_vectnComRxPinSource[COMn] = {EVAL_COM1_RX_SOURCE};

static const uint16_t s_vectnComTxAF[COMn] = {EVAL_COM1_TX_AF};

static const uint16_t s_vectnComRxAF[COMn] = {EVAL_COM1_RX_AF};

static const IRQn_Type s_vectnComRxIrqIrqn[COMn] = {EVAL_COM1_IRQn}; 

static USART_TypeDef* s_UsartPort;


/**
 * @}
 */


/** @defgroup SDK_EVAL_Io_Private_FunctionPrototypes                   SDK EVAL IO Private Function Prototypes
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_IO_Private_Functions                            SDK EVAL IO Private Functions
 * @{
 */

/**
 * @brief  Configures Uart port.
 * @param  xCom Specifies the COM port to be configured.
 * @retval None.
 */
static void SdkEvalUart_Init(SdkEvalCom xCom)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  s_UsartPort = vectpxComUsart[xCom];

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(s_vectlComTxPortClk[xCom] | s_vectlComRxPortClk[xCom], ENABLE);

  /* Enable UART clock */
  //RCC_APB1PeriphClockCmd(s_vectlComUsartClk[xCom], ENABLE);

/* UART Periph clock enable */
#if EVAL_COM1_USART2
  RCC_APB1PeriphClockCmd(s_vectlComUsartClk[xCom], ENABLE);
#elif EVAL_COM1_USART1
  RCC_APB2PeriphClockCmd(s_vectlComUsartClk[xCom], ENABLE);
#else
#warning "ERROR on USART definition"
#endif
  
  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(vectpxComTxPort[xCom], s_vectnComTxPinSource[xCom], s_vectnComTxAF[xCom]);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(vectpxComRxPort[xCom], s_vectnComRxPinSource[xCom], s_vectnComRxAF[xCom]);

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = s_vectnComTxPin[xCom];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(vectpxComTxPort[xCom], &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = s_vectnComRxPin[xCom];
  GPIO_Init(vectpxComRxPort[xCom], &GPIO_InitStructure);


  /* USARTx configuration ----------------------------------------------------*/
  /* USARTx configured as follow:
  - BaudRate = 9600 baud  
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate =  EVAL_COM1_BAUD_RATE; 
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(s_UsartPort, &USART_InitStructure);
  
  /* NVIC configuration */
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = s_vectnComRxIrqIrqn[xCom];
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; /* Move to 1 for coexistence with BlueNRG SPI irq */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USARTx Receive interrupt: this interrupt is generated when the
  USARTx receive data register is not empty */
  USART_ITConfig(s_UsartPort, USART_IT_RXNE, ENABLE);

  /* Enable USART */
  USART_Cmd(s_UsartPort, ENABLE);
}/* end SdkEvalUart_Init() */


/**
 * @brief  Wait for serial data tx end
 * @param  None.
 * @retval None.
 */
static void SdkEvalUart_Wait_Send_Data()  
{	
  while(!(USART_GetFlagStatus(s_UsartPort, USART_FLAG_TXE)));
}/* end SdkEvalUart_Wait_Send_Data() */

/**
 * @brief  Send a byte on Serial port
 * @param  tx_data byte to be sent
 * @retval None.
 */
static void SdkEvalUart_Send_Data(uint8_t tx_data)
{
  /* wait for the transmission end */
  SdkEvalUart_Wait_Send_Data();
  /* send the data */
  USART_SendData(s_UsartPort, tx_data); 
}/* end SdkEvalUart_Send_Data() */


/**
 * @brief  Serial IRQ handler body to be put on user USARTx_IRQHandler() (stm32l1xx_it.c)
 * @param  None
 * @retval None.
 */
void SdkEval_IO_Uart_Irq_Handler(void)
{
  uint8_t read_data; 
  if(USART_GetITStatus(s_UsartPort, USART_IT_RXNE) != RESET)
  {
     /* Read byte from the receive FIFO */
     read_data = USART_ReceiveData(s_UsartPort);
     
     /* IO Receive Data: it handles the received data within the  User Application Function 
        defined as parameter of SdkEval_IO_Config() */
     SdkEval_IO_Receive_Data(&read_data,1);
  
     /* Clear the USART1 Receive interrupt */
     USART_ClearITPendingBit(s_UsartPort, USART_IT_RXNE);
   }  
}/* SdkEval_IO_Uart_Irq_Handler() */

#undef putchar
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
int putchar(int ch)
{
  SdkEval_IO_Send_Data((uint8_t) ch);
  return ch;
}

void __io_putchar(char c)
{
  putchar(c);
}

#endif /* #ifdef ENABLE_USART */

/**
 * @brief  Configure the selected I/O communciation channel
 * @param  None.
 * @retval None.
 */
void SdkEval_IO_Config(IO_RECEIVE_DATA_USER_CALLBACK_TYPE io_receive_function)
{
#if defined (ENABLE_USB)
  /* Init USB: refer to file hw_config.c for USB APIs */
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();
  IO_Receive_Data.IO_user_application = io_receive_function;
#elif defined (ENABLE_USART)
  /* Init Serial Uart */
  SdkEvalUart_Init(COM1);
  IO_Receive_Data.IO_user_application = io_receive_function;
#endif 

  
}/* end SdkEval_IO_Config() */

/**
 * @brief  Send a byte on selected I/O port
 * @param  tx_data byte to be sent
 * @retval None.
 */
void SdkEval_IO_Send_Data(uint8_t tx_data)
{
#if defined (ENABLE_USB)
  /* Call USB send data API on hw_config.c */
  USB_Send_Data(tx_data);
#elif defined (ENABLE_USART)
  SdkEvalUart_Send_Data(tx_data);
#endif
}/* end SdkEval_IO_Send_Data() */

/**
 * @brief  Receive data on selected I/O port
 * @param  rx_data pointer to buffer of received
 * @param  data_size number of received data 
 * @retval None.
 */
void SdkEval_IO_Receive_Data(uint8_t * rx_data, uint16_t data_size)
{
  IO_Receive_Data.IO_user_application(rx_data,data_size);
  
}/* end SdkEval_IO_Receive_Data() */
/**
 * @}
 */


/**
 * @}
 */


/**
 * @}
 */


/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
