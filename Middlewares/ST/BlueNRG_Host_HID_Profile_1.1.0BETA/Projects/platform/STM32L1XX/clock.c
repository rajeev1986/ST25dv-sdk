#include "clock.h"

/* Added define for clock settings */
#include "SDK_EVAL_Clock.h"

#include "stm32l1xx_systick.h"

// The value that will be load in the SysTick value register.
#define RELOAD_VALUE (SYSCLK_FREQ/1000)-1   // One clock each 1 ms

static volatile tClockTime count = 0;

const uint32_t CLOCK_SECOND = 1000;

/*---------------------------------------------------------------------------*/
void SysTick_Handler(void)
{
  count++;
}

/*---------------------------------------------------------------------------*/

void Clock_Init(void)
{

  __disable_irq();

  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  SysTick_SetReload(RELOAD_VALUE);
  SysTick_ITConfig(ENABLE);
  SysTick_CounterCmd(SysTick_Counter_Enable);

  __enable_irq();

}

/*---------------------------------------------------------------------------*/

tClockTime Clock_Time(void)
{
  return count;
}

/*---------------------------------------------------------------------------*/
/**
 * Wait for a multiple of 1 ms.
 *
 */
void Clock_Wait(uint32_t i)
{
  tClockTime start;

  start = Clock_Time();
  while(Clock_Time() - start < (tClockTime)i);
}
/*---------------------------------------------------------------------------*/

