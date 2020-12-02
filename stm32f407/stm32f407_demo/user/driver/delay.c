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

#include "sys_tick.h"

#define USEC_TO_COUNT(x)	(STK_CNT_USEC * (x))


void delay_us(uint32_t us)
{
	long long int counts = (long long int) USEC_TO_COUNT(us);
	const uint32_t C_RELOAD = sys_tick_get_reload();
	uint32_t now, last = sys_tick_get_val();

	while (counts > 0) {
		now = sys_tick_get_val();
		if (now <= last) { // normal
			counts -= (last - now);
		} else { // rollback
			counts -= (C_RELOAD - now + last);
		}
		last = now;
	}
}

void delay_ms(uint32_t ms)
{
	const uint32_t div = STK_TIMEOUT_10MS - 1;
	const uint32_t idelay = div * 1000;
	uint32_t quotient = ms / div;
	uint32_t remain = ms % div;

	while (quotient > 0) {
		delay_us(idelay);
		quotient -= 1;
	}

	delay_us(remain * 1000);
}

