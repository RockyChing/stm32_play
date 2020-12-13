/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/12/13
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _STM32F40X_EINT_H
#define _STM32F40X_EINT_H


enum {
	EINT_DEACTIVE = 0,
	EINT_ACTIVE = 1,
};


int eint_init(void);

uint8_t eint_state_get(void);


#endif /* _STM32F40X_EINT_H */

