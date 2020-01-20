/**
  ******************************************************************************
  * @file    Menu_definition.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @ingroup ST25DV_Menu_Definition
  * @brief   This file defines the content of the menu for the ST25DV demo.
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

#include "main.h"
#include "Menu_core.h"
#include "Menu_config.h"
#include "Menu_definition.h"
#include "mailbox.h"

void ReturnFunc(void);

// Dummy definition of demo functions
void IdleFunc(void){};
#include "st25dv_features_demo.h"
#include "ndef_demo.h"  

/* icon main menu */
#include "st25dv_100x100_jpg.h"
#include "ftm_100x100_jpg.h"
#include "nmark_100x100_jpg.h"

/* icon file for menu */
#include "url_64x64_jpg.h"
#include "sms_64x64_jpg.h"
#include "email_64x64_jpg.h"
#include "vcard_64x64_jpg.h"
#include "ble_64x64_jpg.h"
#include "geo_64x64_jpg.h"
#include "myapp_64x64_jpg.h"
#include "rfmgt_64x64_jpg.h"
#include "rfgpo_64x64_jpg.h"
#include "mapping_64x64_jpg.h"
#include "about_64x64_jpg.h"
#include "home_64x64_jpg.h"
#include "wifi_64x64_jpg.h"
#include "multirecord_64x64_jpg.h"
#include "eh_64x64_jpg.h"
#include "trash_64x64_jpg.h"

/* Bluetooth BLE includes */
#include "st25dx_bluenrg_ble.h"
#include "bluenrg_aci.h"
#include "hci_le.h"

/* Wifi includes */
#include "ring_buffer.h"
#include "wifi_module.h"

extern WiFi_WIND_State_TypeDef WiFi_WIND_State;
extern TIM_HandleTypeDef    TimHandle;
extern UART_HandleTypeDef UartWiFiHandle;
extern UART_HandleTypeDef hbus_usart3;

/** @defgroup ST25DV_Menu_Definition ST25DV Menu Definition
  * @ingroup  ST25_Discovery_Demo
  * @brief    This module defines the structure and content of the ST25DV demo menu.
  * @details  Menu structure is statically defined in the module, and complies with the expected structure of the menu_demo middleware.
  *           Call Menu_Start() to start the menu main loop.
  * @{
  */
  
#undef LIGHT_MENU
#ifndef LIGHT_MENU
/** @brief   List NDEF icon address, null if nothing to display
  * @details The order in the list defines the position of the icons in the NDEF menu.
  */
static const char * NdefMenuIcons[] = {cUrl,
                                cSms,
                                cEmail,
                                cVcard,
                                cGeo,
                                cMyApp,
                                cMultiRecord,
                                cTrash,
                                cBle,
                                cWifi,
                                NULL,cHome}; 

                                
/** @brief   List ST25DV features icon address, null if nothing to display
  * @details The order in the list defines the position of the icons in the ST25DV menu.
  */
static const char * ST25DVMenuIcons[] = { cRfGpo, cEh, cRfMgt, cMapping, NULL, NULL, NULL, NULL, NULL, NULL, cAbout, cHome };


/** @brief   List main menu icon address, null if nothing to display
  * @details The order in the list defines the position of the icons in the Main menu.
  */
static const char* MainMenuIcons[] = { st25dv_100x100 , ftm_100x100, nmark_100x100 };

#else  /* LIGHT_MENU */

static const char * NdefMenuIcons[] = { cUrl, cUrl, cUrl, cUrl,
                              cUrl, cUrl, cUrl, cUrl, NULL, NULL, NULL, cHome }; 

                              static const char * ST25DVMenuIcons[] = { cUrl,
                              cUrl, cUrl, cUrl, cUrl, NULL, NULL, NULL, NULL, NULL, NULL, cHome }; 

static const char* MainMenuIcons[] = { cUrl , cUrl, cUrl };
#endif  /* LIGHT_MENU */


