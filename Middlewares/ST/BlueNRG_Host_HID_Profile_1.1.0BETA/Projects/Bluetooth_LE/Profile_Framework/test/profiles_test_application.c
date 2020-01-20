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
*   FILENAME        -  profile_test_application.c
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
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
#include <heart_rate.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
#include <proximity_reporter.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
#include <proximity_monitor.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
#include <time_server.h>
#include <time_profile_types.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
#include <time_client.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HID_DEVICE)
#include <hid_device.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
#include <findme_target.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
#include <findme_locator.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
#include <phone_alert_client.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
#include <blood_pressure.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
#include <health_thermometer.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
#include <alertNotification_Client.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
#include <alertNotification_Server.h>
#endif

#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
#include <glucose_service.h>
#include <glucose_sensor.h>
#include <glucose_racp.h>
/* Include record measurement database record */
#include <glucose_database.h>
#endif /* end #if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)*/

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
//User Inputs For Heart Rate
#define APPL_SEND_HEART_RATE_MEASUREMENT        '!' 
#define APPL_SET_BODY_SENSOR_LOCATION           '@' 
// User Inputs For Blood Pressure 
#define APPL_SEND_BP_MEASUREMENT                '#'  
#define APPL_SEND_INTERMEDIATE_CUFF_PRESSURE    '$'  
// User Inputs For Health Thermometer
#define APPL_SEND_HT_MEASUREMENT                '%'   
#define APPL_SEND_HT_INTERMEDIATE_MEASUREMENT   '^'  
#define APPL_UPDATE_MEASUREMENT_INTERVAL        '&'  
#define APPL_UPDATE_TEMPERATURE_TYPE            '*'  
// User Inputs For Time Server 
#define APPL_UPDATE_CURRENT_TIME                '(' 

/* It is used for PTS test TC_CSP_BV_06_C */
#define APPL_DECREASE_CURRENT_TIME              '<'      
/* It is used for PTS test TC_CSP_BV_06_C */
#define  APPL_UPDATE_CURRENT_TIME_MORE_THAN_15  '>'

#define APPL_UPDATE_LOCAL_TIME			')' 
#define APPL_UPDATE_REFERENCE_TIME		'a' 
#define APPL_UPDATE_NEXT_DSTCHANGE_TIME		'b' 
#define APPL_UPDATE_REFERENCE_TIME_INFO		'c'
// User Inputs For HID Device
#define APPL_UPDATE_INPUT_REPORT                'd' 
#define APPL_UPDATE_BOOT_KEYBOARD_IP_REPORT     'e' 
#define APPL_UPDATE_BOOT_MOUSE_IP_REPORT	'f' 
#define APPL_UPDATE_BATTERY_LEVEL		'g' 
#define APPL_UPDATE_SCAN_REFRESH_CHARAC 	'h' 
#define APPL_UPDATE_FEATURE_REPORT 		'i' 
// User Inputs For Alert Notification server
#define APPL_UPDATE_NEW_ALERT 			'j' 
#define APPL_UPDATE_UNREAD_ALERT_STATUS 	'k' 
// User Inputs For Alert Notifications Client
#define APPL_EN_DIS_NEW_ALERT_NOTIF        	'l' 
#define APPL_EN_DIS_UNREAD_ALERT_STATUS_NOTIF   'm' 
#define APPL_WRITE_CTRL_POINT 			'n' 
// User Inputs for Find me Locator
#define APPL_LOCATOR_ALERT_TARGET		'o' 
//User Inputs For Time Clients
#define APPL_GET_REF_TIME_UPDATE		'p' 
#define APPL_TIME_UPDATE_NOTIFICATION		'q' 
#define APPL_READ_NEXT_DST_CHANGE_TIME 		'r' 
#define APPL_READ_SERV_TIME_UPDATE_STATE	's' 
#define APPL_READ_LOCAL_TIME_INFORM		't' 
#define APPL_READ_CURRENT_TIME			'u' 
#define APPL_GET_SERV_TIME_UPDATE_STATE		'v' 
#define APPL_GET_REF_TIME_INFO_ON_SERVER	'w' 
#define APPL_CANCEL_REF_TIME_UPDATE		'x' 
#define APPL_GET_TIME_ACCU_INFO_SERVER		'y' 
// User Inputs For Phone Alert Client
#define APPL_PHONE_READ_ALERT_STATUS		'z' 
#define APPL_PHONE_READ_RINGER_SETTING		'[' 
#define APPL_DISABLE_ALERT_STATUS_NOTIF		']' 
#define APPL_DISABLE_RINGER_SETTING_NOTIF	';' 
#define APPL_CONFIGURE_RINGER_SILENCE		',' 
#define APPL_CONFIGURE_RINGER_MUTE		'.' 
#define APPL_CNFIGURE_RINGER_CANCEL_SILENCE	'/' 

// User Inputs For Glucose Sensor
#define APPL_GLUCOSE_CHANGE_DB_1	         '`' 
#define APPL_GLUCOSE_CHANGE_DB_2	         '~' 

#define APPL_SEND_SLAVE_SECURITY_REQUEST        '|' 

//Macro for advertising the Profiles
#define APPL_START_DISCOVERY		        '+'  

#define REPORT_DESC_SIZE                        (52)

#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
/* HT_TC_SP_BV_01_C to be enabled only for running PTS test 4.10.1: HT_TC_SP_BV_01_C */   
//#define HT_TC_SP_BV_01_C 1

#if HT_TC_SP_BV_01_C 
#define HT_CONNECTED_DO_MEASUREMENT_INTERVAL_WAIT       (0xA0)
#define HT_CONNECTED_DO_MEASUREMENT_INTERVAL_WAIT_STOP  (0xA1)
volatile uint8_t ht_check_next_send_temperature_measurement_flag = 0;
uint8_t ht_check_next_send_temperature_measurement_state = 0; 
#endif /* HT_TC_SP_BV_01_C */

#endif /* BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER */

/******************************************************************************
 * Local Variable Declarations
******************************************************************************/
uint8_t deviceState = APPL_UNINITIALIZED;

#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
tHeartRateMeasure heartRateMeasurmentValue;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & HID_DEVICE)
tApplDataForHidServ appHidServData[HID_SERVICE_MAX];
#endif

#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
tCurrentTime time;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
tTempMeasurementVal tempMeasurementVal;
#endif

static tSecurityParams Security_Param;
//static uint8_t ringer = 3;
uint32_t count = 0;
uint8_t battRead = 0;
uint8_t battIdx;

