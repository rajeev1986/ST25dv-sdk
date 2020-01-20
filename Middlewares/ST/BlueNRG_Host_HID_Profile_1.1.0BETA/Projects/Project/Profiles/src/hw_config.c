/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#ifdef STM32L1XX_MD
#include "stm32l1xx_it.h"
#else
#include "stm32f10x_it.h"
#endif /* STM32L1XX_MD */


#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "platform_config.h"
#include "usb_pwr.h"
//#include "htl.h" //TBR
#include "hal.h"
#include "stm32l1xx_systick.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#ifdef __GNUC__
# define _LLIO_STDIN ((int) stdin)
# define _LLIO_STDOUT ((int) stdout)
# define _LLIO_STDERR ((int) stderr)
# define _LLIO_ERROR  (-1)
#else
# ifdef __ICCARM__
# include <yfuns.h>
# endif
#endif


/* Private macro -------------------------------------------------------------*/

#define USB_OUT_BUFFER_IS_FULL() (Print_ptr_in == Print_ptr_out - 1 || (Print_ptr_in == PRINT_DATA_SIZE && Print_ptr_out == 0))
#define USB_OUT_BUFFER_IS_EMPTY() (Print_ptr_in == Print_ptr_out)

#define READ_BUFFER_IS_FULL() (Read_ptr_in == Read_ptr_out - 1 || (Read_ptr_in == READ_DATA_SIZE && Read_ptr_out == 0))
#define READ_BUFFER_IS_EMPTY() (Read_ptr_in == Read_ptr_out)

/* Private variables ---------------------------------------------------------*/
uint8_t  Print_Buffer [PRINT_DATA_SIZE]; 
uint32_t Print_ptr_in = 0;
uint32_t Print_ptr_out = 0;
uint32_t Print_length  = 0;

uint8_t  Read_Buffer [READ_DATA_SIZE]; 
uint32_t Read_ptr_in = 0;
uint32_t Read_ptr_out = 0;
uint32_t Read_length  = 0;

uint8_t  USB_Tx_State = 0;

static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
int spi_input(uint8_t byte);

#define TEST_DEBUG_MSG 0
#if TEST_DEBUG_MSG
#include "debug.h"
extern void * profiledbgfile;
#endif 
/* Extern variables ----------------------------------------------------------*/

extern LINE_CODING linecoding;

/**
  * @brief  Configures peripherals' clock.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  
  /* Enable the GPIOs Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC| RCC_AHBPeriph_GPIOD| RCC_AHBPeriph_GPIOE| RCC_AHBPeriph_GPIOH, ENABLE);     
    
  /* Enable SYSCFG */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);
  
  /* Set low power configuration */
  RCC->AHBLPENR = RCC_AHBENR_GPIOAEN|RCC_AHBENR_GPIOBEN|RCC_AHBENR_GPIOCEN|RCC_AHBENR_GPIODEN;   
  RCC->APB1LPENR = RCC_APB1ENR_PWREN|RCC_APB1ENR_TIM2EN;
  RCC->APB2LPENR = 0;  
}


/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{       
    
#if (!defined(STM32F10X_CL) && !defined(STM32L1XX_MD)) || defined(USB_USE_EXTERNAL_PULLUP)
    GPIO_InitTypeDef GPIO_InitStructure;
#endif /* STM32F10X_CL && STM32L1XX_MD */  
    
    /*!< At this stage the microcontroller clock setting is already configured, 
    this is done through SystemInit() function which is called from startup
    file (startup_stm32f10x_xx.s) before to branch to application main.
    To reconfigure the default setting of SystemInit() function, refer to
    system_stm32f10x.c file
    */   
#ifdef STM32L1XX_MD
    /* Enable the SYSCFG module clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
#endif /* STM32L1XX_MD */ 
    
    
#if !defined(STM32F10X_CL) && !defined(STM32L1XX_MD) 
    /* Enable USB_DISCONNECT GPIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);
    
    /* Configure USB pull-up pin */
    GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
#endif /* STM32F10X_CL && STM32L1XX_MD */
    
#if defined(USB_USE_EXTERNAL_PULLUP)
    /* Enable the USB disconnect GPIO clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_DISCONNECT, ENABLE);
    
    /* USB_DISCONNECT used as USB pull-up */
    GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
#endif /* USB_USE_EXTERNAL_PULLUP */
    
}

/**
  * @brief  To initialize the I/O ports
  * @caller main
  * @param None
  * @retval None
  */
