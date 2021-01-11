/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2021/1/8
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */
#include <cmn_hdr.h>
#define LOG_TAG "rng"

#include "stm32f4xx.h"
#include <log_util.h>
#include <delay.h>
#include <driver_util.h>
#include <rng.h>


int rng_init(void)
{
	struct stf4_clock_info clk;

	// 1. clk enable
	clk.periph = RCC_AHB2Periph_RNG;
	clk.periph_bus = PERIPH_BUS_AHB2;
	//stf4_clock_cmd(&clk, ENABLE);
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

	// 2. RNG enable
	RNG_Cmd(ENABLE);

	rng_get();
	return 0;
}

uint32_t rng_get(void)
{
	uint32_t pseudo_random;
	int retry;

	retry = 0;
	do {
		if (RNG_GetFlagStatus(RNG_FLAG_DRDY) == SET)
			break;
		retry += 1;
		delay_us(100);
	} while (retry < 10000);

	if (retry >= 10000) {
		log_warn("using pseudo random...");
		return pseudo_random;
	}

	return RNG_GetRandomNumber();
}

