/**
  ******************************************************************************
  * @file    st25dv_features_demo.c
  * @author  MMY Application Team
  * @version $Revision:$
  * @date    $Date:$
  * @ingroup ST25DV_Features_Demo
  * @brief   This file provides functions to execute ST25DV demos.
  * @details The demos covers the following usecases:
  *          - Energy Harvesting
  *          - GPO interrupts from RF
  *          - ST25DV states: RF off, RF sleep, Low power down
  *          - Multi-area and Password protection
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
#include "string.h"
#include "st25dx_discovery_nfctag.h"
#include "st25dx_discovery_dpot.h"
#include "st25dx_discovery.h"
#include "tagtype5_wrapper.h"
#include "mailboxfunc.h"
#include "commonfunc.h"
#include "ndef_demo.h"
#include "st25dv_features_demo.h"
#include <stdio.h>

/** Compute the positive difference between 2 numbers */
#define DIFF(a,b) ((a)>(b)?(a)-(b):(b)-(a))

/** Only use to display a round value (100k) when DPOT is set to max resistance */
#define DPOT_MAX_VAL      ((uint8_t) 100)
/** ~400 Ohms resistance value for the digital potentiometer - accurate value is unknown */
#define DPOT_400_R        ((uint8_t) 1)
/** Digital potentiometer resolution */
#define DPOT_RESOLUTION   ((float) 0.390625)

/** Analog to Digital Converter max value */
#define ADC_RESOLUTION    ((uint16_t) 4096)
/** Min resistance resistance on the board */

/** Value of the resistor used for current measurement. (Could be 4.3Ohm or formerly 2kOhms) */
#ifndef ST25MBOARD_RMEAS_2K
/* Resistance of 4.3 Ohms, used for the current measurement */
#define RMEAS_K           ((float) 0.0043)
#else
#define RMEAS_K           ((uint16_t) 2)
#endif
/** Analog to Digital Converter reference voltage (3V) - not 3.3V due to on-board filter */
#define VREF              ((float) 3)

/** Min delay between 2 Energy Harvesting measures */ 
#define EH_TEMPO_MS       ((uint8_t) 5)
/** Min delay between 2 Energy Harvesting measures */ 
#define EH_DISPLAY_TEMPO_MS       ((uint8_t) 50)
/** Min Energy Harvesting variation causing a change in the display */ 
#define EH_REFRESH_FILTER ((uint8_t) 30)
/** Min Energy Harvesting value to display a non-zero value */ 
#define EH_0_FILTER       ((uint8_t) 150)

/** Energy Harvesting LED PWM period */ 
#define EH_LED_PWM_PER    ((uint16_t) 20)
/** Energy Harvesting LED PWM divider */ 
#define EH_LED_PWM_DIV    ((uint16_t) (ADC_RESOLUTION - EH_0_FILTER) / EH_LED_PWM_PER)

/** ST URL for ST25 products */
#define NDEF_DEMO_URL                     "st.com/st25-demo"

/** Number of previous values stored to compute the average value of the Energy Harvesting */
#define PREV_VAL_NB ((uint8_t) 50)

/** Number of values measured for the  Energy Harvesting, on which to compute the average. */
#define INPUT_NB ((uint8_t) 3)


/** Energy Harvesting Led Pwm counter */
static uint16_t led_pwm_value =0;
/** Energy Harvesting Led Pwm period */
static uint16_t led_pwm_period = 0;
/* GPO polling variable from stm32f4xx_it.c */
extern volatile uint8_t GPO_Activated;

