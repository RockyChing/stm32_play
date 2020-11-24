/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/24
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#include <cmn_hdr.h>
#include "../driver/delay.h"


void sleep_us(uint32_t us)
{
	if (us < 1000) {
		delay_us(us);
	} else {
		delay_us(1000);
	}
}

void sleep_ms(uint32_t ms)
{
	delay_ms(ms);
}

void sleep_s(uint32_t s)
{
	sleep_ms(1000 * s);
}

