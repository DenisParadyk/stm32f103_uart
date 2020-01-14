/*
 * clock.c
 *
 *  Created on: 10 џэт. 2020 у.
 *      Author: Denis Paradiuk
 */
#include "clock.h"

int ClockInit(void)
{
  __IO int StartUpCounter;

  RCC->CR |= (1<<RCC_CR_HSEON_Pos);

  for(StartUpCounter=0; ; StartUpCounter++)
  {

    if(RCC->CR & (1<<RCC_CR_HSERDY_Pos))
      break;

    if(StartUpCounter > 0x1000)
    {
      RCC->CR &= ~(1<<RCC_CR_HSEON_Pos);
      return 1;
    }
  }

  RCC->CFGR |= (0x07<<RCC_CFGR_PLLMULL_Pos)
            | (0x01<<RCC_CFGR_PLLSRC_Pos);


  RCC->CR |= (1<<RCC_CR_PLLON_Pos);

  for(StartUpCounter=0; ; StartUpCounter++)
  {

    if(RCC->CR & (1<<RCC_CR_PLLRDY_Pos))
      break;

    if(StartUpCounter > 0x1000)
    {
      RCC->CR &= ~(1<<RCC_CR_HSEON_Pos);
      RCC->CR &= ~(1<<RCC_CR_PLLON_Pos);
      return 2;
    }
  }

  FLASH->ACR |= (0x02<<FLASH_ACR_LATENCY_Pos);

  RCC->CFGR |= (0x00<<RCC_CFGR_PPRE2_Pos)
            | (0x04<<RCC_CFGR_PPRE1_Pos)
            | (0x00<<RCC_CFGR_HPRE_Pos);


  RCC->CFGR |= (0x02<<RCC_CFGR_SW_Pos);

  while((RCC->CFGR & RCC_CFGR_SWS_Msk) != (0x02<<RCC_CFGR_SWS_Pos)){}

  RCC->CR &= ~(1<<RCC_CR_HSION_Pos);

  return 0;
}
