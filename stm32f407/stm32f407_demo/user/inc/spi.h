/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/12/29
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _STM32F40X_SPI_H
#define _STM32F40X_SPI_H
#include <driver_util.h>

enum {
	SPI_INDEX_W25Q128 = 0,
	SPI_INDEX_MAX
};


struct spi_gpio {
	struct stf4_clock_info clock;
	GPIO_TypeDef *bank;
	uint32_t pin;
	uint16_t pin_src;
	uint8_t pin_af;
};

struct spi_controller {
	struct stf4_clock_info clock;
	SPI_TypeDef *bank;
	SPI_InitTypeDef spi_config;

	struct spi_gpio sck;
	struct spi_gpio mosi;
	struct spi_gpio miso;
	struct spi_gpio cs;
};


struct spi_driver {
	const struct spi_controller *controller;

	/* index can be one of the SPI_* enum values */
	int (*init)(int index);

	int (*spi_xfer)(const struct spi_controller *controller, uint32_t len, uint8_t *wdata, uint8_t *rdata);
};


extern int spi_init(void);

extern const struct spi_driver *spi_driver_get(int index);

#endif /* _STM32F40X_SPI_H */

