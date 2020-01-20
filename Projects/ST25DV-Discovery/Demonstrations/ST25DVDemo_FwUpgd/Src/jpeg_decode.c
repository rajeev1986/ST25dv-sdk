/**
  ******************************************************************************
  * @file    jpeg_decode.c 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @ingroup Jpeg_Decode
  * @brief   This file contains the JPEG decompressing methods.
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
#include "jpeglib.h"
#include "Menu_core.h"
#include "jpeg_decode.h"
#include "stdlib.h"
/* Private typedef -----------------------------------------------------------*/
  
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @defgroup Jpeg_Decode LibJPEG decode wrapper
  * @ingroup  ST25_Discovery_Demo
  * @brief    Wrapper calling the libJPEG Cube middleware to decode JPEG pictures.
  * @details  This module implements the functions to:
  *           - decode a JPEG picture.
  *           - read picture geometry (heigh & width).
  *           - display errors when decoding fails.
  *
  *           It uses the decoding part of the libJPEG Cube middleware, and the fs_api (to mimic filesystem functions).
  * @{
  */
static uint8_t jpeg_error = 0;

/**
  * @brief  Callback for the libJPEG, executed when an unrecoverable error occured.
  * @param  cinfo Pointer to the jpeg_common_struct with the current libJPEG state.
  * @return None
  */
void jpeg_decode_exit (j_common_ptr cinfo)
{
   /* Silently count errors in jpeg processing */
  jpeg_error++;
}

/**
  * @brief  Decode a jpeg formatted picture calling the Cube libJPEG middleware.
  * @param  jpeg Pointer to the data array with jpeg format picture.
  * @param  callback Callback function to be called after a line of the picture has been decoded.
  * @return Number of errors
  */
uint32_t jpeg_decode(const char* jpeg, uint8_t (*callback)(uint8_t* , uint32_t))
{ 
  /* This struct contains the JPEG decompression parameters */
  struct jpeg_decompress_struct cinfo;
  /* This struct represents a JPEG error handler */
  struct jpeg_error_mgr jerr;

  /* reset global error counter */
  jpeg_error = 0;

	// open the 'file' which is a simple char* array
	FIL fh;
	f_open(&fh,jpeg,FA_READ);

  /* Decode JPEG Image */
  JSAMPROW buffer[2] = {0}; /* Output row buffer */
  uint32_t row_stride = 0; /* Physical row width in image buffer */
	
  
  /* Step 1: Allocate and initialize JPEG decompression object */
  cinfo.err = jpeg_std_error(&jerr);
  cinfo.err->error_exit = &jpeg_decode_exit;
  
  /* Step 2: Initialize the JPEG decompression object */  
  jpeg_create_decompress(&cinfo);
  
  jpeg_stdio_src (&cinfo, &fh);
  
  /* Step 3: read image parameters with jpeg_read_header() */
  jpeg_read_header(&cinfo, TRUE);
  
  buffer[0] = malloc(cinfo.image_width*3);
  if(buffer[0] == NULL)
  {
    //Menu_MsgStatus("Jpeg error","Picture width is too big!!!",MSG_STATUS_ERROR);
    return 1;
  }
  /* Step 4: set parameters for decompression */
  cinfo.dct_method = JDCT_FLOAT;
  
  /* Step 5: start decompressor */
  jpeg_start_decompress(&cinfo);

  /* only display the picture if no error is detected */
  if(!jpeg_error)
  {
    row_stride = cinfo.image_width * 3;
    while (cinfo.output_scanline < cinfo.output_height)
    {
      (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    
      if (callback(buffer[0], row_stride) != 0)
      {
        break;
      }
    }
  }
  /* Step 6: Finish decompression */
  jpeg_finish_decompress(&cinfo);
  
  /* Step 7: Release JPEG decompression object */
  jpeg_destroy_decompress(&cinfo);   
	f_close(NULL)	;
  free (buffer[0]);
  return jpeg_error;
}


/**
  * @brief  Get the geometry of a JPEG picture.
  * @details Calls the Cube libJPEG middleware to read the jpeg header and extract the geometry info.
  * @param  jpeg Pointer to the data array with jpeg format picture.
  * @param  Width Pointer used to return the width of the JPEG picture.
  * @param  Height Pointer used to return the height of the JPEG picture.
  * @return None
  */
void jpeg_getsize(const char* jpeg, uint32_t* Width, uint32_t* Height)
{ 
/* This struct contains the JPEG decompression parameters */
struct jpeg_decompress_struct cinfo;
/* This struct represents a JPEG error handler */
struct jpeg_error_mgr jerr;

	// open the 'file' which is a simple char* array
	FIL fh;
	f_open(&fh,jpeg,FA_READ);
	
  
  /* Step 1: Allocate and initialize JPEG decompression object */
  cinfo.err = jpeg_std_error(&jerr);
  
  /* Step 2: Initialize the JPEG decompression object */  
  jpeg_create_decompress(&cinfo);
  
  jpeg_stdio_src (&cinfo, &fh);
  
  /* Step 3: read image parameters with jpeg_read_header() */
  jpeg_read_header(&cinfo, TRUE);
  
	*Width = cinfo.image_width;
	*Height = cinfo.image_height;


  /* Step 7: Release JPEG decompression object */
  jpeg_destroy_decompress(&cinfo);  
	f_close(NULL)	;
}

/**
  * @brief Gut not decompressed Jpeg buffer size.
  * @param jpeg Pointer to the data array with jpeg format picture.
  * @return Size of buffer in bytes.
  */
uint32_t jpeg_GetBufferSize( uint8_t *jpeg )
{
  uint32_t cnt;
  /* Search jpg end of file marker */
  for ( cnt = 2 ; cnt < 100000 ; cnt++ )
  { /* skip header sections */
    if ( jpeg[cnt] == 0xff ) 
    { /* We detect a SOF */
      if ( jpeg[cnt+1] == 0xda )
      { /* We detect a SOS */
        break;
      }
      else
      {
        /* pass header sections */
        cnt = cnt + ((uint32_t)(jpeg[cnt + 2]) << 8) + jpeg[cnt + 3] + 1;
      }
    }
  }
  
  for ( cnt = cnt ; cnt < 100000 ; cnt++ )
  { /* Encoded Data section search EOF */
    if( ( jpeg[cnt] == 0xff ) &&  ( jpeg[cnt+1] == 0xd9 ) )
    { 
      cnt += 2;
      break;
    }
  }
  return cnt;
}


/** @} */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