/*------------------------------ Menu level 2 -------------------------------*/
/** Defines NDEF URI menu items. */
static struct sMenuItem URIMenuItems[] = {{"Store URL", NDEF_DEMO_Write_URI_URL, IdleFunc,NULL},
                                          {"Store Phone Number", NDEF_DEMO_Write_URI_Tel, IdleFunc,NULL},
                                          {"Read URI from ST25DV", NDEF_DEMO_Read_URI, IdleFunc,NULL},
                                          {"Return", ReturnFunc, IdleFunc,NULL}};
/** Defines NDEF URI menu. */
static struct sMenu URIMenu = {"URI", URIMenuItems, countof(URIMenuItems),NULL};


/** Defines NDEF SMS menu items. */
static struct sMenuItem SMSMenuItems[] = {{"Store SMS in ST25DV", NDEF_DEMO_Write_SMS, IdleFunc,NULL},
                                          {"Read SMS from ST25DV", NDEF_DEMO_Read_SMS, IdleFunc,NULL},
                                          {"Return", ReturnFunc, IdleFunc,NULL}};
/** Defines NDEF SMS menu. */
static struct sMenu SMSMenu = {"SMS", SMSMenuItems, countof(SMSMenuItems),NULL};


/** Defines NDEF Email menu items. */
static struct sMenuItem EmailMenuItems[] = {{"Store Email", NDEF_DEMO_Write_Email, IdleFunc,NULL},
                                           {"Read Email", NDEF_DEMO_Read_Email, IdleFunc,NULL},
                                            {"Return", ReturnFunc, IdleFunc,NULL}};
/** Defines NDEF Email menu. */
static struct sMenu EmailMenu = {"Email", EmailMenuItems, countof(EmailMenuItems),NULL};


/** Defines NDEF vCard menu items. */
static struct sMenuItem VcardMenuItems[] = {{"Store Vcard", NDEF_DEMO_Write_Vcard, IdleFunc,NULL},
                                            {"Read Vcard", NDEF_DEMO_Read_Vcard, IdleFunc,NULL},
                                            {"Return", ReturnFunc, IdleFunc,NULL}};
/** Defines NDEF vCard menu. */
static struct sMenu VcardMenu = {"Vcard", VcardMenuItems, countof(VcardMenuItems),NULL};


/** Defines NDEF Bluetooth menu items. */
static struct sMenuItem BTMenuItems[] = {{"Store BLE info", NDEF_DEMO_Write_BLE_OOB, IdleFunc,NULL},
                                         {"Read BLE info", NDEF_DEMO_Read_Bluetooth_OOB, IdleFunc,NULL},
                                         {"Change BLE Address", NDEF_DEMO_BLE_ChangeDeviceAddr, IdleFunc,NULL},
                                         {"Return", ReturnFunc, IdleFunc,NULL}};
/** Defines NDEF Bluetooth menu. */
static struct sMenu BTMenu = {"BT pairing", BTMenuItems, countof(BTMenuItems),NULL};

/** Defines NDEF Wifi menu items. */
static struct sMenuItem WifiMenuItems[] = {{"Write Wifi OOB", NDEF_DEMO_Write_Wifi_OOB, IdleFunc,NULL},
//                                        {"Store Handover", WriteHandoverSelect, IdleFunc,NULL},
//                                        {"Read Handover", ReadHandover, IdleFunc,NULL},
                                          {"Read Wifi OOB", NDEF_DEMO_Read_Wifi_OOB, IdleFunc,NULL},
                                          {"Return", ReturnFunc, IdleFunc,NULL}};
/** Defines NDEF Wifi menu. */
static struct sMenu WifiMenu = {"Wifi pairing", WifiMenuItems, countof(WifiMenuItems),NULL};


/** Defines NDEF Geolocation menu items. */
static struct sMenuItem GeoMenuItems[] = {{"Store Pos in ST25DV", NDEF_DEMO_Write_Geo, IdleFunc,NULL},
                                          {"Read Pos from ST25DV", NDEF_DEMO_Read_Geo, IdleFunc,NULL},
                                          {"Return", ReturnFunc, IdleFunc,NULL}};

                                          /** Defines NDEF Geolocation menu. */
static struct sMenu GeoMenu = {"Geolocation", GeoMenuItems, countof(GeoMenuItems),NULL};