/******************************************************************************
 * Global Variable Declarations
******************************************************************************/
void *hostdbgfile;
void *profiledbgfile;
void *ftdidbgfile;
/******************************************************************************
 * Function Declarations
******************************************************************************/
uint8_t __io_getcharNonBlocking(uint8_t *data);
/******************************************************************************
 * Function Definitions 
******************************************************************************/

/**
 * @brief 
 * @param event
 * @param evtLen
 * @param evtData
 */
void BLE_Profiles_Evt_Notify_Cb(tNotificationEvent event,uint8_t evtLen,uint8_t* evtData)
{
  //uint8_t idx;
  
  switch(event)
  {
    case EVT_MP_BLUE_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_BLUE_INITIALIZED\n");
    }
    break;
    case EVT_MP_ADVERTISING_TIMEOUT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_MP_ADVERTISING_TIMEOUT\n");
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
      APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
      TimeClient_Make_Discoverable(0x00);
#endif
    }
    break;
    case EVT_MP_CONNECTION_COMPLETE:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_MP_CONNECTION_COMPLETE\n");
      deviceState = APPL_CONNECTED;//uncommented for the time test case csp_bv_02_c(cts).
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
      GPIO_HIGH(LD_PORT_1,LD_YELLOW);
      GPIO_HIGH(LD_PORT_2,LD_RED);
#endif
    }
    break;
    case EVT_MP_DISCONNECTION_COMPLETE:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_MP_DISCONNECTION_COMPLETE %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_MP_PASSKEY_REQUEST:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_MP_PASSKEY_REQUEST\n");
      BLE_Profile_Send_Pass_Key(111111);
    }
    break;
    case EVT_MP_PAIRING_COMPLETE:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_MP_PAIRING_COMPLETE\n");
      deviceState = APPL_CONNECTED;
    }
    break;
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
    case EVT_HRS_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HRS_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_HRS_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HRS_CHAR_UPDATE_CMPLT %x:%x:%x:%x:%x\n",evtData[0],evtData[1],evtData[2],evtData[3],evtData[4]);
    }
    break;
    case EVT_HRS_RESET_ENERGY_EXPENDED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HRS_RESET_ENERGY_EXPENDED\n");
    }
    break;
#endif		
#if (BLE_CURRENT_PROFILE_ROLES & HID_DEVICE)
    case EVT_HID_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HID_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_BATT_LEVEL_READ_REQ:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_BATT_LEVEL_READ_REQ\n");
      /* BAS service test: TC_CR_BV_01_C  */
      battRead = 0x01;
      battIdx = evtData[0];
      HidDevice_Update_Battery_Level(battIdx,98);
    }
    break;
    case EVT_HID_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HID_UPDATE_CMPLT %x %x %x %x\n",evtData[0],evtData[1],evtData[2],evtData[3]);
      deviceState = APPL_CONNECTED;
      if(battRead)
      {
        APPL_MESG_DBG(profiledbgfile,"allow read\n");
        Allow_BatteryLevel_Char_Read(battIdx);
        battRead = 0x00;
      }
    }
    break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
    case EVT_TS_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TS_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_TS_START_REFTIME_UPDATE:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TS_START_REFTIME_UPDATE\n");
      TimeServer_Update_Current_Time_Value(time);
    }
    break;
    case EVT_TS_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TS_CHAR_UPDATE_CMPLT\n");
      deviceState = APPL_CONNECTED;			
    }
    break;
    case EVT_TS_CURTIME_READ_REQ:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TS_CURTIME_READ_REQ\n");
      Allow_Curtime_Char_Read();
    }
    break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
    case EVT_TC_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TC_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_TC_DISCOVERY_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TC_DISCOVERY_CMPLT\n");
      deviceState = APPL_UPDATE;
    }
    break;
    case EVT_TC_READ_REF_TIME_INFO_CHAR:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TC_READ_REF_TIME_INFO_CHAR\n");
      APPL_MESG_DATA(profiledbgfile,evtData,evtLen);

      APPL_MESG_DBG(profiledbgfile,"\n");
    }
    break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
    case EVT_PR_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PR_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_PR_PATH_LOSS_ALERT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PR_PATH_LOSS_ALERT %d\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_PR_LINK_LOSS_ALERT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PR_LINK_LOSS_ALERT %d\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
      switch(evtData[0])
      {
      case 0:
          GPIO_HIGH(LD_PORT_1,LD_YELLOW);
          GPIO_HIGH(LD_PORT_2,LD_RED);
          break;
      case 1:
          GPIO_LOW(LD_PORT_1,LD_YELLOW);
          GPIO_HIGH(LD_PORT_2,LD_RED);
          break;
      case 2:
          GPIO_HIGH(LD_PORT_1,LD_YELLOW);
          GPIO_LOW(LD_PORT_2,LD_RED);
          break;
      default:
          GPIO_HIGH(LD_PORT_1,LD_YELLOW);
          GPIO_HIGH(LD_PORT_2,LD_RED);
      }
    }
    break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
   case EVT_PM_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PM_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_PM_DISCOVERY_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PM_DISCOVERY_CMPLT %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
    case EVT_BPS_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_BPS_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_BPS_BPM_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_BPS_BPM_CHAR_UPDATE_CMPLT %x\n",evtData[0]);
      deviceState = APPL_CONNECTED;
    }
    break;
    case EVT_BPS_ICP_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_BPS_ICP_CHAR_UPDATE_CMPLT %x\n",evtData[0]);
      deviceState = APPL_CONNECTED;
    }
    break;
  #endif
  #if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
    case EVT_HT_INITIALIZED:
    {
         
      APPL_MESG_DBG(profiledbgfile,"EVT_HT_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
      APPL_MESG_DBG(profiledbgfile,"EVT_HT_INITIALIZED  AND DEVICE STATE IS %d\n",deviceState);
    }
    break;
    case EVT_HT_TEMPERATURE_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HT_TEMPERATURE_CHAR_UPDATE_CMPLT %x\n",evtData[0]);
      deviceState = APPL_CONNECTED;
    }
    break;
    case EVT_HT_INTERMEDIATE_TEMP_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HT_INTERMEDIATE_TEMP_CHAR_UPDATE_CMPLT %x\n",evtData[0]);
      deviceState = APPL_CONNECTED;
    }
    break;
    case EVT_HT_MEASUREMENT_INTERVAL_RECEIVED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HT_MEASUREMENT_INTERVAL_RECEIVED %x %x\n",evtData[0],evtData[1]);
      deviceState = APPL_CONNECTED;
    }
    break;
    case EVT_HT_MEASUREMENT_INTERVAL_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HT_MEASUREMENT_INTERVAL_UPDATE_CMPLT\n");
      deviceState = APPL_CONNECTED;
    }
    break;
  #endif
  #if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
    case EVT_ANC_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_ANC_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_ANC_DISCOVERY_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_ANC_DISCOVERY_CMPLT %x\n",evtData[0]);
      deviceState = APPL_CONNECTED;
    }
    break;
    case EVT_ANC_NEW_ALERT_RECEIVED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_ANC_NEW_ALERT_RECEIVED %x %x %x\n",evtLen,evtData[0],evtData[1]);
    }
    break;
    case EVT_ANC_UNREAD_ALERT_STATUS_RECEIVED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_ANC_UNREAD_ALERT_STATUS_RECEIVED %x %x %x\n",evtLen,evtData[0],evtData[1]);
    }
    break;
  #endif
  #if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
    case EVT_ANS_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_ANS_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
    case EVT_PAC_INITIALIZED:
    {
      deviceState = APPL_INIT_DONE;
      APPL_MESG_DBG(profiledbgfile,"EVT_PAC_INITIALIZED\n");
    }
    break;
    case EVT_PAC_DISCOVERY_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PAC_DISCOVERY_CMPLT %x\n",evtData[0]);
    }
    break;
    case EVT_PAC_ALERT_STATUS:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PAC_ALERT_STATUS %x\n",evtData[0]);
      deviceState = APPL_PHONE_READ_ALERT_STATUS;
    }
    break;
    case EVT_PAC_RINGER_SETTING:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PAC_RINGER_SETTING %x\n",evtData[0]);
      deviceState = APPL_PHONE_READ_RINGER_SETTING;
    }
    break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
    case EVT_FMT_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_FMT_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
    case EVT_FML_DISCOVERY_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_FML_DISCOVERY_CMPLT : reason %x\n",evtData[0]);
      deviceState = APPL_LOCATOR_ALERT_TARGET;
      
            
    }
    break;
    case EVT_FML_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_FML_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
    case EVT_GL_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_GL_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
      APPL_MESG_DBG(profiledbgfile,"EVT_GL_INITIALIZED  AND DEVICE STATE IS %d\n",deviceState);
    }
    break;
         
