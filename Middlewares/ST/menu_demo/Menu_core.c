/**
  ******************************************************************************
  * @file    menu.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file contains all the functions for the menu navigation
  *          firmware driver.
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
#include "Menu_core.h"
#include "Menu_config.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

/* Private variables ---------------------------------------------------------*/
uint8_t nMenuLevel=0;
struct {tMenu Menu; uint8_t Index;} psPrevMenu[MAX_MENU_LEVELS];
tMenu psCurrentMenu;
uint32_t NxtCurrentIndex = 0;

volatile uint8_t TS_enable = 0;  

extern sMenu MainMenu;

/* Private defines -----------------------------------------------------------*/

#define MenuMaxIconeIndex (NumberOfIcons-1)

/* icon screen are displayed like this */
/* icon are 64x64 jpeg */

/********************************/
/*                              */
/*                              */
/*  =====  =====  =====  =====  */
/*  = 1 =  = 2 =  = 3 =  = 4 =  */
/*  =====  =====  =====  =====  */
/*                              */
/*  =====  =====  =====  =====  */
/*  = 5 =  = 6 =  = 7 =  = 8 =  */
/*  =====  =====  =====  =====  */
/*                              */
/*  =====  =====  =====  =====  */
/*  = 9 =  = 10=  = 11=  = 12=  */
/*  =====  =====  =====  =====  */
/*                              */
/*                              */
/********************************/
uint32_t FontHeight;

/* Private functions -----------------------------------------------------------*/
/**
  * @brief  Initializes the navigation menu.
  * @param  None No parameters.
  * @return None.
  */
static void Menu_Init(tMenu menu)
{
  psCurrentMenu = menu;
  psPrevMenu[0].Menu = psCurrentMenu;
  psPrevMenu[0].Index = 0;
  NxtCurrentIndex = 0;
}

/**
  * @brief  Get the icon size in current Menu.
  * @param  MaxX Max width of icons.
  * @param  MaxY Max height of icons.
  * @return None.
  */
void Menu_GetMaxIconeSize(uint16_t* MaxX, uint16_t* MaxY)
{
  *MaxX = *MaxY = 0;
  for (int i = 0; i<psCurrentMenu->nItems;i++)
  {
    if(&(psCurrentMenu->IconsList[i]) != NULL && (psCurrentMenu->IconsList[i] != NULL))
    {
      uint32_t Width, Height;
      Menu_GetPictureDim(psCurrentMenu->IconsList[i],&Width,&Height);
      *MaxX = Width > *MaxX ? Width : *MaxX;
      *MaxY = Height > *MaxY ? Height : *MaxY;
    }
  }
}

/**
  * @brief  Get the icon size in current Menu.
  * @param  None No parameters.
  * @return Geometry structure of the menu.
  */
Menu_Geometry_t Menu_GetMenu_Geometry(void)
{
  Menu_Geometry_t geo;
#ifndef MENU_DEMO_CENTER_ICONS
  geo.NbLine = 1 + (psCurrentMenu->nItems -1) / 4;
  geo.NbIconeByLine = psCurrentMenu->nItems < 4 ? psCurrentMenu->nItems  : 4;
#endif
  Menu_GetMaxIconeSize(&(geo.IconeSizeX),&(geo.IconeSizeY));
#ifdef MENU_DEMO_CENTER_ICONS
  geo.NbIconeByLine = Menu_GetDisplayWidth() / (geo.IconeSizeX + 8); // this is the max
  geo.NbLine = (psCurrentMenu->nItems / geo.NbIconeByLine) + (psCurrentMenu->nItems % geo.NbIconeByLine ? 1 : 0);
  geo.NbIconeByLine = psCurrentMenu->nItems / geo.NbLine;
#endif
  geo.IconeSpaceX = Menu_GetDisplayWidth();
  geo.IconeSpaceX -= geo.NbIconeByLine * geo.IconeSizeX;
  geo.IconeSpaceX /= (geo.NbIconeByLine+1);
  geo.IconeMarginX = geo.IconeSpaceX;
  
  geo.IconeSpaceY = Menu_GetDisplayHeight() - Menu_GetFontHeight();
  if(psCurrentMenu->bottomMsg != NULL)
  {
    geo.IconeSpaceY -= Menu_GetFontHeight();
  }  

  geo.IconeSpaceY -= geo.NbLine * geo.IconeSizeY;
  geo.IconeSpaceY /= (geo.NbLine+1);
  geo.IconeMarginY = geo.IconeSpaceY + Menu_GetFontHeight();
  geo.IconeSelectX = geo.IconeSizeX + 4;
  geo.IconeSelectY = geo.IconeSizeY + 4;

  geo.IconeMaxX = geo.IconeMarginX + geo.NbIconeByLine*geo.IconeSizeX + ((geo.NbIconeByLine-1)*geo.IconeSpaceX);
  geo.IconeMaxY = geo.IconeMarginY + (geo.NbLine)*geo.IconeSizeY+ ((geo.NbLine-1)*geo.IconeSpaceY);

  return geo;
}

