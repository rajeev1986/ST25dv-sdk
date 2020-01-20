/**
  ******************************************************************************
  * @file    ndef_demo.c
  * @ingroup NDEF_DEMO
  * @author  MMY Application Team
  * @version $Revision:$
  * @date    $Date:$
  * @brief   This file provides functions to execute NDEF demos.
  * @details The demos covers the following usecases:
  *          - URI NDEF records: URL & Phone
  *          - SMS NDEF record
  *          - Email NDEF record
  *          - vCard NDEF record (with or without an embedded picture)
  *          - Geolocation NDEF record
  *          - MyApp custom NDEF record
  *          - Multi NDEF record with AAR
  *          - Bluetooth Low Energy OOB NDEF record
  *          - Wifi OOB NDEF record
  * 
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
#include "ndef_demo.h"
#include "string.h"
#include "stdlib.h"

/* Display management */
#include "st25dx_discovery_lcd.h"
#include "Menu_core.h"
#include "Menu_config.h"

/* Tag & board management */
#include "commonfunc.h"
#include "mailboxfunc.h"
#include "st25dx_discovery.h"
#include "tagtype5_wrapper.h"
#include "stm32f4xx_it.h"

/* Big vCard data */
#include "VcardCSL1.h"

/* Bluetooth BLE includes */
#include "st25dx_bluenrg_ble.h"
#include "bluenrg_aci.h"


/* Wifi includes */
#include "ring_buffer.h"
#include "wifi_module.h"




/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/** Type5 Capabillity Container byte3 value for android compliancy */
#define NDEF_DEMO_CC3_COMPLIANT_VALUE     ((uint8_t)0x5)
/** Dummy phone number used in the demo */
#define NDEF_DEMO_PHONE_NUMBER            "+33612345678"
/** ST URL to ST25 products page */
#define NDEF_DEMO_URL                     "st.com/st25-demo"

/** Bluetooth expected HW module version */
#define NDEF_DEMO_BLE_MODULE_VERSION      ((uint8_t)0x31)
/** Bluetooth pin code (not used in the demo) */
#define NDEF_DEMO_BLE_PIN                 ((uint32_t)123456)
/** Bluetooth module name */
#define NDEF_DEMO_BLE_NAME                "HID"

/** Bluetooth Device Address big endian (default value, partly overriden to uniquify the DeviceAddress) */
#define NDEF_DEMO_BLE_ADDR_BIG_ENDIAN     {0x02, 0x80, 0xE1, 0x00, 0x34, 0x12}

/** Wifi module SSID */
#define NDEF_DEMO_SET_WIFI_SSID(str)   do{  char uid[3];                                                     \
                                            memcpy(uid,(void*)0x1FFF7A10,3);                                 \
                                            sprintf(str, "%s_%02X%02X%02X","ST25_WIFI",uid[0],uid[1],uid[2]); \
                                          } while (0);

/** Wifi module number of channels */
#define NDEF_DEMO_WIFI_CHANNEL_NUM        ((uint8_t)6)
/** Wifi module baudrate */
#define NDEF_DEMO_WIFI_UART_BAUDRATE      ((uint32_t)115200)



/* Private variables ---------------------------------------------------------*/
/** Control LED blinking duration for the MyApp demo */
static uint32_t MyAppBlinkCounter;
/** Main counter for the MyApp demo */
static uint32_t MyAppCounter;

/** AAR definition for ST Android app */
static const sAARInfo w_aar = {.PackageName = "com.st.st25nfc"};

/** Used by the MemWrite callback to define the total size of a transfer  */
static uint32_t TxSize = 0;
/** Used by the MemWrite callback to track the number of byte already transfered */
static uint32_t TxedSize = 0;


/** Wifi conexion state */
volatile uint8_t WifiConnected = 0;
/** Wifi client mac address */
volatile char WifiClientMac[20];

/** @brief Wifi timer handle. */
extern TIM_HandleTypeDef    TimHandle;

/* Private function prototypes -----------------------------------------------*/
static uint16_t getNDEFRecord(sRecordInfo_t *p_record);
static uint16_t getURI(sURI_Info * r_uri);
static uint16_t getSMS(sSMSInfo * r_sms);
static uint16_t getEmail(sEmailInfo* r_email);
static uint16_t getVcard(sVcardInfo* r_vcard);
static uint16_t getGeo(sGeoInfo *r_geo);
static uint16_t getMyApp(sMyAppInfo *r_myapp);
static void MyAppDemoInit(sMyAppInfo *p_myapp);
static void MyAppDemoRun(sMyAppInfo *p_myapp);
static int BLE_Init(const char * DevAddr);
static int WIFIInit(void);


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  This function reads a record from the Tag and identifies its type.
  * @param  record : pointer on the record structure to fill.
  * @retval Status : Status of the operation.
  */
static uint16_t getNDEFRecord(sRecordInfo_t *p_record)
{
  uint16_t status;
  static uint8_t *p_ndef_buffer;
  
#if (NDEF_DEMO_USE_MALLOC == 1)  
  uint16_t ndef_size;
  /* allocate the ndef buffer */
  status = NDEF_getNDEFSize(&ndef_size);
  if(status != NDEF_OK)
  {
    return status;
  }
  
  if(ndef_buffer != NULL)
  {
    free(ndef_buffer);
    ndef_buffer = NULL;
  }
  ndef_buffer = malloc(ndef_size);
  if(ndef_buffer == NULL)
  {
    return NDEF_ERROR_MEMORY_INTERNAL;
  }
#else /* (NDEF_DEMO_USE_MALLOC != 1) */ 
  p_ndef_buffer = NDEF_Buffer;
#endif /* (NDEF_DEMO_USE_MALLOC == 1) */ 
  
  status = NDEF_ReadNDEF(p_ndef_buffer);
  if(status != NDEF_OK)
  {
    return status;
  }
  status = NDEF_IdentifyBuffer(p_record,p_ndef_buffer);
 
  return status;
}


/**
  * @brief  This function reads an URI from the Tag.
  * @param  r_uri : pointer on the URI structure to fill.
  * @retval Status : Status of the operation.
  */
static uint16_t getURI(sURI_Info * r_uri)
{
	sRecordInfo_t record;
 	uint16_t status;
  
  /* To avoid displaying random memory content in case URI part is missing */
  r_uri->Information[0] = '\0';
  r_uri->protocol[0] = '\0';
  r_uri->URI_Message[0] = '\0';
  
	status= getNDEFRecord(&record);
  if(status != NDEF_OK)
  {
    return status;
  }
  
  status = NDEF_ReadURI(&record, r_uri);
  
  return status;
 
}


/**
  * @brief  This function reads a SMS from the Tag.
  * @param  r_sms : pointer on the SMS structure to fill.
  * @retval Status : Status of the operation.
  */
static uint16_t getSMS(sSMSInfo * r_sms)
{
  uint16_t status;
	sRecordInfo_t record;
  memset(r_sms,0,sizeof(*r_sms));
  
  status = getNDEFRecord(&record);
	if(status != NDEF_OK)
  {
    return status;
  }
  
  status = NDEF_ReadSMS(&record,r_sms);
  return status;

}


/**
  * @brief  This function reads an Email from the Tag.
  * @param  r_email : pointer on the URI structure to fill.
  * @retval Status : Status of the operation.
  */
