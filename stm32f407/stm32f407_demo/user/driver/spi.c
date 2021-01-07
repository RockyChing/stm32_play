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
#include <utils.h>
#include <spi.h>
#include "spi_controller.h"


struct spi_driver g_spi_drivers[SPI_INDEX_MAX];


static int spi_xfer(const struct spi_controller *controller, uint32_t len, uint8_t *wdata, uint8_t *rdata)
{
	uint8_t dummy = 0x5a;
	uint32_t i;

	if (wdata == NULL && rdata == NULL)
		return -1;

	/*clear fifo first*/
	// TODO

	/* cs enable */
	spi_cs_active(controller);

	for (i = 0; i < len; i ++) {
		if (rdata != NULL) {
			if (wdata != NULL) {
				spi_read_write(controller, wdata[i], &rdata[i]);
			} else {
				spi_read_write(controller, dummy, &rdata[i]);
			}
		} else {
			spi_read_write(controller, wdata[i], &dummy);
		}
	}

	/* cs disable */
	spi_cs_deactive(controller);
	return 0;
}

int spi_init(void)
{
	int i;

	spi_controller_init();

	for (i = 0; i < ARRAY_SIZE(g_spi_drivers); i ++) {
		g_spi_drivers[i].controller = spi_controller_get(i);
		if (NULL == g_spi_drivers[i].controller) {
			log_fatal("null spi_controller!\n");
			return -1;
		}

		g_spi_drivers[i].spi_xfer = spi_xfer;
	}
	return 0;
}

const struct spi_driver *spi_driver_get(int index)
{
	if (index >= ARRAY_SIZE(g_spi_drivers))
		return NULL;
	return &g_spi_drivers[index];
}
