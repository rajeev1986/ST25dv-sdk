/**
  ******************************************************************************
  * @file    ili9341_cube.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file includes the LCD driver for ILI9341 LCD.
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

/* Includes ------------------------------------------------------------------*/
#include "ili9341_cube.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */
  
/** @addtogroup ILI9341
  * @brief This file provides a set of functions needed to drive the 
  *        ILI9341 LCD.
  * @{
  */

/** @defgroup ILI9341_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */

/** @defgroup ILI9341_Private_Defines
  * @{
  */
/**
  * @}
  */
  
/** @defgroup ILI9341_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup ILI9341_Private_Variables
  * @{
  */
static uint8_t ili9341_readmem_mode = 0;

LCD_DrvTypeDef   ili9341_drv = 
{
  ili9341_Init,
  ili9341_ReadID,
  ili9341_DisplayOn,
  ili9341_DisplayOff,
  ili9341_SetCursor,
  ili9341_WritePixel,
  ili9341_ReadPixel,
  ili9341_SetDisplayWindow,
  ili9341_DrawHLine,
  ili9341_DrawVLine,
  ili9341_GetLcdPixelWidth,
  ili9341_GetLcdPixelHeight,
  0,
  0,
};

/**
  * @}
  */
  
/** @defgroup ILI9341_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */
  
/** @defgroup ILI9341_Private_Functions
  * @{
  */

/**
  * @brief  Power on the LCD.
  * @param  None
  * @retval None
  */
void ili9341_Init(void)
{
  /* Initialize ILI9341 low level bus layer ----------------------------------*/
  LCD_IO_Init();
  
  /* Configure LCD */
  /************* Start Initial Sequence **********/
  /****************************************/
  /*             Exit Sleep               */
  /****************************************/
  ili9341_WriteReg( LCD_SLEEP_OUT );
  LCD_Delay( 5 );

  /****************************************/
  /*              Set GVDD                */
  /****************************************/
  ili9341_WriteReg( LCD_POWER1 );
  ili9341_WriteData( 0x18 ); /* VRH[5:0]  4.05V */

  /****************************************/
  /* Step-up factor for operating voltage */
  /****************************************/
  ili9341_WriteReg( LCD_POWER2 );
  ili9341_WriteData( 0x11 ); /* BT[2:0] AVDD=VCIx2,VGH=VCIx7,VGL=-VCIx3 */

  /****************************************/
  /*              set VCOM                */
  /****************************************/
  ili9341_WriteReg( LCD_VCOM1 );
  ili9341_WriteData( 0x3e );  /* VMH=4.250 */
  ili9341_WriteData( 0x15 );  /* VML=-1.975 */

  /****************************************/
  /*       Memory Access Control          */
  /****************************************/
  ili9341_WriteReg( LCD_MAC );
  ili9341_WriteData( 0x28 );  /* MV = Row/Col exchange, BGR color filter */

  /****************************************/
  /*      Tearing Effect Line Off         */
  /****************************************/
  ili9341_WriteReg( LCD_TEOFF );

  /****************************************/
  /*   Set Positive Gamma correction      */
  /****************************************/
  ili9341_WriteReg( LCD_PGAMMA );
  ili9341_WriteData( 0x0F );
  ili9341_WriteData( 0x3a );
  ili9341_WriteData( 0x36 );
  ili9341_WriteData( 0x0b );
  ili9341_WriteData( 0x0d );
  ili9341_WriteData( 0x06 );
  ili9341_WriteData( 0x4c );
  ili9341_WriteData( 0x91 );
  ili9341_WriteData( 0x31 );
  ili9341_WriteData( 0x08 );
  ili9341_WriteData( 0x10 );
  ili9341_WriteData( 0x04 );
  ili9341_WriteData( 0x11 );
  ili9341_WriteData( 0x0c );
  ili9341_WriteData( 0x00 );

  /****************************************/
  /*   Set Negative Gamma correction      */
  /****************************************/
  ili9341_WriteReg( LCD_NGAMMA );
  ili9341_WriteData( 0x00 );
  ili9341_WriteData( 0x06 );
  ili9341_WriteData( 0x0a );
  ili9341_WriteData( 0x05 );
  ili9341_WriteData( 0x12 );
  ili9341_WriteData( 0x09 );
  ili9341_WriteData( 0x2c );
  ili9341_WriteData( 0x92 );
  ili9341_WriteData( 0x3f );
  ili9341_WriteData( 0x08 );
  ili9341_WriteData( 0x0e );
  ili9341_WriteData( 0x0b );
  ili9341_WriteData( 0x2e );
  ili9341_WriteData( 0x33 );
  ili9341_WriteData( 0x0F );

  /****************************************/
  /*             Pixel Format             */
  /****************************************/
  ili9341_WriteReg( LCD_PIXEL_FORMAT );
  ili9341_WriteData( 0x55 ); /* RGB & MCU 16 bits/pixel */

  /****************************************/
  /*          Column Address Set          */
  /****************************************/
  ili9341_WriteReg( LCD_COLUMN_ADDR );
  ili9341_WriteData( 0x00 );  /* Column 0 */
  ili9341_WriteData( 0x00 );
  ili9341_WriteData( (((ILI9341_LCD_PIXEL_WIDTH - 1) & 0xFF00) >> 8) );	 /* Column pixel width */
  ili9341_WriteData( ((ILI9341_LCD_PIXEL_WIDTH - 1) & 0x00FF) );

  /****************************************/
  /*           Page Address Set           */
  /****************************************/
  ili9341_WriteReg( LCD_PAGE_ADDR );
  ili9341_WriteData( 0x00 );   /* Page 0 */
  ili9341_WriteData( 0x00 );
  ili9341_WriteData( (((ILI9341_LCD_PIXEL_HEIGHT - 1) & 0xFF00) >> 8) );  /* Page column height */
  ili9341_WriteData( ((ILI9341_LCD_PIXEL_HEIGHT - 1) & 0x00FF) );

  /****************************************/
  /*              Display ON              */
  /****************************************/
  
  ili9341_WriteReg( LCD_DISPLAY_ON );
}