/** Defines NDEF Multi-record menu items. */
static struct sMenuItem MultiRecordMenuItems[] = {{"Store ST URL", NDEF_DEMO_Write_URI_URL, IdleFunc,NULL},
                                                  {"Add ST AAR", NDEF_DEMO_MultiRecord_With_AAR, IdleFunc,NULL},
                                                  {"Return", ReturnFunc, IdleFunc,NULL}};
/** Defines NDEF Multi-record menu. */
static struct sMenu MultiRecordMenu = {"Multi-Record", MultiRecordMenuItems, countof(MultiRecordMenuItems),NULL};

/** Defines ST25DV states menu items. */
static struct sMenuItem ST25DVStatesMenuItems[] = {{"RF Disable", ST25DV_DEMO_RF_Off, IdleFunc,NULL},
                                                   {"RF Sleep", ST25DV_DEMO_RF_Sleep, IdleFunc,NULL},
                                                   {"Low power down", ST25DV_DEMO_Low_Power_Down, IdleFunc,NULL},
                                                   {"       Return       ", ReturnFunc, IdleFunc,NULL}};
/** Defines ST25DV states menu. */
static struct sMenu ST25DVStatesMenu = {"ST25DV states", ST25DVStatesMenuItems, countof(ST25DVStatesMenuItems),NULL};

/** Defines Clear content. */
static struct sMenuItem ClearMenuItems[] = {{"Write empty NDEF", NDEF_DEMO_Write_empty_NDEF, IdleFunc, NULL},
                                          {"Erase CCFile", NDEF_DEMO_Erase_CCFile, IdleFunc, NULL},
                                          {"Clear EEPROM", NDEF_DEMO_Clear_Eeprom, IdleFunc, NULL},
                                          {"Return", ReturnFunc, IdleFunc,NULL}};
/** Defines NDEF Wifi menu. */
static struct sMenu ClearMenu = {"Clear Memory", ClearMenuItems, countof(ClearMenuItems),NULL};


/*------------------------------ Menu level 1 -------------------------------*/
/** Defines NDEF main menu items. */
static struct sMenuItem NdefMenuItems[] = {
  {"URI", IdleFunc, IdleFunc, &URIMenu},
  {"SMS", IdleFunc, IdleFunc, &SMSMenu},
  {"Email", IdleFunc, IdleFunc, &EmailMenu},
  {"Vcard", IdleFunc, IdleFunc, &VcardMenu},
  {"Geolocation", IdleFunc, IdleFunc, &GeoMenu},
  {"My Private App", NDEF_DEMO_Read_MyAPP, IdleFunc, NULL},
  {"Multi-Record", IdleFunc, IdleFunc, &MultiRecordMenu},
  {"Clear EEPROM", IdleFunc, IdleFunc, &ClearMenu},
  {"BT pairing", IdleFunc, IdleFunc, &BTMenu},
  {"Wifi pairing", IdleFunc, IdleFunc, &WifiMenu},
//  {NULL, IdleFunc, IdleFunc, NULL},
  {NULL, IdleFunc, IdleFunc, NULL},
  {"Return to main menu", &ReturnFunc, IdleFunc, NULL}};
/** Defines NDEF main menu. */
static struct sMenu NdefMenu = {"NDEF menu", NdefMenuItems, countof(NdefMenuItems), NdefMenuIcons};

/** Defines ST25DV features main menu items. */
static struct sMenuItem ST25DVMenuItems[] = {
  {"RF GPO interrupts", &ST25DV_DEMO_GPO, IdleFunc, NULL},
  {"Energy Harvesting", &ST25DV_DEMO_EnergyHarvesting, IdleFunc, NULL},
  {"ST25DV states", IdleFunc, IdleFunc, &ST25DVStatesMenu},
  {"MultiArea Password", &ST25DV_DEMO_Memory_Mapping_Password, IdleFunc, NULL},
  {NULL, IdleFunc, IdleFunc, NULL},
  {NULL, IdleFunc, IdleFunc, NULL},
  {NULL, IdleFunc, IdleFunc, NULL},
  {NULL, IdleFunc, IdleFunc, NULL},
  {NULL, IdleFunc, IdleFunc, NULL},
  {NULL, IdleFunc, IdleFunc, NULL},
  {"About", &MenuAbout, IdleFunc, NULL},
  {"Return to main menu", &ReturnFunc, IdleFunc, NULL}};
