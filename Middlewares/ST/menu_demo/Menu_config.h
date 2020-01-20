/**
  ******************************************************************************
  * @file    Menu_config.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @ingroup Menu_Config
  * @brief   Menu configuration header file
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
#ifndef __MENU_CONFIG_H__
#define __MENU_CONFIG_H__

#include "Menu_core.h"
#include "stdint.h"

/** @addtogroup Menu_Demo_Interface
  * @ingroup  Menu_Demo
  * @{
  */

typedef enum {
  HEADER,       /** < Style used for headers. */
  PLAIN,        /** < Style used for plain text */
  CLEAR_PLAIN,  /** < Style used to clear plain text */
  ACTIVE,       /** < Style used to show active item in a list */
  STYLE_ERROR,  /** < Style used to report errors */
  STYLE_SUCCESS, /** < Style used to report success */
  PICTURE_GREYSCALE,
  PICTURE_COLOR,
  GREY,
  PLOT_GRID,
  PLOT_AXES,
  TICKBOX_CHECK,
  TAB,
  CLEAR_TAB
} ColorStyles_t;


typedef struct  { uint32_t X; /** < Position of detected touch on X-axis  */
                  uint32_t Y; /** < Position of detected touch on Y-axis  */
                  uint8_t Sel; /** < Boolean: true if a touch is detected  */
                } Menu_Position_t;

                typedef struct {
  enum {MENU_NEUTRAL=0,
        MENU_UP,
        MENU_DOWN,
        MENU_LEFT,
        MENU_RIGHT} Dir;
  uint8_t             Sel; } Menu_Direction_t;

void Menu_SetStyle(ColorStyles_t style);
void Menu_GetPictureDim(const char * pict, uint32_t* width, uint32_t* height);
void Menu_DisplayPicture(uint32_t PosX, uint32_t PosY, const char* pict);
void Menu_DisplayRectangle(uint32_t PosX, uint32_t PosY,  uint32_t Height,  uint32_t  Width);
void Menu_DisplayString(uint32_t Line, const char* Str);
void Menu_DisplayChar(uint32_t Line, uint32_t Column, uint8_t Ascii);
void Menu_DisplaySpecChar(uint32_t Line, uint32_t Column, const char *c);
void Menu_DisplayStringAt(uint32_t PosX, uint32_t PosY, const char *Str);
void Menu_DisplayClear(void);
void Menu_DisplayClearLine(uint16_t Line);
uint32_t Menu_GetFontHeight(void);
uint32_t Menu_GetFontWidth(void);
uint16_t Menu_GetDisplayWidth(void);
uint16_t Menu_GetDisplayHeight(void);
void Menu_Delay(uint32_t duration);
void Menu_FillRectangle(uint32_t PosX, uint32_t PosY,  uint32_t Height,  uint32_t  Width);
void Menu_DrawTab(uint32_t PosX, uint32_t PosY, uint32_t Width, uint32_t Height, uint32_t Round, uint32_t HeaderHeight, uint32_t CurrentTab, uint32_t NbTab);
void Menu_DrawTabHeader(uint32_t PosX, uint32_t PosY, uint32_t Width, uint32_t Height, uint32_t Round, uint32_t HeaderHeight, uint32_t CurrentTab, uint32_t NbTab);

/* inputs */
uint8_t Menu_ReadPosition(Menu_Position_t* State);
uint8_t Menu_ReadDirection(Menu_Direction_t* State);
uint8_t Menu_ReadSelection(uint8_t* State);

/** @}
  */
  
#endif //  __MENU_CONFIG_H__
