/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/28
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#include <cmn_hdr.h>
#define LOG_TAG "uart"
 
#include "stm32f4xx.h"
#include <uart.h>


struct uart_driver {
	uint32_t status;
	uint32_t gpio_clock;
	uint32_t uart_clock;
	GPIO_TypeDef* gpio_bank;
	USART_TypeDef* uart_bank;
	GPIO_InitTypeDef gpio_config;
	USART_InitTypeDef uart_config;
	NVIC_InitTypeDef uart_int_config;
};

static struct uart_driver g_uarts[UART_MAX];


static void uart_config(struct uart_driver *uart)
{
	/* 3. pin init */
	GPIO_Init(uart->gpio_bank, &uart->gpio_config);
	/* 4. uart init */
	USART_Init(uart->uart_bank, &uart->uart_config);
	USART_ClearFlag(uart->uart_bank, USART_FLAG_TC);

	/* 5. enable interrupt */
	USART_ITConfig(uart->uart_bank, USART_IT_RXNE, ENABLE);
	/* 6. enable uart interrupt */
	NVIC_Init(&uart->uart_int_config);

	/* 7. enable uart */
	USART_Cmd(uart->uart_bank, ENABLE);
	uart->status = ENABLE;
}

/**
 * uart_debug - PA9/TX, PA10/RX
 * config: 115200, 8N1
 */
static void uart_debug_init(void)
{
	struct uart_driver *uart = &g_uarts[UART_DEBUG];

	uart->gpio_clock = RCC_AHB1Periph_GPIOA;
	uart->uart_clock = RCC_APB2Periph_USART1;
	uart->gpio_bank = GPIOA;
	uart->uart_bank = USART1;

	uart->gpio_config.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; ;
	uart->gpio_config.GPIO_Mode = GPIO_Mode_AF; // !!!
	uart->gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
	uart->gpio_config.GPIO_OType = GPIO_OType_PP;
	uart->gpio_config.GPIO_PuPd = GPIO_PuPd_UP;

	uart->uart_config.USART_BaudRate = 115200;
	uart->uart_config.USART_WordLength = USART_WordLength_8b; // 8
	uart->uart_config.USART_StopBits = USART_StopBits_1; // 1
	uart->uart_config.USART_Parity = USART_Parity_No; // N
	uart->uart_config.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	uart->uart_config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	/* 1. enable clock*/
	RCC_AHB1PeriphClockCmd(uart->gpio_clock, ENABLE);
	RCC_APB2PeriphClockCmd(uart->uart_clock, ENABLE);
	/* 2. pin alt set */
	GPIO_PinAFConfig(uart->gpio_bank, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(uart->gpio_bank, GPIO_PinSource10, GPIO_AF_USART1);

	/* Group2: PreemptionPriority - 2bits, SubPriority - 2bits */
	uart->uart_int_config.NVIC_IRQChannel = USART1_IRQn;
	uart->uart_int_config.NVIC_IRQChannelPreemptionPriority = 2;
	uart->uart_int_config.NVIC_IRQChannelSubPriority = 2;
	uart->uart_int_config.NVIC_IRQChannelCmd = ENABLE;
	uart_config(uart);
}

int uart_debug_send(const uint8_t *data, int len)
{
	int i = 0, remain = len;
	FlagStatus status;
	struct uart_driver *uart = &g_uarts[UART_DEBUG];

	if (uart->status != ENABLE ||len <= 0) {
		return -1;
	}

	while (remain > 0) {
		do {
			/* 1: Transmit data register empty */
			status = USART_GetFlagStatus(uart->uart_bank, USART_FLAG_TXE);
		} while (SET != status);

		/* can send data now */
		USART_SendData(uart->uart_bank, (uint16_t) data[i]);
		i += 1;
		remain -= 1;
	}

	return len;
}

void USART1_IRQHandler(void)
{
	uint8_t ch;
	struct uart_driver *uart = &g_uarts[UART_DEBUG];

	if (USART_GetITStatus(uart->uart_bank, USART_IT_RXNE) != RESET) {
		/* read USART_DR register will clear RXNE flag */
		ch = USART_ReceiveData(uart->uart_bank);
		uart_debug_send(&ch, 1);
	}
}

int uart_init(void)
{
	uart_debug_init();

	return 0;
}