/** Defines ST25DV features main menu. */
static struct sMenu ST25Menu = {"ST25DV features", ST25DVMenuItems, countof(ST25DVMenuItems), ST25DVMenuIcons};


/** Defines ST25DV demo main menu items. */
static struct sMenuItem MainMenuItems[] = {
  {"Discover ST25DV", IdleFunc, IdleFunc, &ST25Menu},
  {"Fast Transfer Demo", &FTMManagement, IdleFunc, NULL},
  {"NFC NDEF Demos", &NDEF_DEMO_Init_Tag, IdleFunc, &NdefMenu}};

/** Defines ST25DV demo main menu. */
static struct sMenu MainMenu = {"Main menu", MainMenuItems, countof(MainMenuItems), MainMenuIcons};


/** @brief Check response from the BlueNRG module to check if current board has the BLE module. */
static int BlePresent(void)
{
  int ret;  

  uint8_t hci_version, lmp_pal_version;
  uint16_t hci_revision, manufacturer_name, lmp_pal_subversion;

  /* Initialize the BlueNRG SPI driver */
  BNRG_SPI_Init();
  
  /* Initialize the BlueNRG HCI */
  HCI_Init();

  /* Reset BlueNRG hardware */
  BlueNRG_RST();
    
  /* get the BlueNRG HW and FW versions */
  ret = hci_le_read_local_version(&hci_version, &hci_revision, &lmp_pal_version, 
             &manufacturer_name, &lmp_pal_subversion);
  
  /* 
   * Reset BlueNRG again otherwise we won't
   * be able to change its MAC address.
   * aci_hal_write_config_data() must be the first
   * command after reset otherwise it will fail.
   */
  BlueNRG_RST();
  return (ret == 0);
}

/** @brief Check response from the Wifi module to check if current board has the Wifi module. */
static int WifiPresent(void)
{

  /* configure the timers */
  Timer_Config( );
  
  UART_Configuration(BUS_USART3_BAUDRATE); 
  
  WiFi_Module_Init();

  wifi_wakeup(WIFI_TRUE);//Prevent from going to sleep during configuration    

  /* Soft reset the module */
  wifi_reset();

  /* Disable timer IT, was blocking after the reset for FW upgrade */
  HAL_TIM_Base_Stop_IT(&TimHandle);

  return (WiFi_WIND_State.WiFiHWStarted == WIFI_TRUE);
  
}

/** @brief Starts the main loop for the demo menu. */
void Menu_Start(void)
{
  ST25DV_UID uid;
  uint8_t ProductRef;
  
  if(!BlePresent())
  {
    NdefMenuItems[8].pszTitle = NULL;
    NdefMenuItems[8].psSubMenu = NULL;
    NdefMenuIcons[8] = NULL;
  }
  
  if(!WifiPresent())
  {
    NdefMenuItems[9].pszTitle = NULL;
    NdefMenuItems[9].psSubMenu = NULL;
    NdefMenuIcons[9] = NULL;    
  }
    
  /* Check the GPO technology to identify the package, and disable LPD demo for 8 pins ST25DV */
  ST25_RETRY( BSP_NFCTAG_ReadUID(BSP_NFCTAG_INSTANCE, &uid) );
  ProductRef = (uid.MsbUid & 0xFF00) >> 8;
  if( ST25DV_AM_I_OPEN_DRAIN(ProductRef))
  {
    /* GPO Open drain => 8 pins package, so remove the LPD demo */
    memcpy(&ST25DVStatesMenuItems[2],&ST25DVStatesMenuItems[3],sizeof(struct sMenuItem));
    ST25DVStatesMenu.nItems--;
  }
  
  Menu_MainLoop(&MainMenu);
}

/**
  * @}
  */ 
/**
  * @}
  */ 
/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
