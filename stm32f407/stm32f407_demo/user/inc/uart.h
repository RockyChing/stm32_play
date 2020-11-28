/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/28
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _STM32F40X_UART_H
#define _STM32F40X_UART_H


enum {
	UART_DEBUG = 0,
	UART_MCU = 1,
	UART_MAX
};


int uart_init(void);
int uart_debug_send(const uint8_t *data, int len);





#endif /* _STM32F40X_UART_H */

