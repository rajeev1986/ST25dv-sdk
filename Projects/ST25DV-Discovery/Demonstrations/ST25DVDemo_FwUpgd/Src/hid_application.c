/*******************************************************************************
*
* THE PRESENT CODE AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* (C) 2014 STMicroelectronics International NV
*
*   FILENAME        -  hid_application.c
*
*******************************************************************************
*
*   CHANGE HISTORY
*  ---------------------------
*
*   $Date$:      01/09/2014
*   $Revision$:  first version
*   $Author$:    AMS RF Application team
*   Comments:    Profiles PTS test application using the BlueNRG Host Profiles 
*                Library 
*
*******************************************************************************
*
*  File Description 
*  ---------------------
* 
*******************************************************************************/

/*******************************************************************************
 * Include Files
*******************************************************************************/
/* new from BlueNRG DK framework */
//#include "stm32l1xx.h"
//#include "usb_lib.h"
//#include "hw_config.h"
#include "hal_types.h"
#include "hci.h"
#include "bluenrg_aci.h"
#include "gp_timer.h"
#include "hal.h"
#include "osal.h"
#include "bluenrg_gatt_server.h"
#include "hci_const.h"
#include "bluenrg_gap.h"
#include "sm.h"
#include <stdio.h>

//#include "SDK_EVAL_Config.h"

#include <host_config.h>
#if (PLATFORM_WINDOWS == 1)
#include <Windows.h>
#endif
#include <stdio.h>
#include <hal_types.h>
#include <ble_status.h>
#include <debug.h>
#include <hci.h>
#include <ble_profile.h>
#include <ble_events.h>
//#include <timer.h> 
//#include <platform_config.h> 
#include <hid_device.h>
#include "Menu_core.h"
#include "Menu_config.h"

  
/*******************************************************************************
 * Macros
*******************************************************************************/
/* IO capabilities */
#define SPI_HOST_MAIN_IO_CAPABILITY_DISPLAY_ONLY              (0x00)
#define SPI_HOST_MAIN_IO_CAPABILITY_DISPLAY_YES_NO            (0x01)
#define SPI_HOST_MAIN_IO_CAPABILITY_KEYBOARD_ONLY             (0x02)
#define SPI_HOST_MAIN_IO_CAPABILITY_NO_INPUT_NO_OUTPUT        (0x03)
#define SPI_HOST_MAIN_IO_CAPABILITY_KEYBOARD_DISPLAY          (0x04)

/* MITM modes */
#define MITM_PROTECTION_NOT_REQUIRED	(0x00)
#define MITM_PROTECTION_REQUIRED        (0x01)

/* application states */
#define APPL_UNINITIALIZED		(0x00)
#define APPL_INIT_DONE			(0x01)
#define APPL_ADVERTISING		(0x02)
#define APPL_CONNECTED			(0x03)
#define APPL_READ			(0x04)
#define APPL_WAIT			(0x05)
#define APPL_START_TIMER	        (0x06)
#define APPL_UPDATE			(0x07)

#define APPL_TERMINATE_LINK             '9' 


// User Inputs For HID Device
#define APPL_UPDATE_INPUT_REPORT                'd' 
#define APPL_UPDATE_BOOT_KEYBOARD_IP_REPORT     'e' 
#define APPL_UPDATE_BOOT_MOUSE_IP_REPORT	'f' 
#define APPL_UPDATE_BATTERY_LEVEL		'g' 
#define APPL_UPDATE_SCAN_REFRESH_CHARAC 	'h' 
#define APPL_UPDATE_FEATURE_REPORT 		'i' 


#define APPL_SEND_SLAVE_SECURITY_REQUEST        '|' 

//Macro for advertising the Profiles
#define APPL_START_DISCOVERY		        '+'  

#define REPORT_DESC_SIZE                        (52)


/******************************************************************************
 * Local Variable Declarations
******************************************************************************/
static uint8_t deviceState = APPL_UNINITIALIZED;

static tApplDataForHidServ appHidServData[HID_SERVICE_MAX];

static tSecurityParams Security_Param;
static uint8_t battRead = 0;
static uint8_t battIdx;

