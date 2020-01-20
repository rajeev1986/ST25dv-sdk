/**
  ******************************************************************************
  * @file    st25dx_discovery_lcd.c 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file includes the LCD driver for ILI9341 Liquid Crystal
  *          Display Modules of ST25DX-Discovery kit.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
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

/* File Info : -----------------------------------------------------------------
                                   User NOTES
1. How To use this driver:
--------------------------
   - This driver is used to drive directly an LCD using SPI communication.
   - This driver select dynamically the mounted LCD, ILI9341 240x320 LCD mounted
     on MB1283 discovery board, and use the adequate timing and setting for
	 the specified LCD using device ID of the ILI9341 mounted on MB1283 discovery board

2. Driver description:
---------------------
  + Initialization steps :
     o Initialize the LCD using the LCD_Init() function.
     o Enable the LCD display using LCD_DisplayOn() function.


  + Display on LCD
      o Clear the whole LCD using LCD_Clear() function or only one specified string
        line using LCD_ClearStringLine() function.
      o Display a character on the specified line and column using LCD_DisplayChar()
        function or a complete string line using LCD_DisplayStringAtLine() function.
      o Display a string line on the specified position (x,y in pixel) and align mode
        using BSP_LCD_DisplayStringAt() function.
      o Draw and fill a basic shapes (dot, line, rectangle, circle, ellipse, .. bitmap)
        on LCD using the available set of functions.
 
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "st25dx_discovery_lcd.h"
#include "st25dx_discovery_bus.h"
#include "jpeg_decode.h"
#include "math.h"
#ifdef ENABLE_SCREEN_CAPTURE
  /* Flash is used to store LCD screen capture */
  #include "fw_command.h"
#endif
#include <stdlib.h>

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup ST25DX_DISCOVERY_BSP
  * @{
  */

/** @defgroup ST25DX_DISCOVERY_LCD ST25DX DISCOVERY LCD
  * @brief This file includes the LCD driver for (ILI9341)
  * @{
  */

#define POLY_X(Z)              ((int32_t)((Points + (Z))->X))
#define POLY_Y(Z)              ((int32_t)((Points + (Z))->Y))
#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)<(b))?(a):(b))
#define ABS(X)  ((X) > 0 ? (X) : -(X))

static uint8_t Is_LCD_IO_Initialized = 0;

LCD_DrawPropTypeDef DrawProp;
static LCD_DrvTypeDef  *LcdDrv;
static uint8_t enableGreyScale;

static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c);
static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3);

/**
  * @brief  Initializes the LCD.
  * @retval LCD state
  */
uint8_t BSP_LCD_Init(void)
{ 
  /* On ST25-Discovery, it is not possible to read ILI9341 ID because */
  /* PIN EXTC is not connected to VDD and then LCD_READ_ID4 is not accessible. */
  /* In this case, ReadID function is bypassed.*/
  /*if (ili9341_drv.ReadID() == ILI9341_ID)*/

    /* Select the device */
    LcdDrv = &ili9341_drv;

    /* LCD Init */
    LcdDrv->Init();

    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetColors(LCD_COLOR_WHITE, LCD_COLOR_BLACK);

  return LCD_OK;
}

/**
  * @brief  Initializes the LCD.
  */
/**
  * @brief  Configures the LCD_SPI interface.
  */
void BSP_LCD_IO_Init(void)
{
  GPIO_InitTypeDef gpio_init;

  if (Is_LCD_IO_Initialized == 0)
  {
    Is_LCD_IO_Initialized = 1;

    /* Configure WRX in Output Push-Pull mode */
    BUS_LCD_WRX_GPIO_CLK_ENABLE();
    gpio_init.Pin     = BUS_LCD_WRX_PIN;
    gpio_init.Mode    = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull    = GPIO_NOPULL;
    gpio_init.Speed   = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BUS_LCD_WRX_GPIO_PORT, &gpio_init);

    /* Configure the LCD Control pins ----------------------------------------*/
    BSP_SPI2_Init();
  }
}
/**
  * @brief  Configures the LCD_SPI interface.
  */
void BSP_LCD_IO_SetDataWidth(uint32_t w16)
{
  if (w16 != hbus_spi2.Init.DataSize)
  {
    if (w16 == SPI_DATASIZE_16BIT)
    {
      MX_SPI2_Init(&hbus_spi2, hbus_spi2.Init.BaudRatePrescaler, SPI_DATASIZE_16BIT);
    }
    else
    {
      MX_SPI2_Init(&hbus_spi2, hbus_spi2.Init.BaudRatePrescaler, SPI_DATASIZE_8BIT);
    }
  }
}

/**
  * @brief  Writes 8 bits register address.
  */
void BSP_LCD_IO_WriteReg(uint8_t Reg)
{
  BSP_SPI2_SetDataWidth(SPI_DATASIZE_8BIT);

  /* Reset WRX to send command */
  BSP_LCD_WRX_LOW();

  /* Reset LCD control line(/CS) and Send command */
  BSP_LCD_CS_LOW();
  BSP_SPI2_Send(&Reg, 1);

  /* Deselect: Chip Select high */
  BSP_LCD_CS_HIGH();
}

/**
  * @brief  Writes writes 8 bits data value.
  */
void BSP_LCD_IO_WriteData(uint8_t RegValue)
{
  BSP_SPI2_SetDataWidth(SPI_DATASIZE_8BIT);

  /* Set WRX to send data */
  BSP_LCD_WRX_HIGH();

  /* Reset LCD control line(/CS) and Send data */
  BSP_LCD_CS_LOW();
  BSP_SPI2_Send(&RegValue, 1);

  /* Deselect: Chip Select high */
  BSP_LCD_CS_HIGH();
}

/**
  * @brief  Writes 16bits data value.
  */
void BSP_LCD_IO_WriteData16(uint16_t RegValue)
{
  BSP_SPI2_SetDataWidth(SPI_DATASIZE_16BIT);

  /* Set WRX to send data */
  BSP_LCD_WRX_HIGH();

  /* Reset LCD control line(/CS) and Send data */
  BSP_LCD_CS_LOW();
  BSP_SPI2_Send((uint8_t *)&RegValue, 1);

  /* Deselect: Chip Select high */
  BSP_LCD_CS_HIGH();
}

/**
  * @brief  Writes 16bits data value.
  */