/**
  * @brief  Reads the Display ID.
  * @param  None
  * @retval LCD ID Value.
  */
uint16_t ili9341_ReadID( void )
{
  LCD_IO_Init( );
  return ( (uint16_t)ili9341_ReadData(LCD_READ_ID4, LCD_READ_ID4_SIZE) );
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @return None
  */
void ili9341_DisplayOn( void )
{
  /* Display On */
  ili9341_WriteReg( LCD_DISPLAY_ON );
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @return None
  */
void ili9341_DisplayOff( void )
{
  /* Display Off */
  ili9341_WriteReg( LCD_DISPLAY_OFF );
}

/**
  * @brief  Set the cursor position.
  * @param  Xpos X position to set.
  * @param  Ypos Y position to set.
  * @return None
  */
void ili9341_SetCursor( uint16_t Xpos, uint16_t Ypos )
{
  if( ((Xpos < ILI9341_LCD_PIXEL_WIDTH) && (Ypos < ILI9341_LCD_PIXEL_HEIGHT)) && 
      ((Xpos != ILI9341_NO_CURSOR) && (Ypos != ILI9341_NO_CURSOR)) )
  {
    ili9341_WriteReg( LCD_COLUMN_ADDR );
    ili9341_WriteData( (Xpos & 0xFF00) >> 8 );
    ili9341_WriteData( Xpos & 0x00FF );
    ili9341_WriteData( (ILI9341_LCD_PIXEL_WIDTH & 0xFF00) >> 8 );
    ili9341_WriteData( Xpos & 0x00FF );

    ili9341_WriteReg( LCD_PAGE_ADDR );
    ili9341_WriteData( (Ypos & 0xFF00) >> 8 );
    ili9341_WriteData( Ypos & 0x00FF );
    ili9341_WriteData( (ILI9341_LCD_PIXEL_HEIGHT & 0xFF00) >> 8 );
    ili9341_WriteData( Ypos & 0x00FF );

    ili9341_WriteReg(LCD_GRAM);

  }
  else
  {
    /* Nothing to do */
  }
}

/**
  * @brief  Set the display window.
  * @param  Xpos X position to set.
  * @param  Ypos Y position to set.
  * @param  Width Width size to set.
  * @param  Height Height size to set.
  * @return None
  */
void ili9341_SetDisplayWindow( uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height )
{
  uint16_t x_end = Xpos + Width;
  uint16_t y_end = Ypos + Height;

  if( x_end > ILI9341_LCD_PIXEL_WIDTH )
  {
    x_end = ILI9341_LCD_PIXEL_WIDTH;
  }
  else
  {
    /* Nothing to do */
  }
  if( y_end > ILI9341_LCD_PIXEL_HEIGHT )
  {
    y_end = ILI9341_LCD_PIXEL_HEIGHT;
  }
  else
  {
    /* Nothing to do */
  }
  
  if( Xpos > ILI9341_LCD_PIXEL_WIDTH )
  {
    Xpos = ILI9341_LCD_PIXEL_WIDTH;
  }
  else
  {
    /* Nothing to do */
  }
  if( Ypos > ILI9341_LCD_PIXEL_HEIGHT )
  {
    Ypos = ILI9341_LCD_PIXEL_HEIGHT;
  }
  else
  {
    /* Nothing to do */
  }

  ili9341_WriteReg( LCD_COLUMN_ADDR );
  ili9341_WriteData( (Xpos & 0xFF00) >> 8 );
  ili9341_WriteData( Xpos & 0x00FF );
  ili9341_WriteData( (x_end & 0xFF00) >> 8 );
  ili9341_WriteData( x_end & 0x00FF );

  ili9341_WriteReg( LCD_PAGE_ADDR);
  ili9341_WriteData( (Ypos & 0xFF00) >> 8 );
  ili9341_WriteData( Ypos & 0x00FF );
  ili9341_WriteData( (y_end & 0xFF00) >> 8 );
  ili9341_WriteData( y_end & 0x00FF );

  /* Enter RAM mode */
  ili9341_WriteReg(LCD_GRAM);

}

/**
  * @brief  Write a pixel to RAM.
  * @param  Xpos X position for pixel.
  * @param  Ypos Y position for pixel.
  * @param  Val Pixel color.
  * @return None
  */
void ili9341_WritePixel( uint16_t Xpos, uint16_t Ypos, uint16_t Val )
{
  /* If ILI9341_KEEP_CURSOR is used for Xpos or Ypos -> SetCursor does nothing 
     This allows to write 9341 RAM using HW pixel increment */
  ili9341_SetCursor( Xpos, Ypos );
  
  LCD_IO_WriteData16( Val );
}

/**
  * @brief  Read a pixel from RAM.
  * @param  Xpos X position of pixel.
  * @param  Ypos Y position of pixel.
  * @return None
  */
uint16_t ili9341_ReadPixel( uint16_t Xpos, uint16_t Ypos )
{
  uint32_t raw_pix = 0;
  uint16_t read_pix = 0;
  
  /* If ILI9341_KEEP_CURSOR is used for Xpos or Ypos -> SetCursor does nothing 
     This allows to read 9341 RAM using HW pixel increment */
  ili9341_SetDisplayWindow( Xpos, Ypos, ILI9341_LCD_PIXEL_WIDTH, ILI9341_LCD_PIXEL_HEIGHT );
  
  if( !ili9341_readmem_mode )
  {
    /* Read 4 bytes: 1 dummy byte + Red ,Green & Blue bytes */
    raw_pix = ili9341_ReadData( LCD_RAMRD, 4 );
  } 
  else
  {
    /* Don't go there on purpose due to the above SetDisplayWindow
       The READ_MEM_CONTINUE command miss some data when used with SPI SOFT NSS
    */
    raw_pix = ili9341_ReadData( LCD_READ_MEM_CONTINUE, 4 );
  }

  /* ILI returns the data as: dddddddd rrrrrddd ggggggdd bbbbbddd
  d: dummy bit, r: red bit, g: green bit, b: blue bit */
  read_pix = (raw_pix & 0xf80000) >> 19; /* red */
  read_pix = read_pix << 6;
  read_pix = read_pix | ((raw_pix & 0xfc00) >> 10); /* green */
  read_pix = read_pix << 5;
  read_pix = read_pix | ((raw_pix & 0xf8) >> 3); /* blue */

  return read_pix;
}


/**
  * @brief  Draw horizontal line.
  * @param  RGB_Code Specifies the RGB color.
  * @param  Xpos specifies the X position.
  * @param  Ypos specifies the Y position.
  * @param  Length specifies the Line length.
  * @return None
  */
void ili9341_DrawHLine( uint16_t RGB_Code, uint16_t Xpos, uint16_t Ypos, uint16_t Length )
{
  uint16_t i = 0;
  uint16_t buffer[ILI9341_LCD_PIXEL_WIDTH];

  /* Set Cursor */
  ili9341_SetDisplayWindow( Xpos, Ypos, Length, 1 );


  for( i = 0; i < Length; i++ )
  {
    /* Write 16-bit GRAM Reg */
    buffer[i] = RGB_Code;
  }
  LCD_IO_WriteBuffer16(buffer,Length);
}

/**
  * @brief  Draw vertical line.
  * @param  RGB_Code Specifies the RGB color.
  * @param  Xpos specifies the X position.
  * @param  Ypos specifies the Y position.
  * @param  Length specifies the Line length.
  * @return None
  */
void ili9341_DrawVLine( uint16_t RGB_Code, uint16_t Xpos, uint16_t Ypos, uint16_t Length )
{
  uint16_t i = 0;
  uint16_t buffer[ILI9341_LCD_PIXEL_HEIGHT];

  /* Set Cursor */
  ili9341_SetDisplayWindow( Xpos, Ypos, 0, Length );

  for( i = 0; i < Length; i++ )
  {
    /* Write 16-bit GRAM Reg */
    buffer[i] = RGB_Code;
  }
  LCD_IO_WriteBuffer16(buffer,Length);
}

/**
  * @brief  Writes  to the selected LCD register.
  * @param  LCD_Reg address of the selected register.
  * @return None
  */
void ili9341_WriteReg( uint8_t LCD_Reg )
{
  LCD_IO_WriteReg( LCD_Reg );
  ili9341_readmem_mode = (LCD_Reg == LCD_RAMRD) || (LCD_Reg == LCD_READ_MEM_CONTINUE);
}

/**
  * @brief  Writes data to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @return None
  */
void ili9341_WriteData( uint16_t RegValue )
{
  LCD_IO_WriteData( RegValue );
}

/**
  * @brief  Reads the selected LCD Register.
  * @param  RegValue: Address of the register to read
  * @param  ReadSize: Number of bytes to read
  * @retval LCD Register Value.
  */
uint32_t ili9341_ReadData( uint16_t RegValue, uint8_t ReadSize )
{
  /* Read a max of 4 bytes */
  ili9341_readmem_mode = (RegValue == LCD_RAMRD) || (RegValue == LCD_READ_MEM_CONTINUE);
  return (LCD_IO_ReadData(RegValue, ReadSize));
}

/**
  * @brief  Get LCD PIXEL WIDTH.
  * @param  None
  * @retval LCD PIXEL WIDTH.
  */
uint16_t ili9341_GetLcdPixelWidth( void )
{
  /* Return LCD PIXEL WIDTH */
  return ILI9341_LCD_PIXEL_WIDTH;
}

/**
  * @brief  Get LCD PIXEL HEIGHT.
  * @param  None
  * @retval LCD PIXEL HEIGHT.
  */
uint16_t ili9341_GetLcdPixelHeight( void )
{
  /* Return LCD PIXEL HEIGHT */
  return ILI9341_LCD_PIXEL_HEIGHT;
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

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