/**
  @brief Computes an average value for of  Energy Harvesting measures.
  @details Additional edge detection, to avoid wasting time when the field is switched on/off. 
  @param eh_values pointer on the raw measures, used to compute the metrics. 
*/
static void EH_filter(uint16_t *val)
{
  uint32_t sum[INPUT_NB] = {0};
  static uint16_t prev_values[INPUT_NB][PREV_VAL_NB];
  static uint32_t index = 0;
  static uint16_t prev_val = 0;
  
  if(DIFF(prev_val,val[0]) > EH_REFRESH_FILTER)
  {
    /* edge detected => re-init filter */
    for(int i =0; i<PREV_VAL_NB;i++)
    {
      for (int j = 0; j < INPUT_NB ; j++)
      {
        prev_values[j][i] = val[j];
      }
    }
    
  } else {    
    /* No edge detected: compute the average value */
    for(int j =0; j < INPUT_NB ; j++)
    {
      prev_values[j][index] = val[j];
    }

    /* ~ integrator filter */
    for(int i =0; i<PREV_VAL_NB;i++)
    {
      for (int j = 0; j < INPUT_NB ; j++)
      {
        sum[j] += prev_values[j][i];
      }
    }
    for(int j = 0; j < INPUT_NB ; j++)
    {
      val[j] = sum[j] / PREV_VAL_NB;
    }
  }
  prev_val = val[0];

  index++;
  index %= PREV_VAL_NB;
}


/**
  @brief Computes and displays the Energy Harvesting metrics on the display.
  @param eh_values pointer on the raw measures, used to compute the metrics 
*/
static void DisplayEH(uint32_t vdda, uint16_t *eh_values)
{
  char msg[20] = "";
  Menu_SetStyle(PLAIN);

  sprintf(msg," Voltage  %1.2f  V ",((float)eh_values[0] * (float)vdda / (ADC_RESOLUTION * 1000)));
  Menu_DisplayString(3, msg);
  /* eh_values[1] is used to store the electrical potential at Rmeas boundaries */
  sprintf(msg," Current  %1.1f  mA ",(((float)eh_values[1]) * (float)vdda / (ADC_RESOLUTION * 1000)));
  Menu_DisplayString(4, msg);
  sprintf(msg," Power   %4.1f  mW ", (((float)eh_values[1]) * (float)vdda / (ADC_RESOLUTION * 1000)) * 
                                                ((float)eh_values[0] * (float)vdda / (ADC_RESOLUTION * 1000)));
  Menu_DisplayString(5, msg);
}

/** @brief This functions runs the Energy Harvesting demo
  * @param dpot_val Digital Potentiometer resistance value
  */
static void EnergyHarvesting(uint32_t dpot_val)
{
  uint16_t eh_values[3] = {0,0,0};
  uint16_t last_value = 0;
  uint8_t refresh_display = 0;
  uint32_t vdda;

  ST25_RETRY(BSP_NFCTAG_SetEHENMode_Dyn(BSP_NFCTAG_INSTANCE));
  
  BSP_DPOT_Init();
  BSP_DPOT_SetResistor(dpot_val);
  

  Menu_MsgStatus("Energy Harvesting","Please bring an RF field to see the measures of the power provided by the ST25DV",MSG_INFO);
  
  uint8_t first_display = 1;
      
  BSP_ST25DV_EH_Init(&vdda);
  
  BSP_LED_Init(LED2);
	BSP_LED_Off(LED2);
  led_pwm_period = EH_LED_PWM_PER;
  
  while(Menu_GetUserInput() == 0)
  {
    BSP_ST25DV_EH_Measure(eh_values);

    for(int i = 0; i< ST25DX_DISCOVERY_EH_CHANNELn ; i++)
    {
      if(eh_values[i] < EH_0_FILTER)
      {
        eh_values[i] = 0;
      }
    }

    /* As the Current Amplifier output is not exploitable, use the corresponding item in the EH array to store the potential at meas resistor boundaries */
    eh_values[1] = (eh_values[0] > eh_values[2]) ? eh_values[0] - eh_values[2] : 0;
    EH_filter(eh_values);
   
    /* Set PWM on EH LED to simulate effect of the field moving */
    led_pwm_value = (eh_values[0] - EH_0_FILTER) / EH_LED_PWM_DIV;
    

    if(first_display == 1)
    {
      /* eh_values[1] is used to store the electrical potential at Rmeas boundaries */
      if( DIFF(eh_values[1],last_value))
      {
        /* Clear the instruction screen */
        Menu_MsgStatus("Energy Harvesting","",MSG_INFO);
        first_display = 0;
        DisplayEH(vdda, eh_values);
      }

      last_value = eh_values[1];
    } else {
      if(refresh_display++ > EH_DISPLAY_TEMPO_MS)
      {
        DisplayEH(vdda, eh_values);
        refresh_display = 0;
      }
    }

    HAL_Delay(EH_TEMPO_MS); 
  }
  /*
  if(BSP_NFCTAG_ResetEHENMode_Dyn(BSP_NFCTAG_INSTANCE) != NFCTAG_OK)
  {
    Menu_MsgStatus("Energy Harvesting", "Error when disabling EH!\n", MSG_STATUS_ERROR);
  }
  */
  ST25_RETRY(BSP_NFCTAG_ResetEHENMode_Dyn(BSP_NFCTAG_INSTANCE));
  
  BSP_LED_DeInit(LED2);
  led_pwm_period = 0;

  BSP_ST25DV_EH_DeInit();
  BSP_DPOT_SetResistor(0xFF);

}