#endif /* BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR */

  }/* end switch() */
}/* end BLE_Profiles_Evt_Notify_Cb() */

void Init_Profile()
{
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
  printf ("Test Application: Initializing Proximity Reporter Profile \n" );
  APPL_MESG_DBG(profiledbgfile,"Initializing Proximity Reporter Profile \n" );

  if (ProximityReporter_Init(0xff,BLE_Profiles_Evt_Notify_Cb) == 
                             BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Initialized Proximity Reporter Profile \n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Proximity Reporter Profile init failure\n" );
  }
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  printf ("Test Application: Initializing Proximity Monitor Profile \n" );
  APPL_MESG_DBG(profiledbgfile,"Initializing Proximity Monitor Profile \n" );

  if (ProximityMonitor_Init(BLE_Profiles_Evt_Notify_Cb) == BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Initialized Proximity Monitor Profile \n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Failed Proximity Monitor Profile \n" );
  }
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  printf ("Test Application: Initializing Time Server Profile \n" );
  APPL_MESG_DBG(profiledbgfile,"Initializing Time Server Profile \n" );

  if (TimeServer_Init (NEXT_DST_CHANGE_SERVICE_BITMASK|REFERENCE_TIME_UPDATE_SERVICE_BITMASK, 
                       BLE_Profiles_Evt_Notify_Cb) == BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Initialized Time Server Profile \n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Failed Time Server Profile \n" );
  }
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  printf ("Test Application: Initializing Time Client Profile \n" );
  APPL_MESG_DBG(profiledbgfile,"Initializing Time Client Profile \n" );

  if (TimeClient_Init(BLE_Profiles_Evt_Notify_Cb) == BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Initialized Time Client Profile \n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Failed Time Client Profile \n" );
  }
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HID_DEVICE)
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
  printf ( "Test Application: Initializing HID Device Profile \n" );

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
    APPL_MESG_DBG(profiledbgfile,"Initialized HID Device Profile \n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Failed HID Device Profile \n" );
  }
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
  printf ("Test Application: Initializing Heart Rate Profile \n" );
  //fprintf(profiledbgfile,"Initializing Heart Rate Profile \n" );

  if (HRProfile_Init((uint8_t)0xFF,
                     BLE_Profiles_Evt_Notify_Cb,
                     0x04) == BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Initialized Heart Rate Profile \n" );
  }
  else
  {
  APPL_MESG_DBG(profiledbgfile,"Failed Heart Rate Profile \n" );
  }
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
  APPL_MESG_DBG(profiledbgfile,"Initializing Find me profile\n");
  printf ("Test Application: Initializing Find me profile\n" );
  FindMeTarget_Init(BLE_Profiles_Evt_Notify_Cb);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
  APPL_MESG_DBG(profiledbgfile,"Initializing Find me profile\n");
  printf ("Test Application: Initializing Find me profile\n" );
  FindMeLocator_Init(&Security_Param,BLE_Profiles_Evt_Notify_Cb);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
  printf ("Test Application: Initializing Phone Alert Status profile\n" );
  PAC_Init(BLE_Profiles_Evt_Notify_Cb);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
  APPL_MESG_DBG(profiledbgfile,"Initializing blood pressure sensor profile\n");
  printf ("Test Application: Initializing blood pressure sensor profile\n" );
  BPS_Init(0x01,
           BODY_MOVEMENT_DETECTION_SUPPORT_BIT|CUFF_FIT_DETECTION_SUPPORT_BIT|MEASUREMENT_POSITION_DETECTION_SUPPORT_BIT,
           BLE_Profiles_Evt_Notify_Cb);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
  APPL_MESG_DBG(profiledbgfile,"Initializing health thermometer profile\n");
  printf ("Test Application: Initializing health htermometer profile\n" );
  HT_Init(INTERMEDIATE_TEMPERATURE_CHAR|MEASUREMENT_INTERVAL_CHAR|TEMPERATURE_TYPE,
          2, /* minValidInterval 2 */
          60, /* maxValidInterval */
          BLE_Profiles_Evt_Notify_Cb);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
  APPL_MESG_DBG(profiledbgfile,"Initializing alert notification client profile\n");
  ANC_Client_Init(BLE_Profiles_Evt_Notify_Cb);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
  uint8_t alertCategory[2] = {0xFF,0x03};
  uint8_t unreadAlertCategory[2] = {0xFF,0x00};
  APPL_MESG_DBG(profiledbgfile,"Initializing alert notification server profile\n");
  ANS_Init(alertCategory,unreadAlertCategory,BLE_Profiles_Evt_Notify_Cb);
