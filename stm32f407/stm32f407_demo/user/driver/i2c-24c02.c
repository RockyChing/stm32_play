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
#define LOG_TAG "i2c-24c02"

#include "stm32f4xx.h"

enum {
	AT24C02_I2C_HW = 0,
	AT24C02_I2C_SW
};

#define AT24C02_I2C_SEL (AT24C02_I2C_HW)

#if AT24C02_I2C_SEL == AT24C02_I2C_SW
#include "i2c-software.h"
#endif



#define AT24C02_I2C_ADDR (0x)


/**
 * SCL - PB8
 * SDA - PB9
 */
struct i2c_controller g_at24c02_controller;

int at24c02_init(void)
{
	struct i2c_controller *ctl = &g_at24c02_controller;
	ctl->state = I2C_SW_ENABLE;
	ctl->sda.clock = RCC_AHB1Periph_GPIOB;
	ctl->sda.bank  = GPIOB;
	ctl->sda.pin = GPIO_Pin_9;
	ctl->scl.clock = RCC_AHB1Periph_GPIOB;
	ctl->scl.bank  = GPIOB;
	ctl->scl.pin = GPIO_Pin_8;

	i2c_controller_register(I2C_CTL_SW_AT24C, ctl);
	return 0;
}

