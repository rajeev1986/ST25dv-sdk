/******************************************************************************
*
*  File Description 
*  ---------------------
* This header file defines Hardware abstraction layer APIs used by the 
* BLE stack. HAL defines the set of functions which needs
* to be ported to target operating system and target platform
* Implementation of the this header file should exists in /platform folder
*
*******************************************************************************/
#ifndef _LOW_POWER_H
#define _LOW_POWER_H

void GPIO_LowPower_Config(void);
void Restore_GPIO_Config(void);
void RTC_AutoWakeup_Config(void);
void User_Timer_Enter_Sleep(void);
void User_Timer_Exit_Sleep(void);
void Enter_LP_Sleep_Mode(void);
void Enter_Stop_Mode(uint32_t RTC_WakeUpCounter);
void Exit_LP_Sleep_Mode(void);
void Timer_Enter_Sleep(void);
void Timer_Exit_Sleep(void);
void Exit_LP_Sleep_Mode(void);
void Exit_Stop_Mode(void);

#endif /* _LOW_POWER_H */