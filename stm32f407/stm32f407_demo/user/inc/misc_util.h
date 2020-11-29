/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/24
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _MISC_UTIL_H
#define _MISC_UTIL_H


typedef struct {
	uint32_t SYSCLK_Frequency; /*!<  SYSCLK clock frequency expressed in Hz */
	uint32_t HCLK_Frequency;   /*!<  HCLK clock frequency expressed in Hz   */
	uint32_t PCLK1_Frequency;  /*!<  PCLK1 clock frequency expressed in Hz  */
	uint32_t PCLK2_Frequency;  /*!<  PCLK2 clock frequency expressed in Hz  */
} stf4_clock_t;

void stf4_get_cpuid(uint32_t u_id[3]);

void stf4_get_sysclk(stf4_clock_t *clk);




#endif /* _MISC_UTIL_H */