void BSP_LCD_IO_WriteBuffer16(uint16_t* buffer, uint32_t length)
{
  BSP_SPI2_SetDataWidth(SPI_DATASIZE_16BIT);

  /* Set WRX to send data */
  BSP_LCD_WRX_HIGH();

  /* Reset LCD control line(/CS) and Send data */
  BSP_LCD_CS_LOW();
  BSP_SPI2_Send((uint8_t*)buffer, length);

  /* Deselect: Chip Select high */
  BSP_LCD_CS_HIGH();
}

/**
  * @brief  Reads register value.
  * @param  RegValue Address of the register to read
  * @param  ReadSize Number of bytes to read (Max = 4)
  * @retval Content of the register value
  */
uint32_t BSP_LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize)
{
  uint32_t readvalue = 0;

  if (ReadSize > sizeof(readvalue))
  {
    ReadSize = 4;
  }

  BSP_SPI2_SetDataWidth(SPI_DATASIZE_8BIT);

  /* Reset WRX to send command */
  BSP_LCD_WRX_LOW();

  /* Select: Chip Select low */
  BSP_LCD_CS_LOW();

  BSP_SPI2_Send((uint8_t *)&RegValue, 1);
  BSP_SPI2_Recv((uint8_t *)&readvalue, ReadSize);

  /* Deselect: Chip Select high */
  BSP_LCD_CS_HIGH();

  /* Set WRX to send data */
  BSP_LCD_WRX_HIGH();

  /* Changing endianess */
  readvalue = ((readvalue & 0xff)       << 24) |
              ((readvalue & 0xff00)     << 8)  |
              ((readvalue & 0xff0000)   >> 8)  |
              ((readvalue & 0xff000000) >> 24);

  return readvalue;
}

/**
  * @brief  Wait for loop in ms.
  * @param  Delay in ms.
  */
void BSP_LCD_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

#ifdef ENABLE_SCREEN_CAPTURE
/**
  * @brief  Reads the LCD memory and copy it to the internal flash, used to do screen captures.
  * @retval None.
  */
void BSP_LCD_CaptureScreen(void)
{
  uint32_t width = BSP_LCD_GetXSize();
  uint32_t height = BSP_LCD_GetYSize();

  /* Lower the SPI freq to enable reads */
  BSP_SPI2_LowFreq();

  /* Prepare the flash */
  COMMAND_EraseFlash(FIRMWARE_ADDRESS);

  uint16_t buffer[320];
  for (int y = 0 ; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
        buffer[x] =  LcdDrv->ReadPixel(x,y);
    }
    Command_WriteBufferToFlash(FIRMWARE_ADDRESS, (y * sizeof(buffer)), (uint8_t*)buffer, sizeof(buffer));
 }
  /* Set back the SPI freq for write only */
 BSP_SPI2_HighFreq();

}
#endif

/**
  * @brief  Gets the LCD X size.
  * @retval The used LCD X size
  */
uint32_t BSP_LCD_GetXSize(void)
{
  return LcdDrv->GetLcdPixelWidth();
}

/**
  * @brief  Gets the LCD Y size.
  * @retval The used LCD Y size
  */
uint32_t BSP_LCD_GetYSize(void)
{
  return LcdDrv->GetLcdPixelHeight();
}

/**
  * @brief  Initializes the LCD Layers.
  * @param  None
  * @retval None
  */

void BSP_LCD_LayerInit(void)
{
  // No multiple layer support
}

/**
  * @brief  Sets the LCD Layer.
  * @param  Layerx: specifies the Layer foreground or background.
  * @retval None
  */
void BSP_LCD_SetLayer(uint32_t Layerx)
{
  // No layer support
}

/**
  * @brief  Sets the LCD Text and Background colors.
  * @param  aTextColor: specifies the Text Color.
  * @param  aBackColor: specifies the Background Color.
  * @retval None
  */
void BSP_LCD_SetColors(uint16_t aTextColor,  uint16_t aBackColor)
{
  DrawProp.TextColor = aTextColor;
  DrawProp.BackColor = aBackColor;
}

/**
  * @brief  Gets the LCD Text and Background colors.
  * @param  Text: pointer to the variable that will contain the Text
            Color.
  * @param  Back: pointer to the variable that will contain the Background
            Color.
  * @retval None
  */
void BSP_LCD_GetColors(uint16_t *Text,  uint16_t *Back)
{
  *Text = DrawProp.TextColor;
  *Back = DrawProp.BackColor;
}

/**
  * @brief  Gets the LCD text color.
  * @retval Used text color.
  */
uint16_t BSP_LCD_GetTextColor(void)
{
  return DrawProp.TextColor;
}

/**
  * @brief  Gets the LCD background color.
  * @retval Used background color
  */
uint16_t BSP_LCD_GetBackColor(void)
{
  return DrawProp.BackColor;
}

/**
  * @brief  Sets the Text color.
  * @param  Color: specifies the Text color code RGB(5-6-5).
  * @retval None
  */
void BSP_LCD_SetTextColor(uint16_t Color)
{
  DrawProp.TextColor = Color;
}

/**
  * @brief  Sets the Background color.
  * @param  Color: specifies the Background color code RGB(5-6-5).
  * @retval None
  */
void BSP_LCD_SetBackColor(uint16_t Color)
{
  DrawProp.BackColor = Color;
}

/**
  * @brief  Configure the transparency.
  * @param  transparency: specifies the transparency,
  *         This parameter must range from 0x00 to 0xFF.
  * @retval None
  */
void BSP_LCD_SetTransparency(uint8_t transparency)
{
  // No transparancy support
}

/**
  * @brief  Sets the LCD text font.
  * @param  fonts: Font to be used.
  * @retval None
  */
void BSP_LCD_SetFont(sFONT *fonts)
{
  DrawProp.pFont = fonts;
}

/**
  * @brief  Gets the LCD text font.
  * @retval Used font
  */
sFONT *BSP_LCD_GetFont(void)
{
  return DrawProp.pFont;
}

/**
  * @brief  Sets the char cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void BSP_LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  BSP_LCD_SetDisplayWindow(Xpos, Ypos, DrawProp.pFont->Width-1, DrawProp.pFont->Height-1);
}

/**
  * @brief  Config and Sets the color Keying.
  * @param  RGBValue: Specifies the Color reference.
  * @retval None
  */

void BSP_LCD_SetColorKeying(uint32_t RGBValue)
{
  // Color Keying not supported
}

/**
  * @brief  Disable the color Keying.
  * @param  RGBValue: Specifies the Color reference.
  * @retval None
  */
