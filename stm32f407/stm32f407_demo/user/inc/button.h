/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/12/9
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _STM32F40X_BTN_H
#define _STM32F40X_BTN_H


enum {
	BTN_UP = 0,     /* Pin-PA0 */
	BTN_DOWN = 1,   /* Pin-PE4 */
	BTN_MAX
};


int btn_init(void);

bool is_btn_active(int which);


#endif /* _STM32F40X_BTN_H */