#endif

#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
  APPL_MESG_DBG(profiledbgfile,"Initializing glucose sensor profile\n");
  printf ("Test Application: Initializing glucose sensor profile with user database\n" );
  /* Number of records  on glucose database + pointer to glucose measurement and context database */
  GL_Init(GLUCOSE_NUMBER_OF_STORED_RECORDS, &glucoseDatabase_MeasurementRecords[0], &glucoseDataBase_MeasurementContextRecords[0], BLE_Profiles_Evt_Notify_Cb);  
#endif/* (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR) */

}/* end Init_Profile() */


void Advertize()
{
#if (BLE_CURRENT_PROFILE_ROLES & (HEART_RATE_SENSOR|TIME_CLIENT|TIME_SERVER|PROXIMITY_REPORTER|PROXIMITY_MONITOR|HID_DEVICE))
  uint8_t useBoundedDeviceList = 0;
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & (FIND_ME_TARGET|FIND_ME_LOCATOR|PHONE_ALERT_CLIENT))
  //uint8_t bdAddr[] = {0x00,0x1b,0xdc,0x05,0xb6,0x82};
#endif

#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
  printf("Test Application: advertize\n");
  BPS_Advertize(0x00);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
  printf("Test Application: add to whitelist\n");
  //FMT_Add_Device_To_WhiteList(0x00,bdAddr);
  printf("Test Application: advertize\n");
  FMT_Advertize();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
  printf("Test Application: add to whitelist\n");
  //FML_Add_Device_To_WhiteList(bdAddr);
  printf("Test Application: advertize\n");
  FML_Advertize();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
				
  //PAC_Add_Device_To_WhiteList(bdAddr);
  printf("Test Application: advertize\n");
  APPL_MESG_DBG(profiledbgfile,"writing the ringer command\n");
  PAC_Advertize();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HID_DEVICE)
  HidDevice_Make_Discoverable(useBoundedDeviceList);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  TimeServer_Make_Discoverable(useBoundedDeviceList);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  TimeClient_Make_Discoverable(useBoundedDeviceList);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
  APPL_MESG_DBG(profiledbgfile,"set sensor location\n");
  HRProfile_Set_Body_Sensor_Location(BODY_SENSOR_LOCATION_HAND);
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  HR_Sensor_Make_Discoverable( useBoundedDeviceList );
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  ProximityReporter_Make_Discoverable(useBoundedDeviceList);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  ProximityMonitor_Make_Discoverable(useBoundedDeviceList);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  char systemId[8]={0x00,0x01,'.',0x00,0x00,0x00,0x00,0x01};
  char pnpid[7]={0x01,0x01,0x01,0x01,0x01,0x01,0x01};
  //BLE_Profile_Update_DIS_SystemID(8,systemId);
  //BLE_Profile_Update_DIS_ModelNum(8,systemId);
  //BLE_Profile_Update_DIS_SerialNum(8,systemId);
  //BLE_Profile_Update_DIS_FirmwareRev(8,systemId);
  //BLE_Profile_Update_DIS_HardwareRev(8,systemId);
  //BLE_Profile_Update_DIS_SoftwareRev(8,systemId);
  //BLE_Profile_Update_DIS_manufacturerName(8,systemId);
  //BLE_Profile_Update_DIS_IEEECertification(8,systemId);
  BLE_Profile_Update_DIS_pnpId(7,pnpid);
  HT_Update_Temperature_Type(0x00);
  HT_Advertize(0x00);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  ANC_Advertize(0x00);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  ANS_Advertize(0x00);
#endif	

#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
  APPL_MESG_DBG(profiledbgfile,"glucose enable advertising\n");
  
  /* Set Glucose Feature characteristic value: all features except multi bond */
  GL_Set_Glucose_Feature_Value(GLUCOSE_FEATURE_ALL_SUPPORTED_NO_MULTI_BOND);

  /* Start advertising (no whitelist, fast) */
  GL_Advertize(0x00);
#endif/* (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR) */

}/* end Advertize() */



#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
void Init_Current_Time()
{
  time.date = 06;
  time.day_of_week = 04;
  time.hours = 1;
  time.minutes = 1;
  time.seconds = 4;
  
  /* NOTES: Values to be set for running PTS tests CTS, CSP */
  time.adjustReason = 0x01; /* ADJUST_REASON_MANUAL_TIME_UPDATE: TP/CSP/BV-02-C */
  //time.adjustReason = 0x04; /* ADJUST_REASON_CHANGE_OF_TIME_ZONE: TP/CSP/BV-03-C */
  //time.adjustReason = 0x08; /* ADJUST_REASON_CHANGE_OF_DST:     TP/CSP/BV-04-C */
  //time.adjustReason = 0x02; /* ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE: TP/CSP/BV-05-C, TP/CSP/BV-06-C*/
  
  time.fractions256 = 0x00;
  time.year = 2013;

}
#endif

#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
void Init_temperature()
{
  tempMeasurementVal.flags = (FLAG_TEMPERATURE_UNITS_FARENHEIT|FLAG_TIMESTAMP_PRESENT|FLAG_TEMPERATURE_TYPE);
  tempMeasurementVal.temperatureType = TEMP_MEASURED_FOR_BODY;
  tempMeasurementVal.timeStamp.year = 2012;
  tempMeasurementVal.timeStamp.month = 10;
  tempMeasurementVal.timeStamp.day = 26;
  tempMeasurementVal.timeStamp.hours = 19;
  tempMeasurementVal.timeStamp.minutes = 42;
  tempMeasurementVal.timeStamp.seconds = 50;
}
#endif

