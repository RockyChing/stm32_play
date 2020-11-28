/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/24
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#include "stm32f4xx.h"


int interrupt_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	return 0;
}

