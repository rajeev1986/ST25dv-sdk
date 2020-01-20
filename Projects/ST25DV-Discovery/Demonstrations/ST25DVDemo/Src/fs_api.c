/**
  ******************************************************************************
  * @file    fs_api.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @ingroup Filesystem_API
  * @brief   Simple filesystem api implementation.
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
#include "ff.h"
#include "stdlib.h"

/** @defgroup Filesystem_API Simple filesystem api
  * @ingroup  ST25_Discovery_Demo
  * @brief    Simple implementation of a filesystem API.
  * @details  This module implements the functions to open, read and close a file.
  *           It does **NOT** implements a real filesystem.
  *           A file is considered being a starting address in memory, from where the API reads a stream of data.
  *           The purpose of this module is to provide a standard filesystem api to feed data to the libJPEG.
  * @{
  */

/** @brief    Open a stream of data from an address in memory.
  * @details  This function mimic the `open` standard function from a filesystem.
  *           The usage of such function in the ST25DV demo is to feed data to the jpeg decoder module, without having a real filesystem.
  * @param    fp Pointer to FIL structure - mimic a filehandle.
  * @param    start Pointer to an address in memory, that will be used as source of the data. 
  * @param    mode File access control, only `FA_READ` is supported (read-only). 
  * @retval   FR_OK Data stream is opened.
  * @retval   FR_INVALID_OBJECT `fp` is not allocated/initialized.
  * @retval   FR_DENIED mode is different from `FA_READ`.
  */ 
FRESULT f_open (FIL* fp, const char* start, uint8_t mode)
{				/* Open or create a file */
   
	if(fp == NULL)  
		return FR_INVALID_OBJECT;
  if(mode == FA_READ)
  {
		fp->ptr = start;
		fp->open = 1;
		return FR_OK;
	}
	return FR_DENIED;
	
}

/** @brief    Close a stream of data from an address in memory.
  * @details  This function mimic the `close` standard function from a filesystem.
  *           The usage of such function in the ST25DV demo is to feed data to the jpeg decoder module, without having a real filesystem.
  * @param    fp Pointer to FIL structure - mimic a filehandle.
  * @retval   FR_OK Data stream has been successfully closed.
  * @retval   FR_INVALID_OBJECT `fp` is not allocated or not open.
  */ 
FRESULT f_close (FIL* fp)
{ /* Close an open file object */
	if((fp != NULL) && (fp->open))
	{
		fp->open = 0;
		return FR_OK;
	}
	return FR_INVALID_OBJECT;
}


#include "string.h"
/** @brief    Reads bytes from a stream of data.
  * @details  This function mimic the `read` standard function from a filesystem.
  *           The usage of such function in the ST25DV demo is to feed data to the jpeg decoder module, without having a real filesystem.
  * @param    fp Pointer to FIL structure - mimic a filehandle.
  * @param    buff Memory buffer to store read data.
  * @param    btr Number of bytes to read.
  * @param    br Number of bytes actually read.
  * @retval   FR_OK Data stream has been successfully read.
  * @retval   FR_INVALID_OBJECT `fp` is not allocated or not open.
  */ 
FRESULT f_read (FIL* fp, void* buff, uint32_t btr, size_t* br)
{	/* Read data from a file */
	if((fp != NULL) && (fp->open == 1))
	{
			memcpy(buff,fp->ptr,btr);
			fp->ptr += btr;
			*br = btr;
			return FR_OK;
	}
	return FR_INVALID_OBJECT;
   
	
}

/** @brief    Do nothing.
  * @details  This function mimic the `write` standard function from a filesystem.
  *           The usage of such function in the ST25DV demo is to feed data to the jpeg decoder module, without having a real filesystem.
  * @param    fp Pointer to FIL structure - mimic a filehandle.
  * @param    buff Memory buffer with source of data.
  * @param    btw Number of bytes to read.
  * @param    bw Number of bytes actually written.
  * @retval   FR_OK Always.
  */ 
FRESULT f_write (FIL* fp, const void* buff, uint32_t btw, size_t* bw)
{
	return FR_OK;
}

/** @} */

/************************ (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