static uint16_t getEmail(sEmailInfo* r_email)
{
  uint16_t status;
	sRecordInfo_t record;
  
  status = getNDEFRecord(&record);
	if(status != NDEF_OK)
  {
    return status;
  }
  
  status = NDEF_ReadEmail(&record,r_email);
  return status;
 }

 /**
  * @brief  This function reads a vCard from the Tag.
  * @param  r_vcard : pointer on the URI structure to fill.
  * @retval Status : Status of the operation.
  */
static uint16_t getVcard(sVcardInfo* r_vcard)
{
  uint16_t status;
	sRecordInfo_t record;
  
  status = getNDEFRecord(&record);
	if(status != NDEF_OK)
  {
    return status;
  }
  
  status = NDEF_ReadVcard(&record,r_vcard);
  return status;
  
}

/**
  * @brief  This function reads a Geolocation from the Tag.
  * @param  r_geo : pointer on the URI structure to fill.
  * @retval Status : Status of the operation.
  */
static uint16_t getGeo(sGeoInfo *r_geo)
{
  
  uint16_t status;
	sRecordInfo_t record;
  
  status = getNDEFRecord(&record);
	if(status != NDEF_OK)
  {
    return status;
  }
  
  status = NDEF_ReadGeo(&record,r_geo);
  return status;
  
}

/*********************************** MyApp private functions *****************************************/

/**
  * @brief  This function reads a MyApp record from the Tag.
  * @param  r_myapp : pointer on the URI structure to fill.
  * @retval Status : Status of the operation.
  */
static uint16_t getMyApp(sMyAppInfo *r_myapp)
{
  
  uint16_t status;
	sRecordInfo_t record;
  
  status = getNDEFRecord(&record);
	if(status != NDEF_OK)
  {
    return status;
  }
  
  status = NDEF_ReadMyApp(&record,r_myapp);
  return status;
  
}

/**
  * @brief  This function initializes the board for the MyApp demo using the parameters from the MyApp record
  * @param  p_myapp : pointer on the URI structure to fill.
  * @retval Status : Status of the operation.
  */
static void MyAppDemoInit(sMyAppInfo *p_myapp)
{
  
  Menu_DisplayClear();
  for(int i=0; i<MYAPP_NB_LINES; i++)
	{
		/* Set the Back Color */
		BSP_LCD_SetBackColor(p_myapp->LineX[i].BackGroundColor);
		/* Set the Text Color */
		BSP_LCD_SetTextColor(p_myapp->LineX[i].FontColor);
		/* Display the string */
		BSP_LCD_DisplayStringAtLine(p_myapp->LineX[i].LineNb, (uint8_t*)(p_myapp->LineX[i].String));
	}		
  
	switch(p_myapp->LedBlinkConf.Speed)
	{
		case BLINKING_SLOW:
			MyAppBlinkCounter = 2000;
      break;		
		case BLINKING_MEDIUM:
			MyAppBlinkCounter = 500;
      break;
		case BLINKING_FAST:
      MyAppBlinkCounter = 100;
      break;
		case BLINKING_NONE:
    default:
			MyAppBlinkCounter = 0;      
      break;
	}
  
  if( p_myapp->LedBlinkConf.LedConf[0] &(MYAPP_LED_CONF_INITAL_STATE) )
  {
		BSP_LED_On(LED3);
  }
  else
  {
    BSP_LED_Off(LED3);
  }
  
	if( (p_myapp->LedBlinkConf.LedConf[0] &(MYAPP_LED_CONF_BLINKING)) && (MyAppBlinkCounter != 0) )
  {
    MyAppCounter = MyAppBlinkCounter;
  }
}

/**
  * @brief  This function runs the MyApp demo with parameters from MyApp record
  * @param  r_uri : pointer on the URI structure to fill.
  * @retval Status : Status of the operation.
  */
static void MyAppDemoRun(sMyAppInfo *p_myapp)
{
	if( (p_myapp->LedBlinkConf.LedConf[0] &(MYAPP_LED_CONF_BLINKING)) && (MyAppBlinkCounter != 0) )
	{
    if(MyAppCounter-- == 0)
    {
      BSP_LED_Toggle(LED3);
      MyAppCounter = MyAppBlinkCounter; 
    }
  }
  HAL_Delay(1);
}


/*********************************** Bluetooth Low Energy private functions *****************************************/

/* Defined in hid_application.c */
void Hid_Profile_Application (void);

/**
  * @brief  This function initializes the Bluetooth module.
  * @retval Status : Status of the operation.
  */
static int BLE_Init(const char * DevAddr)
{
  int ret;  

	/* Initialize the BlueNRG SPI driver */
  BNRG_SPI_Init();
  
  /* Initialize the BlueNRG HCI */
  HCI_Init();

  /* Reset BlueNRG hardware */
  BlueNRG_RST();
    
  uint8_t dev_addr_low_endian[CONFIG_DATA_PUBADDR_LEN];
  for(int i = 0; i< CONFIG_DATA_PUBADDR_LEN; i++)
  {
    dev_addr_low_endian[i] = DevAddr[CONFIG_DATA_PUBADDR_LEN - i - 1];
  }
  
  /* The Nucleo board must be configured as SERVER */
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                                  CONFIG_DATA_PUBADDR_LEN,
                                  dev_addr_low_endian);
	if(ret != 0)
  {
    return ret;
	}
  
    /* Set output power level */
  ret = aci_hal_set_tx_power_level(1,4);
	if(ret != 0)
  {
    return ret;
  }

  Hid_Profile_Application();


	return 0;
}

/*********************************** Wifi private functions *****************************************/


/**
  * @brief  This function initializes the Wifi module.
  * @retval Status : Status of the operation.
  */
static int WIFIInit(void)
{
  char wifi_ssid[30];
  /* Defined a static only to force the compiler to 0-initialize the wifi_config structure */
	static wifi_config wifi_config;

  WiFi_Status_t status = WiFi_MODULE_SUCCESS;

  /* configure the timers */
  Timer_Config( );
  
  UART_Configuration(BUS_USART3_BAUDRATE); 
	
  wifi_config.power=wifi_active;
  wifi_config.power_level=high;
  wifi_config.dhcp=on; /* use DHCP IP address */
  
  /* Init the wi-fi module */  
  status = wifi_init(&wifi_config);
  if(status!=WiFi_MODULE_SUCCESS)
  {
    return status;
  }
  NDEF_DEMO_SET_WIFI_SSID(wifi_ssid);
  
	status = wifi_ap_start((uint8_t *)wifi_ssid, NDEF_DEMO_WIFI_CHANNEL_NUM);
 
	return status;
	
}



/* Callbacks ---------------------------------------------------------*/

/**
  * @brief  Callback from NFCMEM_IO_MemWrite to manage the progress bar when writing a big NDEF.
  * @param  size : number of byte written in memory.
  */
void NFCMEM_IO_MemWriteCompleted_Callback(uint32_t size)
{
  TxedSize += size;
  if (TxSize != 0)
  {
    Menu_DrawProgressBarLine(Line7, (TxedSize * 100) / (float)TxSize);
    if(TxedSize >= TxSize)
    {
      /* Bar completed, disable */
      TxSize = 0;
      TxedSize = 0;
    }
  }
}

/**
  * @brief  This function is the callback for the Wifi Middleware, called when a client has joined
  * @param  mac : string used to return the MAC address of connected device.
  */