void  Init_GPIOs(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIOD->MODER   = 0xFFFFFFFF;
  GPIOE->MODER   = 0xFFFFFFFF;
  GPIOH->MODER   = 0xFFFFFFFF;

#ifndef ENABLE_USB  
   /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) except debug pins. */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8 \
                                 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |  GPIO_Pin_12;
#else
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8 \
                                 | GPIO_Pin_9 | GPIO_Pin_10;
#endif
  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8 \
                                 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8 \
                                 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
   
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8 \
                                 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;   
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
#if defined(STM32L1XX_MD)
    /* Enable USB clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
    
#elif defined(STM32F10X_CL)
    /* Select USBCLK source */
    RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);
    
    /* Enable the USB clock */ 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;
    
#else 
    /* Select USBCLK source */
    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
    
    /* Enable the USB clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
#endif /* STM32F10X_CL */
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
    /* Set the device state to suspend */
    bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
    DEVICE_INFO *pInfo = &Device_Info;
    
    /* Set the device state to the correct state */
    if (pInfo->Current_Configuration != 0)
    {
        /* Device configured */
        bDeviceState = CONFIGURED;
    }
    else
    {
        bDeviceState = ATTACHED;
    }
}

/*******************************************************************************
* Function Name  : GPIO_SetHiZ
* Description    : Set GPIO pin in Hi-Z state.
* Input          : None.
* Return         : None.
*******************************************************************************/
void GPIO_SetHiZ(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    /* Configure USART Tx as output open drain HI-Z state */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET);
    
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
#ifdef STM32L1XX_MD
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
#elif defined(STM32F10X_CL) 
    /* Enable the USB Interrupts */
    NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
#else
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif /* STM32L1XX_MD */
    
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
#ifdef STM32L1XX_MD
    if (NewState != DISABLE)
    {
        STM32L15_USB_CONNECT;
    }
    else
    {
        STM32L15_USB_DISCONNECT;
    }  
    
#elif defined(USE_STM3210C_EVAL)  
    if (NewState != DISABLE)
    {
        USB_DevConnect();
    }
    else
    {
        USB_DevDisconnect();
    }
    
#else /* USE_STM3210B_EVAL or USE_STM3210E_EVAL */
    if (NewState != DISABLE)
    {
        GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
    }
    else
    {
        GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
    }
#endif /* USE_STM3210C_EVAL */
}


/*******************************************************************************
* Function Name  : Handle_USBAsynchXfer.
* Description    : send data to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void Handle_USBAsynchXfer (void)
{
    
    uint16_t USB_Tx_ptr;
    uint16_t USB_Tx_length;
    
    if(USB_Tx_State != 1)
    {
        if (Print_ptr_out == PRINT_DATA_SIZE)
        {
            Print_ptr_out = 0;
        }
        
        if(Print_ptr_out == Print_ptr_in) 
        {
            USB_Tx_State = 0; 
            return;
        }
        
        if(Print_ptr_out > Print_ptr_in) /* rollback */
        { 
            Print_length = PRINT_DATA_SIZE - Print_ptr_out;
        }
        else 
        {
            Print_length = Print_ptr_in - Print_ptr_out;
        }
        
        if (Print_length > VIRTUAL_COM_PORT_DATA_SIZE)
        {
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
        USB_Tx_State = 1; 
        
#ifdef USE_STM3210C_EVAL
        USB_SIL_Write(EP1_IN, &Print_Buffer[USB_Tx_ptr], USB_Tx_length);  
#else
        UserToPMABufferCopy(&Print_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
        SetEPTxCount(ENDP1, USB_Tx_length);
        SetEPTxValid(ENDP1); 
#endif /* USE_STM3210C_EVAL */
    }  
    
}

void Read_Buffer_Push(uint8_t byte)
{
    Read_Buffer[Read_ptr_in] = byte;
    
    Read_ptr_in++;
    
    /* To avoid buffer overflow */
    if(Read_ptr_in == READ_DATA_SIZE)
    {
        Read_ptr_in = 0;
    }        
}

int Read_Buffer_Pop(uint8_t *byte)
{
    if(READ_BUFFER_IS_EMPTY())
        return 0;
    
    *byte = Read_Buffer[Read_ptr_out];
    
    Read_ptr_out++;
    
    if(Read_ptr_out == READ_DATA_SIZE)
        Read_ptr_out = 0;  
    
    return 1;
}