/******************************************************************************
 * Global Variable Declarations
******************************************************************************/
void *profiledbgfile;
uint8_t is_connected = 0;
extern uint8_t hidDevicePeerAddr[6];
/******************************************************************************
 * Function Declarations
******************************************************************************/
/******************************************************************************
 * Function Definitions 
******************************************************************************/
/**
  * @addtogroup NDEF_DEMO
  * @{
  */

/**
 * @brief Bluetooth LE application callback.
 * @param event
 * @param evtLen
 * @param evtData
 */
static void BLE_Profiles_Evt_Notify_Cb(tNotificationEvent event,uint8_t evtLen,uint8_t* evtData)
{
  //uint8_t idx;
  static uint8_t power_lvl = 99;
	
  switch(event)
  {
    case EVT_MP_BLUE_INITIALIZED:
    {
      PRINTF(profiledbgfile,"EVT_BLUE_INITIALIZED\n");
    }
    break;
    case EVT_MP_ADVERTISING_TIMEOUT:
    {
      PRINTF(profiledbgfile,"EVT_MP_ADVERTISING_TIMEOUT\n");
    }
    break;
    case EVT_MP_CONNECTION_COMPLETE:
    {
      PRINTF(profiledbgfile,"EVT_MP_CONNECTION_COMPLETE\n");
      deviceState = APPL_CONNECTED;//uncommented for the time test case csp_bv_02_c(cts).
      is_connected = 1;   
      char msg[50];
      sprintf(msg,"BlueNRG connected with device:\n%02X:%02X:%02X:%02X:%02X:%02X",
                hidDevicePeerAddr[0],
                hidDevicePeerAddr[1],
                hidDevicePeerAddr[2],
                hidDevicePeerAddr[3],
                hidDevicePeerAddr[4],
                hidDevicePeerAddr[5]          );
      Menu_MsgStatus("BlueNRG connection",msg,MSG_INFO_SUCCESS);
      Menu_DisplayCenterString(10, "Touch here to exit");
    }
    break;
    case EVT_MP_DISCONNECTION_COMPLETE:
    {
      PRINTF(profiledbgfile,"EVT_MP_DISCONNECTION_COMPLETE %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_MP_PASSKEY_REQUEST:
    {
      PRINTF(profiledbgfile,"EVT_MP_PASSKEY_REQUEST\n");
      BLE_Profile_Send_Pass_Key(111111);
    }
    break;
    case EVT_MP_PAIRING_COMPLETE:
    {
      PRINTF(profiledbgfile,"EVT_MP_PAIRING_COMPLETE\n");
      deviceState = APPL_CONNECTED;
      is_connected = 1;          
    }
    break;
    case EVT_HID_INITIALIZED:
    {
      PRINTF(profiledbgfile,"EVT_HID_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_BATT_LEVEL_READ_REQ:
    {
      PRINTF(profiledbgfile,"EVT_BATT_LEVEL_READ_REQ\n");
      /* BAS service test: TC_CR_BV_01_C  */
      battRead = 0x01;
      battIdx = evtData[0];
      HidDevice_Update_Battery_Level(battIdx,power_lvl);
      char tmp[20];
      sprintf(tmp,"Power level: %02X",power_lvl--);
      Menu_SetStyle(PLAIN);
      Menu_DisplayCenterString(8, tmp);
      if(power_lvl == 0)
        power_lvl = 99;
    }
    break;
    case EVT_HID_UPDATE_CMPLT:
    {
      PRINTF(profiledbgfile,"EVT_HID_UPDATE_CMPLT %x %x %x %x\n",evtData[0],evtData[1],evtData[2],evtData[3]);
      deviceState = APPL_CONNECTED;
      if(battRead)
      {
        PRINTF(profiledbgfile,"allow read\n");
        Allow_BatteryLevel_Char_Read(battIdx);
        battRead = 0x00;
      }
      is_connected = 1;          
    }
    break;

  }/* end switch() */
}/* end BLE_Profiles_Evt_Notify_Cb() */


/**
  * @brief Initializes the HID bleutooth LE profile.
  */
static void Init_Profile()
{
  uint8_t ipRepLen[HID_SERVICE_MAX] = {8,8,8,8,8};
  uint8_t opRepLen[HID_SERVICE_MAX];
  uint8_t featureRepLen[HID_SERVICE_MAX];
  uint8_t ipRepIDs[HID_SERVICE_MAX] = {0,1,2,3,4};
  uint8_t opRepIDs[HID_SERVICE_MAX];
  uint8_t featureRepIDs[HID_SERVICE_MAX];
  
  /* report Map characteristic */
  uint8_t report_descriptor[REPORT_DESC_SIZE] =
  {
    0x05, 0x01,   /* Usage Page (Generic Desktop)*/
    0x09, 0x02,   /* Usage (Mouse) */
    0xA1, 0x01,   /* Collection (Application) */
    0x09, 0x01,   /* Usage (Pointer) */
    0xA1, 0x00,   /* Collection (Physical) */
    0x05, 0x09,   /* Usage Page (Buttons) */
    0x19, 0x01,   /* Usage Minimun (01) */
    0x29, 0x03,   /* Usage Maximum (03) */
    0x15, 0x00,   /* logical Minimun (0) */
    0x25, 0x01,   /* logical Maximum (1) */
    0x95, 0x03,   /* Report Count (3) */
    0x75, 0x01,   /* Report Size (1) */
    0x81, 0x02,   /* Input(Data, Variable, Absolute) 3 button bits */
    0x95, 0x01,   /* Report count (1) */
    0x75, 0x05,   /* Report Size (5) */
    0x81, 0x01,   /* Input (Constant), 5 bit padding */
    0x05, 0x01,   /* Usage Page (Generic Desktop) */
    0x09, 0x30,   /* Usage (X) */
    0x09, 0x31,   /* Usage (Y) */
    0x09, 0x38,   /* Usage (Z) */
    0x15, 0x81,   /* Logical Minimum (-127) */
    0x25, 0x7F,   /* Logical Maximum (127) */
    0x75, 0x08,   /* Report Size (8) */
    0x95, 0x03,   /* Report Count (2) */
    0x81, 0x06,   /* Input(Data, Variable, Relative), 2 position bytes (X & Y)*/
    0xC0,         /* end collection */
    0xC0          /* end collection */
  };
  tnonHIDService nonHIDService ;

  Osal_MemSet ( appHidServData, 0, (sizeof(tApplDataForHidServ)*HID_SERVICE_MAX) );

  appHidServData[0].inputReportNum = 1;
  appHidServData[0].ipReportLen = ipRepLen;
  appHidServData[0].ipReportID = ipRepIDs;

  appHidServData[0].outputReportNum = 0;
  appHidServData[0].opReportLen = opRepLen;
  appHidServData[0].opReportID = opRepIDs;
    
  appHidServData[0].featureReportNum = 0;
  appHidServData[0].ftrReportLen = featureRepLen;
  appHidServData[0].ftrReportID = featureRepIDs;

  appHidServData[0].reportMapValueLen = REPORT_DESC_SIZE;
  appHidServData[0].reportDesc = report_descriptor;
    
  /*Default settings:  appHidServData[0].extReportCharRefNum = 0;
     NOTE: Set to 1 for HIDS tests: TP/DES/BV-03-C, TP/DR/BV-03-C */
  appHidServData[0].extReportCharRefNum = 0; 
  
  nonHIDService.servUUID = 0xA00A;
  nonHIDService.charUUID = 0xA00B;
  nonHIDService.charValLen = 0x06;
  nonHIDService.isVariable = 0x00;
  appHidServData[0].nonHidServInfo = &nonHIDService;
  /* Default settings: BOOT_DEVICE_MOUSE_MASK. 
     Use BOOT_DEVICE_KEYBOARD_MASK for all PTS HIDS tests requesting boot 
     keboard input and outpur report characteristics */
  appHidServData[0].bootModeDeviceType = BOOT_DEVICE_MOUSE_MASK; 
  
  appHidServData[0].bootIpReportLenMax = 10;
  appHidServData[0].bootOpReportLenMax = 16;
    
  /*
  appHidServData[1].inputReportNum = 1;
  appHidServData[1].ipReportLen = ipRepLen;
  appHidServData[1].ipReportID = ipRepIDs;

  appHidServData[1].outputReportNum = 0;
  appHidServData[1].opReportLen = opRepLen;
  appHidServData[1].opReportID = opRepIDs;
    
  appHidServData[1].featureReportNum = 1;
  appHidServData[1].ftrReportLen = featureRepLen;
  appHidServData[1].ftrReportID = featureRepIDs;

  appHidServData[1].reportMapValueLen = 2;
  appHidServData[1].reportDesc = report_descriptor;
    
  appHidServData[1].extReportCharRefNum = 1;
  nonHIDService.servUUID = 0xA00A;
  nonHIDService.charUUID = 0xA00B;
  nonHIDService.charValLen = 0x06;
  nonHIDService.isVariable = 0x00;
  appHidServData[1].nonHidServInfo = &nonHIDService;
    
  appHidServData[1].bootModeDeviceType = BOOT_DEVICE_MOUSE_MASK;
  appHidServData[1].bootIpReportLenMax = 10;
  appHidServData[1].bootOpReportLenMax = 16;
  APPL_MESG_DBG(profiledbgfile,"Initializing HID Device Profile \n" );*/

  if (HidDevice_Init (1, /* numOfHIDServices */
                      appHidServData, 
                      1, /* numOfBatteryServices: 1 is default settings. Set to 2 for BAS service test TC_DES_BV_01_C */
                      (uint16_t)PNP_ID_CHAR_MASK, 
                      0x01,
                      0x01, 
                      BLE_Profiles_Evt_Notify_Cb) == BLE_STATUS_SUCCESS)
  {
    PRINTF(profiledbgfile,"Initialized HID Device Profile \n" );
  }
  else
  {
    PRINTF(profiledbgfile,"Failed HID Device Profile \n" );
  }

}/* end Init_Profile() */

/**
  * @brief Advertizes the HID bluetooth LE profile.
  */
static void Advertize()
{
  uint8_t useBoundedDeviceList = 0;
  HidDevice_Make_Discoverable(useBoundedDeviceList);

}/* end Advertize() */




/**
  * @brief Starts the HID application, using touchscreen detection to send mouse input reports and user button as mouse button.
  */
void Hid_Profile_Application (void)
{
  int indx;

  uint8_t stop = 0;
  /* input state */
  Menu_Position_t pos;
  uint8_t button_state, ts_state;
  uint8_t previous_button_state = 0;
  is_connected = 0;

  /* initialize the security parameters */
//  Security_Param.ioCapability = SPI_HOST_MAIN_IO_CAPABILITY_DISPLAY_ONLY;
  Security_Param.ioCapability = SPI_HOST_MAIN_IO_CAPABILITY_NO_INPUT_NO_OUTPUT;
  Security_Param.mitm_mode = MITM_PROTECTION_NOT_REQUIRED;
  Security_Param.bonding_mode = 0x00;
  Security_Param.OOB_Data_Present = 0x00;
  Security_Param.Use_Fixed_Pin = 0x00;
  Security_Param.encryptionKeySizeMin = 8;
  Security_Param.encryptionKeySizeMax = 16;
  Security_Param.Fixed_Pin = 111111;
  Security_Param.initiateSecurity = NO_SECURITY_REQUIRED;
  for (indx=0;indx<16;indx++)
  {
    Security_Param.OOB_Data[indx] = (uint8_t)indx;
  }

  /* initialize profile */
  extern uint8_t bleProfileIintFlag;
  bleProfileIintFlag = 0;

  BLE_Profile_Init(&Security_Param,BLE_Profiles_Evt_Notify_Cb);

  /* initialize the individual profiles */
  Init_Profile();
	

  deviceState = APPL_UNINITIALIZED;
  while(stop == 0)
  {
    HCI_Process();
    HIDProfile_StateMachine();

  ts_state = Menu_ReadPosition(&pos);
  Menu_ReadSelection(&button_state);
    
  /* Can stop the demo by any input before the connection is done. */
  if((button_state || ts_state) && (deviceState < APPL_CONNECTED))
  {
    stop = 1;
  }


    if(Profile_Process_Q() == 0x00)
    {
      /* if the profiles have nothing more to process, then
       * wait for application input
       */ 

      
      if ((deviceState < APPL_CONNECTED) && (is_connected))
      {
        /* Some smartphone (SamsungJ7) can disconnect if the HID is not allowed to take control. */
        is_connected = 0;
        Menu_MsgStatus("Disconnected...", "Check if HID control is enabled on the smartphone.",MSG_INFO);
      }
      
      if(deviceState >= APPL_CONNECTED)
      {
        
        uint8_t input = 200;


     
        if(ts_state)
        {
          /* Last line of the LCD is used for exiting the demo. */
          if( pos.Y > (Menu_GetDisplayHeight() - 20)) {
            stop = 1;            
          } else {
            /* other input control the pointer. */
            input = APPL_UPDATE_INPUT_REPORT;            
          }
        }

        /* The user button sends both a report when pushed and when released. */
        if((button_state != 0) || (previous_button_state != 0)) 
        {
          input = APPL_UPDATE_INPUT_REPORT; 
          Menu_Delay(200);
        }
        previous_button_state = button_state;

        deviceState = input;
      }/* end if(deviceState >= APPL_CONNECTED) */

    }/* end if(Profile_Process_Q() == 0x00) */
                
    /* application specific processing */

    switch(deviceState)
    {
      case APPL_INIT_DONE:
      {
        Advertize();
        deviceState = APPL_ADVERTISING;
      }
      break;
      case APPL_TERMINATE_LINK:
      {
        /* on disconnection complete, the state will be changed to INIT_DONE
         */ 
        PRINTF(profiledbgfile,"Call BLE_Profile_Disconnect\n"); 
        BLE_Profile_Disconnect();
        deviceState = APPL_UNINITIALIZED;
      }
      break;
      case APPL_UPDATE_INPUT_REPORT:	
      {
        uint8_t ipRepVal[10] = {0,0,0,0,0,0,0,0};        
        //uint8_t ipRepVa11[10] = {1,2,3,4,5,6,7,9};
        ipRepVal[0] = button_state;
        if(ts_state)
        {
          /* Be careful here: pos.X & Y are not necessarily 8 bits. */
          int8_t X = ((int)pos.X - (int)(Menu_GetDisplayWidth()/2))/25;
          int8_t Y = ((int)pos.Y - (int)(Menu_GetDisplayHeight()/2))/25;
          ipRepVal[1] = X;
          ipRepVal[2] = Y;
        }
        HidDevice_Update_Input_Report(0,0,8,ipRepVal);
        
      }
      break;		 
      case APPL_UPDATE_BOOT_KEYBOARD_IP_REPORT:
      {
        uint8_t ipRepVal[10] = {1,2,3,4,5,6,7,8,9,10}; 
        //uint8_t ipRepVal1[10] = {1,2,3,4,5,6,7,8,9,11};
        HidDevice_Update_Boot_Keyboard_Input_Report(0,10,ipRepVal);

              
      }
      break;
      case APPL_UPDATE_BOOT_MOUSE_IP_REPORT:	 
      {
        uint8_t mouseRepVal[10] = {1,2,3,4,5,6,7,8,0,0}; 
        HidDevice_Update_Boot_Mouse_Input_Report(0,10,mouseRepVal);	
              
      }
      break;
      case APPL_UPDATE_BATTERY_LEVEL:
      {
        PRINTF(profiledbgfile,"HID_Device: APPL_UPDATE_BATTERY_LEVEL state (update battery level)\n");
        HidDevice_Update_Battery_Level(battIdx,98);

      }		
      break;
      case APPL_UPDATE_SCAN_REFRESH_CHARAC: 	
      {
        HidDevice_Update_Scan_Refresh_Char(0x01);
        //deviceState = APPL_WAIT;				
      } 
      break;
      case APPL_UPDATE_FEATURE_REPORT:
      {
        uint8_t featRepVal[8] = {8,7,6,5,4,3,2,1};
        HidDevice_Update_Feature_Report(0,0,8,featRepVal);
      }
      break;

      case APPL_SEND_SLAVE_SECURITY_REQUEST:
      { 
        PRINTF(profiledbgfile,"profiles_test_application: call BLE_Profile_Send_Security_Request() \n");
        BLE_Profile_Send_Security_Request();
      }
      break;   
                
    }/* end switch(devicestate) */

  } /* end while(1) */
          BLE_Profile_Disconnect();
        deviceState = APPL_UNINITIALIZED;

}/* end Host_Profile_Test_Application() */

/**
  * @}
  */ 