void ind_wifi_ap_client_joined(uint8_t *mac)
{
	strcpy((char *)WifiClientMac , (char *)mac);
	WifiConnected = 1;
}

/* Public functions ---------------------------------------------------------*/

/** @defgroup NDEF_DEMO NDEF Demo
  * @brief This module implements the NDEF demo functions.
  * @details It covers the following usecases:
  * - URI NDEF records: URL & Phone
  * - SMS NDEF record
  * - Email NDEF record
  * - vCard NDEF record (with and without an embedded picture)
  * - Geolocation NDEF record
  * - MyApp custom NDEF record
  * - Multi NDEF record with AAR
  * - Bluetooth Low Energy OOB NDEF record
  * - Wifi OOB NDEF record
  * @ingroup ST25_Discovery_Demo
  * @{
  */

/**
  * @brief  This function initializes the NFC Tag to perform the NDEF demos.
  * @details The tag is configured to its default and a CC file is written.
  */
void NDEF_DEMO_Init_Tag(void)
{
    /* Tag configuration for NDEF */
    BSP_LPD_WritePin(GPIO_PIN_RESET);
    PresentPasswd( true );

    /* Disable Mailbox mode */
    DeInitMailBoxMode();
    /* Activate RF */
    BSP_NFCTAG_WriteRFMngt(BSP_NFCTAG_INSTANCE, 0);
    
    /* Disable Energy Harvesting */
    BSP_NFCTAG_ResetEHENMode_Dyn(BSP_NFCTAG_INSTANCE);
  
    /* De-activate any protection */
    uint32_t mem_size = BSP_NFCTAG_GetByteSize(BSP_NFCTAG_INSTANCE);
    ST25DV_PROTECTION_ZONE ZoneRF;
    ST25DV_RF_PROT_ZONE no_rf_prot = {.PasswdCtrl = ST25DV_NOT_PROTECTED , .RWprotection = ST25DV_NO_PROT};
    for(ZoneRF = ST25DV_PROT_ZONE1; ZoneRF <= ST25DV_PROT_ZONE4 ; ZoneRF++)
    {
      ST25_RETRY(BSP_NFCTAG_WriteRFZxSS(BSP_NFCTAG_INSTANCE, ZoneRF, no_rf_prot));
      ST25_RETRY(BSP_NFCTAG_WriteI2CProtectZonex(BSP_NFCTAG_INSTANCE, ZoneRF, ST25DV_NO_PROT));
    }
    ST25_RETRY(BSP_NFCTAG_InitEndZone(BSP_NFCTAG_INSTANCE));

    ST25_RETRY(BSP_NFCTAG_WriteLockCCFile(BSP_NFCTAG_INSTANCE, ST25DV_CCFILE_2BLCK, ST25DV_UNLOCKED));
    
    NfcTag_SelectProtocol(NFCTAG_TYPE5);
    
    if(NfcType5_NDEFDetection() != NDEF_OK)
    {
      /* Write CC file content */
      CCFileStruct.MagicNumber  = NFCT5_MAGICNUMBER_E1_CCFILE;
      CCFileStruct.Version      = NFCT5_VERSION_V1_0;
      /* Size must be specified by block of 8 bytes */
      if(mem_size > (0xFF*8))
      {
        CCFileStruct.MagicNumber  = NFCT5_MAGICNUMBER_E2_CCFILE;
        CCFileStruct.MemorySize   = NFCT5_EXTENDED_CCFILE;
        CCFileStruct.ExtMemorySize   = (uint16_t)(mem_size / 8);
      
      } else {
        CCFileStruct.MemorySize   = (uint8_t)(mem_size / 8);
      }
      CCFileStruct.TT5Tag       =  NDEF_DEMO_CC3_COMPLIANT_VALUE;

      if(NfcType5_TT5Init() != NDEF_OK)
      {
        Menu_MsgStatus("No CCFILE","CCFILE init failed!",MSG_STATUS_ERROR);
      }
    }
 
}



/*********************************** URI *****************************************/

/**
  * @brief  This function writes an URL to the Tag.
  */
void NDEF_DEMO_Write_URI_URL(void)
{

	uint16_t status;
	sURI_Info r_uri = {0};
  
  NDEF_DEMO_Init_Tag();

	/* Write URI for ST25 products web page */
	sURI_Info w_uri = {URI_ID_0x01_STRING, NDEF_DEMO_URL ,""};
	status = NDEF_WriteURI(&w_uri);

  /* Read back written URI */	
	if(status == NDEF_OK)
  {
    status = getURI(&r_uri);
	}
  
	if((status != NDEF_OK) || \
      strcmp(r_uri.protocol,w_uri.protocol) || \
      strcmp(r_uri.URI_Message,w_uri.URI_Message))
	{
		Menu_MsgStatus("Store URI failure!","URL has not been written!",MSG_STATUS_ERROR);
	}
  else
  {
    char msg[200] = "The URL has been written:\n";
    strcat(msg,URI_ID_0x01_STRING);
    strcat(msg,NDEF_DEMO_URL);
    strcat(msg,"\n\nPlease read it with a phone!");
		Menu_MsgStatus("Store URI success!",msg,MSG_STATUS_SUCCESS);
	}

}	

/**
  * @brief  This function writes a phone number to the Tag.
  */
void NDEF_DEMO_Write_URI_Tel(void)
{

	uint16_t status;
	sURI_Info w_uri = {URI_ID_0x05_STRING,NDEF_DEMO_PHONE_NUMBER,""};
  sURI_Info r_uri = {0};

  NDEF_DEMO_Init_Tag();

	status = NDEF_WriteURI(&w_uri);
	
  /* Read back written URI */	
	if(status == NDEF_OK)
    status = getURI(&r_uri);

	if((status != NDEF_OK) || \
    strcmp(r_uri.protocol,w_uri.protocol) || \
    strcmp(r_uri.URI_Message,w_uri.URI_Message))
  {
		Menu_MsgStatus("Store URI failure!","Phone number has not been written!",MSG_STATUS_ERROR);
	}
  else
  {
    char msg[200] = "The phone number has been written:\n";
    strcat(msg,NDEF_DEMO_PHONE_NUMBER);
    strcat(msg,"\n\nPlease read it with a phone!");
		Menu_MsgStatus("Store URI success!",msg,MSG_STATUS_SUCCESS);
  }
}	

  
/**
  * @brief  This function reads and URI from the Tag and displays its content.
  */
void NDEF_DEMO_Read_URI(void)
{
  sURI_Info r_uri = {0};

  NDEF_DEMO_Init_Tag();

  uint16_t status = getURI(&r_uri);
  if(status != NDEF_OK)
  {
		Menu_MsgStatus("Read URI failure!","URI cannot be read!",MSG_STATUS_ERROR);    
  }
  else
  {
    /* Displays the URI */
    char msg[200] = "";
    if(!strcmp(r_uri.protocol,URI_ID_0x01_STRING) || \
      !strcmp(r_uri.protocol,URI_ID_0x02_STRING))
    {
      strcat (msg, "!URL:\n");
    }
    else if (!strcmp(r_uri.protocol,"tel:"))
    {
      strcat (msg, "!Phone number:\n");
    }
    else
    {
        strcat (msg, "!URI:\n");
    }
    strcat(msg,r_uri.protocol);
    strcat(msg,r_uri.URI_Message);
    Menu_MsgStatus("Read URI success!",msg,MSG_STATUS_SUCCESS);
  }
}