/*******************************************************************************
* Function Name  : USB_Send_Data.
* Description    : Insert a byte to USB buffer for transmission.
* Input          : None.
* Return         : none.
*******************************************************************************/
void USB_Send_Data(uint8_t byte)
{
    
    Print_Buffer[Print_ptr_in] = byte;
    
    Print_ptr_in++;
    
    /* To avoid buffer overflow */
    if(Print_ptr_in == PRINT_DATA_SIZE)
    {
        Print_ptr_in = 0;
    }
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
    uint32_t Device_Serial0, Device_Serial1, Device_Serial2;
    
#ifdef STM32L1XX_MD
    Device_Serial0 = *(uint32_t*)(0x1FF80050);
    Device_Serial1 = *(uint32_t*)(0x1FF80054);
    Device_Serial2 = *(uint32_t*)(0x1FF80064);
#else  
    Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
    Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
    Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);
#endif /* STM32L1XX_MD */  
    
    Device_Serial0 += Device_Serial2;
    
    if (Device_Serial0 != 0)
    {
        IntToUnicode (Device_Serial0, &Virtual_Com_Port_StringSerial[2] , 8);
        IntToUnicode (Device_Serial1, &Virtual_Com_Port_StringSerial[18], 4);
    }
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
    uint8_t idx = 0;
    
    for( idx = 0 ; idx < len ; idx ++)
    {
        if( ((value >> 28)) < 0xA )
        {
            pbuf[ 2* idx] = (value >> 28) + '0';
        }
        else
        {
            pbuf[2* idx] = (value >> 28) + 'A' - 10; 
        }
        
        value = value << 4;
        
        pbuf[ 2* idx + 1] = 0;
    }
}

#ifdef ENABLE_USB_PRINTF
#undef putchar

int putchar(int c)
{
  int i = 0xFFFFFF;/* Buffer full: wait till previous data is transmitted */
  while(USB_OUT_BUFFER_IS_FULL() && i-- > 0);
  if(i==0){
      const char *str = "\nOVERFLOW\n";
      while(*str != '\0')
          USB_Send_Data(*str++);
  }
  
  USB_Send_Data(c);
  return c;
}

void __io_putchar(char c)
{
  putchar(c);
}

size_t _write(int handle, const unsigned char * buffer, size_t size)
{
  size_t nChars = 0;

  if (handle != _LLIO_STDOUT && handle != _LLIO_STDERR) {
    return _LLIO_ERROR;
  }

  if (buffer == 0) { 
    // This means that we should flush internal buffers.
    //spin until TX complete (TX is idle)
    while (!USB_OUT_BUFFER_IS_EMPTY()) {}
    return 0;
  }

  while(size--) {
    __io_putchar(*buffer++);
    ++nChars;
  }

  return nChars;
}


/* NEW: no blocking character read */
//BOOL __io_getcharNonBlocking(uint8 *data)
uint8_t __io_getcharNonBlocking(uint8_t *data)
{
  if (__read(_LLIO_STDIN,data,1))
    return TRUE;
  else
    return FALSE;
}/* end serialReadByte() */



#if 0 /* TBR: original _read() function: scanf blocking issue:*/
size_t _read(int handle, unsigned char * buffer, size_t size)
{    
    size_t bytes = 0;

    while(bytes == 0){    
      while(bytes < size && Read_Buffer_Pop(buffer+bytes)){
          bytes++;
      }
    }    
    return bytes;
}
#else /* NEW: remove while(bytes == 0) */
size_t _read(int handle, unsigned char * buffer, size_t size)
{    
    size_t bytes = 0;
  
      while(bytes < size && Read_Buffer_Pop(buffer+bytes)){
          bytes++;
      }
    return bytes;
}

#endif /* original _read() function */

size_t __read(int handle, unsigned char * buffer, size_t size){
    return _read(handle, buffer, size);
}


#endif //ENABLE_USB_PRINTF

int USB_free_buffer_size()
{
  int free_space;
  
  if(Print_ptr_in == Print_ptr_out){
    free_space = PRINT_DATA_SIZE;
  }
  else if(Print_ptr_out > Print_ptr_in) /* rollback */
  { 
    free_space = Print_ptr_out - Print_ptr_in;
  }
  else 
  {
    free_space = PRINT_DATA_SIZE - Print_ptr_in + Print_ptr_out;
  }
  
  return free_space;
}

/*  User Function where serial received data should be processed */
void processInputData(uint8_t * rx_data, uint16_t data_size)
{
  for(int i = 0; i < data_size; i++){
      Read_Buffer_Push(rx_data[i]);
  }
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

