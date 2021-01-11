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
#include <stdlib.h>

#define LOG_TAG "n25q128"

#include "stm32f4xx.h"
#include <log_util.h>
#include <utils.h>
#include <delay.h>
#include <md5.h>
#include <rng.h>
#include <spi_w25qxx.h>


extern void n25qxx_write_sector(uint32_t index, void *buf, size_t count);
extern void n25qxx_read_sector(uint32_t index, void *buf, size_t count);

static uint8_t   g_buffer[64 * 1024] = { 0, };


static void random_get(uint8_t *data, uint32_t len)
{
#if 0
	unsigned int i;
	long int r;

	srandom(i);
	for (i = 0; i < len; i + sizeof(r)) {
		r = random();
		memcpy(data, &r, sizeof(r));
		data += sizeof(r);
	}
#else
	uint32_t i, r;
	uint32_t *pdata = (uint32_t *) data;

	for (i = 0; i < len / 4; i ++) {
		r= rng_get();
		pdata[i] = r;
	}
#endif
}


/**
 * 1. read random number from hard-random-generator
 * 2. calc the md5sum
 * 3. write to n25q128 flash
 * 4. read back from n25q128
 * 5. calc the read-back-data md5sum
 * 6. compare md5sum
 * each loop data changed and data-len increased
 */
static void n25qxx_write_read_test(void)
{
	uint32_t i, j = 0;
	uint32_t len = 0, len_step = 50;
	uint32_t fail_count = 0;
	uint8_t sec_index = 200;
	uint8_t *data = (uint8_t *) g_buffer;
	uint8_t md5_w[16], md5_r[16];

	for (i = 0; i < sizeof(g_buffer); i += len_step) { // test loop
		len += len_step;
		if (len > sizeof(g_buffer))
			break;

		random_get(data, len);
		MD5_calc(data, len, md5_w);

		n25qxx_write_sector(sec_index, data, len);

		n25qxx_read_sector(sec_index, data, len);
		MD5_calc(data, len, md5_r);

		printf("n25qxx_write_read_test time %u, data_len %u", ++j, len);
		if (0 == memcmp(md5_w, md5_r, sizeof(md5_r))) {
			printf(" ok.\n");
		} else {
			printf(" failed!\n\n");
			fail_count ++;
		}
		delay_ms(10);
	}

	if (fail_count) {
		log_warn("Total test time %u, failed time %u!", j, fail_count);
	} else {
		log_info("Total test time %u, all success ^_^", j);
	}
}

void demo_n25qxx(void)
{
	const char *otp_data = "https://github.com/RockyChing/stm32_play.git";
	uint32_t op_addr = 0xC80000;
	uint16_t chipid;
	uint8_t buf[255];

	do {
		chipid = w25qxx_chipid();
		if (SPI_FLASH_CHIPID_N25Q128 == chipid) {
			log_info("chipid: %04x", chipid);
			break;
		}

		log_warn("Incalid chipid %04x, retry...", chipid);
		delay_ms(1000);
	} while (1);

	log_info("chip Identification:");
	n25qxx_read_id();

	//n25qxx_write_data(op_addr, otp_data, strlen(otp_data));
	//delay_ms(1500);
	//n25qxx_read_data(op_addr + 4, buf, sizeof(buf));
	//n25qxx_write_otp(otp_data, strlen(otp_data));
	//while (1) {
	//	delay_ms(1500);
	//}
	n25qxx_write_read_test();
}

#else /* DEMO_SPI_N25QXX_EN */
void demo_n25qxx(void) {}
#endif /* DEMO_SPI_N25QXX_EN */

