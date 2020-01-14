/*
 * led.c
 *
 *  Created on: 10 џэт. 2020 у.
 *      Author: Denis Paradiuk
 */

#include "led.h"

void LedSetHi(void)
{
  GPIOC->ODR &= ~(1<<13);
}

void LedSetLow(void)
{
  GPIOC->ODR |= (1<<13);
}

void LedInit(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
  GPIOC->CRH |= (0x02 << GPIO_CRH_MODE13_Pos) | (0x00 << GPIO_CRH_CNF13_Pos);

  LedSetLow();
}