/**
  * @brief  Get the icon size in current Menu.
  * @param  Index Index of icon to get width.
  * @return Width of indexed icon.
  */
static uint32_t Menu_GetIconeX(uint32_t Index)
{
  Menu_Geometry_t geo = psCurrentMenu->geo;
  return (Index%geo.NbIconeByLine)*(geo.IconeSizeX+geo.IconeSpaceX) + geo.IconeMarginX; 
}

/**
  * @brief  Get the icon size in current Menu.
  * @param  Index Index of icon to get height.
  * @return Height of indexed icon.
  */
static uint32_t Menu_GetIconeY(uint32_t Index)
{
  Menu_Geometry_t geo = psCurrentMenu->geo;
  return (Index/geo.NbIconeByLine)*(geo.IconeSizeY+geo.IconeSpaceY) + geo.IconeMarginY; 
}

/**
  * @brief  Get the icon size in current Menu.
  * @param  None No parameters.
  * @retval 0 No input from user.
  * @retval x Any value from different User input.
  */
uint8_t Menu_GetUserInput(void)
{
  Menu_Position_t Pos;
  Menu_Direction_t JoyState;
  uint8_t ButtonState;
   uint8_t Select = 0;
  Select = Menu_ReadPosition(&Pos);
  Select |= Menu_ReadDirection(&JoyState);
  Select |= Menu_ReadSelection(&ButtonState);
  return Select;
  
}

/**
  * @brief  Wait for an action from User.
  * @param  None No parameters.
  * @return None.
  */
void Menu_WaitForUser(void)
{
  while(!Menu_GetUserInput())
  {
     Menu_Delay(10);
  }
}

/**
  * @brief  Display string centered on specified line.
  * @param  Line The line where to display string.
  * @param  Str pointer to string to display on LCD.
  * @return None.
  */
void Menu_DisplayCenterString(uint32_t Line, const char* Str)
{
  uint32_t maxCharPerLine = round((Menu_GetDisplayWidth() / (float)Menu_GetFontWidth()));
  
  if(strlen(Str) > maxCharPerLine)
  {
    return;
  }
  uint32_t startChar = (maxCharPerLine - strlen(Str))/2;
  char tmpStr[MENU_MAX_CHAR];
  for(int i = 0 ; i<startChar ; i++)
  {
    tmpStr[i] = ' ';
  }
  memcpy(tmpStr+startChar,Str,strlen(Str));
  for(int i = startChar + strlen(Str) ; i<maxCharPerLine  ; i++)
  {
    tmpStr[i] = ' ';
  }
  tmpStr[maxCharPerLine+1] = '\0';

  Menu_DisplayString(Line,tmpStr);
}

/**
  * @brief  Format string for a multiline string.
  * @param  MaxChar Maximum of caracters per line.
  * @param  MaxLine Maximum of lines for the string.
  * @param  Str Pointer to string to display on LCD.
  * @param  SizeArr Pointer to array to store Lines size.
  * @return Number of line for the string to display on LCD.
  */
uint32_t Menu_FormatString(uint32_t MaxChar, uint32_t MaxLine, const char* Str, uint32_t *SizeArr)
{
  uint32_t StrLen = strlen(Str);
  uint32_t CurrentPos = 0;
  uint32_t LastBreakPos = 0;
  uint32_t NextBreakPos = 0;
  uint32_t nbLine = 0;
  uint32_t LinePos = 0;
  
  while(nbLine<MaxLine)
  {
    LinePos++;
    if((Str[CurrentPos] == '\f') || (CurrentPos >= StrLen))
    {
      SizeArr[nbLine++] = LinePos - 1;
      break;
    }
    if(Str[CurrentPos] == '\n')
    {
      SizeArr[nbLine++] = LinePos - 1;
      LinePos = 0;
      LastBreakPos = NextBreakPos = ++CurrentPos;
      continue;
    }
    if(LinePos >= MaxChar)
    {
      if(NextBreakPos <= LastBreakPos)
      {
        /* NextBreakPos has'not been updated (no space detected)
           force linebreak */
        NextBreakPos = CurrentPos + 1;
        SizeArr[nbLine++] = NextBreakPos - LastBreakPos;
        LastBreakPos = NextBreakPos;
        LinePos = 0;
      } else {
        SizeArr[nbLine++] = NextBreakPos - LastBreakPos;
        if (Str[NextBreakPos] == ' ') {
          /* Ignore whitespace when splitting the line */
          LastBreakPos = NextBreakPos + 1;
        } else {
          LastBreakPos = NextBreakPos;          
        }
        LinePos = CurrentPos - LastBreakPos + 1;
      }
    }
    if((Str[CurrentPos] == ' ') || (Str[CurrentPos] == '/') || (Str[CurrentPos] == '@'))
    {
      NextBreakPos = CurrentPos;
    }
    CurrentPos++;
  } 

  return nbLine;
}


