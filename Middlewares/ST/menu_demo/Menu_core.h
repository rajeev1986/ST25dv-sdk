/**
  ******************************************************************************
  * @file    menu.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file contains all the functions prototypes for the
  *                  menu navigation firmware driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MENU_H
#define __MENU_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stddef.h"
/* Exported types ------------------------------------------------------------*/

typedef enum {
  MENU_CALLBACK_CONTINUE = 0,
  MENU_CALLBACK_LAST = 1
} Menu_Callback_Status_t;

typedef Menu_Callback_Status_t (*Menu_Callback_t)(void);

typedef void (* tMenuFunc)(void);
typedef struct sMenuItem * tMenuItem;
typedef struct sMenu * tMenu;

typedef struct {
uint16_t NbLine;
uint16_t NbIconeByLine;
uint16_t IconeMarginX;
uint16_t IconeMaxX;
uint16_t IconeSizeX;
uint16_t IconeSpaceX;
uint16_t IconeMarginY;
uint16_t IconeMaxY;
uint16_t IconeSizeY;
uint16_t IconeSpaceY;
uint16_t IconeSelectX;
uint16_t IconeSelectY;

} Menu_Geometry_t;

 typedef struct sMenuItem
{
  char* pszTitle;
  tMenuFunc pfMenuFunc;
  tMenuFunc pfUpDownMenuFunc;
  tMenu psSubMenu;
} sMenuItem;

typedef struct sMenu
{
  char* pszTitle;
  tMenuItem psItems;
  uint8_t nItems;
  const char ** IconsList;
  char* bottomMsg;
  Menu_Callback_t callback;
  Menu_Geometry_t geo;
} sMenu;


/* Exported constants --------------------------------------------------------*/
typedef enum {
  NOKEY=0,
  SEL,
  RIGHT,
  LEFT,
  UP,
  DOWN,
  KEY
} Joystick_button_t;




typedef enum {
  ICONES,
  LIST  
} Menu_Mode_t;

typedef enum {
  MSG_INFO,
  MSG_INFO_ERROR,
  MSG_STATUS_ERROR,
  MSG_STATUS_SUCCESS,
  MSG_INFO_SUCCESS,
  MSG_ABOUT
} Msg_type_t;

typedef enum {
  MENU_OK = 0,
  MENU_ERROR = 1
} Menu_Status_t;

#define MAX_MENU_LEVELS 4
#define MENU_MAX_LINE 100
#define MENU_MAX_CHAR 30

/* Module private variables --------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private functions ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Menu_MainLoop(tMenu main);
void Menu_MsgStatus(const char* Title, const char* Msg, Msg_type_t isError);
uint8_t Menu_GetUserInput(void);
void Menu_DisplayCenterString(uint32_t Line, const char* Str);
uint8_t Menu_DisplayLongString(uint32_t x_label, uint32_t y_label, char *Str, uint32_t maxChar);
void Menu_DisplayKeyboard(char *keyboard, char **spec_char, uint8_t nb_lines , uint8_t nb_columns, uint8_t nb_spaces, uint8_t first_line, int (*fn_callback)(char));
void Menu_DrawProgressBarLine(uint16_t Line, float perc);
uint8_t Menu_SelectImage( const char *img[], const uint8_t nbPicture, const uint32_t *picture_size );
void Menu_DisplayIcons(void);
void Menu_WaitForUser(void);
void Menu_DisplayIconsMenu(void);

#endif /* __MENU_H */

/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
