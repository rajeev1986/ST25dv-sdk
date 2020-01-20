/**
  ******************************************************************************
  * @file    Menu_config.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @ingroup Menu_Config
  * @brief   Menu configuration file
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
#include "Menu_core.h"
#include "Menu_config.h"
#include "st25dx_discovery.h"
#include "st25dx_discovery_ts.h"
#include "st25dx_discovery_lcd.h"
#include "version.h"


/** @defgroup Menu_Demo_Interface ST25DV Menu interface configuration
  * @ingroup  Menu_Demo
  * @brief    Interface file for the menu_demo middleware.
  * @details  This module implements the functions for:
  *           - Accessing the display.
  *           - Reading user inputs: touchscreen, joystick and simple button.
  * @{
  */

/**
  * @brief Interface function to retrieve the display width.
  * @retval Display width in pixels.
  */
uint16_t Menu_GetDisplayWidth(void)
{
  return BSP_LCD_GetXSize();
}

/**
  * @brief  Interface function to retrieve the display height.
  * @retval Display height in pixels.
  */
uint16_t Menu_GetDisplayHeight(void)
{
  return BSP_LCD_GetYSize();
}

/**
  * @brief Interface function to set the display front & back colors.
  * @param style One of the value defined in enum ColorStyles_t.
  */
void Menu_SetStyle(ColorStyles_t style)
{
  switch (style)
  {
    case HEADER:
    case CLEAR_PLAIN:
      BSP_LCD_SetBackColor(LCD_COLOR_BLUEST);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    break;
    case STYLE_SUCCESS:
    case ACTIVE:
      BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    break;
    case STYLE_ERROR:
      BSP_LCD_SetBackColor(LCD_COLOR_RED);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    break;
    case GREY:
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);      
    break;
    case TAB:
      BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
      BSP_LCD_SetTextColor(LCD_COLOR_BLUEST);
    break;
    case CLEAR_TAB:
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    break;
    case PICTURE_GREYSCALE:
      BSP_LCD_SetGreyScale(1);
    break;
    case PICTURE_COLOR:
      BSP_LCD_SetGreyScale(0);
    break;
    case PLAIN:
    default:
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_BLUEST);
    break;
  }
}

/** @brief Interface function to get the FONT height.
  * @retval Font height in pixels.
  */
uint32_t Menu_GetFontHeight(void){
  return BSP_LCD_GetFont( )->Height;
}

/** @brief  Interface function to get the FONT width.
  * @retval Font width in pixels.
  */
uint32_t Menu_GetFontWidth(void){
  return BSP_LCD_GetFont( )->Width;
}

/** @brief Interface function to print a picture (bmp or jpeg) on the display
  * @param PosX Position of the top left corner of the picture on X-axis. 
  * @param PosY Position of the top left corner of the picture on Y-axis.
  * @param pict Pointer to the picture adress in memory.
  */
void Menu_DisplayPicture(uint32_t PosX, uint32_t PosY, const char* pict)
{
  BSP_LCD_DisplayPicture(PosX, PosY, pict);
}

/** @brief Interface function to compute the dimensions of a picture.
  * @param pict Pointer to the picture adress in memory.
  * @param width Picture position on X-axis. 
  * @param height Picture position on Y-axis.
  */
void Menu_GetPictureDim(const char * pict, uint32_t* width, uint32_t* height)
{
  BSP_LCD_GetPictureDim(pict,width,height);
}

/** @brief Interface function to print a rectangle on the display.
  * @param PosX Position of the top left corner of the rectangle on the X-axis.
  * @param PosY Position of the top left corner of the rectangle on the Y-axis. 
  * @param Height Rectangle height.
  * @param Width Rectangle width.
  */
void Menu_DisplayRectangle(uint32_t PosX, uint32_t PosY,  uint32_t Height,  uint32_t  Width)
{
  BSP_LCD_DrawRect(PosX, PosY, Height, Width);
}

/** @brief Interface function to fill a rectangle on the display.
  * @param PosX Position of the top left corner of the rectangle on the X-axis.
  * @param PosY Position of the top left corner of the rectangle on the Y-axis. 
  * @param Height Rectangle height.
  * @param Width Rectangle width.
  */
void Menu_FillRectangle(uint32_t PosX, uint32_t PosY,  uint32_t Height,  uint32_t  Width)
{
  BSP_LCD_FillRect(PosX, PosY, Height, Width);
}

/** @brief Interface function to draw a tab on the display.
  * @param PosX Position of the top left corner of the tab on the X-axis.
  * @param PosY Position of the top left corner of the tab on the Y-axis. 
  * @param Height Tab height.
  * @param Width  Tab width.
  * @param Round  Radius of round corners.
  * @param HeaderHeight Tab header height.
  * @param CurrentTab Currently displayed tab.
  * @param NbTab Number of tabs.
  */
void Menu_DrawTab(uint32_t PosX, uint32_t PosY, uint32_t Width, uint32_t Height, uint32_t Round, uint32_t HeaderHeight, uint32_t CurrentTab, uint32_t NbTab)
{
    BSP_LCD_DrawTab(PosX, PosY, Width, Height, Round, HeaderHeight, CurrentTab, NbTab);
}


