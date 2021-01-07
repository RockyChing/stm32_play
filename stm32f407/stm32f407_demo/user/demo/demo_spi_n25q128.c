/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/12/31
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#include <demo.h>

#if DEMO_SPI_N25QXX_EN
#include <cmn_hdr.h>
#define LOG_TAG "n25q128"

#include "stm32f4xx.h"
#include <log_util.h>
#include <utils.h>
#include <delay.h>
#include <spi_w25qxx.h>


void demo_n25qxx(void)
{
	uint16_t chipid;
	do {
		chipid = w25qxx_chipid();
		if (SPI_FLASH_CHIPID_N25Q128 == chipid) {
			log_info("chipid: %04x", chipid);
			break;
		}

		log_warn("Incalid chipid %04x, retry...", chipid);
		delay_ms(1000);
	} while (1);
}

#else /* DEMO_SPI_N25QXX_EN */
void demo_n25qxx(void) {}
#endif /* DEMO_SPI_N25QXX_EN */