/** @brief  This function writes data to the tag at a specified offset.
  * @param  Type    Type of the data: NDEF or proprietary
  * @param  Length  Number of byte to write.
  * @param  pData   Pointer on buffer to copy.
  * @param  offset  Address of the Tag where the data has to be written
  * @retval NDEF_ERROR            No NDEF in the tag.
  * @retval NDEF_ERROR_MEMORY_TAG Size not compatible with memory.
  * @retval NDEF_OK               The operation is completed.
  */
static uint16_t NfcTag_WriteData_AtOffset(uint8_t Type, uint16_t Length , uint8_t *pData, uint16_t offset )
{
  TT5_TLV_t tlv;
  uint8_t tlv_size;
  uint8_t NfcT5_Terminator = NFCT5_TERMINATOR_TLV;
  
  uint32_t max_length = BSP_NFCTAG_GetByteSize(BSP_NFCTAG_INSTANCE)
                        - ((Length >= 0xFF) ? 4 : 2) -    /* TLV length */
                        - sizeof(NfcT5_Terminator)        /* Terminator TLV */
                        - offset;

  /* If too many data to write return error */
  if( Length > max_length )
  {
    return NDEF_ERROR_MEMORY_TAG;
  }
  
  /* Detect NDEF message in memory */
  if( NfcType5_NDEFDetection( ) != NDEF_OK )
  {
    return NDEF_ERROR;
  }
  
  /* Prepare TLV */
  tlv.Type = Type;
  if(Length >= 0xFF)
  {
    tlv.Length = NFCT5_3_BYTES_L_TLV;
    tlv.Length16 = ((Length&0xff)<<8) | ((Length>>8)&0xff) ;
    tlv_size = 4;
    
  } else {
    tlv.Length = Length;
    tlv_size = 2;
  }

  /* Start write TLV to EEPROM */
  if(BSP_NFCTAG_WriteData(BSP_NFCTAG_INSTANCE, (uint8_t*)&tlv, offset, tlv_size)!= NFCTAG_OK)
    return NDEF_ERROR;
  offset += tlv_size;

  /* Continue write TLV data  to EEPROM */
  if(BSP_NFCTAG_WriteData(BSP_NFCTAG_INSTANCE, pData , offset, Length) != NFCTAG_OK )
    return NDEF_ERROR;
  offset +=Length;
  
  /* Write Terminator TLV */
  if(BSP_NFCTAG_WriteData(BSP_NFCTAG_INSTANCE, &NfcT5_Terminator, offset, sizeof(NfcT5_Terminator)) != NFCTAG_OK)
    return NDEF_ERROR;
  
  return NDEF_OK;

}


/** @brief   This function switches the LED2 on/off as a PWM
  * @details This function has to be regularly called from a Tick routine.
             It emulates the effect of a LED powered by the Energy Harvesting from the ST25DV.
             It relies on local variables: led_pwm_value, led_pwm_period
*/
void ST25DV_DEMO_Led2_Pwm(void)
{
  static uint16_t led_counter = 0;
  
  if(led_pwm_period == 0)
  {
    return;
  }
  
  if(led_counter < led_pwm_value)
  {
      BSP_LED_On(LED2);
  } else {
      BSP_LED_Off(LED2);    
  }
  if(led_counter < led_pwm_period)
  {
    led_counter++;
  } else {
    led_counter = 0;
  }
}

