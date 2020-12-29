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
#define LOG_TAG "i2c-sw"

#include "stm32f4xx.h"
#include <utils.h>
#include <delay.h>

#include "i2c-software.h"


/**
 * i2c implementation of software, in stead of i2c-controller
 */

#define i2c_sda_dir_in(bank, pinpos) do { \
		bank->MODER &= ~(GPIO_MODER_MODER0 << (pinpos * 2)); \
		bank->MODER |= (((uint32_t)GPIO_Mode_IN) << (pinpos * 2)); \
	} while (0)

#define i2c_sda_dir_out(bank, pinpos) do { \
		bank->MODER &= ~(GPIO_MODER_MODER0 << (pinpos * 2)); \
		bank->MODER |= (((uint32_t)GPIO_Mode_OUT) << (pinpos * 2)); \
	} while (0)

#define i2c_sda_out_h(bank, pinpos) GPIO_SetBits(bank, pinpos)
#define i2c_sda_out_l(bank, pinpos) GPIO_ResetBits(bank, pinpos)
#define i2c_sda_read(bank, pinpos)  GPIO_ReadInputDataBit(bank, pinpos)

#define i2c_scl_out_h(bank, pinpos) GPIO_SetBits(bank, pinpos)
#define i2c_scl_out_l(bank, pinpos) GPIO_ResetBits(bank, pinpos)


struct i2c_controller *g_i2c_controllers[I2C_CTL_SW_MAX];

static void i2c_pin_init(struct i2c_pin_info *pin)
{
	GPIO_InitTypeDef pin_cfg;
	RCC_AHB1PeriphClockCmd(pin->clock, ENABLE);

	pin_cfg.GPIO_Pin = pin->pin;
	pin_cfg.GPIO_Mode = GPIO_Mode_OUT;
	pin_cfg.GPIO_OType = GPIO_OType_PP;
	pin_cfg.GPIO_Speed = GPIO_Speed_100MHz;
	pin_cfg.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(pin->bank, &pin_cfg);
	/* idle high */
	GPIO_SetBits(pin->bank, pin->pin);
}

static void i2c_start(int index)
{
	struct i2c_controller *ctl;
	struct i2c_pin_info *sda, *scl;

	sda = &ctl->sda;
	scl = &ctl->scl;
	i2c_sda_dir_out(sda->bank, sda->pin);

	i2c_sda_out_h(sda->bank, sda->pin);
	i2c_scl_out_h(scl->bank, scl->pin);
	delay_us(5);
	i2c_sda_out_l(sda->bank, sda->pin);
	delay_us(5);
	i2c_scl_out_l(scl->bank, scl->pin);
}

static int i2c_stop(int index)
{
	struct i2c_controller *ctl;
	struct i2c_pin_info *sda, *scl;

	sda = &ctl->sda;
	scl = &ctl->scl;
	i2c_sda_dir_out(sda->bank, sda->pin);

	i2c_sda_out_l(sda->bank, sda->pin);
	i2c_scl_out_h(scl->bank, scl->pin);
	delay_us(5);

	i2c_sda_out_h(sda->bank, sda->pin);
	delay_us(5);
}

static int i2c_wait_ack(int index)
{
	struct i2c_controller *ctl = g_i2c_controllers[index];
	struct i2c_pin_info *sda, *scl;
	int timeout = 0;

	sda = &ctl->sda;
	scl = &ctl->scl;
	i2c_sda_dir_in(sda->bank, sda->pin);

	i2c_scl_out_l(sda->bank, sda->pin);
	delay_us(2);
	i2c_scl_out_h(scl->bank, scl->pin);
	delay_us(2);

	while (1) {
		if (i2c_sda_read(sda->bank, sda->pin)) {
			break;
		}
		timeout ++;
		if (timeout > 200) {
			i2c_stop(index);
			return -1;
		}
	}

	i2c_scl_out_l(scl->bank, scl->pin);
	delay_us(2);
	return 0;
}

static int i2c_send_ack(int index)
{
	struct i2c_controller *ctl = g_i2c_controllers[index];
	struct i2c_pin_info *sda, *scl;

	sda = &ctl->sda;
	scl = &ctl->scl;
	i2c_sda_dir_out(sda->bank, sda->pin);

	i2c_sda_out_l(sda->bank, sda->pin);
	i2c_scl_out_h(scl->bank, scl->pin);
	delay_us(2);
	i2c_scl_out_l(scl->bank, scl->pin);
	delay_us(2);
}

static int i2c_send_nack(int index)
{
	struct i2c_controller *ctl = g_i2c_controllers[index];
	struct i2c_pin_info *sda, *scl;

	sda = &ctl->sda;
	scl = &ctl->scl;
	i2c_sda_dir_out(sda->bank, sda->pin);

	i2c_sda_out_h(sda->bank, sda->pin);
	i2c_scl_out_h(scl->bank, scl->pin);
	delay_us(2);
	i2c_scl_out_l(scl->bank, scl->pin);
	delay_us(2);
}

void i2c_send_byte(int index, uint8_t data)
{
	struct i2c_controller *ctl = g_i2c_controllers[index];
	struct i2c_pin_info *sda, *scl;
	int i;

	sda = &ctl->sda;
	scl = &ctl->scl;
	i2c_sda_dir_out(sda->bank, sda->pin);

	for (i = 0; i < 8; i ++) {
		i2c_scl_out_l(scl->bank, scl->pin);
		if ((data << i) & 0x80)
			i2c_sda_out_h(sda->bank, sda->pin);
		else
			i2c_sda_out_l(sda->bank, sda->pin);

		delay_us(2);
		i2c_scl_out_h(scl->bank, scl->pin);
		delay_us(2);
		i2c_scl_out_l(scl->bank, scl->pin);
		delay_us(2);
	}

	i2c_wait_ack(index);
}

uint8_t i2c_recv_byte(int index, int ack)
{
	struct i2c_controller *ctl = g_i2c_controllers[index];
	struct i2c_pin_info *sda, *scl;
	uint8_t data = 0;
	int i;

	sda = &ctl->sda;
	scl = &ctl->scl;
	i2c_sda_dir_in(sda->bank, sda->pin);

	for (i = 0; i < 8; i++) {
		data <<= 1;
		i2c_scl_out_h(scl->bank, scl->pin);
		delay_us(2);

		if (i2c_sda_read(sda->bank, sda->pin)) {
			data |= 1;
		}

		i2c_scl_out_l(scl->bank, scl->pin);
		delay_us(2);
	}

	if (ack) {
		//i2c_send_ack(index);
	} else {
		//i2c_send_nack(index);
	}

	return data;
}

void i2c_controller_register(int index, struct i2c_controller *controller)
{
	g_i2c_controllers[index] = controller;
}
