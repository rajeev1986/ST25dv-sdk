/**
  ******************************************************************************
  * @file    ff.h 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @ingroup Filesystem_API
  * @brief   Simple file system api header.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MMY-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#ifndef __FS_API__
#define __FS_API__

#include "stdint.h"
#include "stddef.h"

/** @addtogroup Filesystem_API
  * @ingroup  ST25_Discovery_Demo
  * @{
  */

/**
  * @brief  File object structure.
  */
typedef struct {
	const char * ptr; /**< Pointer to the next data to be read. */
	uint8_t open;     /**< Boolean, true if the file is currently opened . */
} FIL;



/**
  * @brief  Function return codes.
  */
typedef enum {
	FR_OK = 0,				        /**< (0) Succeeded */
	FR_DENIED = 7,				    /**< (7) Access denied due to prohibited access */
	FR_INVALID_OBJECT = 9,		/**< (9) The file object is invalid */
} FRESULT;



/*--------------------------------------------------------------*/
/* Simple Fs module application interface                           */

FRESULT f_open (FIL* fp, const char* path, uint8_t mode);				/* Open or create a file */
FRESULT f_close (FIL* fp);											                /* Close an open file object */
FRESULT f_read (FIL* fp, void* buff, uint32_t btr, size_t* br);			/* Read data from a file */
FRESULT f_write (FIL* fp, const void* buff, uint32_t btw, size_t* bw);	/* Write data to a file */






/** @brief File access control: Read-Only (no other value supported) */
#define	FA_READ				0x01

/** @} */


#endif /* __FS_API__ */