/**
  * @brief  Display a long string on several lines (max is 10 lines).
  * @param  x_label Position of the text on X-axis.
  * @param  y_label Position of the text on Y-axis.
  * @param  Str pointer to string to display on LCD.
  * @param  maxChar Maximum of caracters per line (max is 30).
  * @return Number of lines displayed.
  */
uint8_t Menu_DisplayLongString(uint32_t x_label, uint32_t y_label, char *Str, uint32_t maxChar)
{
  uint8_t nbLines = 0;
  if (Str != NULL)
  {
    uint32_t sizes[10] = {0};
    nbLines = Menu_FormatString(maxChar,10,Str,sizes);
    for(int line = 0; line < nbLines ; line++)
    {
      char str_line[30] ={0};
      memcpy(str_line, Str, sizes[line]);
      Str += sizes[line];
      if(Str[0] == ' ')
        Str++;
      Menu_DisplayStringAt(x_label, y_label, str_line);
      y_label += Menu_GetFontHeight();
    }
  }
  return nbLines;
}

/**
  * @brief  Display window menu background and title.
  * @param  Title Pointer to string for title.
  * @param  Msg Pointer to string to display message on middle of the screen.
  * @param  MsgType Type of message to display.
  * @return None.
  */
void Menu_MsgStatus(const char* Title, const char* Msg, Msg_type_t MsgType)
{

  uint32_t maxNbLine = round(Menu_GetDisplayHeight() / (float)Menu_GetFontHeight());
  uint32_t maxCharPerLine = round(Menu_GetDisplayWidth() / (float)Menu_GetFontWidth());
   
  const char * MsgPtr = Msg;

  do {  
    Menu_DisplayClear();
    /* Display header */
    if((MsgType == MSG_STATUS_ERROR) || (MsgType == MSG_INFO_ERROR))
      Menu_SetStyle(STYLE_ERROR);
    else if((MsgType == MSG_STATUS_SUCCESS) || (MsgType == MSG_INFO_SUCCESS))
      Menu_SetStyle(STYLE_SUCCESS);
    else
      Menu_SetStyle(HEADER);
    
    if(strlen(Title)<= maxCharPerLine)
      Menu_DisplayCenterString(0,Title);
    else
      Menu_DisplayCenterString(0," ");


    /* Display formatted/adjusted strings on 1 page */
    uint32_t LineSize[MENU_MAX_LINE];
    uint32_t nbLine = Menu_FormatString(maxCharPerLine,maxNbLine-2,MsgPtr,LineSize);
    uint32_t startLine = (maxNbLine - nbLine)/2;
    for(int i=0; i<nbLine;i++ )
    {
      uint32_t displayLine;
      if(*MsgPtr == '!')
      {
        /* this is a title */
        displayLine = 1;
        MsgPtr++;
        LineSize[i]--;
        Menu_SetStyle(CLEAR_PLAIN);
      } else {
        if( MsgType == MSG_ABOUT )
        {
          Menu_SetStyle(CLEAR_PLAIN);
        }
        else
        {
          Menu_SetStyle(PLAIN);
        }
         displayLine = startLine+i;
      }
      /* skip required for empty lines */
      while(((*MsgPtr == '\n') || (*MsgPtr == '\f'))&&(LineSize[i]>0))
      {
        MsgPtr++;
      }
      char tmp[MENU_MAX_CHAR];
      memcpy(tmp,MsgPtr,LineSize[i]);
      tmp[LineSize[i]] = '\0';
      MsgPtr += LineSize[i];
      /* jump over the final space at end of line */
      if((*MsgPtr == ' ') ||(*MsgPtr == '\n') || (*MsgPtr == '\f'))
      {
          MsgPtr++;
      }
      if(*tmp == '<')
      {
        Menu_DisplayString(displayLine,&tmp[1]);
      } else {
        Menu_DisplayCenterString(displayLine,tmp);
      }
    } /*  end of for loop */
    
    /* Wait user action before displaying next page */
    if((MsgType == MSG_STATUS_ERROR)||(MsgType == MSG_INFO_ERROR))
      Menu_SetStyle(STYLE_ERROR);
    else if((MsgType == MSG_STATUS_SUCCESS) || (MsgType == MSG_INFO_SUCCESS))
      Menu_SetStyle(STYLE_SUCCESS);
    else
      Menu_SetStyle(HEADER);
     
    if((MsgType == MSG_STATUS_SUCCESS) || (MsgType == MSG_STATUS_ERROR) || (MsgType == MSG_ABOUT))
    {
     if(*MsgPtr != '\0')
      {
        Menu_DisplayCenterString(maxNbLine-1,"Touch for next page!");
      } else {
        Menu_DisplayCenterString(maxNbLine-1,"Touch to exit!");
      }
      Menu_WaitForUser();     
    } else {
      /*  MSG_INFO */
      Menu_DisplayCenterString(maxNbLine-1," ");

    } 

  } while (*MsgPtr != '\0');
}