void Display_Appl_Menu()
{
  printf("APPL_TERMINATE_LINK: %u\n",APPL_TERMINATE_LINK);
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
  printf("APPL_SEND_HEART_RATE_MEASUREMENT: %u\n",APPL_SEND_HEART_RATE_MEASUREMENT);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HID_DEVICE)
  printf("APPL_UPDATE_INPUT_REPORT: %u\n",APPL_UPDATE_INPUT_REPORT);
  printf("APPL_UPDATE_BOOT_KEYBOARD_IP_REPORT: %u\n",APPL_UPDATE_BOOT_KEYBOARD_IP_REPORT);
  printf("APPL_UPDATE_BOOT_MOUSE_IP_REPORT: %u\n",APPL_UPDATE_BOOT_MOUSE_IP_REPORT);
  printf("APPL_UPDATE_BATTERY_LEVEL: %u\n",APPL_UPDATE_BATTERY_LEVEL);
  printf("APPL_UPDATE_SCAN_REFRESH_CHARAC: %u\n",APPL_UPDATE_SCAN_REFRESH_CHARAC);
  printf("APPL_UPDATE_FEATURE_REPORT: %u\n",APPL_UPDATE_FEATURE_REPORT);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
  printf("APPL_SEND_BP_MEASUREMENT: %u\n",APPL_SEND_BP_MEASUREMENT);
  printf("APPL_SEND_INTERMEDIATE_CUFF_PRESSURE: %u\n",APPL_SEND_INTERMEDIATE_CUFF_PRESSURE);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
  printf("APPL_SEND_HT_MEASUREMENT: %u\n",APPL_SEND_HT_MEASUREMENT);
  printf("APPL_SEND_HT_INTERMEDIATE_MEASUREMENT: %u\n",APPL_SEND_HT_INTERMEDIATE_MEASUREMENT);
  printf("APPL_UPDATE_MEASUREMENT_INTERVAL: %u\n",APPL_UPDATE_MEASUREMENT_INTERVAL);
				 
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  printf("APPL_UPDATE_CURRENT_TIME: %u\n",APPL_UPDATE_CURRENT_TIME);
  printf("APPL_UPDATE_LOCAL_TIME: %u\n",APPL_UPDATE_LOCAL_TIME);
  printf("APPL_UPDATE_REFERENCE_TIME: %u\n",APPL_UPDATE_REFERENCE_TIME);
  printf("APPL_UPDATE_NEXT_DSTCHANGE_TIME: %u\n",APPL_UPDATE_NEXT_DSTCHANGE_TIME);
  printf("APPL_UPDATE_REFERENCE_TIME_INFO: %u\n",APPL_UPDATE_REFERENCE_TIME_INFO);	 
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
  printf("APPL_UPDATE_NEW_ALERT: %u\n",APPL_UPDATE_NEW_ALERT);
  printf("APPL_UPDATE_UNREAD_ALERT_STATUS: %u\n",APPL_UPDATE_UNREAD_ALERT_STATUS);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
	
  printf("APPL_START_DISCOVERY: %u\n",APPL_START_DISCOVERY);
  printf("APPL_EN_DIS_NEW_ALERT_NOTIF: %u\n",APPL_EN_DIS_NEW_ALERT_NOTIF);
  printf("APPL_EN_DIS_UNREAD_ALERT_STATUS_NOTIF: %u\n",APPL_EN_DIS_UNREAD_ALERT_STATUS_NOTIF);
  printf("APPL_WRITE_CTRL_POINT: %u\n",APPL_WRITE_CTRL_POINT);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
	
  printf("APPL_START_DISCOVERY: %u\n",APPL_START_DISCOVERY);            
  printf("APPL_LOCATOR_ALERT_TARGET: %u\n",APPL_LOCATOR_ALERT_TARGET);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)	
  printf("APPL_START_DISCOVERY: %u\n",APPL_START_DISCOVERY);
  printf("APPL_GET_REF_TIME_UPDATE: %u\n",APPL_GET_REF_TIME_UPDATE);
  printf("APPL_TIME_UPDATE_NOTIFICATION: %u\n",APPL_TIME_UPDATE_NOTIFICATION);
  printf("APPL_GET_SERV_TIME_UPDATE_STATE: %u\n",APPL_GET_SERV_TIME_UPDATE_STATE);
  printf("APPL_READ_NEXT_DST_CHANGE_TIME: %u\n",APPL_READ_NEXT_DST_CHANGE_TIME);
  printf("APPL_READ_SERV_TIME_UPDATE_STATE: %u\n",APPL_READ_SERV_TIME_UPDATE_STATE);	
  printf("APPL_READ_LOCAL_TIME_INFORM: %u\n",APPL_READ_LOCAL_TIME_INFORM);
  printf("APPL_READ_CURRENT_TIME: %u\n",APPL_READ_CURRENT_TIME);
  printf("APPL_GET_TIME_ACCU_INFO_SERVER: %u\n",APPL_GET_TIME_ACCU_INFO_SERVER);
  printf("APPL_CANCEL_REF_TIME_UPDATE: %u\n",APPL_CANCEL_REF_TIME_UPDATE);
				 
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
  printf("APPL_START_DISCOVERY: %u\n",APPL_START_DISCOVERY);
  printf("APPL_PHONE_READ_ALERT_STATUS: %u\n",APPL_PHONE_READ_ALERT_STATUS);
  printf("APPL_PHONE_READ_RINGER_SETTING: %u\n",APPL_PHONE_READ_RINGER_SETTING);
  printf("APPL_DISABLE_ALERT_STATUS_NOTIF: %u\n",APPL_DISABLE_ALERT_STATUS_NOTIF);
  printf("APPL_DISABLE_RINGER_SETTING_NOTIF: %u\n",APPL_DISABLE_RINGER_SETTING_NOTIF);
  printf("APPL_CONFIGURE_RINGER_SILENCE: %u\n",APPL_CONFIGURE_RINGER_SILENCE);
  printf("APPL_CONFIGURE_RINGER_MUTE: %u\n",APPL_CONFIGURE_RINGER_MUTE);
  printf("APPL_CNFIGURE_RINGER_CANCEL_SILENCE: %u\n",APPL_CNFIGURE_RINGER_CANCEL_SILENCE);
 

#endif
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
  printf("APPL_GLUCOSE_CHANGE_DB_1: %u\n",APPL_GLUCOSE_CHANGE_DB_1);
  printf("APPL_GLUCOSE_CHANGE_DB_2: %u\n",APPL_GLUCOSE_CHANGE_DB_2);
#endif /* (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR) */       
                                
                             
  printf("Enter any other number to run the processQ without input from user\n");
    
}/* end Display_Appl_Menu() */


#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
#if HT_TC_SP_BV_01_C /* to be enabled only for PTS test 4.10.1: HT_TC_SP_BV_01_C */