void BSP_LCD_ReSetColorKeying(void)
{
  // Color Keying not supported
} 

/**
  * @brief  Clears the whole LCD
  * @param  Color: Color to use for clearing.
  * @retval None
  */
void BSP_LCD_Clear(uint16_t Color)
{
  uint32_t counter = 0;
  uint32_t color_backup = DrawProp.TextColor;
  DrawProp.TextColor = Color;

  for (counter = 0; counter < BSP_LCD_GetYSize(); counter++)
  {
    BSP_LCD_DrawHLine(0, counter, BSP_LCD_GetXSize());
  }
  DrawProp.TextColor = color_backup;
  BSP_LCD_SetTextColor(DrawProp.TextColor);
}

/**
  * @brief  Clears the selected line.
  * @param  Line: Line to be cleared
  *          This parameter can be one of the following values:
  *            @arg  0..9: if the Current fonts is Font16x24
  *            @arg  0..19: if the Current fonts is Font12x12 or Font8x12
  *            @arg  0..29: if the Current fonts is Font8x8
  */
void BSP_LCD_ClearStringLine(uint16_t Line)
{
  uint32_t color_backup = DrawProp.TextColor;
  DrawProp.TextColor = DrawProp.BackColor;;

  /* Draw a rectangle with background color */
  BSP_LCD_FillRect(0, (Line * DrawProp.pFont->Height), BSP_LCD_GetXSize(), DrawProp.pFont->Height);

  DrawProp.TextColor = color_backup;
  BSP_LCD_SetTextColor(DrawProp.TextColor);
}

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: X position (in pixel).
  * @param  Ypos: Y position (in pixel).
  * @param  c: pointer to the character data.
  * @retval None
  */
void BSP_LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
{
  DrawChar(Xpos, Ypos, c);
}

/**
  * @brief  Displays one character.
  * @param  Line: Line where to display the character shape.
  * @param  Column: Column where to display the character shape.
  * @param  c: pointer to the character data.
  * @retval None
  */
void BSP_LCD_DrawCharLine(uint16_t Line, uint16_t Columns, const uint8_t *c)
{
  uint16_t Xpos = 0;
  uint16_t Ypos = 0;

  Ypos = (Line * DrawProp.pFont->Height);
  Xpos = (Columns * DrawProp.pFont->Width);

  DrawChar(Xpos, Ypos, c);
}

/**
  * @brief  Displays one character.
  * @param  Line: Line where to display the character shape.
  * @param  Column: Column where to display the character shape.
  * @param  Ascii: Character ascii code
  *           This parameter must be a number between Min_Data = 0x20 and Max_Data = 0x7E
  * @retval None
  */
void BSP_LCD_DisplayChar(uint16_t Line, uint16_t Columns, uint8_t Ascii)
{
  uint16_t Xpos = 0;
  uint16_t Ypos = 0;

  Ypos = (Line * DrawProp.pFont->Height);
  Xpos = (Columns * DrawProp.pFont->Width);

  BSP_LCD_DisplayCharAt(Xpos, Ypos, Ascii);
}

/**
  * @brief  Displays one character.
  * @param  Xpos: X position (in pixel).
  * @param  Ypos: Y position (in pixel).
  * @param  Ascii: Character ascii code
  *           This parameter must be a number between Min_Data = 0x20 and Max_Data = 0x7E
  * @retval None
  */
void BSP_LCD_DisplayCharAt(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
  DrawChar(Xpos, Ypos, &DrawProp.pFont->table[(Ascii-' ') * \
  DrawProp.pFont->Height * ((DrawProp.pFont->Width + 7) / 8)]);
}

/**
  * @brief  Displays characters on the LCD.
  * @param  Xpos: X position (in pixel)
  * @param  Ypos: Y position (in pixel)
  * @param  Text: Pointer to string to display on LCD
  * @param  Mode: Display mode
  *          This parameter can be one of the following values:
  *            @arg  CENTER_MODE
  *            @arg  RIGHT_MODE
  *            @arg  LEFT_MODE
  * @retval None
  */
void BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Line_ModeTypdef Mode)
{
  uint16_t refcolumn = 1, i = 0;
  uint32_t size = 0, xsize = 0;
  uint8_t  *ptr = Text;

  /* Get the text size */
  while (*ptr++) size ++ ;

  /* Characters number per line */
  xsize = (BSP_LCD_GetXSize()/DrawProp.pFont->Width);

  switch (Mode)
  {
  case CENTER_MODE:
    {
      refcolumn = Xpos + ((xsize - size)* DrawProp.pFont->Width) / 2;
      break;
    }
  case LEFT_MODE:
    {
      refcolumn = Xpos;
      break;
    }
  case RIGHT_MODE:
    {
      refcolumn =  - Xpos + ((xsize - size)*DrawProp.pFont->Width);
      break;
    }
  default:
    {
      refcolumn = Xpos;
      break;
    }
  }

  /* Send the string character by character on lCD */
  while ((*Text != 0) & (((BSP_LCD_GetXSize() - (i*DrawProp.pFont->Width)) & 0xFFFF) >= DrawProp.pFont->Width))
  {
    /* Display one character on LCD */
    BSP_LCD_DisplayCharAt(refcolumn, Ypos, *Text);
    /* Decrement the column position by 16 */
    refcolumn += DrawProp.pFont->Width;
    /* Point on the next character */
    Text++;
    i++;
  }
}

/**
  * @brief  Displays a character on the LCD.
  * @param  Line: Line where to display the character shape
  *          This parameter can be one of the following values:
  *            @arg  0..9: if the Current fonts is Font16x24
  *            @arg  0..19: if the Current fonts is Font12x12 or Font8x12
  *            @arg  0..29: if the Current fonts is Font8x8
  * @param  ptr: Pointer to string to display on LCD
  * @retval None
  */
void BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr)
{
  BSP_LCD_DisplayStringAt(0, LINE(Line), ptr, LEFT_MODE);
}

