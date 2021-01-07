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
#include <log_util.h>
#include <spi.h>

/**
 * W25Q128 128Mb, 16 MByte
 * 16 MByte, 256 Block, 64KB/Block
 * Block, 16 Sector, 4KB/Sector
 */


static const struct spi_driver *g_w25qxx_driver;

static uint8_t dummy = 0x5a;



int w25qxx_init(void)
{
	g_w25qxx_driver = spi_driver_get(SPI_INDEX_W25Q128);
	if (NULL == g_w25qxx_driver) {
		log_fatal("null w25qxx_driver!\n");
		return -1;
	}

	return 0;
}

int w25qxx_xfer(uint32_t len, uint8_t *wdata, uint8_t *rdata)
{
	return g_w25qxx_driver->spi_xfer(g_w25qxx_driver->controller, len, wdata, rdata);
}

/**w25qxx
 * EF13 -> W25Q80
 * EF14 -> W25Q16
 * EF15 -> W25Q32
 * EF16 -> W25Q64
 * EF17 -> W25Q128
 **n25qxx
 * 5217 -> N25Q128
 */
uint16_t w25qxx_chipid(void)
{
	uint8_t req[6] = { 0x90, dummy, dummy, 0x00, dummy, dummy };
	uint8_t resp[sizeof(req)];
	uint16_t id;

	w25qxx_xfer(sizeof(req), req, resp);
	id = resp[4];
	id = (id << 8) | resp[5];
	return id;
}