void HT_Send_Next_Temperature_measurement(void)
{
  /* check if the sending a second temperature measurement process should be initiated */
  if (ht_check_next_send_temperature_measurement_flag)
  {
    switch (ht_check_next_send_temperature_measurement_state)
    {
      case(HT_CONNECTED_DO_MEASUREMENT_INTERVAL_WAIT):
      {
        if (HT_Get_IntervalTimerID() == 0xFF)
        {
          ht_check_next_send_temperature_measurement_state = HT_CONNECTED_DO_MEASUREMENT_INTERVAL_WAIT_STOP;
          /* send a second measurement */
          HT_Send_Temperature_Measurement(tempMeasurementVal);
        }
      }
      break;
      case(HT_CONNECTED_DO_MEASUREMENT_INTERVAL_WAIT_STOP):
      {
        /* reset state */
        ht_check_next_send_temperature_measurement_state = 0; 
        ht_check_next_send_temperature_measurement_flag = 0; 
      }
      break;
    }
  }
}/* end HT_Send_Next_Temperature_measurement() */

#endif /* HT_TC_SP_BV_01_C */
#endif /* BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER */

     
void Host_Profile_Test_Application (void)
{
  int indx;
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  uint8_t status;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & (FIND_ME_TARGET|FIND_ME_LOCATOR|PHONE_ALERT_CLIENT))
  //uint8_t bdAddr[6] = {0x82,0xB6,0x05,0xDC,0x1B,0x00};
#endif

  printf ("Test Application: Starting testing the BLE Profiles \n" );

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
  
  printf ("Test Application: BLE main profile initialized. \n" );

  /* initialize the individual profiles */
  Init_Profile();
	
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  Init_Current_Time();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
  Init_temperature();
#endif

  deviceState = APPL_UNINITIALIZED;
  #include "Menu_core.h"
  #include "Menu_config.h"
  uint8_t stop = 0;
  /* input state */
  Menu_Position_t pos;
  uint8_t button_state, ts_state;
  uint8_t previous_button_state = 0;
  uint8_t was_connected = 0;
  while(stop == 0)
  {
    HCI_Process();
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
    HRProfile_StateMachine();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
    GL_StateMachine();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)     
    BPS_StateMachine();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
    HT_StateMachine(); 
#if HT_TC_SP_BV_01_C /* to be enabled only for PTS test 4.10.1: HT_TC_SP_BV_01_C */
    HT_Send_Next_Temperature_measurement(); 
#endif //HT_TC_SP_BV_01_C
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
    FMLProfile_StateMachine();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
    FMTProfile_StateMachine();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
    PACProfile_StateMachine();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
    TimeServer_StateMachine();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)      
    TimeClient_StateMachine();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
    ANCProfile_StateMachine();
#endif      
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
    ANSProfile_StateMachine();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
    ProximityMonitorProfile_StateMachine();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
    ProximityReporterProfile_StateMachine();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HID_DEVICE)
    HIDProfile_StateMachine();
#endif
  uint8_t ts_state = Menu_ReadPosition(&pos);
  Menu_ReadSelection(&button_state);
  /* Can stop the demo by any input before the connection is done. */
  if((button_state || ts_state) && (deviceState < APPL_CONNECTED))
  {
    stop = 1;
  }