/**
  * @brief  Displays a maximum of Size char on the LCD.
  * @param  Line: the Line where to display the character shape.
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  */
void BSP_LCD_DisplayStringLineWSize(uint16_t Line, uint8_t *ptr, uint8_t size)
{
  uint32_t i = 0;
  uint16_t refcolumn;
  char space[]={" "};

  refcolumn = 0;
  if (size>20) size=20;

  //LCD_ClearLine(Line);

  /* Send the string character by character on lCD */
  while (i < size)
  {
    /* Display one character on LCD */
    BSP_LCD_DisplayChar(Line, refcolumn, *ptr);
    /* Decrement the column position by deltacolumn(16 or -16 for HX8347D) */
    refcolumn ++;
    /* Point on the next character */
    ptr++;
    /* Increment the character counter */
    i++;
  }

  do
  {
    /* Display one character on LCD */
    BSP_LCD_DisplayChar(Line, refcolumn, (uint8_t)(space[0]));
    /* Decrement the column position by deltacolumn(16 or -16 for HX8347D) */
    refcolumn ++;
    /* Increment the character counter */
    i++;
  }
  while(i < 20);
}

/**
  * @brief  Draws a pixel on LCD.
  * @param  Xpos: X position (in pixels)
  * @param  Ypos: Y position (in pixels)
  * @param  RGB_Code: Pixel color in RGB mode (5-6-5)
  * @retval None
  */
void BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code)
{
  if (LcdDrv->WritePixel != NULL)
  {
    LcdDrv->WritePixel(Xpos, Ypos, RGB_Code);
  }
}

/**
  * @brief  Draws an horizontal line.
  * @param  Xpos: X position (in pixels)
  * @param  Ypos: Y position (in pixels)
  * @param  Length: Line length
  * @retval None
  */
void BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t index = 0;

  if (LcdDrv->DrawHLine != NULL)
  {
    LcdDrv->DrawHLine(DrawProp.TextColor, Xpos, Ypos, Length);
  }
  else
  {
    for (index = 0; index < Length; index++)
    {
      BSP_LCD_DrawPixel((Xpos + index), Ypos, DrawProp.TextColor);
    }
  }
}

/**
  * @brief  Draws a vertical line.
  * @param  Xpos: X position (n pixels)
  * @param  Ypos: Y position (in pixels)
  * @param  Length: Line length
  */
void BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t index = 0;

  if (LcdDrv->DrawVLine != NULL)
  {
    LcdDrv->DrawVLine(DrawProp.TextColor, Xpos, Ypos, Length);
  }
  else
  {
    for (index = 0; index < Length; index++)
    {
      BSP_LCD_DrawPixel(Xpos, Ypos + index, DrawProp.TextColor);
    }
  }
}

/**
  * @brief  Draws an uni-line (between two points).
  * @param  x1: Point 1 X position (in pixels)
  * @param  y1: Point 1 Y position (in pixels)
  * @param  x2: Point 2 X position (in pixels)
  * @param  y2: Point 2 Y position (in pixels)
  * @retval None
  */
void BSP_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;

  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */

  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    BSP_LCD_DrawPixel(x, y, DrawProp.TextColor);  /* Draw the current pixel */
    num += numadd;                            /* Increase the numerator by the top of the fraction */
    if (num >= den)                           /* Check if numerator >= denominator */
    {
      num -= den;                             /* Calculate the new numerator value */
      x += xinc1;                             /* Change the x as appropriate */
      y += yinc1;                             /* Change the y as appropriate */
    }
    x += xinc2;                               /* Change the x as appropriate */
    y += yinc2;                               /* Change the y as appropriate */
  }
}

/**
  * @brief  Displays a rectangle.
  * @param  Xpos: specifies the X position (in pixels).
  * @param  Ypos: specifies the Y position (in pixels).
  * @param  Height: display rectangle height (in pixels).
  * @param  Width: display rectangle widthin (in pixels).
  * @retval None
  */
void BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Draw horizontal lines */
  BSP_LCD_DrawHLine(Xpos, Ypos, Width);
  BSP_LCD_DrawHLine(Xpos, (Ypos+ Height), Width);

  /* Draw vertical lines */
  BSP_LCD_DrawVLine(Xpos, Ypos, Height);
  BSP_LCD_DrawVLine((Xpos + Width), Ypos, Height);
}

/**
  * @brief  Draw a circle.
  * @param  Xpos: specifies the X position (in pixels).
  * @param  Ypos: specifies the Y position (in pixels).
  * @param  Radius: radius of the circle.
  * @retval None
  */
void BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;       /* Decision Variable */
  uint32_t  CurX;   /* Current X Value */
  uint32_t  CurY;   /* Current Y Value */

  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;

  while (CurX <= CurY)
  {
    BSP_LCD_DrawPixel((Xpos + CurX), (Ypos - CurY), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos - CurX), (Ypos - CurY), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos + CurY), (Ypos - CurX), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos - CurY), (Ypos - CurX), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos + CurX), (Ypos + CurY), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos - CurX), (Ypos + CurY), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos + CurY), (Ypos + CurX), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos - CurY), (Ypos + CurX), DrawProp.TextColor);


    if (D < 0)
    {
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
}

/**
  * @brief  Draws an poly-line (between many points).
  * @param  Points: Pointer to the points array
  * @param  PointCount: Number of points
  * @retval None
  */
void BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount)
{
  int16_t X = 0, Y = 0;

  if (PointCount < 2)
  {
    return;
  }

  BSP_LCD_DrawLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    BSP_LCD_DrawLine(X, Y, Points->X, Points->Y);
  }

}

/**
  * @brief  Draws an ellipse on LCD.
  * @param  Xpos: X position (in pixels)
  * @param  Ypos: Y position (in pixels)
  * @param  XRadius: Ellipse X radius (in pixels)
  * @param  YRadius: Ellipse Y radius (in pixels)
  * @retval None
  */
void BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;

  rad1 = XRadius;
  rad2 = YRadius;

  K = (float)(rad2/rad1);

  do {
    BSP_LCD_DrawPixel((Xpos-(uint16_t)(x/K)), (Ypos+y), DrawProp.TextColor);
    BSP_LCD_DrawPixel((Xpos+(uint16_t)(x/K)), (Ypos+y), DrawProp.TextColor);
    BSP_LCD_DrawPixel((Xpos+(uint16_t)(x/K)), (Ypos-y), DrawProp.TextColor);
    BSP_LCD_DrawPixel((Xpos-(uint16_t)(x/K)), (Ypos-y), DrawProp.TextColor);

    e2 = err;
    if (e2 <= x) {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}

/**
  * @brief  Draws a bitmap picture (16 bpp).
  * @param  Xpos: Bmp X position in the LCD (in pixels)
  * @param  Ypos: Bmp Y position in the LCD (in pixels)
  * @param  pbmp: Pointer to Bmp picture address.
  * @retval None
  */
void BSP_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
  uint32_t height = 0;
  uint32_t width  = 0;


  /* Read bitmap width */
  width = *(uint16_t *) (pbmp + 18);
  width |= (*(uint16_t *) (pbmp + 20)) << 16;

  /* Read bitmap height */
  height = *(uint16_t *) (pbmp + 22);
  height |= (*(uint16_t *) (pbmp + 24)) << 16;

  BSP_LCD_SetDisplayWindow(Xpos, Ypos, width, height);

  if (LcdDrv->DrawBitmap != NULL)
  {
    LcdDrv->DrawBitmap(Xpos, Ypos, pbmp);
  }
  BSP_LCD_SetDisplayWindow(0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}

/**
  * @brief  Draws RGB Image (16 bpp).
  * @param  Xpos:  X position in the LCD (in pixels)
  * @param  Ypos:  Y position in the LCD (in pixels)
  * @param  Xsize: X size in the LCD (in pixels)
  * @param  Ysize: Y size in the LCD (in pixels)
  * @param  pdata: Pointer to the RGB Image address.
  * @retval None
  */
void BSP_LCD_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata)
{

  BSP_LCD_SetDisplayWindow(Xpos, Ypos, Xsize, Ysize);

  if (LcdDrv->DrawRGBImage != NULL)
  {
    LcdDrv->DrawRGBImage(Xpos, Ypos, Xsize, Ysize, pdata);
  }
  BSP_LCD_SetDisplayWindow(0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}

/**
  * @brief  Draws a full rectangle.
  * @param  Xpos: X position (in pixels)
  * @param  Ypos: Y position (in pixels)
  * @param  Width: Rectangle width (in pixels)
  * @param  Height: Rectangle height (in pixels)
  * @retval None
  */
void BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  BSP_LCD_SetTextColor(DrawProp.TextColor);
  do
  {
    BSP_LCD_DrawHLine(Xpos, Ypos++, Width);
  }
  while(Height--);
}

/**
  * @brief  Draws a full circle.
  * @param  Xpos: X position (in pixels)
  * @param  Ypos: Y position (in pixels)
  * @param  Radius: Circle radius (in pixels)
  * @retval None
  */
void BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;        /* Decision Variable */
  uint32_t  CurX;    /* Current X Value */
  uint32_t  CurY;    /* Current Y Value */

  D = 3 - (Radius << 1);

  CurX = 0;
  CurY = Radius;

  BSP_LCD_SetTextColor(DrawProp.TextColor);

  while (CurX <= CurY)
  {
    if (CurY > 0)
    {
      BSP_LCD_DrawHLine(Xpos - CurY, Ypos + CurX, 2*CurY);
      BSP_LCD_DrawHLine(Xpos - CurY, Ypos - CurX, 2*CurY);
    }

    if (CurX > 0)
    {
      BSP_LCD_DrawHLine(Xpos - CurX, Ypos - CurY, 2*CurX);
      BSP_LCD_DrawHLine(Xpos - CurX, Ypos + CurY, 2*CurX);
    }
    if (D < 0)
    {
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }

  BSP_LCD_SetTextColor(DrawProp.TextColor);
  BSP_LCD_DrawCircle(Xpos, Ypos, Radius);
}

/**
  * @brief  Draws a full poly-line (between many points).
  * @param  Points: Pointer to the points array
  * @param  PointCount: Number of points
  * @retval None
  */
void BSP_LCD_FillPolygon(pPoint Points, uint16_t PointCount)
{

  int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0;
  int16_t X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, counter = 0;
  uint16_t  IMAGE_LEFT = 0, IMAGE_RIGHT = 0, IMAGE_TOP = 0, IMAGE_BOTTOM = 0;

  IMAGE_LEFT = IMAGE_RIGHT = Points->X;
  IMAGE_TOP= IMAGE_BOTTOM = Points->Y;

  for (counter = 1; counter < PointCount; counter++)
  {
    pixelX = POLY_X(counter);
    if (pixelX < IMAGE_LEFT)
    {
      IMAGE_LEFT = pixelX;
    }
    if (pixelX > IMAGE_RIGHT)
    {
      IMAGE_RIGHT = pixelX;
    }

    pixelY = POLY_Y(counter);
    if (pixelY < IMAGE_TOP)
    {
      IMAGE_TOP = pixelY;
    }
    if (pixelY > IMAGE_BOTTOM)
    {
      IMAGE_BOTTOM = pixelY;
    }
  }

  if (PointCount < 2)
  {
    return;
  }

  X_center = (IMAGE_LEFT + IMAGE_RIGHT)/2;
  Y_center = (IMAGE_BOTTOM + IMAGE_TOP)/2;

  X_first = Points->X;
  Y_first = Points->Y;

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    X2 = Points->X;
    Y2 = Points->Y;

    FillTriangle(X, X2, X_center, Y, Y2, Y_center);
    FillTriangle(X, X_center, X2, Y, Y_center, Y2);
    FillTriangle(X_center, X2, X, Y_center, Y2, Y);
  }

  FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
  FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
  FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first);
}

/**
  * @brief  Draws a full ellipse.
  * @param  Xpos: X position (in pixels)
  * @param  Ypos: Y position (in pixels)
  * @param  XRadius: Ellipse X radius (in pixels)
  * @param  YRadius: Ellipse Y radius (in pixels)
  * @retval None
  */
void BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;

  rad1 = XRadius;
  rad2 = YRadius;

  K = (float)(rad2/rad1);

  do
  {
    BSP_LCD_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1));
    BSP_LCD_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1));

    e2 = err;
    if (e2 <= x)
    {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}


void BSP_LCD_DrawRectangleWithRoundCorner(uint16_t Xpos, uint16_t Ypos,
                                      uint16_t Width, uint16_t Height, uint16_t Radius)
{
  uint16_t r = Radius;

  BSP_LCD_FillRect(Xpos+r,Ypos + 1,Width-2*r + 1,1);
  BSP_LCD_FillRect(Xpos+r,Ypos+Height - 1,Width-2*r + 1,1);
  BSP_LCD_FillRect(Xpos,Ypos+r - 2,2,Height-2*r + 5);
  BSP_LCD_FillRect(Xpos+Width,Ypos+r - 2,2,Height+2-2*r + 3);

  for (uint16_t p = 1; p <= r ; p++)
  {
    uint16_t arc = round(sqrt(r*r - p*p));
    BSP_LCD_FillRect(Xpos + r - p, Ypos + r - arc + 1,2,1);
    BSP_LCD_FillRect(Xpos + Width -r + p, Ypos  + r - arc + 1,2,1);
    BSP_LCD_FillRect(Xpos + r - p, Ypos + Height - r + arc - 1,2,1);
    BSP_LCD_FillRect(Xpos + Width - r + p, Ypos + Height - r + arc - 1,2,1);
  }

}