/** @defgroup ST25DV_Features_Demo ST25DV Features demo
  * @ingroup  ST25_Discovery_Demo
  * @brief    This module implements the functions to execute the demos of the ST25DV specific features.
  * @details  This module covers the following usecases:
  *           - Energy Harvesting
  *           - GPO interrupts from RF
  *           - ST25DV states: RF off, RF sleep, Low power down
  *           - Memory map configuration and Password protection
  * @{
  */

/** @brief This functions runs the Energy Harvesting demo with digital potentiometer set to 240 Ohms.
  */
void  ST25DV_DEMO_EnergyHarvesting(void)
{
  EnergyHarvesting(DPOT_400_R);
}


/** @brief   This function runs the GPO interrupts demos.
  * @details This function displays the type of received GPO interrupts.
  *          It also displays the number of interrupt received
  */
void ST25DV_DEMO_GPO(void)
{
  uint32_t irqCount = 0;
  uint32_t refresh_display = 0;
  IT_GPO_STATUS gpo_status = {0};
  IT_GPO_STATUS gpo_previous_count = {0};
  IT_GPO_STATUS gpo_count = {0};

  /* Write ST25 AAR in memory before starting the demo */
  const sAARInfo w_aar = {.PackageName = "com.st.st25nfc"};
  NDEF_DEMO_Init_Tag();
  NDEF_ClearNDEF();
  (void)NDEF_AddAAR (&w_aar);
  
  PresentPasswd( true );
  /* Enables all GPO interrupt sources but the RF busy which is too frequent with smartphones */
  ST25_RETRY(InitITGPOMode(ST25DV_GPO_ALL_MASK & ST25DV_GPO_RFACTIVITY_FIELD));  
  uint8_t user_exit = 0;
  Menu_MsgStatus("GPO interrupts","Please generate an activity on RF",MSG_INFO);  

  while(!user_exit)
  {
    user_exit = Menu_GetUserInput();

    if(GPO_Activated )
    {
      irqCount++;
      /* Check if GPO IT has raised */
      ManageGPO( &gpo_status );
      // Display detected irq 
      gpo_count.FieldOn += gpo_status.FieldOn;
      gpo_count.MailboxMsgRead += gpo_status.MailboxMsgRead;
      gpo_count.MsgInMailbox += gpo_status.MsgInMailbox;
      gpo_count.RfActivity += gpo_status.RfActivity;
      gpo_count.RfInterrupt += gpo_status.RfInterrupt;
      gpo_count.Rfuser += gpo_status.Rfuser;
      gpo_count.WriteInEEPROM += gpo_status.WriteInEEPROM;
      gpo_count.FieldOff += gpo_status.FieldOff;
    
      memset(&gpo_status,0,sizeof(gpo_status));
    }
    if(refresh_display++ > 250)
    {
      refresh_display = 0;
      if( memcmp(&gpo_count, &gpo_previous_count ,sizeof(gpo_count)))
      {
        // Display detected irq 
        char Msg[200] = "";
        if(gpo_count.FieldOn > 0)
        {
          char line[25];
          sprintf(line," Field On:     %3d\n",gpo_count.FieldOn);
          strcat(Msg,line);
        }
        if(gpo_count.MailboxMsgRead > 0)
        {
          char line[25];
          sprintf(line," MB RF Read:   %3d\n",gpo_count.MailboxMsgRead);
          strcat(Msg,line);
        }
        if(gpo_count.MsgInMailbox > 0)
        {
          char line[25];
          sprintf(line," MB RF Write:  %3d\n",gpo_count.MsgInMailbox);
          strcat(Msg,line);
        }
        if(gpo_count.RfActivity > 0)
        {
          char line[25];
          sprintf(line," RF Activity:  %3d\n",gpo_count.RfActivity);
          strcat(Msg,line);
        }
        if(gpo_count.RfInterrupt > 0)
        {
          char line[25];
          sprintf(line," RF Interrupt: %3d\n",gpo_count.RfInterrupt);
          strcat(Msg,line);
        }
        if(gpo_count.Rfuser > 0)
        {
          char line[25];
          sprintf(line," RF User:      %3d\n",gpo_count.Rfuser);
          strcat(Msg,line);
        }
        if(gpo_count.WriteInEEPROM > 0)
        {
          char line[25];
          sprintf(line," RF Write:     %3d\n",gpo_count.WriteInEEPROM);
          strcat(Msg,line);
        }
        if(gpo_count.FieldOff > 0)
        {
          char line[25];
          sprintf(line," Field Off:    %3d\n",gpo_count.FieldOff);
          strcat(Msg,line);
        }
        /* Remove last \n*/
        uint8_t last_char = strlen(Msg) - 1;
        if(Msg[last_char] == '\n')
        {
          Msg[last_char] = '\0';
        }
        Menu_MsgStatus("GPO interrupt(s)!",Msg,MSG_INFO);
                  
        memcpy(&gpo_previous_count,&gpo_count,sizeof(gpo_count));

      }
    }
  }  
  DeInitITGPOMode();
}



