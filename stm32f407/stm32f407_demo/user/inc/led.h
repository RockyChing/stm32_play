/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/27
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _STM32F40X_H
#define _STM32F40X_H

enum {
	LED_RED = 0,
	LED_GREEN = 1,
};

enum {
	LED_CTL_OFF = 0,
	LED_CTL_ON = 1,
};


int led_init(void);

/**
 * led_ctl - led control on or off
 * @which: LED_RED or LED_GREEN
 * @onoff: LED_CTL_OFF or LED_CTL_ON
 */
int led_ctl(int which, int onoff);

#endif /* _STM32F40X_H */