void BSP_LCD_DrawTabHeader(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height,
                                uint16_t Radius, uint16_t tabHeight, uint32_t tabId, uint32_t nbTab)
{
  uint16_t r = Radius;
  uint16_t textColor = DrawProp.TextColor;
  uint16_t backColor = DrawProp.BackColor;
  uint16_t tabWidth = Width / nbTab;
  int i;

  // draw all tabs in backcolor but the tabId
  for (i = 0; i < nbTab; i++)
  {
    if (i == tabId)
      continue;
    DrawProp.TextColor = backColor;

    // upper horizontal bar
    BSP_LCD_FillRect(Xpos+r + i * tabWidth,Ypos + 1,tabWidth-2*r + 1,1);
    //   BSP_LCD_FillRect(Xpos+r,Ypos+Height - 1,Width-2*r + 1,1);

    // left vertical bar
    if (i != (tabId +1))
      BSP_LCD_FillRect(Xpos + i * tabWidth,Ypos+r - 2,2,tabHeight +1 -r  /*- ((i!= 0)?  (r - 2) :0)*/) ;
//      BSP_LCD_FillRect(Xpos + ((i+1) * tabWidth),Ypos+r - 2 + tabHeight,2,tabHeight+2 + 3);
      
    for (uint16_t p = 1; p <= r ; p++)
    {
      uint16_t arc = round(sqrt(r*r - p*p));

      // upper left corner
      BSP_LCD_FillRect(Xpos + r - p  + (i * tabWidth), Ypos + r - arc + 1,2,1);
      // upper right corner
      BSP_LCD_FillRect(Xpos  -r + p  + ((i + 1) * tabWidth), Ypos  + r - arc + 1,2,1);
    }


    DrawProp.TextColor = textColor;

    uint8_t save_left_vert_bar = (i == 0) ? 2 : 0; 
    // lower horizontal bar
    if (i == tabId -1)
      BSP_LCD_FillRect(Xpos + i * tabWidth + save_left_vert_bar,Ypos+tabHeight,tabWidth-r -save_left_vert_bar + 1,1); 
    else if (i == tabId + 1)
      BSP_LCD_FillRect(Xpos + i * tabWidth +save_left_vert_bar +r,Ypos+tabHeight,tabWidth -r -save_left_vert_bar,1);
    else
      BSP_LCD_FillRect(Xpos + i * tabWidth + save_left_vert_bar,Ypos+tabHeight,tabWidth-save_left_vert_bar ,1);
  } // draw all tabs but the tabId

  // ****** Draw selected Tab as textcolor
  DrawProp.TextColor = textColor;

  // upper horizontal bar
  BSP_LCD_FillRect(Xpos+r + tabId * tabWidth,Ypos + 1,tabWidth-2*r + 1,1);

  // left vertical bar
  BSP_LCD_FillRect(Xpos + tabId * tabWidth,Ypos+r - 2,2,tabHeight - r + 1  - ((tabId!= 0) ?  r - 4 :0)) ;
  // right vertical bar
  if (tabId != nbTab-1)
    BSP_LCD_FillRect(Xpos + (tabId + 1) * tabWidth,Ypos+r - 2,2,tabHeight  - 2*r + 4) ;

  for (uint16_t p = 1; p <= r ; p++)
  {
    uint16_t arc = round(sqrt(r*r - p*p));

    // upper left corner
    BSP_LCD_FillRect(Xpos + r - p  + (tabId * tabWidth), Ypos + r - arc + 1,2,1);
    // upper right corner
    BSP_LCD_FillRect(Xpos  -r + p  + ((tabId + 1) * tabWidth), Ypos  + r - arc + 1,2,1);
    // lower left corner
    if (tabId != 0)
      BSP_LCD_FillRect(Xpos + tabId * tabWidth - r + p, Ypos + tabHeight - r + arc - 1,2,1);
    // lower right corner
    if (tabId != nbTab-1)
      BSP_LCD_FillRect(Xpos + ((tabId+1) * tabWidth) + r - p, Ypos + tabHeight - r + arc - 1,2,1);

  }



  if (tabId != (nbTab - 1))
    DrawProp.TextColor = backColor;
  else
    DrawProp.TextColor = textColor;
  // last  right vertical bar
  BSP_LCD_FillRect(Xpos + nbTab * tabWidth,Ypos+r - 2,2,tabHeight-r + 1);
//    BSP_LCD_FillRect(Xpos + i * tabWidth,Ypos+r - 2,2,tabHeight);

}


void BSP_LCD_DrawTab(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height,
                            uint16_t Radius, uint16_t tabHeight, uint32_t tabId, uint32_t nbTab)
{
  uint16_t r = Radius;
  uint16_t textColor = DrawProp.TextColor;
//  uint16_t backColor = DrawProp.BackColor;

//  BSP_LCD_FillRect(Xpos+r,Ypos + 1,Width-2*r + 1,1);
  BSP_LCD_FillRect(Xpos+r,Ypos+Height - 1,Width-2*r + 1,1);
  BSP_LCD_FillRect(Xpos,Ypos + tabHeight - 2,2,Height-1*r + 5 - tabHeight);
  BSP_LCD_FillRect(Xpos+Width,Ypos - 2 + tabHeight,2,Height+2- 1*r + 3 - tabHeight);

  for (uint16_t p = 1; p <= r ; p++)
  {
    uint16_t arc = round(sqrt(r*r - p*p));
//    BSP_LCD_FillRect(Xpos + r - p, Ypos + r - arc + 1,2,1);
//    BSP_LCD_FillRect(Xpos + Width -r + p, Ypos  + r - arc + 1,2,1);
    BSP_LCD_FillRect(Xpos + r - p, Ypos + Height - r + arc - 1,2,1);
    BSP_LCD_FillRect(Xpos + Width - r + p, Ypos + Height - r + arc - 1,2,1);
  }
  BSP_LCD_DrawTabHeader(Xpos, Ypos, Width, Height, Radius, tabHeight, tabId, nbTab);

  // last
  DrawProp.TextColor = textColor;
  //   BSP_LCD_FillRect(Xpos + i * tabWidth,Ypos+tabHeight,2,r);

}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void BSP_LCD_DisplayOn(void)
{
  LcdDrv->DisplayOn();
}


