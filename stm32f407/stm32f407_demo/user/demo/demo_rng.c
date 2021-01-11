/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2021/1/8
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */
#include <demo.h>

#if DEMO_RNG_EN
#include <cmn_hdr.h>
#define LOG_TAG "rng"

#include <rng.h>


void demo_rng(void)
{
	uint32_t i, r;
	for (i = 0; i < 1000; i ++) {
		r = rng_get();
		printf("%u ", r);
	}
}


#else /* DEMO_RNG_EN */
void demo_rng(void) {}
#endif /* DEMO_RNG_EN */