//PROFILE_MESG_DBG ( profiledbgfile, "calling profile process Q%d\n",deviceState);
    if(Profile_Process_Q() == 0x00)
    {
      /* if the profiles have nothing more to process, then
       * wait for application input
       */ 

      
      if ((deviceState < APPL_CONNECTED) && (was_connected))
      {
        /* Some smartphone (SamsungJ7) can disconnect if the HID is not allowed to take control. */
        was_connected = 0;
        Menu_MsgStatus("Disconnected...", "Check if HID control is enabled on the smartphone.",MSG_INFO);
      }
      
      if(deviceState >= APPL_CONNECTED)
      {
        
        uint8_t input = 200;

        was_connected = 1;          

        if (deviceState == APPL_CONNECTED)
        {
          /* Send a neutral report to make the pointer appear on the smartphone */
          input = APPL_UPDATE_INPUT_REPORT;            
        }
        
        if(ts_state)
        {
          /* Last line is used for exiting the demo. */
          if( pos.Y > (Menu_GetDisplayHeight() - 20)) {
            stop = 1;            
          } else {
            /* other input control the pointer. */
            input = APPL_UPDATE_INPUT_REPORT;            
          }
        }

          /* The button sends both a report when pushed and when released. */
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
        PROFILE_MESG_DBG(profiledbgfile,"Call BLE_Profile_Disconnect\n"); 
        BLE_Profile_Disconnect();
        deviceState = APPL_UNINITIALIZED;
      }
      break;
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
      case APPL_SEND_HEART_RATE_MEASUREMENT:
      {
        //HRProfile_Set_Sensor_Contact_Support_Bit(); //for test case TC_CN_BV_05
        heartRateMeasurmentValue.valueformat = HRM_VALUE_FORMAT_UINT16;
        heartRateMeasurmentValue.sensorContact = HRM_SENSOR_CONTACT_PRESENT;
        heartRateMeasurmentValue.energyExpendedStatus = HRM_ENERGY_EXPENDED_PRESENT;
        heartRateMeasurmentValue.rrIntervalStatus = HRM_RR_INTERVAL_PRESENT;
        heartRateMeasurmentValue.heartRateValue = (uint8_t)0xE4;
        heartRateMeasurmentValue.enrgyExpended = 0x00;
        heartRateMeasurmentValue.numOfRRIntervalvalues = 0x05;
        for (indx=0; indx<9; indx++)
        {
                heartRateMeasurmentValue.rrIntervalValues[indx] = (uint16_t)(indx + 0);
        }
        //for test case TC_CN_BV_05
        //HRProfile_Send_HRM_Value (heartRateMeasurmentValue);
        //heartRateMeasurmentValue.sensorContact = HRM_SENSOR_CONTACT_NOT_PRESENT;//for test case TC_CN_BV_05
        HRProfile_Send_HRM_Value (heartRateMeasurmentValue);
        deviceState = APPL_WAIT;
      }
      break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HID_DEVICE)
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
        APPL_MESG_DBG(profiledbgfile,"HID_Device: APPL_UPDATE_BATTERY_LEVEL state (update battery level)\n");
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
#endif
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
      case APPL_SEND_BP_MEASUREMENT:
      {
        tBPMValue bpmval;
        
        bpmval.flags = (FLAG_TIME_STAMP_PRESENT|
                        FLAG_PULSE_RATE_PRESENT|
                                        FLAG_USER_ID_PRESENT|
                                        FLAG_MEASUREMENT_STATUS_PRESENT);
        bpmval.systolic = 0xAAAA;
        bpmval.diastolic = 0xBBBB;
        bpmval.map = 0xCCCC;
        bpmval.pulseRate = 80;
        bpmval.userID = 0x01;
        bpmval.timeStamp.year = 2012;
        bpmval.timeStamp.month = 10;
        bpmval.timeStamp.day = 26;
        bpmval.timeStamp.hours = 19;
        bpmval.timeStamp.minutes = 42;
        bpmval.timeStamp.seconds = 50;
        bpmval.measurementStatus.bodyMovementDetectionFlag = 0x01;
        bpmval.measurementStatus.cuffFitDetectionFlag = 0x01;
        bpmval.measurementStatus.measurementPositionDetectionFlag = 0x01;
       
        APPL_MESG_DBG(profiledbgfile,"Sending blood pressure measurment\n");
        BPS_Send_Blood_Pressure_Measurement(bpmval);
      }
      break;
      case APPL_SEND_INTERMEDIATE_CUFF_PRESSURE:
      {
        tICPValue icpval;
        APPL_MESG_DBG(profiledbgfile,"Sending intermediate cuff pressure measurment\n");
        icpval.flags = (FLAG_USER_ID_PRESENT|FLAG_TIME_STAMP_PRESENT|FLAG_PULSE_RATE_PRESENT|FLAG_MEASUREMENT_STATUS_PRESENT);
        icpval.icp = 0xAAAA;
        icpval.userID = 0x01;
        icpval.measurementStatus.bodyMovementDetectionFlag = 0x01;
        icpval.measurementStatus.cuffFitDetectionFlag = 0x01;
        icpval.measurementStatus.measurementPositionDetectionFlag = 0x01;
        BPS_Send_Intermediate_Cuff_Pressure(icpval);
      }
      break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
      case APPL_SEND_HT_MEASUREMENT:
      {
        printf("sending ht measurement\n");
        if(tempMeasurementVal.timeStamp.minutes == 60)
        {
          tempMeasurementVal.timeStamp.minutes = 0;
          tempMeasurementVal.timeStamp.hours+=1;
        }
        HT_Send_Temperature_Measurement(tempMeasurementVal);
        
#if HT_TC_SP_BV_01_C /* to be enabled only for PTS test 4.10.1: HT_TC_SP_BV_01_C */
        ht_check_next_send_temperature_measurement_flag = 1;
        ht_check_next_send_temperature_measurement_state = HT_CONNECTED_DO_MEASUREMENT_INTERVAL_WAIT;
#endif
      }
      break;
      case APPL_SEND_HT_INTERMEDIATE_MEASUREMENT:
      {
        printf("sending ht intermediate measurement\n");
        if(tempMeasurementVal.timeStamp.minutes == 60)
        {
          tempMeasurementVal.timeStamp.minutes = 0;
          tempMeasurementVal.timeStamp.hours+=1;
        }
        HT_Send_Intermediate_Temperature(tempMeasurementVal);
      }
      break;
      case APPL_UPDATE_MEASUREMENT_INTERVAL:
      {
        APPL_MESG_DBG(profiledbgfile,"Call HT_Update_Measurement_Interval()\n");
        HT_Update_Measurement_Interval(30); 
      }
      break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
      case APPL_UPDATE_NEW_ALERT:
      {
        tTextInfo textInfo;
        uint8_t str[4] = {'J','O','H','N'};
        APPL_MESG_DBG(profiledbgfile,"ALERT_NOTIFICATION_SERVER\n");
        textInfo.len = 0x04;
        textInfo.str = str;
        ANS_Update_New_Alert(CATEGORY_ID_EMAIL,0x01,textInfo);
  
      }			 	 
      break;
      case APPL_UPDATE_UNREAD_ALERT_STATUS: 
      {
        ANS_Update_Unread_Alert_Status(CATEGORY_ID_SMS_MMS,0x01);			  
      }
      break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
      case APPL_EN_DIS_NEW_ALERT_NOTIF:
      {
        ANC_Enable_Disable_New_Alert_Notification(0x0001); //Change to 0x0000 for test TP/ANPCF/ANPC/BV-02-I
              
      }			 	 
      break;
      case APPL_EN_DIS_UNREAD_ALERT_STATUS_NOTIF: 
      {
        ANC_Enable_Disable_Unread_Alert_Status_Notification(0x0001);//Change to 0x0000 for test TP/ANPCF/ANPC/BV-04-I
      }
      break;
      case APPL_WRITE_CTRL_POINT:
      {
        /*
        * The parameter will be:
        * Test TP/ANPNF/ANPC/BV-01-C command = ENABLE_NEW_ALERT_NOTIFICATION          category = CATEGORY_ID_EMAIL
        * Test TP/ANPNF/ANPC/BV-02-I command = ENABLE_NEW_ALERT_NOTIFICATION          category = CATEGORY_ID_EMAIL
        * Test TP/ANPSF/ANPC/BV-01-I command = NOTIFY_NEW_ALERT_IMMEDIATELY           category = CATEGORY_ID_ALL
        * Test TP/ANPSF/ANPC/BV-02-I command = NOTIFY_UNREAD_ALERT_STATUS_IMMEDIATELY category = CATEGORY_ID_ALL
        */
        ANC_Write_Control_Point(ENABLE_NEW_ALERT_NOTIFICATION, CATEGORY_ID_EMAIL);
        
      }
      break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
      case APPL_UPDATE_CURRENT_TIME:
      {
        APPL_MESG_DBG(profiledbgfile,"updating current time (minute: %d) as +2 minutes\n", time.minutes); 
        
        //time.minutes += 1;
        //if(time.minutes == 60)
        //{
          //time.minutes = 0;
          //time.hours+=1;
        //}
        
        //TBR: new code for PTS tests including also PTS TEST TC_CSP_BV_06_C */
        if ((time.minutes + 2) >= 60)
        {
          time.minutes = (time.minutes + 2) - 60;
          time.hours+=1;
        }
        else
          time.minutes += 2;

        status = TimeServer_Update_Current_Time_Value(time);
        APPL_MESG_DBG(profiledbgfile,"updating current time status is %d\n", status);
        //deviceState = APPL_WAIT;
      }
      break;
      
      case APPL_DECREASE_CURRENT_TIME: //New code only for PTS TEST TC_CSP_BV_06_C 
      {
        /* Adjusting time less than 1 minute from previous one */
        APPL_MESG_DBG(profiledbgfile,"updating current time (minute: %d) less than 1 minute (decrease 1 second)\n",time.minutes);
        /* Just decrease current time of 1 second: required for PTS test TC_CSP_BV_06_C */
        time.seconds -= 1;

        status = TimeServer_Update_Current_Time_Value(time);
        APPL_MESG_DBG(profiledbgfile,"updating current time status is %d\n", status); 
      }
      break;
      case APPL_UPDATE_CURRENT_TIME_MORE_THAN_15://New code (only for PTS TEST TC_CSP_BV_06_C?  TBR if needed)
      {
        APPL_MESG_DBG(profiledbgfile,"updating current time (minute: %d) as +15 minutes\n", time.minutes);
        if ((time.minutes + 15) >= 60)
        {
          time.minutes = (time.minutes + 15) - 60;
          time.hours+=1;
        }
        else
          time.minutes += 15;
        status = TimeServer_Update_Current_Time_Value(time);
        APPL_MESG_DBG(profiledbgfile,"updating current time status is %d\n", status);
      }
      break;
        
      case APPL_UPDATE_LOCAL_TIME:
      {
        tLocalTimeInformation localTimeInfo;
        localTimeInfo.timeZone = 0x03;
        localTimeInfo.dstOffset = 0x12;
        TimeServer_Update_Local_Time_Information(localTimeInfo);
      }
      break;
      case APPL_UPDATE_NEXT_DSTCHANGE_TIME:
      {
        tTimeWithDST timeDST;
        timeDST.date = 06;
        timeDST.year = 2000;
        timeDST.month = 04;
        timeDST.hours = 1;
        timeDST.minutes = 1;
        timeDST.seconds = 4;
                      
        TimeServer_Update_Next_DST_Change(timeDST);
      }
      break;
      case APPL_UPDATE_REFERENCE_TIME_INFO:
      {
        tReferenceTimeInformation refTimeInfo;
        refTimeInfo.source = 0x01;	
        refTimeInfo.accuracy = 0x02;	 
        refTimeInfo.daysSinceUpdate = 0x05;	 
        refTimeInfo.hoursSinceUpdate = 0x03;	 
        TimeServer_Update_Reference_Time_Information(refTimeInfo);
              
      }
      break;
