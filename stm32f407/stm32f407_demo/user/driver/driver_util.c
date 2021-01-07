/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/12/29
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#include <cmn_hdr.h>
#define LOG_TAG "DRV-CMN"

#include "stm32f4xx.h"
#include <utils.h>
#include <log_util.h>
#include <driver_util.h>


struct stf4_clock_config {
	uint32_t periph;
	uint32_t periph_bus;
	int status;
	void (*rcc_clk_cmd)(uint32_t periph, FunctionalState state);

	int reset_status;
	void (*rcc_clk_reset_cmd)(uint32_t periph, FunctionalState state);
};

static struct stf4_clock_config g_clock_config[] = {
	/* AHB1 */
	{ RCC_AHB1Periph_GPIOA,  PERIPH_BUS_AHB1,  -1, RCC_AHB1PeriphClockCmd,  -1, RCC_AHB1PeriphResetCmd },
	{ RCC_AHB1Periph_GPIOB,  PERIPH_BUS_AHB1,  -1, RCC_AHB1PeriphClockCmd,  -1, RCC_AHB1PeriphResetCmd },
	{ RCC_AHB1Periph_GPIOC,  PERIPH_BUS_AHB1,  -1, RCC_AHB1PeriphClockCmd,  -1, RCC_AHB1PeriphResetCmd },
	{ RCC_AHB1Periph_GPIOD,  PERIPH_BUS_AHB1,  -1, RCC_AHB1PeriphClockCmd,  -1, RCC_AHB1PeriphResetCmd },
	{ RCC_AHB1Periph_GPIOE,  PERIPH_BUS_AHB1,  -1, RCC_AHB1PeriphClockCmd,  -1, RCC_AHB1PeriphResetCmd },
	{ RCC_AHB1Periph_GPIOF,  PERIPH_BUS_AHB1,  -1, RCC_AHB1PeriphClockCmd,  -1, RCC_AHB1PeriphResetCmd },
	{ RCC_AHB1Periph_GPIOG,  PERIPH_BUS_AHB1,  -1, RCC_AHB1PeriphClockCmd,  -1, RCC_AHB1PeriphResetCmd },
	{ RCC_AHB1Periph_GPIOH,  PERIPH_BUS_AHB1,  -1, RCC_AHB1PeriphClockCmd,  -1, RCC_AHB1PeriphResetCmd },
	{ RCC_AHB1Periph_GPIOI,  PERIPH_BUS_AHB1,  -1, RCC_AHB1PeriphClockCmd,  -1, RCC_AHB1PeriphResetCmd },
	{ RCC_AHB1Periph_GPIOJ,  PERIPH_BUS_AHB1,  -1, RCC_AHB1PeriphClockCmd,  -1, RCC_AHB1PeriphResetCmd },
	{ RCC_AHB1Periph_GPIOK,  PERIPH_BUS_AHB1,  -1, RCC_AHB1PeriphClockCmd,  -1, RCC_AHB1PeriphResetCmd },
	{ RCC_AHB1Periph_CRC,    PERIPH_BUS_AHB1,  -1, RCC_AHB1PeriphClockCmd,  -1, RCC_AHB1PeriphResetCmd },

	/* AHB2 */
	{ RCC_AHB2Periph_DCMI,   PERIPH_BUS_AHB2,  -1, RCC_AHB2PeriphClockCmd,  -1, RCC_AHB2PeriphResetCmd },
	{ RCC_AHB2Periph_CRYP,   PERIPH_BUS_AHB2,  -1, RCC_AHB2PeriphClockCmd,  -1, RCC_AHB2PeriphResetCmd },
	{ RCC_AHB2Periph_HASH,   PERIPH_BUS_AHB2,  -1, RCC_AHB2PeriphClockCmd,  -1, RCC_AHB2PeriphResetCmd },
	{ RCC_AHB2Periph_RNG,    PERIPH_BUS_AHB2,  -1, RCC_AHB2PeriphClockCmd,  -1, RCC_AHB2PeriphResetCmd },

	/* AHB3 */
	{ RCC_AHB3Periph_FSMC,   PERIPH_BUS_AHB3,  -1, RCC_AHB3PeriphClockCmd,  -1, RCC_AHB3PeriphResetCmd },

	/* APB1 */
	{ RCC_APB1Periph_TIM2,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_TIM3,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_TIM4,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_TIM5,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_TIM6,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_TIM7,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_TIM12,  PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_TIM13,  PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_TIM14,  PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_WWDG,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_SPI2,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_SPI3,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_USART2, PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_USART3, PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_UART4,  PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_UART5,  PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_I2C1,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_I2C2,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_I2C3,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_CAN1,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_CAN2,   PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_PWR,    PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_DAC,    PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_UART7,  PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },
	{ RCC_APB1Periph_UART8,  PERIPH_BUS_APB1,   -1, RCC_APB1PeriphClockCmd,  -1, RCC_APB1PeriphResetCmd },

	/* APB2 */
	{ RCC_APB2Periph_TIM1,    PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_TIM8,    PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_USART1,  PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_USART6,  PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_ADC,     PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_ADC1,    PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_ADC2,    PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_ADC3,    PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_SDIO,    PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_SPI1,    PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_SPI4,    PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_SYSCFG,  PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_EXTIT,   PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_TIM9,    PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_TIM10,   PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_TIM11,   PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_SPI5,    PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
	{ RCC_APB2Periph_SPI6,    PERIPH_BUS_APB2,   -1, RCC_APB2PeriphClockCmd,  -1, RCC_APB2PeriphResetCmd },
};


void stf4_clock_cmd(struct stf4_clock_info *clk, FunctionalState state)
{
	uint32_t found = 0;
	uint32_t i;
	for (i = 0; i < ARRAY_SIZE(g_clock_config); i ++) {
		if (clk->periph == g_clock_config[i].periph && clk->periph_bus == g_clock_config[i].periph_bus) {
			found = 1;
			if (state != g_clock_config[i].status) {
				g_clock_config[i].status = state;
				g_clock_config[i].rcc_clk_cmd(clk->periph, state);
			}
		}
	}

	if (!found) {
		log_fatal("Unknow clock_cmd periph %08x, bus %08x", clk->periph, clk->periph_bus);
	}
}

void stf4_clock_reset_cmd(struct stf4_clock_info *clk, FunctionalState state)
{
	uint32_t found = 0;
	uint32_t i;
	for (i = 0; i < ARRAY_SIZE(g_clock_config); i ++) {
		if (clk->periph == g_clock_config[i].periph && clk->periph_bus == g_clock_config[i].periph_bus) {
			found = 1;
			if (state != g_clock_config[i].reset_status) {
				g_clock_config[i].reset_status = state;
				g_clock_config[i].rcc_clk_reset_cmd(clk->periph, state);
			}
		}
	}

	if (!found) {
		log_fatal("Unknow clock_reset_cmd periph %08x, bus %08x", clk->periph, clk->periph_bus);
	}
}

