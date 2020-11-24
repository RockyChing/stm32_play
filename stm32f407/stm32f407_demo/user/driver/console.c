/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/23
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#include <stdio.h>


#pragma import(__use_no_semihosting)

struct __FILE {
	int handle;
};

FILE __stdout;

void _sys_exit(int x)
{
	x = x;
}

int fputc(int ch, FILE *f)
{
#if 0
	while((USART1->ISR & 0X40) == 0) {}

	USART1->TDR = (u8) ch;
#endif
	return ch;
}

int fgetc(FILE *f)
{
	int ch;
#if 0
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) {}
	ch = USART_ReceiveData(USART1);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}
	USART_SendData(USART1, (uint8_t) ch);
#endif
	return ch;
}

