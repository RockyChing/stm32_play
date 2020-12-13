/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/12/9
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#include <cmn_hdr.h>
#define LOG_TAG "btn"

#include "stm32f4xx.h"
#include <button.h>
#include <delay.h>


enum {
	BTN_ACTIVE_LOW = 0,
	BTN_ACTIVE_HIGH = 1,
};

struct btn_driver {
	uint32_t btn_clock;
	GPIO_TypeDef* btn_bank;
	uint16_t btn_pin;
	uint32_t btn_active_state;
};


static struct btn_driver btn_devices[BTN_MAX];


static void _btn_config(struct btn_driver *pbtn)
{
	GPIO_InitTypeDef btn_pin_cfg;

	/* enable clk */
	if (IS_RCC_AHB1_CLOCK_PERIPH(pbtn->btn_clock))
		RCC_AHB1PeriphClockCmd(pbtn->btn_clock, ENABLE);
	else if (IS_RCC_AHB2_PERIPH(pbtn->btn_clock))
		RCC_AHB2PeriphClockCmd(pbtn->btn_clock, ENABLE);
	else if (IS_RCC_AHB3_PERIPH(pbtn->btn_clock))
		RCC_AHB3PeriphClockCmd(pbtn->btn_clock, ENABLE);
	else { /**/ }

	btn_pin_cfg.GPIO_Pin = pbtn->btn_pin;
	btn_pin_cfg.GPIO_Mode = GPIO_Mode_IN;
	btn_pin_cfg.GPIO_Speed = GPIO_Speed_100MHz;
	btn_pin_cfg.GPIO_PuPd = (BTN_ACTIVE_HIGH == pbtn->btn_active_state) ? GPIO_PuPd_DOWN : GPIO_PuPd_UP;
	GPIO_Init(pbtn->btn_bank, &btn_pin_cfg);
}

static void btn_config(void)
{
	struct btn_driver *pbtn = &btn_devices[BTN_UP];
	/* BTN_UP: PA0 */
	pbtn->btn_clock = RCC_AHB1Periph_GPIOA;
	pbtn->btn_bank = GPIOA;
	pbtn->btn_pin = GPIO_Pin_0;
	pbtn->btn_active_state = BTN_ACTIVE_HIGH;
	_btn_config(pbtn);

	/* BTN_DOWN: PE4 */
	pbtn = &btn_devices[BTN_DOWN];
	pbtn->btn_clock = RCC_AHB1Periph_GPIOE;
	pbtn->btn_bank = GPIOE;
	pbtn->btn_pin = GPIO_Pin_4;
	pbtn->btn_active_state = BTN_ACTIVE_HIGH;
	_btn_config(pbtn);
}

static inline bool is_btn_down(int which)
{
	//if (which < BTN_UP || which >= BTN_MAX)
	//	return false;
	return (Bit_SET == GPIO_ReadInputDataBit(btn_devices[which].btn_bank, btn_devices[which].btn_pin));
}

int btn_init(void)
{
#if BTN_INT

#else
	btn_config();
#endif
	return 0;
}

bool is_btn_active(int which)
{
#if BTN_INT
	return false;
#else
	/* do debounce things */
	if (is_btn_down(which))
		delay_ms(50);

	/* re-check */
	return is_btn_down(which);
#endif
}

