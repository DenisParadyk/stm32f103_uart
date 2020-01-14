#include "main.h"

int main()
{
  ClockInit();
  UART1_Init();
  LedInit();

  uint8_t a[] = "12345";

  UART_Transmit(a, sizeof(a) - 1);

  while(TRUE)
  {

  }

  return 0;
}