/** @brief   This function sets the ST25DV RF off.
  */
void ST25DV_DEMO_RF_Off(void)
{
  
  	/* Write URI for ST25 products web page */
  NDEF_DEMO_Init_Tag();
	sURI_Info w_uri = {URI_ID_0x01_STRING, NDEF_DEMO_URL ,""};
	NDEF_WriteURI(&w_uri);

  ST25_RETRY(BSP_NFCTAG_SetRFDisable_Dyn(BSP_NFCTAG_INSTANCE));
  
  Menu_MsgStatus("RF disable","RF command interpreter is disabled.\nThe ST25DV responds errors (0xF)!",MSG_STATUS_SUCCESS);  
  
  ST25_RETRY(BSP_NFCTAG_ResetRFDisable_Dyn(BSP_NFCTAG_INSTANCE));
}

/** @brief   This function sets the ST25DV RF to sleep state.
  */
void ST25DV_DEMO_RF_Sleep(void)
{
  
	/* Write URI for ST25 products web page */
  NDEF_DEMO_Init_Tag();
	sURI_Info w_uri = {URI_ID_0x01_STRING, NDEF_DEMO_URL ,""};
	NDEF_WriteURI(&w_uri);
  
  ST25_RETRY(BSP_NFCTAG_SetRFSleep_Dyn(BSP_NFCTAG_INSTANCE));
  
  Menu_MsgStatus("RF sleep","RF is in sleep state.\nThe ST25DV does not respond!",MSG_STATUS_SUCCESS);  
  
  ST25_RETRY(BSP_NFCTAG_ResetRFSleep_Dyn(BSP_NFCTAG_INSTANCE));
}

/** @brief   This function sets the ST25DV in Low Power Mode.
  */
void ST25DV_DEMO_Low_Power_Down(void)
{
	/* Write URI for ST25 products web page */
  NDEF_DEMO_Init_Tag();
	sURI_Info w_uri = {URI_ID_0x01_STRING, NDEF_DEMO_URL ,""};
	NDEF_WriteURI(&w_uri);
  
  BSP_LPD_Init();
  
  BSP_LPD_WritePin(GPIO_PIN_SET);
  /* Detect NDEF message in memory */
  if( NfcType5_NDEFDetection( ) == NDEF_OK )
  {
    Menu_MsgStatus("LPD error","Check that you are using a 12 pins package for ST25DV.\nAnd check the board:\nThe ST25DXSPARE jumper must be on the MCU pin.",MSG_STATUS_ERROR);   
  } else {
    Menu_MsgStatus("Low power down","Low power down activated, memory can only be accessed using the RF!",MSG_STATUS_SUCCESS);  
  }
  HAL_Delay(500);
  BSP_LPD_WritePin(GPIO_PIN_RESET);

}


/** @brief   This function configures the ST25DV memory mapping and sets a password protection.
  * @details Sets 2 memory areas:
  *          1. ReadOnly with a vCard NDEF record.
  *          2. NoRead, NoWrite with another vCard NDEF record.
  */