/*********************************** SMS *****************************************/

/**
  * @brief  This function reads a SMS from the Tag and displays it.
  */
void NDEF_DEMO_Read_SMS(void)
{
	sSMSInfo r_sms = {0};

  NDEF_DEMO_Init_Tag();
  
	if (getSMS(&r_sms) != NDEF_OK)
  {
		Menu_MsgStatus("Read SMS failure!","SMS cannot be read!",MSG_STATUS_ERROR);    
	}
  else
  {
    char Msg[400] = "";
    strcat(Msg,"!Phone number:\n");
    strcat(Msg,r_sms.PhoneNumber);
    strcat(Msg,"\f!Message:\n");    
    strcat(Msg,r_sms.Message);
 		Menu_MsgStatus("SMS content",Msg,MSG_STATUS_SUCCESS);       
	}
}

/**
  * @brief  This function writes a SMS to the Tag.
  */
void NDEF_DEMO_Write_SMS(void)
{
	sSMSInfo r_sms = {0};
	uint16_t status;

  NDEF_DEMO_Init_Tag();

	sSMSInfo w_sms = { NDEF_DEMO_PHONE_NUMBER,
                    "This SMS was generated automatically by tapping your phone near ST25DV",
                    ""};
	
	status = NDEF_WriteSMS ( &w_sms );
  

  if(status == NDEF_OK)
  {
    status = getSMS(&r_sms);
  }
  
	if((status != NDEF_OK) || \
      strcmp(r_sms.Information,w_sms.Information) || \
      strcmp(r_sms.Message,w_sms.Message) || \
      strcmp(r_sms.PhoneNumber,w_sms.PhoneNumber))
  {
		Menu_MsgStatus("Store SMS failure!","SMS has not been written!",MSG_STATUS_ERROR);
	}
  else
  {
		Menu_MsgStatus("Store SMS success!","The SMS has been written!\n\nPlease read it with a phone!",MSG_STATUS_SUCCESS);
	}
}



/*********************************** Email *****************************************/

/**
  * @brief  This function reads an Email from the Tag and displays its content.
  */
void NDEF_DEMO_Read_Email(void)
{
  sEmailInfo	r_email = {0};

  NDEF_DEMO_Init_Tag();
  
	if (getEmail(&r_email) != NDEF_OK)
  {
		Menu_MsgStatus("Email failure!","Email cannot be read!",MSG_STATUS_ERROR);    
	}
  else
  {
    /* 50 bytes for headers + data */
    char *msg = (char *)malloc(50 + 
                               strlen(r_email.EmailAdd) + 
                               strlen(r_email.Subject) +
                               strlen(r_email.Message));
    
    if(msg == NULL)
    { Menu_MsgStatus("Memory error!","Cannot allocate memory",MSG_STATUS_ERROR);    
      return;
    }
    strcpy(msg,"!To:\n");
    strcat(msg,r_email.EmailAdd);
    strcat(msg,"\f!Subject:\n");    
    strcat(msg,r_email.Subject);
    strcat(msg,"\f!Message:\n");
    strcat(msg,r_email.Message);
    
 		Menu_MsgStatus("Email content",msg,MSG_STATUS_SUCCESS);
    
    free (msg);
    NDEF_closeEmail(&r_email);
	}
}


/**
  * @brief  This function writes an Email to the Tag.
  */
void NDEF_DEMO_Write_Email(void)
{
  uint16_t status;
  sEmailInfo r_email = {0};

  NDEF_DEMO_Init_Tag();

  sEmailInfo w_email = {.EmailAdd = "customer.service@st.com",
                        .Subject = "ST25DV S/N 754FHFGJF46G329 WARRANTY",
                        .Message = "this is a demo message to illustrate an automatic warranty activation email",
                        .Information = ""};

	
	status = NDEF_WriteEmail ( &w_email );
  if(status == NDEF_OK)
    status = getEmail(&r_email);
  
	if((status != NDEF_OK) || \
    strcmp(r_email.Message,w_email.Message) || \
    strcmp(r_email.EmailAdd,w_email.EmailAdd) || \
    strcmp(r_email.Subject,w_email.Subject) || \
    strcmp(r_email.Information,w_email.Information))
  {
		Menu_MsgStatus("Email failure!","Email has not been written!",MSG_STATUS_ERROR);
	}
  else
  {
		Menu_MsgStatus("Email success!","The Email has been written!\n\nPlease read it with a phone!",MSG_STATUS_SUCCESS);
  }
  
  NDEF_closeEmail(&r_email);

}

/*********************************** vCard *****************************************/

/**
  * @brief  This function reads a vCard record from the Tag and display its content.
  */
void NDEF_DEMO_Read_Vcard(void)
{
  sVcardInfo	r_vcard = {0};

  NDEF_DEMO_Init_Tag();
  
	if (getVcard(&r_vcard) != NDEF_OK)
  {
		Menu_MsgStatus("Read vCard failure!","vCard cannot be read!",MSG_STATUS_ERROR);    
	}
  else
  {
    /* 50 bytes for headers + data */
    char *msg = (char *)malloc(50 + strlen(r_vcard.FirstName)
                                  + strlen(r_vcard.Title)
                                  + strlen(r_vcard.Org)
                                  + strlen(r_vcard.CellTel)
                                  + strlen(r_vcard.WorkEmail)
                                  + strlen(r_vcard.WorkAddress));
    if(msg == NULL)
    {
      Menu_MsgStatus("Memory error!","Cannot allocate memory",MSG_STATUS_ERROR);    
      return;
    }
    strcpy(msg,"!Name & Position\n");
    strcat(msg,r_vcard.FirstName);
    strcat(msg,"\n");    
    strcat(msg,r_vcard.Title);
    strcat(msg,"\n");
    strcat(msg,r_vcard.Org);
    strcat(msg,"\f!Tel & e-mail\n");    
    strcat(msg,r_vcard.CellTel);
    strcat(msg,"\n");
    strcat(msg,r_vcard.WorkEmail);
    strcat(msg,"\f!Mail address\n");
    if(*r_vcard.WorkAddress != '\0')
    {      
      strcat(msg,r_vcard.WorkAddress);
    }
    else
    {
      strcat(msg,r_vcard.Address);
 		}
    
    Menu_MsgStatus("vCard content",msg,MSG_STATUS_SUCCESS);
    
    /* Display a picture if present */
    uint8_t* pPict = malloc(8192);
    if(pPict != NULL)
    {
      sRecordInfo_t record;
      getNDEFRecord(&record);
      if(NDEF_getVcardPicture( record.PayloadBufferAdd,  record.PayloadLength,  pPict ) == 0 )
      {
        Menu_MsgStatus("vCard picture","",MSG_INFO_SUCCESS);
        uint32_t width, height;
        BSP_LCD_GetPictureDim((const char *)pPict, &width, &height);
        width = (BSP_LCD_GetXSize() - width) / 2;
        height = (BSP_LCD_GetYSize() - height) / 2;
        BSP_LCD_DisplayPicture(width,height, (const char*)pPict);
        while(Menu_GetUserInput() == 0);
      }
      free(pPict);
    }
    
    free (msg);
	}
}