/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void BSP_LCD_DisplayOff(void)
{
  /* Display Off */
  LcdDrv->DisplayOff();
}

/**
  * @brief  Sets a display window
  * @param  Xpos: specifies the X buttom left position.
  * @param  Ypos: specifies the Y buttom left position.
  * @param  Height: display window height.
  * @param  Width: display window width.
  * @retval LCD_OK when completed
  */
int BSP_LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width , uint16_t Height)
{

  LcdDrv->SetDisplayWindow(	Xpos,Ypos,Width,Height);

  return LCD_OK;
}

/**
 * @brief  Disables LCD Window mode.
 * @retval None.
 */
void BSP_LCD_WindowModeDisable(void)
{
  LcdDrv->SetDisplayWindow(0, 0, LcdDrv->GetLcdPixelWidth(), LcdDrv->GetLcdPixelHeight());
}


/**  
 * @brief  Displays a bitmap picture loaded in the SPI Flash.
 * @param  Xpos: specifies the X position (in pixels).
 * @param  Ypos: specifies the Y position (in pixels).
 * @param  BmpAddress: Bmp picture address in the SPI Flash.
 * @retval None.
 */
void BSP_LCD_WriteBMP_FAT(uint16_t Xpos, uint16_t Ypos, const char* BmpName)
{
  uint32_t index = 0, size = 0, width=0, height=0;
  uint16_t *pBmpWord=0;
  uint16_t data;

  /* Read bitmap width*/
  width  = BmpName[0]+1;
  /* Read bitmap height*/
  height = BmpName[1]+1;
  /* Read bitmap size */
  size   = width * height;  /* nb of 16 bits */

  if (BSP_LCD_SetDisplayWindow(Xpos, Ypos, width-1 , height-1) != LCD_OK)
  {
    BSP_LCD_WindowModeDisable();
    return;
  }

  pBmpWord = (uint16_t *) (&BmpName[5]);
  /* Send them on the screen */
  for (index = 0; index < size; index++)
  {
    data = (*pBmpWord & 0xFF00)>>8;
    data += (*pBmpWord & 0x00FF)<<8;

    BSP_LCD_DrawPixel(ILI9341_NO_CURSOR, ILI9341_NO_CURSOR, data);
    pBmpWord++;
  }

  BSP_LCD_WindowModeDisable();
}

typedef struct RGB
{
  uint8_t B;
  uint8_t G;
  uint8_t R;
}RGB_typedef;
RGB_typedef *RGB_matrix;


/* This structure is used to control picture cropping 
    (to only display part of big pictures, fitting the LCD screen dimensions) */
static struct {
  uint32_t factor;        /* Only display one out of <factor> pixels. */
  uint32_t line_counter;  /* Count the line position, as the information is not provided by the libJPEG. */
  uint32_t first_row;     /* Used to center the crop. */
  uint32_t first_col;     /* Used to center the crop. */
  uint32_t last_row;     /* Used to center the crop. */
  uint32_t last_col;     /* Used to center the crop. */
} crop;


uint16_t BSP_LCD_FadeColor(uint16_t startColor, uint16_t endColor,int  nbSteps,int  currentStep)
{
  uint16_t returnColor;
  int sBlue = (startColor) & 0x1F;
  int sGreen = (startColor >> 5) & 0x3F;
  int sRed = (startColor >> 11) & 0x1F;

  int eBlue = (endColor) & 0x1F;
  int eGreen = (endColor >> 5) & 0x3F;
  int eRed = (endColor >> 11) & 0x1F;

  int cBlue = sBlue + /*round*/((float)(eBlue - sBlue)* currentStep / (float)nbSteps) ;
  int cGreen = sGreen + /*round*/((float)(eGreen - sGreen) * currentStep / (float)nbSteps);
  int cRed = sRed + /*round*/((float)(eRed - sRed) * currentStep / (float)nbSteps);

  returnColor = cBlue | ((cGreen & 0x3F) << 5) | ((cRed & 0x1F) << 11);
  return returnColor;
}

static uint16_t BSP_LCD_ConvertToGreyScale(uint8_t b, uint8_t g, uint8_t r)
{
  uint16_t greyValue = (1 * r + 1 * g + 1 * b) / 3;

  return BSP_LCD_FadeColor(LCD_COLOR_GRAY, LCD_COLOR_WHITE, 255, greyValue);
}

/**
  * @brief  Draws a 24 Bytes Pixel Line on LCD.
  * @param  Row: pointer to the row to draw.
  * @param  DataLength: length of data.
  * @retval 0 when finish.
  */
static uint8_t BSP_LCD_Write24BPixelLine(uint8_t* Row, uint32_t DataLength)
{

  RGB_matrix =  (RGB_typedef*)Row;
  uint16_t  ARGB32Buffer[320];
  uint32_t counter = 0;
  uint32_t width =  DataLength / 3;
  uint32_t crop_width = 0;

  if (((crop.line_counter % crop.factor) != 0) ||  /* If crop factor is > 1, bypass some lines */
    (crop.line_counter < crop.first_row) ||       /* Bypass the first picture rows, to center the crop */
    (crop.line_counter >= crop.last_row))         /* Bypass the last picture rows, to center the crop */
  {
    crop.line_counter++;
    return 0; 
  }
  crop.line_counter++;

  for (counter = 0; counter < width; counter++)
  {
    if ((counter % crop.factor) ||      /* If crop factor is > 1, bypass some columns */
         (counter < crop.first_col) ||  /* Bypass the first columns, to center the crop */
          (counter >= crop.last_col))   /* Bypass the last columns, to center the crop */
    {
      continue;
    }

    if (enableGreyScale)
    {
      ARGB32Buffer[crop_width]  = BSP_LCD_ConvertToGreyScale(RGB_matrix[counter].R,
                                                  RGB_matrix[counter].G,RGB_matrix[counter].B);
//      ARGB32Buffer[crop_width]  = LCD_COLOR_BLUE;
    } else {
      ARGB32Buffer[crop_width] = (uint16_t)
      (
      (((RGB_matrix[counter].B >> 3)<<11)|
        ((RGB_matrix[counter].G >> 2)<<5)|
        (RGB_matrix[counter].R >> 3))
      );
    }
    crop_width++;
  }
  LCD_IO_WriteBuffer16(ARGB32Buffer,crop_width);
  return 0;
}