/** @brief Interface function to draw tab headers on the display.
  * @param PosX Position of the top left corner of the tab on the X-axis.
  * @param PosY Position of the top left corner of the tab on the Y-axis. 
  * @param Height Tab height.
  * @param Width  Tab width.
  * @param Round  Radius of round corners.
  * @param HeaderHeight Tab header height.
  * @param CurrentTab Currently displayed tab.
  * @param NbTab Number of tabs.
  */
void Menu_DrawTabHeader(uint32_t PosX, uint32_t PosY, uint32_t Width, uint32_t Height, uint32_t Round, uint32_t HeaderHeight, uint32_t CurrentTab, uint32_t NbTab)
{
    BSP_LCD_DrawTabHeader(PosX, PosY, Width, Height, Round, HeaderHeight, CurrentTab, NbTab);
}


/** @brief Interface function to print a string on the display.
  * @param Line Indicates the line number where the string as to be printed.
  * @param Str String to be printed on the display. 
  */
void Menu_DisplayString(uint32_t Line, const char* Str)
{
  BSP_LCD_DisplayStringAtLine(Line, (uint8_t *)Str);
}

/** @brief Interface function to print a text at a specific point on the display.
  * @param PosX Position of the text on X-axis.
  * @param PosY Position of the Text on Y-axis. 
  * @param Str String to be printed on the display. 
  */
void Menu_DisplayStringAt(uint32_t PosX, uint32_t PosY, const char *Str)
{
  BSP_LCD_DisplayStringAt(PosX, PosY, (uint8_t *)Str, LEFT_MODE);
}


/** @brief Interface function to print a character on the display.
  * @param Line Indicates the line number where the string as to be printed.
  * @param Column The position of the character on the line. 
  * @param Ascii Character to be written. 
  */
void Menu_DisplayChar(uint32_t Line, uint32_t Column, uint8_t Ascii)
{
  BSP_LCD_DisplayChar(Line, Column, Ascii);
}

/** @brief Interface function to print a special character on the display.
  * @param Line Indicates the line number where the string as to be printed.
  * @param Column The position of the character on the line. 
  * @param c pointer to the character data. 
  */
void Menu_DisplaySpecChar(uint32_t Line, uint32_t Column, const char *c)
{
  BSP_LCD_DrawCharLine( Line, Column, (uint8_t *)c);
}

/** @brief Interface function to clear the display.
  */
void Menu_DisplayClear(void)
{
  BSP_LCD_Clear(LCD_COLOR_WHITE);
}

/** @brief Interface function to clear a line on display.
  * @param Line Indicates the line number to clear.
  */
void Menu_DisplayClearLine(uint16_t Line)
{
  BSP_LCD_ClearStringLine(Line);
}

/* Interface inputs functions */
/** @brief  Interface function to get the position of a touch on a touchscreen.
  * @param  State Pointer on a Menu_Position_t structure, used to return the X,Y coordinates of last detected touch.
  * @retval Return not-null if a touch has been detected, null otherwise. 
  */
uint8_t Menu_ReadPosition(Menu_Position_t* State)
{
  TS_StateTypeDef TS_State;
  BSP_TS_GetState(&TS_State);

  State->Sel = TS_State.TouchDetected;
  State->X = TS_State.X;
  State->Y = TS_State.Y;

  return TS_State.TouchDetected;

}


/** @brief  Interface function to get the direction and push (select) of a joystick.
  * @param  State Pointer on a Menu_Direction_t structure, used to return the up,down,left,right direction and selection of the joystick.
  * @retval Return null if the joystick is in neutral position, not-null otherwise. 
  */
uint8_t Menu_ReadDirection(Menu_Direction_t* State)
{
  State->Dir = MENU_NEUTRAL;

  if(BSP_PB_GetState(JOYSTICK_UP))
  {
    State->Dir = MENU_UP;
    while(BSP_PB_GetState(JOYSTICK_UP));
  }
  else if (BSP_PB_GetState(JOYSTICK_DOWN)) 
  {
    State->Dir = MENU_DOWN;
    while(BSP_PB_GetState(JOYSTICK_DOWN));
  }
  else if (BSP_PB_GetState(JOYSTICK_LEFT)) 
  {
    State->Dir = MENU_LEFT;
    while(BSP_PB_GetState(JOYSTICK_LEFT));
  }
  else if (BSP_PB_GetState(JOYSTICK_RIGHT)) 
  {
    State->Dir = MENU_RIGHT;
    while(BSP_PB_GetState(JOYSTICK_RIGHT));
  }
  if(BSP_PB_GetState(JOYSTICK_SELECT))
  {
    State->Sel = 1;
    while(BSP_PB_GetState(JOYSTICK_SELECT));
  } else {
    State->Sel = 0;
  }

  return ((State->Dir != MENU_NEUTRAL) || State->Sel);
}

/** @brief  Interface function to get the state of a simple button.
  * @param  State Pointer on a boolean, used to return the state of the button.
  * @retval Return not-null if the button is pushed, not-null otherwise. 
  */
uint8_t Menu_ReadSelection(uint8_t* State)
{
  *State = BSP_PB_GetState(BUTTON_USER);

#ifdef ENABLE_SCREEN_CAPTURE  
  if(*State) BSP_LCD_CaptureScreen();
#endif
  return *State;
}

/** @brief  Interface function to add latence in the menu.
  * @param  duration Latence in ms.
  */
void Menu_Delay(uint32_t duration)
{
  HAL_Delay(duration);
}

/** @}
  */

/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/

