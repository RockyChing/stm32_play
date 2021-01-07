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
#define LOG_TAG "spi"

#include "stm32f4xx.h"
#include <spi.h>

#include <utils.h>
#include <driver_util.h>
#include "spi_controller.h"


static struct spi_controller g_spi_controllers[SPI_INDEX_MAX];


/**
 * spi_read_write - read or write one byte
 * @controller: controller we belong to
 * @wb: one byte to write
 * @rb: ont byte to read
 */
int spi_read_write(const struct spi_controller *controller, uint8_t wb, uint8_t *rb)
{
	//if (NULL == controller)
	//	return -1;

	/* wait for tx empty*/
	while (SPI_I2S_GetFlagStatus(controller->bank, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(controller->bank, wb);

	/* wait for rx not empty*/
	while (SPI_I2S_GetFlagStatus(controller->bank, SPI_I2S_FLAG_RXNE) == RESET);
	*rb = SPI_I2S_ReceiveData(controller->bank);
	return 0;
}

static void spi_controller_config(struct spi_controller *controller)
{
	GPIO_InitTypeDef gpio_config;
	struct stf4_clock_info clock[5];
	int i;

	memcpy(&clock[0], &controller->clock, sizeof(struct stf4_clock_info));
	memcpy(&clock[1], &controller->sck.clock, sizeof(struct stf4_clock_info));
	memcpy(&clock[2], &controller->mosi.clock, sizeof(struct stf4_clock_info));
	memcpy(&clock[3], &controller->miso.clock, sizeof(struct stf4_clock_info));
	memcpy(&clock[4], &controller->cs.clock, sizeof(struct stf4_clock_info));

	// 1. clock enable
	for (i = 0; i < ARRAY_SIZE(clock); i ++) {
		stf4_clock_cmd(&clock[i], ENABLE);
	}

	// 2. gpio init
	gpio_config.GPIO_Mode = GPIO_Mode_AF;
	gpio_config.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_config.GPIO_OType = GPIO_OType_PP;
	gpio_config.GPIO_PuPd = GPIO_PuPd_UP;

	// 3.1 SCK pin AF
	gpio_config.GPIO_Pin = controller->sck.pin;
	GPIO_Init(controller->sck.bank, &gpio_config);
	GPIO_PinAFConfig(controller->sck.bank, controller->sck.pin_src, controller->sck.pin_af);

	// 3.2 MOSI pin AF
	gpio_config.GPIO_Pin = controller->mosi.pin;
	GPIO_Init(controller->mosi.bank, &gpio_config);
	GPIO_PinAFConfig(controller->mosi.bank, controller->mosi.pin_src, controller->mosi.pin_af);

	// 3.3 MISO pin AF
	gpio_config.GPIO_Pin = controller->miso.pin;
	GPIO_Init(controller->miso.bank, &gpio_config);
	GPIO_PinAFConfig(controller->miso.bank, controller->miso.pin_src, controller->miso.pin_af);

	// 3.4 CS pin
	gpio_config.GPIO_Mode = GPIO_Mode_OUT;
	gpio_config.GPIO_Pin = controller->cs.pin;
	GPIO_Init(controller->cs.bank, &gpio_config);

	// 4. spi init
	stf4_clock_reset_cmd(&controller->clock, ENABLE);   // reset SPI1
	stf4_clock_reset_cmd(&controller->clock, DISABLE);  // stop reseting
	SPI_Init(controller->bank, &controller->spi_config);

	// 5. spi enabled
	SPI_Cmd(controller->bank, ENABLE);

	spi_cs_deactive(controller);
}

static void _spi_controller_init(void)
{
	struct spi_controller *controller;
	int i;

	/**
	 * SPI_INDEX_W25Q128 -> SPI1, master
	 * SPI1_SCK -> PB3
	 * SPI1_MISO -> PB4
	 * SPI1_MOSI -> PB5
	 * CS -> PB14
	 */
	controller = &g_spi_controllers[SPI_INDEX_W25Q128];
	controller->clock.periph = RCC_APB2Periph_SPI1;
	controller->clock.periph_bus = PERIPH_BUS_APB2;

	// PB3
	controller->sck.clock.periph = RCC_AHB1Periph_GPIOB;
	controller->sck.clock.periph_bus = PERIPH_BUS_AHB1;
	controller->sck.bank = GPIOB;
	controller->sck.pin = GPIO_Pin_3;
	controller->sck.pin_src = GPIO_PinSource3;
	controller->sck.pin_af = GPIO_AF_SPI1;
	// PB4
	controller->miso.clock.periph = RCC_AHB1Periph_GPIOB;
	controller->sck.clock.periph_bus = PERIPH_BUS_AHB1;
	controller->miso.bank = GPIOB;
	controller->miso.pin = GPIO_Pin_4;
	controller->miso.pin_src = GPIO_PinSource4;
	controller->miso.pin_af = GPIO_AF_SPI1;
	// PB5
	controller->mosi.clock.periph = RCC_AHB1Periph_GPIOB;
	controller->sck.clock.periph_bus = PERIPH_BUS_AHB1;
	controller->mosi.bank = GPIOB;
	controller->mosi.pin = GPIO_Pin_5;
	controller->mosi.pin_src = GPIO_PinSource5;
	controller->mosi.pin_af = GPIO_AF_SPI1;
	// PB14
	controller->cs.clock.periph = RCC_AHB1Periph_GPIOB;
	controller->sck.clock.periph_bus = PERIPH_BUS_AHB1;
	controller->cs.bank = GPIOB;
	controller->cs.pin = GPIO_Pin_14;

	controller->bank = SPI1;
	controller->spi_config.SPI_Direction = SPI_Direction_2Lines_FullDuplex;;
	controller->spi_config.SPI_Mode = SPI_Mode_Master;
	controller->spi_config.SPI_DataSize = SPI_DataSize_8b;
	controller->spi_config.SPI_CPOL = SPI_CPOL_High;
	controller->spi_config.SPI_CPHA = SPI_CPHA_2Edge;
	controller->spi_config.SPI_NSS = SPI_NSS_Soft;
	/* 84Mhz, 4 Prescaler -> 21MHz clock */
	controller->spi_config.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	controller->spi_config.SPI_FirstBit = SPI_FirstBit_MSB;
	controller->spi_config.SPI_CRCPolynomial = 7;

	for (i = 0; i < ARRAY_SIZE(g_spi_controllers); i ++) {
		spi_controller_config(&g_spi_controllers[i]);
	}
}

void spi_controller_init(void)
{
	_spi_controller_init();
}

const struct spi_controller *spi_controller_get(int index)
{
	if (index >= ARRAY_SIZE(g_spi_controllers))
		return NULL;
	return &g_spi_controllers[index];
}