/**
  * @brief Displays a keyboard and runs the callback function when a touch is detected.
  * @param keyboard Array of char to be displayed on the keyboard.
  *   @details Unidimensional array, starting with line1 column elements, then line2 column elts,...
  *            Size of the array must be nb_lines * nb_columns.
  *   @note C doesn't support passing a multi-dimension array with a dynamic size as function a parameter.
  * @param nb_lines   Number of lines of the keyboard array.
  * @param nb_columns Number of columns of the keyboard array.
  * @param nb_spaces  Number of space characters to add between 2 elements of the keyboard.
  * @param first_line Defines on which line of the display the keyboard must start.  
  * @param fn_callback Pointer on a callback function of type 'int fn(char)'.
  *   @details  The input char corresponds to the character touched on the keyboard.
  *             The output int corresponds to keep the keyboard (0) or close the keyboard (1).
  * @Return None.
  */
void Menu_DisplayKeyboard(char *keyboard, char **spec_char, uint8_t nb_lines , uint8_t nb_columns, uint8_t nb_spaces, uint8_t first_line, int (*fn_callback)(char))
{
  Menu_SetStyle( PLAIN );
  uint32_t i = 0;
  uint32_t speccharcount = 0;
  uint32_t speccharline[10];
  uint32_t speccharcolumn[10];
  uint32_t maxCharPerLine = round((Menu_GetDisplayWidth() / (float)Menu_GetFontWidth()));
  uint32_t startmsg = 0;
 
  /* First: draw the keyboard */
  for(int line_index = 0; line_index < nb_lines; line_index++)
  {
    char msg[20];
    int columns_index = 0;
    speccharcount = 0;
    /* Start the line with the first keyboard element */
    msg[columns_index] = keyboard[line_index*nb_columns];
    if( (msg[columns_index] < ' ') || (msg[columns_index] > '^') )
    {
      speccharcolumn[speccharcount] = columns_index;
      speccharline[speccharcount] = first_line + line_index*2;
      speccharcount++;
    }
    for(columns_index = 1; columns_index < nb_columns; columns_index++)
    {
     /* Continue the line with the spaces between the keyboard elements */
      int k = 0;
      for(k = 0; k < nb_spaces ; k++)
      {
        msg[((columns_index-1)*(nb_spaces+1) + 1) + k] = ' ';
      }
      /* Add nest keyboard element */
     msg[((columns_index-1)*(nb_spaces+1) + 1) + k] = keyboard[line_index*nb_columns + columns_index];
      
     if( (keyboard[line_index*nb_columns + columns_index] < ' ') || (keyboard[line_index*nb_columns + columns_index] > '^') )
     {
       speccharcolumn[speccharcount] = ((columns_index-1)*(nb_spaces+1) + 1) + k;
       speccharline[speccharcount] = first_line + line_index*2;
       speccharcount++;
     }
    }
    
    /* Complete the string with a Null char */
    msg[((columns_index-1)*(nb_spaces+1) + 1)] = '\0';
    /* The keyboards elements must not be on consecutive lines, to avoid touch errors  */
    
    Menu_DisplayCenterString( first_line + line_index*2, msg );
    for( i = 1; i <= speccharcount; i++)
    {
      startmsg = (maxCharPerLine - strlen(msg))/2;
      Menu_DisplaySpecChar( speccharline[i - 1], startmsg + speccharcolumn[i - 1], spec_char[i - 1] );
    }
  }
  
  /* Second: Compute keyboard geometry */
  uint32_t left_border    = (Menu_GetDisplayWidth()/2) - ((nb_spaces+1) * nb_columns * Menu_GetFontWidth() / 2);
  uint32_t right_border   = (Menu_GetDisplayWidth()/2) + ((nb_spaces+1) * nb_columns * Menu_GetFontWidth() / 2 );
  uint32_t top_border     = (first_line * Menu_GetFontHeight() - Menu_GetFontHeight()/2);
  uint32_t bottom_border  = (first_line + nb_lines) * 2 * Menu_GetFontHeight() + Menu_GetFontHeight()/2;
  
  /* Third: Wait user input and run the callback. */
  while(1)
  {
    
    Menu_Position_t Pos;
    uint8_t touch = 0;

    while(touch == 0)
    {
      if(Menu_ReadPosition(&Pos) != 0)
      {
        if (((Pos.X > left_border) &&
            (Pos.X < right_border) &&
            (Pos.Y > top_border ) &&
            (Pos.Y < bottom_border ) ) ||
            (Pos.Y > (Menu_GetDisplayHeight() - Menu_GetFontHeight()) ) )
        {
          touch = 1;
          /* To avoid double touches */
          Menu_Delay(200);
        }
      }
    }
    if (Pos.Y > (Menu_GetDisplayHeight() - Menu_GetFontHeight()) )
    {
      if(fn_callback(0x1B))
      {
        break;
      }
    }
    else
    {
      /* Forth: Call the callback function with touched char as parameter */
      uint16_t line_index   = (Pos.Y - top_border) / (2 * Menu_GetFontHeight()) ;
      uint16_t column_index = (Pos.X - left_border) / ((nb_spaces+1) * Menu_GetFontWidth()) ;
      if(keyboard[line_index*nb_columns + column_index] == '@')
      {
        if(fn_callback('>'))
        {
          break;
        }
      }
      else
      {
        if(fn_callback(keyboard[line_index*nb_columns + column_index]))
        {
          break;
        }
      }
    }
  }
}

