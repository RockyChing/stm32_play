/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/27
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#include <cmn_hdr.h>
#define LOG_TAG "led"

#include "stm32f4xx.h"
#include <led.h>


enum {
	LED_ACTIVE_LOW = 0,
	LED_ACTIVE_HIGH = 1,
};

struct led_driver {
	uint32_t gpio_clock;
	GPIO_TypeDef* gpio_bank;
	GPIO_InitTypeDef gpio_config;
	uint32_t gpio_active_state;
};

static struct led_driver g_leds[LED_MAX];

static void led_config(struct led_driver *led)
{
	RCC_AHB1PeriphClockCmd(led->gpio_clock, ENABLE);
	GPIO_Init(led->gpio_bank, &led->gpio_config);
	if (LED_ACTIVE_LOW == led->gpio_active_state) {
		GPIO_SetBits(led->gpio_bank, led->gpio_config.GPIO_Pin);
	} else if (LED_ACTIVE_HIGH == led->gpio_active_state) {
		GPIO_ResetBits(led->gpio_bank, led->gpio_config.GPIO_Pin);
	} else {
	}
}

static void led_red_init(void)
{
	struct led_driver *led = &g_leds[LED_RED];
	led->gpio_clock = RCC_AHB1Periph_GPIOF;
	led->gpio_bank = GPIOF;
	led->gpio_active_state = LED_ACTIVE_LOW;

	led->gpio_config.GPIO_Pin = GPIO_Pin_9;
	led->gpio_config.GPIO_Mode = GPIO_Mode_OUT;
	led->gpio_config.GPIO_OType = GPIO_OType_PP;
	led->gpio_config.GPIO_Speed = GPIO_Speed_100MHz;
	led->gpio_config.GPIO_PuPd = GPIO_PuPd_UP;
	led_config(led);
}

static void led_green_init(void)
{
	struct led_driver *led = &g_leds[LED_GREEN];
	led->gpio_clock = RCC_AHB1Periph_GPIOF;
	led->gpio_bank = GPIOF;
	led->gpio_active_state = LED_ACTIVE_LOW;

	led->gpio_config.GPIO_Pin = GPIO_Pin_10;
	led->gpio_config.GPIO_Mode = GPIO_Mode_OUT;
	led->gpio_config.GPIO_OType = GPIO_OType_PP;
	led->gpio_config.GPIO_Speed = GPIO_Speed_100MHz;
	led->gpio_config.GPIO_PuPd = GPIO_PuPd_UP;
	led_config(led);
}

int led_init(void)
{
	led_red_init();
	led_green_init();
	return 0;
}

int led_ctl(int which, int onoff)
{
	struct led_driver *led;

	if (LED_RED != which && LED_GREEN != which) {
		// log_warn("Unknow led number %d", which);
		return -1;
	}

	led = &g_leds[which];
	if (LED_CTL_ON == onoff) {
		if (LED_ACTIVE_HIGH == led->gpio_active_state) {
			GPIO_SetBits(led->gpio_bank, led->gpio_config.GPIO_Pin);
		} else if (LED_ACTIVE_LOW == led->gpio_active_state) {
			GPIO_ResetBits(led->gpio_bank, led->gpio_config.GPIO_Pin);
		}
	} else {
		if (LED_ACTIVE_HIGH == led->gpio_active_state) {
			GPIO_ResetBits(led->gpio_bank, led->gpio_config.GPIO_Pin);
		} else if (LED_ACTIVE_LOW == led->gpio_active_state) {
			GPIO_SetBits(led->gpio_bank, led->gpio_config.GPIO_Pin);
		}
	}

	return 0;
}

