/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/30
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

/**
 * SysTick timer, refer to 4.5 SysTick timer (STK), PM0214 Programming manual
 * <STM32F3 and STM32F4 Series Cortex®-M4 programming manual>
 *
 * Notice: When the processor is halted for debugging the counter does not decrement
 */

#include <cmn_hdr.h>

#include "stm32f4xx.h"
#include "core_cm4.h"
#include "sys_tick.h"


int sys_tick_init(void)
{
	uint32_t reload;

	reload = STK_CNT_USEC; /* 1us */
	reload *= 1000; /* 1ms */
	reload *= STK_TIMEOUT_10MS; /* 10ms */

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); /* 168M / 8*/
	SysTick->VAL = 0;
	SysTick->LOAD = reload - 1;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; /* Counting down to zero to asserts the SysTick exception request. */
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; /* enable STK */

	return 0;
}

uint32_t sys_tick_get_reload(void)
{
	return (uint32_t) SysTick->LOAD;
}

uint32_t sys_tick_get_val(void)
{
	return (uint32_t) SysTick->VAL;
}