/**
  * @brief  This function writes a small vCard record to the Tag.
  */
void NDEF_DEMO_Write_NoPicture_Vcard(void)
{
  uint16_t status;
  sVcardInfo r_vcard = {0};

  NDEF_DEMO_Init_Tag();

  sVcardInfo w_vcard = { .Version = VCARD_VERSION_2_1,
                        .Name = "You;CanIHelp;;;",
                        .FirstName = "CanIHelp You",
                        .Title = "Customer support",
                        .Org = "STMicroelectronics",
                        .HomeAddress = "",
                        .WorkAddress = "190 avenue Celestin Coq 13106 ROUSSET FRANCE;;;;;;;",
                        .HomeTel = "",
                        .WorkTel = "",
                        .CellTel = NDEF_DEMO_PHONE_NUMBER,
                        .HomeEmail = "",
                        .WorkEmail = "customer.service@st.com",         
                        .Url = "www.st.com/nfc-rfid"};         

	status = NDEF_WriteVcard ( &w_vcard );
  if(status == NDEF_OK)
  {
    status = getVcard(&r_vcard);
	}
  
  if((status != NDEF_OK) ||
      strcmp(r_vcard.Version,w_vcard.Version) ||
      strcmp(r_vcard.FirstName,w_vcard.FirstName) ||
      strcmp(r_vcard.Title,w_vcard.Title) ||
      strcmp(r_vcard.Org,w_vcard.Org) ||
      strcmp(r_vcard.WorkAddress,w_vcard.WorkAddress) ||
      strcmp(r_vcard.CellTel,w_vcard.CellTel) ||
      strcmp(r_vcard.WorkEmail,w_vcard.WorkEmail))
  {
    Menu_MsgStatus("vCard failure!","vCard has not been written!",MSG_STATUS_ERROR);
  }
	else
  {
		Menu_MsgStatus("vCard success!","The vCard has been written!\n\nPlease read it with a phone!",MSG_STATUS_SUCCESS);
  }
}



/**
  * @brief  This function writes a vCards with an embedded picture to the Tag.
  */
void NDEF_DEMO_Write_Picture_Vcard(void)
{
  uint16_t status;

  NDEF_DEMO_Init_Tag();

  uint16_t ndef_size = sizeof(uVcardCSL1);
  
  Menu_MsgStatus("Work in progress!!","Storing 3kB vCard in memory\nPlease wait...",MSG_INFO);
	
  TxSize = ndef_size;
  TxedSize = 0;
  Menu_DrawProgressBarLine(Line7, 0);
  status = NDEF_WriteNDEF (ndef_size, (uint8_t*)uVcardCSL1);
  if(status != NDEF_OK)
  {
    if(status == NDEF_ERROR_MEMORY_TAG)
    {
      Menu_MsgStatus("vCard failure!","vCard is bigger than memory!",MSG_STATUS_ERROR);
    } else {
      Menu_MsgStatus("vCard failure!","vCard has not been written!",MSG_STATUS_ERROR);
    }
    
	}
  else
  {
		Menu_MsgStatus("vCard success!","The vCard has been written!\n\nPlease read it with a phone!",MSG_STATUS_SUCCESS);
  }
  
}

/**
  * @brief  This function writes a vCard record to the Tag.
  */
void NDEF_DEMO_Write_Vcard(void)
{
  
  if(BSP_NFCTAG_GetByteSize(BSP_NFCTAG_INSTANCE) > sizeof(uVcardCSL1))
  {
    NDEF_DEMO_Write_Picture_Vcard();
  } else {
    NDEF_DEMO_Write_NoPicture_Vcard();    
  }
  
}

/*********************************** GeoLocation *****************************************/


/**
  * @brief  This function reads a Geolocation record from the Tag and displays it content.
  */
void NDEF_DEMO_Read_Geo(void)
{
  sGeoInfo	r_geo = {0};

  NDEF_DEMO_Init_Tag();
  
	if (getGeo(&r_geo) != NDEF_OK)
  {
		Menu_MsgStatus("Read Geo failure!","Geo cannot be read!",MSG_STATUS_ERROR);    
	} else {
    /* 50 bytes for headers + data */
    char *msg = (char *)malloc(50 + strlen(r_geo.Latitude)
                                  + strlen(r_geo.Longitude));
    
    if(msg == NULL)
    {
      Menu_MsgStatus("Memory error!","Cannot allocate memory",MSG_STATUS_ERROR);    
      return;
    }
    
    strcpy(msg,"Latitude:\n");
    strcat(msg,r_geo.Latitude);
    strcat(msg,"\n\nLongitude:\n");    
    strcat(msg,r_geo.Longitude);
    
 		Menu_MsgStatus("Geo content",msg,MSG_STATUS_SUCCESS);
    
    free (msg);
	}

}


/**
  * @brief  This function writes a Geolocation record to the Tag.
  */
void NDEF_DEMO_Write_Geo(void)
{
  uint16_t status;
  sGeoInfo r_geo = {0};

  NDEF_DEMO_Init_Tag();

 /* Field to fill for the Geolocation demo */
  sGeoInfo w_geo = {.Latitude = "32.251454", .Longitude = "-64.822339", .Information = ""};
	
	status = NDEF_WriteGeo ( &w_geo );
  if(status == NDEF_OK)
    status = getGeo(&r_geo);
  
	if((status != NDEF_OK) || \
      strcmp(r_geo.Latitude,w_geo.Latitude) || \
      strcmp(r_geo.Longitude,w_geo.Longitude) || \
      strcmp(r_geo.Information,w_geo.Information))
  {
		Menu_MsgStatus("Store Geo failure!","Gelocalization has not been written!",MSG_STATUS_ERROR);
  }
	else
  {
		Menu_MsgStatus("Store Geo success!","Geolocalization has been written!\n\nPlease read it with a phone!",MSG_STATUS_SUCCESS);
  }

}

/*********************************** MyApp *****************************************/

/**
  * @brief  This function reads a MyApp record from the Tag and starts the associated demo.
  */
void NDEF_DEMO_Read_MyAPP(void)
{
  sMyAppInfo r_myapp = {0};

  /* Only use LED3 on ST25 discovery */
  BSP_LED_Init(LED3);

  /* Clear NDEF before adding AAR */
  NDEF_ClearNDEF();

  /* Initiate memory with Demo AAR */
	uint16_t status = NDEF_AddAAR (&w_aar);
  if(status != NDEF_OK)
		Menu_MsgStatus("Store AAR failure!","AAR has not been written!",MSG_STATUS_ERROR);

  
  /* Initialization for the GPO IT */
  /* Present password to change ST25DV configuration */
  PresentPasswd(true);
  InitITGPOMode(ST25DV_GPO_RFWRITE_MASK | ST25DV_GPO_ENABLE_MASK);
  
	MyAppBlinkCounter = 0;
  uint8_t user_exit = 0;
  Menu_MsgStatus("MyApp Instructions","Please write a MyApp demo data to the memory using the RF.\nRead the tag to get the ST25 NFC app.",MSG_INFO);  
  Menu_DisplayCenterString(Line10,"Touch to exit!");
  while(user_exit == 0)
  {
    while((GPO_Activated == 0) && (user_exit == 0))
    {
      /* Check user input to force exit */
      user_exit = Menu_GetUserInput();
      /* run the APP demo if configured */
        MyAppDemoRun(&r_myapp);
    }

    if(GPO_Activated != 0)
    {
      Menu_MsgStatus("Write on-going","Please wait...",MSG_INFO); 
			/* few ms to let the RF write operation complete */
      HAL_Delay(2000);
      int timeout = 10;
      while ((getMyApp(&r_myapp) != NDEF_OK) && timeout-- > 0)
      {
        HAL_Delay(500);
      }
      
      if (timeout <= 0)
      {
        Menu_MsgStatus("Read MyApp failure!","MyApp cannot be read! Please retry...",MSG_INFO_ERROR); 
      }
      else
      {
        MyAppDemoInit(&r_myapp);
      }
      GPO_Activated = 0;
    }
  };
  
  DeInitITGPOMode();
  BSP_LED_DeInit(LED3);

}


