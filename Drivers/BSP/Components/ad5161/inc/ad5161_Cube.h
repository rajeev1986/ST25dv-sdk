/**
  ******************************************************************************
  * File Name          : ad5161.h
  * Description        : AD5161 digital potentiometer driver header
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
	
	
#ifndef __AD5161_H
#define __AD5161_H

/* Includes ------------------------------------------------------------------*/
#include "dpot.h"

/* Exported types ------------------------------------------------------------*/
typedef enum 
{
  AD5161_ADDR0=0x58,
  AD5161_ADDR1=0x5A
} ad5161_addr_t;

typedef enum {
  AD5161_NA = 0,
  AD5161_RS = 0x40,
  AD5161_SD = 0x20
} ad5161_instr_t;

typedef struct
{
  uint8_t (*ReadSavedResistor)( uint8_t );
  uint8_t (*ReadResistorTolerance)( uint8_t );
} DPOT_ExtDrvTypeDef;

/* Exported constants --------------------------------------------------------*/
extern DPOT_DrvTypeDef ad5161_dpot_drv;
extern DPOT_ExtDrvTypeDef ad5161_dpot_Extdrv;

/* Exported macro ------------------------------------------------------------*/
#define DPOT_I2C_TIMEOUT (0x3000)
#define AD5161_MIDRANGE (128)
#define AD5161_MAX (255)
#define AD5161_WIPER_RESISTANCE (60)
#define AD5161_WIPER_PRECISION  (256)
#define AD5161_RAB_RESISTANCE   (100000)


/* Exported functions --------------------------------------------------------*/
void ad5161_Init( uint8_t Addr );
uint8_t ad5161_GetResistor( uint8_t Addr );
void ad5161_SetResistor( uint8_t Addr, uint8_t Data );
void ad5161_Reset( uint8_t Addr );
void ad5161_Shutdown( uint8_t Addr );

/* DPOT IO BUS functions */
void DPOT_IO_Init( uint8_t Addr );
uint8_t DPOT_IO_Read( uint8_t Addr );
void DPOT_IO_WriteBuffer( uint8_t Addr, uint8_t *pBuf, uint16_t Len );

#endif /* __AD5161_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