/**
  * @brief  Draw a progress bar on screen at specific Line position.
  * @param  Line The line to position the bar.
  * @param  perc percentage filled in the bar.
  * @return None.
  */
void Menu_DrawProgressBarLine(uint16_t Line, float perc)
{
  static uint32_t prev_pix = 0;
  uint32_t target_pix;
  uint32_t width = Menu_GetDisplayWidth() * 0.8;
  uint32_t height = Menu_GetDisplayHeight() / 20;
  uint32_t pos_x = (Menu_GetDisplayWidth() - width) / 2;
  uint32_t pos_y  = (Line*Menu_GetFontHeight());
  
  Menu_SetStyle(PLAIN);
  
  /* Max Progress bar size is 100% */
  if(perc > 100)
  {
    perc = 100;
  }

  /* This is a new bar */
  if( perc == 0 )
  {
    prev_pix = 0;
    Menu_DisplayRectangle( pos_x, pos_y, width, height );
  } 

  target_pix = (perc*width)/100;

  if (prev_pix < (target_pix + 1))
  {
    for(; prev_pix <= target_pix; prev_pix += 1)
    {
      Menu_DisplayRectangle( prev_pix+ pos_x, pos_y, 1, height );
    }
  } else if(prev_pix > (target_pix + 1)) {
    /* Percentage decreased */
    Menu_SetStyle(CLEAR_PLAIN);
    for(; prev_pix > target_pix; prev_pix -= 1)
    {
      Menu_DisplayRectangle( prev_pix+ pos_x, pos_y+1, 1, height-2 );
    }
    Menu_SetStyle(PLAIN);
  }
}

/**
  * @brief  Screen interface to select picture to send by FTM.
  * @param  Picture Pointer to array of picture pointers.
  * @param  nbPicture Number of pictures available in array.
  * @return Index array of the picture.
  */
uint8_t Menu_SelectImage( const char *Picture[], const uint8_t nbPicture, const uint32_t *picture_size )
{
  uint8_t exitselimg = 0;
  uint8_t idx = 0;
  uint8_t Select = 0;
  uint32_t dx = 0;
  uint32_t dy = 0;
  uint32_t centerline = 0;
  uint32_t iwidth = 0;
  uint32_t iheight = 0;
  uint32_t cnt = 0;
  Menu_Position_t Pos;
  char msg[20];
  
  dx = Menu_GetDisplayWidth( );
  dy = Menu_GetDisplayHeight( );
  centerline = round((dy / (float)Menu_GetFontHeight( )) / 2);
  
  Menu_MsgStatus( "Select Picture", "", MSG_INFO);
  Menu_DisplayCenterString( (centerline * 2), "<--");
  Menu_SetStyle(PLAIN);
  
  
  Menu_GetPictureDim( *Picture, &iwidth, &iheight );
  Menu_DisplayPicture( ((dx - iwidth) / 2), ((dy - iheight) / 2), *Picture );
  Menu_DisplayChar(centerline, 0, ' ');
  Menu_DisplayChar(centerline, 19, '>');

  if(picture_size != NULL)
  {
    sprintf(msg,"%lu Bytes",picture_size[0]);
    Menu_DisplayCenterString(((centerline * 2) - 1), msg);
  }
  while( exitselimg == 0 )
  {
    Select = Menu_ReadPosition(&Pos);
    if( Select != 0 )
    {
      /* touchscreen has been touched */
      if( (Pos.X < 40) || (Pos.X > 280) )
      {
        uint8_t update_picture = 0;
        if( (Pos.X < 40) && (idx > 0) )
        { /* Left side touched */
          idx--;
          update_picture = 1;
        }
        else if( (Pos.X > 280) && (idx < (nbPicture - 1) ) )
        { /* Right side touched */
          idx++;
          update_picture = 1;
        }

        if(update_picture == 1)
        {
          /* Clear picture area */
          for( cnt = 1; cnt < 10; cnt++ )
          {
            Menu_DisplayString( cnt , "                    " );
          }
        
          if( idx == 0 )
          {
            Menu_DisplayChar(centerline, 0, ' ');
            Menu_DisplayChar(centerline, 19, '>');
          }
          else if( idx == (nbPicture - 1) )
          {
            Menu_DisplayChar(centerline, 0, '<');
            Menu_DisplayChar(centerline, 19, ' ');
          }
          else
          {
            Menu_DisplayChar(centerline, 0, '<');
            Menu_DisplayChar(centerline, 19, '>');
          }

          Menu_GetPictureDim( &(*Picture[idx]), &iwidth, &iheight );
          Menu_DisplayPicture( ((dx - iwidth) / 2), ((dy - iheight) / 2), &(*Picture[idx]) );
          if(picture_size != NULL)
          {
            sprintf(msg,"%lu Bytes",picture_size[idx]);
            Menu_DisplayCenterString(((centerline * 2) - 1), msg);
          }
       } /* update_picture */
      }
      
      
      if( (Pos.X > 50) && (Pos.X < 270) && (Pos.Y > 200) )
      { /* Back touched */
        return 0xFF;
      }
      
      if( (Pos.X > ((dx - iwidth) / 2)) && (Pos.X < (((dx - iwidth) / 2)) + iwidth) && \
          (Pos.Y > ((dy - iheight) / 2)) && (Pos.Y < (((dy - iheight) / 2)) + iheight) )
      { /* Picture touched */
        exitselimg = 1;
      }

      /* To avoid double touches */
      while(Menu_ReadPosition(&Pos));
      Menu_Delay(250);

    }
  }
  
  return idx;
}