void ST25DV_DEMO_Memory_Mapping_Password(void)
{
    uint16_t public_size,secret_size;
    /* De-activate any protection & present password */
    NDEF_DEMO_Init_Tag();  
  
    /* Set protection for the first 2 areas */
    ST25DV_RF_PROT_ZONE rf_wr_prot = {.PasswdCtrl = ST25DV_PROT_PASSWD1 , .RWprotection = ST25DV_WRITE_PROT};
    ST25_RETRY(BSP_NFCTAG_WriteRFZxSS(BSP_NFCTAG_INSTANCE, ST25DV_PROT_ZONE1, rf_wr_prot));

    ST25DV_RF_PROT_ZONE rf_rd_prot = {.PasswdCtrl = ST25DV_PROT_PASSWD1 , .RWprotection = ST25DV_READWRITE_PROT};
    ST25_RETRY(BSP_NFCTAG_WriteRFZxSS(BSP_NFCTAG_INSTANCE, ST25DV_PROT_ZONE2, rf_rd_prot));

    /* Write 1st NDEF in readable area */
    /* TODO: set CCfile size to area 1? */
    sVcardInfo w_public_vcard = { .Version = VCARD_VERSION_2_1,
                                  .Name = "Clark;Kent;;;",
                                  .FirstName = "Kent Clark",
                                  .Title = "Journalist",
                                  .Org = "Daily Planet",
                                  .HomeAddress = "",
                                  .WorkAddress = "5th street & Concord Lane, Metropolis;;;;;;;",
                                  .HomeTel = "",
                                  .WorkTel = "",
                                  .CellTel = "750-1234",
                                  .HomeEmail = "",
                                  .WorkEmail = "clark.kent@daily-planet.com",         
                                  .Url = ""};         

    NDEF_WriteVcard ( &w_public_vcard );
    NDEF_getNDEFSize(&public_size);
    uint32_t area1_end =  public_size +
                          CCFileStruct.NDEF_offset +
                          (public_size> 0xff ? 4 : 2) + /* TLV size depends on ndef size */
                          1;                            /* TLV terminator */

   /* Area ends @ byte : end1 * 32 + 31 */
   /* Adding +1 here, because tested android smartphones seems to read more bytes than required and thus cross the area border */
    BSP_NFCTAG_WriteEndZonex(BSP_NFCTAG_INSTANCE, ST25DV_ZONE_END1,(area1_end)/32 + 1);
   /* area2_starts at next 32bit aligned address */
   /* Adding +1 here, because tested android smartphones seems to read more bytes than required and thus cross the area border */
   uint32_t area2_start = ((area1_end/32) + 2)*32;

    /* Write the area2 CCfile */
    uint8_t area2_ccfile[4] = {NFCT5_MAGICNUMBER_E1_CCFILE,NFCT5_VERSION_V1_0, (uint8_t)((BSP_NFCTAG_GetByteSize(BSP_NFCTAG_INSTANCE) / 8) - public_size), 0x05};
    ST25_RETRY(BSP_NFCTAG_WriteData(BSP_NFCTAG_INSTANCE, area2_ccfile, area2_start, 4));
                                                
    sVcardInfo w_secret_vcard = { .Version = VCARD_VERSION_2_1,
                                  .Name = "Super;Man;;;",
                                  .FirstName = "Superman",
                                  .Title = "Super Hero",
                                  .Org = "",
                                  .HomeAddress = "",
                                  .WorkAddress = "Everywhere in Metropolis;;;;;;;",
                                  .HomeTel = "",
                                  .WorkTel = "",
                                  .CellTel = "911",
                                  .HomeEmail = "",
                                  .WorkEmail = "superman@superman.com",         
                                  .Url = ""};         

    //NDEF_WriteVcard ( &w_secret_vcard );
   NDEF_PrepareVcardMessage(&w_secret_vcard , NDEF_Buffer, &secret_size );
   NfcTag_WriteData_AtOffset( NFCT5_NDEF_MSG_TLV, secret_size , NDEF_Buffer, area2_start + sizeof(area2_ccfile));

   Menu_MsgStatus("Area protection","Area 1: read-only.\nArea 2: no access.\nPassword 1 must be presented from RF to proceed...",MSG_STATUS_SUCCESS);  
              
  /*  BSP_NFCTAG_WriteEndZonex(BSP_NFCTAG_INSTANCE, ST25DV_ZONE_END2, 0x20);*/
 
    /* De-activate any protection */
    NDEF_DEMO_Init_Tag();
 
}

/**
  * @}
  */