/**
  * @brief  This function writes an AAR record (selecting the ST NFC application) to the Tag.
  */
void NDEF_DEMO_Write_AAR(void)
{
  uint16_t status;

  NDEF_DEMO_Init_Tag();

  /* Clear NDEF before adding AAR */
  NDEF_ClearNDEF();

	status = NDEF_AddAAR (&w_aar);
  if(status != NDEF_OK)
  {
		Menu_MsgStatus("Store AAR failure!","AAR has not been written!",MSG_STATUS_ERROR);
	}
  else
  {
		Menu_MsgStatus("Store AAR success!","ST25 NFC app AAR has been written!\n\nPlease read it with a phone!",MSG_STATUS_SUCCESS);
  }
}

/**
  * @brief  This function adds an AAR record (selecting the ST NFC application) to an existing NDEF file on the Tag.
  */
void NDEF_DEMO_MultiRecord_With_AAR(void)
{
  uint16_t status;

  NDEF_DEMO_Init_Tag();

  /* No clear NDEF before adding AAR */

	status = NDEF_AddAAR (&w_aar);
  if(status != NDEF_OK)
  {
		Menu_MsgStatus("Add AAR failure!","AAR has not been added!",MSG_STATUS_ERROR);
  }
	else
  {
		Menu_MsgStatus("Add AAR success!","ST25 NFC app AAR has been added to existing URL!\n\nPlease read it with a phone!",MSG_STATUS_SUCCESS);
  }
}



/*********************************** Bluetooth Low Energy *****************************************/

  /* Set OOB values aligned with BLE_Init */
  Ndef_Bluetooth_OOB_t BleOOB = {.Type = NDEF_BLUETOOTH_BLE,
                                    .DeviceAddress = NDEF_DEMO_BLE_ADDR_BIG_ENDIAN,
                                    .DeviceAddressType = NDEF_BLE_PUBLIC_ADDRESS_TYPE,
                                    .Role = NDEF_BLE_ROLE_PERIPH_ONLY,
                                    .OptionalMask = NDEF_BLUETOOTH_OPTION(BLUETOOTH_EIR_COMPLETE_LOCAL_NAME),
                                    .LocalName = NDEF_DEMO_BLE_NAME,                                
                                    };

/**
  * @brief  This function change the Bluetooth Low Energy Device Address (to be used when the module is paired with a nearby smartphone).
  */
void NDEF_DEMO_BLE_ChangeDeviceAddr(void)
{
  srand(HAL_GetTick());
  for(int i = 3; i < 6; i++)
  {
    BleOOB.DeviceAddress[i] = rand();
  } 
  char msg[40];
  sprintf(msg,"New address:\n%02X:%02X:%02X:%02X:%02X:%02X",BleOOB.DeviceAddress[0],
                                                            BleOOB.DeviceAddress[1],
                                                            BleOOB.DeviceAddress[2],
                                                            BleOOB.DeviceAddress[3],
                                                            BleOOB.DeviceAddress[4],
                                                            BleOOB.DeviceAddress[5]);
  Menu_MsgStatus("BLE address",msg,MSG_STATUS_SUCCESS);
}


/**
  * @brief  This function writes a Bluetooth Low Energy OOB record to the Tag and starts the BLE module, waiting for a connection to occur.
  */
void NDEF_DEMO_Write_BLE_OOB(void)
{
  uint16_t status;
  static uint8_t get_unique_id = 1;
  
  if(get_unique_id == 1)
  {
    /* Make sure, that initial DeviceAddress is different between boards at reset, using the STM32 Unique Id LSB */
    memcpy(&BleOOB.DeviceAddress[3],(void*)0x1FFF7A10,3);
    get_unique_id = 0;
  }
  
  NDEF_DEMO_Init_Tag();
  /* Clear NDEF before adding OOB */
  NDEF_ClearNDEF();

  /* Write the OOB to the Tag */
	status = NDEF_AppendBluetoothOOB ( &BleOOB, NULL );
  
  if(status != NDEF_OK)
  {	
    Menu_MsgStatus("Store OOB failure!","OOB has not been written!",MSG_STATUS_ERROR);
    return;
	}
  else
  {
    Menu_MsgStatus("Store OOB success!","OOB has been written!\n\nPlease read it with a phone!",MSG_INFO);
    Menu_DisplayCenterString(Line10,"Wait connection...");
  }
  
   /* Start BLE module */
  PresentPasswd(true);
  InitITGPOMode(ST25DV_GPO_RFACTIVITY_MASK | ST25DV_GPO_ENABLE_MASK);
  
    /* Start BLE module */
  while(1)
  {
    if(Menu_GetUserInput())
    {
      break;
    }
    if(GPO_Activated)
    {
      BLE_Init(BleOOB.DeviceAddress);
      GPO_Activated = 0;
      break;
    }
  }
  DeInitITGPOMode();
/* Prevent connection outside the demo. */
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_RESET);
 
}


/**
  * @brief  This function reads a BLE OOB record from the Tag and displays it content.
  */
void NDEF_DEMO_Read_Bluetooth_OOB(void)
{
  uint16_t status;
	sRecordInfo_t record;
  Ndef_Bluetooth_OOB_t bluetooth_oob;

  NDEF_DEMO_Init_Tag();

  status = getNDEFRecord(&record);
	if(status != NDEF_OK)
  {
		Menu_MsgStatus("Read OOB failure!","Cannot get the record!",MSG_STATUS_ERROR);
    return;
  }
  
  status = NDEF_ReadBluetoothOOB(&record,&bluetooth_oob);
  if(status != NDEF_OK)
  {
    Menu_MsgStatus("Read OOB failure!","OOB has not been read!",MSG_STATUS_ERROR);
	}
  else
  {
    /* 50 bytes for headers + data */
    char *msg = (char *)malloc(50 +
                        sizeof(bluetooth_oob.DeviceAddress) +
                        strlen(bluetooth_oob.LocalName));
    if(msg == NULL)
    {
      Menu_MsgStatus("Memory error!","Cannot allocate memory",MSG_STATUS_ERROR);    
      return;
    }
    
    strcpy(msg,"Type: ");
    if(bluetooth_oob.Type == NDEF_BLUETOOTH_BREDR)
    {
      strcat(msg,"BR/EDR\n\n");
    }
    else if (bluetooth_oob.Type == NDEF_BLUETOOTH_BLE)
    {
      strcat(msg,"BLE\n\n");
    }      
    strcat(msg,"DeviceAddress:\n");
    
    char device_address[18];
    sprintf(device_address, "%02X:%02X:%02X:%02X:%02X:%02X",bluetooth_oob.DeviceAddress[0],
                                                bluetooth_oob.DeviceAddress[1],
                                                bluetooth_oob.DeviceAddress[2],
                                                bluetooth_oob.DeviceAddress[3],
                                                bluetooth_oob.DeviceAddress[4],
                                                bluetooth_oob.DeviceAddress[5]);
    
    strcat(msg,device_address);
    strcat(msg,"\n\nLocal name:\n");    
    strcat(msg,bluetooth_oob.LocalName);
 		Menu_MsgStatus("OOB content",msg,MSG_STATUS_SUCCESS);

    free (msg);
  
  }  
}