/**
  * @brief  Display menu icons.
  * @param  None.
  * @return None.
  */
void Menu_DisplayIcons(void)
{
  uint32_t i = 0;
  
  for(i=0; i<psCurrentMenu->nItems; i++)
  {
    if(  psCurrentMenu->IconsList[i] != NULL)
    {
      // icons with no action are displayed in gray scale
      uint8_t useGreyScale = ( /* psCurrentMenu->psItems[i].psSubMenu->callback == NULL) && */ (psCurrentMenu->psItems[i].pfMenuFunc == NULL));
      if(useGreyScale)
      {
        Menu_SetStyle(PICTURE_GREYSCALE);
      }
      Menu_DisplayPicture(Menu_GetIconeX(i), Menu_GetIconeY(i), psCurrentMenu->IconsList[i]);
      if(useGreyScale)
      {
        Menu_SetStyle(PICTURE_COLOR);
      }
    }
  }
}

/**
  * @brief  Clear selected menu.
  * @param  CurrentIndex Current index of icon on the menu to clear.
  * @param  MenuMode Type of current menu.
  * @return None.
  */
static void Menu_ClearSelected(uint32_t CurrentIndex, Menu_Mode_t MenuMode)
{
  Menu_Geometry_t geo;
  switch(MenuMode)
  {
    case ICONES:
      geo = psCurrentMenu->geo;
      Menu_SetStyle(CLEAR_PLAIN);
      Menu_DisplayRectangle(Menu_GetIconeX(CurrentIndex)-2, Menu_GetIconeY(CurrentIndex)-2, geo.IconeSelectX, geo.IconeSelectY);
    break;
    case LIST:
      Menu_SetStyle(PLAIN);
      Menu_DisplayCenterString((CurrentIndex+1)*2, psCurrentMenu->psItems[CurrentIndex].pszTitle);
    break;
  }
}

/**
  * @brief  Display title and selection on menu.
  * @param  CurrentIndex Current index of icon on the menu to clear.
  * @param  MenuMode Type of current menu.
  * @return None.
  */
static void Menu_DisplaySelected(uint32_t CurrentIndex, Menu_Mode_t MenuMode)
{
  Menu_Geometry_t geo;

  switch(MenuMode)
  {
    case ICONES:
      geo = psCurrentMenu->geo;
      Menu_SetStyle(HEADER);
      Menu_DisplayCenterString(0, psCurrentMenu->psItems[CurrentIndex].pszTitle);
      Menu_SetStyle(PLAIN);
      Menu_DisplayRectangle(Menu_GetIconeX(CurrentIndex)-2, Menu_GetIconeY(CurrentIndex)-2, geo.IconeSelectX, geo.IconeSelectY);

    break;
    case LIST:
      Menu_SetStyle(ACTIVE);
      Menu_DisplayCenterString((CurrentIndex+1)*2, psCurrentMenu->psItems[CurrentIndex].pszTitle);
    break;

  }

}

/**
  * @brief  Get index of menu from a position.
  * @param  State Current position of the menu.
  * @param  MenuMode Type of current menu.
  * @param  MaxIndex Max number of menu.
  * @param  ResIndex Result index.
  * @retval 0 No index at position.
  * @retval 1 Index found.
  */
