/******************************************************************************
File:   uart.c
MPU:    STM32F103xx
Ver     1.0
Date:   2019/06/26
Autor:  Denis Paradiuk
******************************************************************************/
#include "uart.h"

#define USART_IT_EN

#define RXNE(UARTx)     (UARTx->SR & USART_SR_RXNE)
#define TXE(UARTx)      (UARTx->SR & USART_SR_TXE)

#define UART_ENABLE_IT(USART, IRQ)          ((((IRQ) >> 28U) == UART_CR1_REG_INDEX)? (USART->CR1 |= ((IRQ) & UART_IT_MASK)): \
                                                           (((IRQ) >> 28U) == UART_CR2_REG_INDEX)? (USART->CR2 |= ((IRQ) & UART_IT_MASK)): \
                                                           (USART->CR3 |= ((IRQ) & UART_IT_MASK)))
int counter = 0;
int SizeTx = 0;
char* DataTx;

void UART1_Init(void)
{
	__IO uint32_t REG;

	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
	REG = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);

	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
	REG = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
	MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE10 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF9_0,
						  GPIO_CRH_CNF10_0 | GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1);

	NVIC_EnableIRQ(USART1_IRQn);

	if(READ_BIT(USART1->CR1, USART_CR1_UE) != (USART_CR1_UE))
	{
	MODIFY_REG(USART1->CR1,
		   USART_CR1_M | USART_CR1_PCE | USART_CR1_PS ,USART_CR1_TE |USART_CR1_RE);
	}

	CLEAR_BIT(USART1->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
	CLEAR_BIT(USART1->CR3, (USART_CR3_SCEN | USART_CR3_IREN | USART_CR3_HDSEL));

	WRITE_REG(USART1->BRR, ((0x27)<<4)|0x01);

	SET_BIT(USART1->CR1, USART_CR1_UE);

  }

void UART_Transmit(uint8_t *Data, uint16_t Size)
{
	DataTx = Data;
	SizeTx = Size;
	counter = 0;
#ifdef USART_IT_EN
		UART_ENABLE_IT(USART1, UART_IT_TXE);
#else
	if(Size != 0)
	{
		for(int i = 0; i < Size; i++)
		{
			USART1->DR = *Data;
			DataTx = Data;
			while(READ_BIT(USART1->SR, USART_SR_TXE) != (USART_SR_TXE));
			Data++;

		}
	}
#endif
}

void USART1_IRQHandler(void)
{
	if(counter < SizeTx)
	{
		USART1->DR = DataTx[counter];
	}
	counter++;
}