/**
  * @brief  Get picture dimensions.
  * @Param  Pict: pointer to the picture data.
  * @param  Width: pointer to store width.
  * @param  Height: pointer to store height.
  * @retval None
  */
void BSP_LCD_GetPictureDim(const char * Pict, uint32_t* Width, uint32_t* Height)
{
  if (IS_JPEG(Pict))
  {
    // JPEG
    jpeg_getsize(Pict,Width,Height);

  } else {
    // Assume it's in-house BMP
    *Width = Pict[0];
    *Height = Pict[1];
  }

}


/**
  * @brief  Display picture file on LCD.
  * @param  Xpos: specifies the X position (in pixels).
  * @param  Ypos: specifies the Y position (in pixels).
  * @param  cropX: specifies the maximum display width.
  * @param  cropY: specifies the maximum display height.
  * @Param  Pict: pointer to the picture data.
  * @retval None
  */
uint32_t BSP_LCD_DisplayCropPicture(uint16_t Xpos, uint16_t Ypos, uint16_t cropX, uint16_t cropY, const char* Pict)
{
  if (IS_JPEG(Pict))
  {
    // JPEG
    uint32_t w,h;
    jpeg_getsize(Pict,&w,&h);

    /* Set default values to the crop settings. => no crop required*/
    crop.factor = 1;
    crop.first_row = 0;
    crop.first_col = 0;
    crop.last_col = w;
    crop.last_row = h;
    crop.line_counter = 0;

    /* Picture is too big: need to crop it. */
    if ((w > cropX) || (h > cropY))
    {
      /* Compute croping factor */
      uint32_t x_factor = w / cropX;
      uint32_t y_factor = h / cropY;
      /* Select smaller factor to use the whole LCD (no white band) */
      crop.factor = x_factor > y_factor ? y_factor : x_factor;
      /* Just for safety: should never happen. */
      crop.factor = crop.factor == 0 ? 1 : crop.factor;

      /* Compute number of pixels for the cropped picture */
      uint32_t crop_x_length = crop.factor * cropX;
      uint32_t crop_y_length = crop.factor * cropY;
      /* Compute crop margins to center the crop on the picture */
      if (h > crop_y_length)
      {
        crop.first_row = (h - crop_y_length)/2;
        crop.last_row = crop.first_row + crop_y_length;
      }
      if (w > crop_x_length)
      {
        crop.first_col = (w - crop_x_length)/2;
        crop.last_col = crop.first_col + crop_x_length;
      }
    }

    uint16_t winX = (w/crop.factor);
    uint16_t winY = (h/crop.factor);
    if (winX > cropX)
      winX = cropX;
    if (winY > cropY)
      winY = cropY;

//    BSP_LCD_SetDisplayWindow(Xpos, Ypos, (w/crop.factor)-1 , (h/crop.factor)-1);
        BSP_LCD_SetDisplayWindow(Xpos, Ypos, winX-1 , winY-1);

    // Display on LCD
    return jpeg_decode(Pict,BSP_LCD_Write24BPixelLine);
  } else {
    // Assume it's in-house BMP
    BSP_LCD_WriteBMP_FAT(Xpos, Ypos, Pict);
    return 0;
  }
}


/**
  * @brief  Display picture file on LCD.
  * @param  Xpos: specifies the X position (in pixels).
  * @param  Ypos: specifies the Y position (in pixels).
  * @Param  Pict: pointer to the picture data.
  * @retval None
  */
void BSP_LCD_DisplayPicture(uint16_t Xpos, uint16_t Ypos, const char* Pict)
{
  BSP_LCD_DisplayCropPicture(Xpos,Ypos,BSP_LCD_GetXSize(),BSP_LCD_GetYSize(),Pict);
}

void BSP_LCD_SetGreyScale(uint8_t enable)
{
  enableGreyScale = enable;
}

/******************************************************************************
                            Static Function
*******************************************************************************/

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: specifies the X position (in pixels).
  * @param  Ypos: specifies the Y position (in pixels).
  * @param  c: Pointer to the character data
  * @retval None
  */
static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t offset;
  uint8_t *pchar;
  uint32_t line;
  uint16_t *pBuff;
// Font limit for this function: 30x30
  uint16_t buffer[30*30];

  height = DrawProp.pFont->Height;
  width  = DrawProp.pFont->Width;

  /* overhead, Ystart & Yend are used as a workaround when the screen height is not a multiple of the font height,
     the last line is being croped.
  */
  uint32_t Ystart = 0;
  uint32_t Yend = height;
  int overhead = (Ypos + height - BSP_LCD_GetYSize());
  if (overhead > 0)
  {
    Ystart += overhead / 2;
    Yend -= overhead / 2 +  (overhead % 2);
  } else {
    // if overhead is negative -> no overhead at all
    overhead = 0;
  }

  pBuff = buffer;

  BSP_LCD_SetCursor(Xpos, Ypos);

  offset =  8 *((width + 7)/8) -  width ;

  for (i = Ystart; i < (Yend); i++)
  {
    pchar = ((uint8_t *)c + (width + 7)/8 * i);

    switch(((width + 7)/8))
    {
    case 1:
      line =  pchar[0];
      break;

    case 2:
      line =  (pchar[0]<< 8) | pchar[1];
      break;

    case 3:
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
      break;

    case 4:
    default:
      line =  (pchar[0]<< 24) | (pchar[1]<< 16) | (pchar[2]<< 8) | pchar[3];
      break;
    }
    
    for (j = 0; j < width; j++)
    {
      if (line & (1 << (width- j + offset- 1)))
      {
        *(pBuff++) = DrawProp.TextColor;
      }
      else
      {
        *(pBuff++) = DrawProp.BackColor;

      }
    }
  }
  LCD_IO_WriteBuffer16(buffer, width * (height - overhead));
}

/**
  * @brief  Fills a triangle (between 3 points).
  * @param  x1: Point 1 X position (in pixels).
  * @param  y1: Point 1 Y position (in pixels).
  * @param  x2: Point 2 X position (in pixels).
  * @param  y2: Point 2 Y position (in pixels).
  * @param  x3: Point 3 X position (in pixels).
  * @param  y3: Point 3 Y position (in pixels).
  * @retval None
  */
static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3)
{ 
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
  curpixel = 0;

  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */

  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    BSP_LCD_DrawLine(x, y, x3, y3);

    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}


/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