#endif
    
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)	
      case APPL_GET_REF_TIME_UPDATE:	
      {
        TimeClient_Update_Reference_Time_On_Server(0x01);
      }	
      break;
      case APPL_CANCEL_REF_TIME_UPDATE:	
      {
        TimeClient_Update_Reference_Time_On_Server(0x02);
      }	
      break;
      case APPL_TIME_UPDATE_NOTIFICATION:
      {
        TimeClient_SetReset_Time_Update_Notification(0x01);
      }	
      break;
      case APPL_GET_SERV_TIME_UPDATE_STATE: 
      {
        TimeClient_Get_Server_Time_Update_State();
      }
      break;
      case APPL_READ_NEXT_DST_CHANGE_TIME: 		 
      {
        TimeClient_Get_Next_DST_Change_Time();
      }
      break;
      case APPL_READ_LOCAL_TIME_INFORM:
      {
        TimeClient_Get_Local_Time_Information();
      }
      break;
      case APPL_READ_CURRENT_TIME:
      {
        TimeClient_Get_Current_Time();
      }
      break;
      case APPL_GET_TIME_ACCU_INFO_SERVER:	
      {
        TimeClient_Get_Time_Accuracy_Info_Of_Server();
      }
      break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
      case APPL_PHONE_READ_ALERT_STATUS:		 
      {
        PAC_Read_AlertStatus();
      }
      break;
      case APPL_PHONE_READ_RINGER_SETTING:	
      {
        PAC_Read_RingerSetting(); 
      }	
      break;
      case APPL_DISABLE_ALERT_STATUS_NOTIF:	
      {
        PAC_Disable_ALert_Status_Notification();
      }
      break;
      case APPL_DISABLE_RINGER_SETTING_NOTIF:
      {
        PAC_Disable_Ringer_Status_Notification();
      }
      break;	
      case APPL_CONFIGURE_RINGER_SILENCE:
      {
        PAC_Configure_Ringer(0x01);
      }
      break;
      case APPL_CONFIGURE_RINGER_MUTE:
      {
        PAC_Configure_Ringer(0x02); 
      }
      break;
      case APPL_CNFIGURE_RINGER_CANCEL_SILENCE: 
      {
        PAC_Configure_Ringer(0x03);
      }
      break;
#endif	
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
      case APPL_LOCATOR_ALERT_TARGET:
      {
        FML_ALert_Target(0x00);// Set to 1 for test TC_FMF_FL_BV_01_I. Set to 0 for test TC_FMF_FL_BV_02_I
      }
      break;
#endif
                    
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
      case APPL_GLUCOSE_CHANGE_DB_1: 
      {
        APPL_MESG_DBG(profiledbgfile,"PTS GLS test 4.10.1 SPD BV_01: call glucoseDatabase_Change_Database_Manually \n");
        /* Change database manually: store new sequence number and reset flags field on 
        current database measurement for simulating new records measurements */
        glucoseDatabase_Change_Database_Manually(1, SET_FLAG); /* PTS GLS test 4.10.1 SPD BV_01 */
      }
      break;
      case APPL_GLUCOSE_CHANGE_DB_2: 
      {
        APPL_MESG_DBG(profiledbgfile,"PTS GLS test 4.12.2 SPN BV_03: call glucoseDatabase_Change_Database_Manually \n");
        /* Change database manually: store new sequence number and reset flags field on 
        current database measurement for simulating new records measurements */
        glucoseDatabase_Change_Database_Manually(3, UNSET_FLAG); /* PTS GLS test 4.12.2 SPN BV_03 */
      }
      break;
  #endif /* (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR) */

      case APPL_SEND_SLAVE_SECURITY_REQUEST:
      { 
        APPL_MESG_DBG(profiledbgfile,"profiles_test_application: call BLE_Profile_Send_Security_Request() \n");
        BLE_Profile_Send_Security_Request();
      }
      break;   
                
    }/* end switch(devicestate) */

  } /* end while(1) */
          BLE_Profile_Disconnect();
        deviceState = APPL_UNINITIALIZED;

}/* end Host_Profile_Test_Application() */

