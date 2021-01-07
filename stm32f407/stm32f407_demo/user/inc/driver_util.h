/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/12/29
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _STM32F40X_DRV_UTIL_H
#define _STM32F40X_DRV_UTIL_H


enum {
	PERIPH_BUS_AHB1 = 0,
	PERIPH_BUS_AHB2,
	PERIPH_BUS_AHB3,
	PERIPH_BUS_APB1,
	PERIPH_BUS_APB2,
	PERIPH_BUS_MAX
};

struct stf4_clock_info {
	uint32_t periph;
	uint32_t periph_bus;
};

void stf4_clock_cmd(struct stf4_clock_info *clk, FunctionalState state);
void stf4_clock_reset_cmd(struct stf4_clock_info *clk, FunctionalState state);


#endif /* _STM32F40X_DRV_UTIL_H */