uint8_t Menu_Position2Index(Menu_Position_t State, Menu_Mode_t MenuMode,uint32_t MaxIndex, uint32_t* ResIndex)
{
  uint32_t Res;
  Menu_Geometry_t geo;
  switch (MenuMode)
  {
    case ICONES:
      geo = psCurrentMenu->geo;
      /* Check if the touch is in the active window */
      if((State.X < geo.IconeMarginX) || (State.X > geo.IconeMaxX) || (State.Y < geo.IconeMarginY) || (State.Y > geo.IconeMaxY))
        return 0;

      Res = (State.X - geo.IconeMarginX)/(geo.IconeSizeX+geo.IconeSpaceX) + ((State.Y-geo.IconeMarginY)/(geo.IconeSizeY+geo.IconeSpaceY) * geo.NbIconeByLine);
    break;
    case LIST:
      /* header, blank line, option1, blank line, option2, blank line, ...  */
      if((State.Y>FontHeight) && (State.Y<((3*FontHeight)/2)))
        Res = 0;
      else
        Res = (State.Y - ((3*FontHeight)/2))/(2 * FontHeight) + 0;
    break;  
    default:
      return 0;
  }
  if(Res <= (MaxIndex))
  {  
    *ResIndex = Res;
    return 1;
  } else { 
    return 0;
  }
}

/**
  * @brief  Get index of menu from a direction.
  * @param  State Current position of the menu.
  * @param  MenuMode Type of current menu.
  * @param  MaxIndex Max number of menu.
  * @param  ResIndex Result index.
  * @retval 0 No index at position.
  * @retval 1 Index found.
  */
uint8_t Menu_Direction2Index(Menu_Direction_t State, Menu_Mode_t MenuMode, uint32_t MaxIndex, uint32_t* ResIndex)
{
  Menu_Geometry_t geo;

  switch (MenuMode)
  {
    case ICONES:
      geo = psCurrentMenu->geo;
      switch(State.Dir)
      {
        case MENU_UP:
          do { *ResIndex = (*ResIndex < geo.NbIconeByLine) ?
                      *ResIndex + MaxIndex +1 - geo.NbIconeByLine :
                      *ResIndex - geo.NbIconeByLine;
          } while(psCurrentMenu->IconsList[*ResIndex] == NULL);
        break;
        case MENU_DOWN:
          do {*ResIndex = (*ResIndex >= (1+ MaxIndex - geo.NbIconeByLine)) ?
                      *ResIndex + geo.NbIconeByLine - MaxIndex - 1:
                      *ResIndex + geo.NbIconeByLine;
          } while(psCurrentMenu->IconsList[*ResIndex] == NULL);

        break;
        case MENU_LEFT:
          do { *ResIndex = (*ResIndex == 0) ? MaxIndex:
                      *ResIndex - 1 ;
          } while(psCurrentMenu->IconsList[*ResIndex] == NULL);
        break;
        case MENU_RIGHT:
          do {*ResIndex = (*ResIndex == MaxIndex) ?
                      0 :
                      *ResIndex + 1 ;
          } while(psCurrentMenu->IconsList[*ResIndex] == NULL);
        break;
        case MENU_NEUTRAL:
        default:
        break;
      }
    break;
    case LIST:
      switch(State.Dir)
      {
        case MENU_UP:
          *ResIndex = (*ResIndex == 0) ?
                      MaxIndex :
                      *ResIndex - 1 ;
        break;
        case MENU_DOWN:
          *ResIndex = (*ResIndex == MaxIndex) ?
                      0 :
                      *ResIndex + 1 ;
        break;
        case MENU_LEFT:
        case MENU_RIGHT:
        case MENU_NEUTRAL:
        default:
        break;
      }
    break;
    default:
      return 0;
  }
  return State.Sel;
}

/**
  * @brief  Show the main menu icon.
  * @param  None No parameters.
  * @return None.
  */
void Menu_DisplayIconsMenu(void)
{
  uint32_t maxNbLine = Menu_GetDisplayHeight() / Menu_GetFontHeight();

  /* Clear*/
  Menu_DisplayClear();
  Menu_SetStyle(HEADER);

  Menu_DisplayCenterString(0, psCurrentMenu->pszTitle);
  if(psCurrentMenu->bottomMsg != NULL)
  {
    Menu_DisplayCenterString(maxNbLine, psCurrentMenu->bottomMsg);
  }
  /* Displays Icons */    
  Menu_DisplayIcons();

}



/**
  * @brief  Displays the current menu.
  * @param  None No parameters.
  * @retval None.
  */
static void Menu_DisplayListMenu(void)
{
  uint32_t Line = 0, index = 0;
  
  /* Clear the LCD Screen */
  Menu_DisplayClear();

  Menu_SetStyle(HEADER);
  Menu_DisplayCenterString(0, psCurrentMenu->pszTitle);
  Line += 2;

  Menu_SetStyle(PLAIN);

  while(index < psCurrentMenu->nItems)
  {
    if(index == NxtCurrentIndex) Menu_SetStyle(ACTIVE);
    Menu_DisplayCenterString(Line, psCurrentMenu->psItems[index].pszTitle);
    if(index == NxtCurrentIndex) Menu_SetStyle(PLAIN);
    index++;
    Line += 2;
  }
}

