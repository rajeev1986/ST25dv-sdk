/**
  ******************************************************************************
  * @file    testmenu.c 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   test menu
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

/* Includes ------------------------------------------------------------------*/
#include "display_tools.h"
#include "tests.h"


/* Global variables ----------------------------------------------------------*/
/***************************************
*    M E N U   D E F I N I T I O N S   *
***************************************/
											
// defines all submenus
DT_OPEN_MENU(BASIC)
DT_TEST(Test_basic_init)
DT_TEST(Test_ButtonLed)
DT_BACK()
DT_CLOSE_MENU()
							
											
DT_OPEN_MENU(LCD)
DT_TEST(Test_LcdRegister)
DT_TEST(Test_LcdColor)
DT_TEST(Test_LcdDisplay)
DT_TEST(Test_LcdText)
DT_BACK()
DT_CLOSE_MENU()

DT_OPEN_MENU(JOYSTICK)
DT_TEST(Test_Joystick)
DT_TEST(Test_Joystick_LCD)
DT_BACK()
DT_CLOSE_MENU()

DT_OPEN_MENU(TOUCHSCREEN)
DT_TEST(Test_TouchScreenRegister)
DT_TEST(Test_TouchScreenDetect)
DT_TEST(Test_TouchScreenPosition)
DT_BACK()
DT_CLOSE_MENU()

DT_OPEN_MENU(USB)
DT_TEST(Test_USB)
DT_BACK()
DT_CLOSE_MENU()

DT_OPEN_MENU(DPOT_ADC)
DT_TEST(Test_DpotRegister)
DT_TEST(Test_ADCread)
DT_TEST(Test_ADCdpot)
DT_BACK()
DT_CLOSE_MENU()

DT_OPEN_MENU(BLE_WIFI)
DT_TEST(Test_BLEconnect)
DT_TEST(Test_WIFIconnect)
DT_BACK()
DT_CLOSE_MENU()

// Main menu
DT_OPEN_MENU(MBOARDVALID)
DT_SUBMENU(BASIC)
DT_SUBMENU(LCD)
DT_SUBMENU(JOYSTICK)
DT_SUBMENU(TOUCHSCREEN)
DT_SUBMENU(USB)
DT_SUBMENU(DPOT_ADC)
DT_SUBMENU(BLE_WIFI)
DT_CLOSE_MENU()

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