/*********************************** WIFI *****************************************/

/**
  * @brief  This function writes a Wifi OOB record to the Tag and starts the Wifi module, waiting for a connection to occur.
  */
void NDEF_DEMO_Write_Wifi_OOB(void)
{
  static uint8_t wifi_initialized = 0;
  WifiConnected = 0;

  NDEF_DEMO_Init_Tag();

  /* Clear NDEF before the Wifi is initialized. */
  NDEF_ClearNDEF();

  /* Don't re-do the Wifi module init to save some time */
  if(!wifi_initialized)
  {
    Menu_MsgStatus("Wifi init!","Wifi module being initialized, please wait a few seconds!",MSG_INFO);
    if (WIFIInit() != WiFi_MODULE_SUCCESS)
    {
      Menu_MsgStatus("Wifi init failure!","Wifi module initialization has failed!",MSG_STATUS_ERROR);
    } else {
      wifi_initialized = 1;

    }
  } else {
    HAL_TIM_Base_Start_IT(&TimHandle);
  }
  
  

  
  /* When the Wifi module is configured in Access Point mode it only support open mode - no authentication  */
  sWifiTokenInfo wps_oob = {.NetworkSSID        = "",
                            .AuthenticationType = NDEF_WIFI_AUTHENTICATION_NONE,
                            .EncryptionType     = NDEF_WIFI_ENCRYPTION_NONE,
                            .NetworkKey         = ""};
  /* Set SSID */
  NDEF_DEMO_SET_WIFI_SSID(wps_oob.NetworkSSID);
                            
  if(NDEF_WriteWifiToken(&wps_oob)!= NDEF_OK)
  {
    Menu_MsgStatus("Write Wifi failure!","Wifi token cannot be written!",MSG_STATUS_ERROR);
  } else {
    Menu_MsgStatus("Store Wifi success!","Wifi OOB has been written!\n\nPlease read it with a phone!",MSG_INFO);
    
    /* Wait for connection or user action to exit */
    while(!WifiConnected && !Menu_GetUserInput());

    /* displays phone connection on Wifi */
    if(WifiConnected)
    {
      char msg[50];
      sprintf(msg,"Wifi new connexion with device:\n%02X:%02X:%02X:%02X:%02X:%02X",
              WifiClientMac[0],
              WifiClientMac[1],
              WifiClientMac[2],
              WifiClientMac[3],
              WifiClientMac[4],
              WifiClientMac[5]);
      Menu_MsgStatus("Wifi connection",msg,MSG_STATUS_SUCCESS);
    }
  }
  /* Disable timer IT, was blocking after the reset for FW upgrade */
  HAL_TIM_Base_Stop_IT(&TimHandle);
}

/**
  * @brief  This function reads a Wifi OOB record from the Tag and displays it's content.
  */
void NDEF_DEMO_Read_Wifi_OOB(void)
{
  
 	sRecordInfo_t record;
 	sWifiTokenInfo wps = {0};

  NDEF_DEMO_Init_Tag();
  
	getNDEFRecord(&record);
  if(NDEF_ReadWifiToken(&record, &wps) != NDEF_OK)
  {	
    Menu_MsgStatus("Read Wifi failure!","Wifi token cannot be read!",MSG_STATUS_ERROR);
	}
  else
  {
    char Msg[100];
    sprintf(Msg,"SSID:\n%s\n\nKEY:\n%s",wps.NetworkSSID,wps.NetworkKey);
    Menu_MsgStatus("Read Wifi success!",Msg,MSG_STATUS_SUCCESS);
  }
                        
}

/**
  * @brief  This function writes an empty NDEF message.
  */
void NDEF_DEMO_Write_empty_NDEF( void )
{
  uint32_t mem_size = 0;
  
  NDEF_DEMO_Init_Tag();

  mem_size = BSP_NFCTAG_GetByteSize(BSP_NFCTAG_INSTANCE);
  /* Write CC file content */
  CCFileStruct.MagicNumber  = NFCT5_MAGICNUMBER_E1_CCFILE;
  CCFileStruct.Version      = NFCT5_VERSION_V1_0;
  /* Size must be specified by block of 8 bytes */
  if(mem_size > (0xFF*8))
  {
    CCFileStruct.MagicNumber  = NFCT5_MAGICNUMBER_E2_CCFILE;
    CCFileStruct.MemorySize   = NFCT5_EXTENDED_CCFILE;
    CCFileStruct.ExtMemorySize   = (uint16_t)(mem_size / 8);
  
  } else {
    CCFileStruct.MemorySize   = (uint8_t)(mem_size / 8);
  }
  CCFileStruct.TT5Tag       =  NDEF_DEMO_CC3_COMPLIANT_VALUE;

  if(NfcType5_TT5Init() != NDEF_OK)
  {
    Menu_MsgStatus( "No CCFILE", "CCFILE init failed!", MSG_STATUS_ERROR );
  }
  else
  {
    Menu_MsgStatus( "Store success!", "Empty NDEF message written!", MSG_STATUS_SUCCESS );
  }
}

/**
  * @brief  This function writes 0xFF to the 4 first bytes in EEPROM.
  */
void NDEF_DEMO_Erase_CCFile( void )
{
  uint8_t adata[] = { 0xFF, 0xFF, 0xFF, 0xFF };

  NDEF_DEMO_Init_Tag();
  
  if( BSP_NFCTAG_WriteData(BSP_NFCTAG_INSTANCE, adata, 0x00, 0x4) != NFCTAG_OK )
  {
    Menu_MsgStatus( "Erase error!", "CCFile not erased!", MSG_STATUS_ERROR );
  }
  else
  {
    Menu_MsgStatus( "Erase success!", "CCFile erased!", MSG_STATUS_SUCCESS );
  }
}

/**
  * @brief  This function writes 0xFF to the entire EEPROM.
  */
void NDEF_DEMO_Clear_Eeprom( void )
{
  uint8_t adata[256];
  uint32_t size = 0;
  uint32_t cnt;
  int32_t ret = NFCTAG_OK;
  
  NDEF_DEMO_Init_Tag();
  
  for( cnt = 0; cnt < 0x100; cnt++ )
  {
    adata[cnt] = 0xFF;
  }
  size = BSP_NFCTAG_GetByteSize(BSP_NFCTAG_INSTANCE);
  
  Menu_MsgStatus( "Clear EEPROM", "EEPROM is being cleared...\n", MSG_INFO );
  TxSize = size;
  TxedSize = 0;
  Menu_DrawProgressBarLine(Line7, 0);

  cnt = 0;
  while( size > 0)
  {
    if( size > 256 )
    {
      ret |= BSP_NFCTAG_WriteData(BSP_NFCTAG_INSTANCE, adata, (cnt * 256), 256);
      size -= 256;
      cnt++;
    }
    else
    {
      ret |= BSP_NFCTAG_WriteData(BSP_NFCTAG_INSTANCE, adata, (cnt * 256), size);
      size -= size;
    }
  }
  if( ret != NFCTAG_OK )
  {
    Menu_MsgStatus( "Clear error!", "EEPROM not cleared!", MSG_STATUS_ERROR );
  }
  else
  {
    Menu_MsgStatus( "Clear success!", "EEPROM cleared!", MSG_STATUS_SUCCESS );
  }
}
/**
  * @}
  */ 



