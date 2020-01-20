/******************************************************************************
*
* THE PRESENT CODE AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* (C) 2013 STMicroelectronics International NV
*
*   FILENAME        -  host_config.h
*
*   COMPILER        -  EWARM (gnu gcc v4.4.5)
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:       11/07/2011
*   $Revision$:   first version
*   $Author$:     
*   Comments:     Define BLE host stack configuration macros
*
*******************************************************************************
*
*  File Description 
*  ---------------------
* This header file defines the host stack configuration values. 
* 
* 
*******************************************************************************/
#ifndef _HOST_CONFIG_H_
#define _HOST_CONFIG_H_

#define PLATFORM_LINUX         (0x01)
#define PLATFORM_WINDOWS       (0x02)
#define PLATFORM_ARM           (0x03)

#define PLATFORM_TYPE          (PLATFORM_ARM)

/* profile codes */

#define HID_DEVICE              	(0x20)



/* active profiles */
#ifndef BLE_CURRENT_PROFILE_ROLES
#define BLE_CURRENT_PROFILE_ROLES  (HID_DEVICE)
#endif

#endif /* _HOST_CONFIG_H_ */ 