/**
  * @brief  Return to previous menu.
  * @param  None No parameters.
  * @retval None.
  */
void ReturnFunc(void)
{
  nMenuLevel--;
  psCurrentMenu = psPrevMenu[nMenuLevel].Menu;
  NxtCurrentIndex = psPrevMenu[nMenuLevel].Index;
}

/**
  * @brief  Main loop to display menumenu.
  * @param  main Menu to display.
  * @retval None.
  */
void Menu_MainLoop(tMenu main)
{
  /* Input state handlers */
  Menu_Position_t TouchState;
  Menu_Direction_t JoyState;
  uint8_t ButtonState;
  
  /* Menu state vars */
  uint32_t CurrentIndex = 0;  
  uint8_t NxtSelect = 0;
  Menu_Callback_Status_t NxtLast = MENU_CALLBACK_CONTINUE;

  Menu_Mode_t MenuMode = ICONES;  
  FontHeight = Menu_GetFontHeight();
  
  /* Initializes the Menu global vars */
  Menu_Init(main);
  psCurrentMenu->geo = Menu_GetMenu_Geometry();
  
  /* Display initial menu */
  Menu_DisplayIconsMenu();
  Menu_DisplaySelected(0,ICONES);

  /* Main loop */
  while(1)
  {
    /* Manage inputs: TS, Joystick & Button */
    NxtSelect = 0;
    uint8_t TouchDetected = 0;
    if(Menu_ReadPosition(&TouchState))
    {
      NxtSelect |= Menu_Position2Index(TouchState,MenuMode, psCurrentMenu->nItems-1,&NxtCurrentIndex);
      TouchDetected = 1;
    }
    if(Menu_ReadDirection(&JoyState))
    {
      NxtSelect |= Menu_Direction2Index(JoyState, MenuMode, psCurrentMenu->nItems-1,&NxtCurrentIndex);
    }
    NxtSelect |= Menu_ReadSelection(&ButtonState);
    /* Callback function may also report an input, just like a button, to return to previous menu for instance */
    if(psCurrentMenu->callback != NULL)
    {
      NxtLast = psCurrentMenu->callback();
      NxtSelect |= (NxtLast == MENU_CALLBACK_LAST);
    } else {
      // no callback: add delays
      Menu_Delay(10);
    }

    /* Update the display */ 
    if((NxtCurrentIndex < psCurrentMenu->nItems) && (psCurrentMenu->IconsList[NxtCurrentIndex] != NULL))
    {
      if(CurrentIndex != NxtCurrentIndex)
      {
        Menu_ClearSelected(CurrentIndex,MenuMode);
        Menu_DisplaySelected(NxtCurrentIndex,MenuMode);
        CurrentIndex = NxtCurrentIndex;
      }
      if(TouchDetected && (psCurrentMenu->callback == NULL))
      {
        Menu_Delay(250);
        while (Menu_ReadPosition(&TouchState))
        {
          Menu_Delay(250);
          /* de-select after displaying icon help */
          NxtSelect = 0;
        }
      }

      if(NxtSelect)
      {
        // disable Menu redraw when the demo is done using the Menu callback 
        uint8_t redraw = psCurrentMenu->callback == NULL;
        tMenuFunc  execFunc = psCurrentMenu->psItems[CurrentIndex].pfMenuFunc;
        if(psCurrentMenu->psItems[CurrentIndex].psSubMenu != NULL)
        {
          /* Update the current Item by the submenu */
          psPrevMenu[nMenuLevel].Index = CurrentIndex;
          psCurrentMenu = psCurrentMenu->psItems[CurrentIndex].psSubMenu;
          nMenuLevel++;
          NxtCurrentIndex = 0;
          psPrevMenu[nMenuLevel].Menu = psCurrentMenu;
          psCurrentMenu->geo = Menu_GetMenu_Geometry();
        }
        // don't run menu function when notified by callback 
        if((execFunc != NULL) && (NxtLast != MENU_CALLBACK_LAST))
          execFunc();
      
        if(NxtLast == MENU_CALLBACK_LAST)
        {
          // this is the last callback iteration, prepare to return to previous menu
          ReturnFunc();
          redraw = 1;
          NxtLast = MENU_CALLBACK_CONTINUE;
        }

        /* Level 0 displays icons, while sub-menus use option list */
        if(psCurrentMenu->IconsList != NULL)
        {
          /* Used by the Touchscreen handler to compute NxtCurrentIndex */
          MenuMode = ICONES;
          if(redraw)
          {
            Menu_DisplayIconsMenu();
          }
        } 
        else 
        {
          MenuMode = LIST;
          if(redraw)
          {
            Menu_DisplayListMenu();
          }
        }
        Menu_DisplaySelected(NxtCurrentIndex, MenuMode);
        CurrentIndex = NxtCurrentIndex;
      }   /* If NxtSelect */
    } /* NxtCurrentIndex < psCurrentMenu->nItems */
  } /* while(1) */
}