/***************************************** Handover ******************************************/
/*

// Static handover case (only write Handover Select)
void WriteHandoverSelect(void)
{
   Ndef_Handover_t wHandover = {.type = NDEF_HANDOVER_SELECT_TYPE, .version = NDEF_HANDOVER_VERSION_1_2};
   sRecordInfo_t HandoverRecord;
   Ndef_Handover_alternative_carrier_t wAC = {.cps = NDEF_HANDOVER_AC_CPS_ACTIVE,
                                              .aux_data_ref_count = 0};

  BLE_Init();

  // Prepare the beg of the response 
  NDEF_CreateHandover(&wHandover,&HandoverRecord);
//  NDEF_AddAlternativeCarrier(&wAC,"urn:nfc:handover:bredr", NULL,&HandoverRecord );
  NDEF_AddAlternativeCarrier(&wAC,"urn:nfc:handover:ble", NULL,&HandoverRecord );
  NDEF_AddAlternativeCarrier(&wAC,"urn:nfc:handover:wifi", NULL,&HandoverRecord );
  // TO DO: add Alternative Carrier for Wifi
  NDEF_WriteHandover(&HandoverRecord, NDEF_Buffer);
                                              
  // Write BLE OOB
     WriteBleOOB();                                         
  //   WriteBrEdrOOB();                                         
}

// Static handover case (only write Handover Select)
void ReadHandover(void)
{
  uint16_t status ;
   // Read buffers 
  Ndef_Handover_t rHandover;
  sRecordInfo_t rRecord;
  Ndef_Handover_alternative_carrier_t rAC;
  
  status = getNDEFRecord(&rRecord);
	if(status != NDEF_OK)
  {
		Menu_MsgStatus("Read OOB failure!","Cannot get the record!",MSG_STATUS_ERROR);
    return;
  }
    // Read the request 
  status = NDEF_ReadHandover(&rRecord ,  &rHandover ); 

  if(status != NDEF_OK)
  {
    Menu_MsgStatus("Read Handover failure!","Handover has not been read!",MSG_STATUS_ERROR);
	} else {
    // 50 bytes for headers + data
    char *Msg = (char *)malloc(60);
     if(Msg == NULL)
    { Menu_MsgStatus("Memory error!","Cannot allocate memory",MSG_STATUS_ERROR);    
      return;
    }
    strcpy(Msg,"Type: ");
    if(rHandover.type == NDEF_HANDOVER_SELECT_TYPE)
      strcat(Msg,"SELECT\n\n");
    else if (rHandover.type == NDEF_HANDOVER_REQUEST_TYPE)
      strcat(Msg,"REQUEST\n\n");
      
    strcat(Msg,"Version: ");
    
    char Version_str[4];
    sprintf(Version_str, "%d.%d",rHandover.version >> 4, rHandover.version &0xf);
    strcat(Msg,Version_str);
    
 		Menu_MsgStatus("Handover content",Msg,MSG_STATUS_SUCCESS);
    free (Msg);

    for(uint16_t ac_index=0;ac_index<rHandover.nb_alternative_carrier;ac_index++)
  {
     status = NDEF_ReadAC( ac_index, &rHandover  , &rAC );
     if(status != NDEF_OK)
     {
        Menu_MsgStatus("Read AC failure!","Handover has not been read!",MSG_STATUS_ERROR);
     } else {
        char *Msg = (char *)malloc(60);
        if(Msg == NULL)
        { Menu_MsgStatus("Memory error!","Cannot allocate memory",MSG_STATUS_ERROR);    
          return;
        }
        
        strcat(Msg,"ID: \n");
        strncat(Msg,(char*)rAC.ac_record.ID,rAC.ac_record.IDLength);
       
        strcat(Msg,"\n\nCPS: ");
        char cps[4];
        sprintf(cps, "%d",rAC.cps);
        strcat(Msg,cps);
            
        Menu_MsgStatus("AC content",Msg,MSG_STATUS_SUCCESS);
        
    
        free (Msg);
      }
    }

  
  }  

}




void HandoverDemo(void)
{
  uint8_t ac_index, aux_index;
  uint16_t status;
  // Read buffers 
  Ndef_Handover_t rHandover;
  sRecordInfo_t rRecord;
  Ndef_Handover_alternative_carrier_t rAC;
  getNDEFRecord(&rRecord);
  // Response buffer 
  Ndef_Handover_t wHandover = {.type = NDEF_HANDOVER_SELECT_TYPE, .version = NDEF_HANDOVER_VERSION_1_2};
  sRecordInfo_t HandoverRecord;
   Ndef_Handover_alternative_carrier_t wAC = {.cps = NDEF_HANDOVER_AC_CPS_ACTIVATING,
                                              .aux_data_ref_count = 0};
  

  // Prepare the beg of the response 
  status = NDEF_CreateHandover(&wHandover,&HandoverRecord);
  if (status != NDEF_OK) goto error;
  
  // Read the request 
  NDEF_ReadHandover(&rRecord ,  &rHandover ); 
  if(rHandover.type != NDEF_HANDOVER_REQUEST_TYPE)
  {
    Menu_MsgStatus("Handover demo","No handover request in memory!",MSG_STATUS_ERROR);
    return;
  }
  
  for(ac_index=0;ac_index<rHandover.nb_alternative_carrier;ac_index++)
  {
    NDEF_ReadAC( ac_index, &rHandover  , &rAC );
    for(aux_index = 0; aux_index < rAC.aux_data_ref_count; aux_index++)
    {
      sRecordInfo_t AuxRecord;
      NDEF_ReadAuxData( aux_index, &rAC, &AuxRecord );
      // Extract Auxiliary data
    }
    // 1. Extract OOB/or whatever data
    // 2. If this is a supported Carrier, add it to the response
       
    status = NDEF_AddAlternativeCarrier(&wAC,"urn:nfc:handover:ble", NULL,&HandoverRecord );
    if (status != NDEF_OK) goto error;
  
  }
  
  status = NDEF_AddAlternativeCarrier(&wAC,"urn:nfc:handover:ble", NULL,&HandoverRecord );
   if (status != NDEF_OK) goto error;
 
  
  status = NDEF_AddAlternativeCarrier(&wAC,"urn:nfc:handover:wifi", NULL,&HandoverRecord );
  if (status != NDEF_OK) goto error;
  
  status = NDEF_WriteHandover(&HandoverRecord, NDEF_Buffer);
  if (status != NDEF_OK) goto error;

  Menu_MsgStatus("Store Handover","Handover has been written!",MSG_STATUS_SUCCESS);

  return;
  error:
    Menu_MsgStatus("Store Handover","Handover has not been written!",MSG_STATUS_ERROR);
    return;

}
*/


/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/


