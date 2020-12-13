/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/12/13
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#include <cmn_hdr.h>
#define LOG_TAG "eint"

#include "stm32f4xx.h"
#include <button.h>
#include <ext_int.h>
#include <delay.h>


struct eint_driver {
	uint32_t eint_clock;
	uint32_t eint_gpio_clock;
	uint8_t eint_port_src;
	uint8_t eint_gpio_src;
	GPIO_TypeDef* eint_gpio_bank;
	GPIO_InitTypeDef eint_gpio_cfg;
	EXTI_InitTypeDef eint_exit_cfg;
	NVIC_InitTypeDef eint_nvic_cfg;
	uint8_t state;
};


static struct eint_driver g_btn_int;

/**
 * BTN_UP: PA0
 */
static void btn_int_init(void)
{
	struct eint_driver *pint = &g_btn_int;

	pint->eint_gpio_clock = RCC_AHB1Periph_GPIOA;
	pint->eint_gpio_bank = GPIOA;
	pint->eint_gpio_cfg.GPIO_Pin = GPIO_Pin_0;
	pint->eint_gpio_cfg.GPIO_Mode = GPIO_Mode_IN;
	pint->eint_gpio_cfg.GPIO_Speed = GPIO_Speed_100MHz;
	pint->eint_gpio_cfg.GPIO_PuPd = GPIO_PuPd_DOWN;

	pint->eint_clock = RCC_APB2Periph_SYSCFG;

	pint->eint_port_src = EXTI_PortSourceGPIOA;
	pint->eint_gpio_src = EXTI_PinSource0;

	pint->eint_exit_cfg.EXTI_Line = EXTI_Line0;
	pint->eint_exit_cfg.EXTI_Mode = EXTI_Mode_Interrupt;
	pint->eint_exit_cfg.EXTI_Trigger = EXTI_Trigger_Rising;
	pint->eint_exit_cfg.EXTI_LineCmd = ENABLE;

	pint->eint_nvic_cfg.NVIC_IRQChannel = EXTI0_IRQn;
	pint->eint_nvic_cfg.NVIC_IRQChannelPreemptionPriority = 2;
	pint->eint_nvic_cfg.NVIC_IRQChannelSubPriority = 2;
	pint->eint_nvic_cfg.NVIC_IRQChannelCmd = ENABLE;

	/* gpio config */
	RCC_AHB1PeriphClockCmd(pint->eint_gpio_clock, ENABLE);
	GPIO_Init(pint->eint_gpio_bank, &pint->eint_gpio_cfg);

	/* enable SYSCFG clk */
	RCC_APB2PeriphClockCmd(pint->eint_clock, ENABLE);

	/* gpio bind */
	SYSCFG_EXTILineConfig(pint->eint_port_src, pint->eint_gpio_src);

	/* EXIT config */
	EXTI_Init(&pint->eint_exit_cfg);

	/* NVIC config */
	NVIC_Init(&pint->eint_nvic_cfg);

	pint->state = EINT_DEACTIVE;
}

void EXTI0_IRQHandler()
{
	BitAction action;

	delay_ms(50);
	action = GPIO_ReadInputDataBit(g_btn_int.eint_gpio_bank, g_btn_int.eint_gpio_cfg.GPIO_Pin);
	if (Bit_SET == action) {
		g_btn_int.state ^= Bit_SET;
	}

	/* if not clear, ISR will enter through no interrupt happens */
	EXTI_ClearITPendingBit(EXTI_Line0);
}

int eint_init(void)
{
#if BTN_INT
	btn_int_init();
#else
#endif
	return 0;
}

uint8_t eint_state_get(void)
{
	return g_btn_int.state;
}

