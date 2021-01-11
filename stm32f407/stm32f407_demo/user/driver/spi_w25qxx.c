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
#include <delay.h>
#include <spi.h>
#include <spi_w25qxx.h>

/**
 * W25Q128 128Mb, 16 MByte
 * 16 MByte, 256 Block, 64KB/Block
 * Block, 16 Sector, 4KB/Sector
 */

/**
 * N25Q128 128Mb, 16 MByte
 * 256 sector, 64KB/sector
 * Erase as sector
 */
#define N25Q128_SEC_SIZE    (64 * 1024)
#define N25Q128_PAGE_SIZE (256)
#define N25Q128_PAGE_NUM    (N25Q128_SEC_SIZE / N25Q128_PAGE_SIZE)


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
	uint8_t req[6] = { N25QX_CMD_RDCID, dummy, dummy, 0x00, dummy, dummy };
	uint8_t resp[sizeof(req)];
	uint16_t id;

	w25qxx_xfer(sizeof(req), req, resp);
	id = resp[4];
	id = (id << 8) | resp[5];
	return id;
}

static uint8_t n25qxx_read_reg(uint8_t reg)
{
	uint8_t req[2];
	uint8_t resp[sizeof(req)];

	req[0] = reg;
	w25qxx_xfer(sizeof(req), req, resp);
	//hex_dump(resp, sizeof(resp));
	return resp[1];
}

static void n25qxx_write_reg(uint8_t reg, uint8_t val)
{
	uint8_t req[2];
	req[0] = reg;
	req[1] = val;
	w25qxx_xfer(sizeof(req), req, NULL);
}

void n25qxx_read_id(void)
{
	uint8_t req[1+20] = { N25QX_CMD_RDID, };
	uint8_t resp[sizeof(req)];
	w25qxx_xfer(sizeof(req), req, resp);
	hex_dump(&resp[1], sizeof(resp) - 1);
}

static uint8_t n25qxx_cmd_rdsr(void)
{
	return n25qxx_read_reg(N25QX_CMD_RDSR);
}

static void n25qxx_cmd_wrsr(uint8_t val)
{
	n25qxx_write_reg(N25QX_CMD_WRSR, val);
}

static void n25qxx_cmd_wren(void)
{
	uint8_t req = N25QX_CMD_WREN;
	w25qxx_xfer(1, &req, NULL);
}

static void n25qxx_cmd_wrdi(void)
{
	uint8_t req = N25QX_CMD_WRDI;
	w25qxx_xfer(1, &req, NULL);
}

static void n25qxx_write_enable(void)
{
	uint8_t status = 0;
	n25qxx_cmd_wren();
	do {
		status = n25qxx_cmd_rdsr();
		if (status & 0x02)
			break;
		delay_us(500);
	} while (1);
}

static void n25qxx_busy_wait(void)
{
	uint8_t status = 0;
	do {
		status = n25qxx_cmd_rdsr();
		if (status & 0x01) {
			// log_info("n25qxx_busy...");
			delay_us(500);
		} else {
			break;
		}
	} while (1);
}

void n25qxx_read_otp(void)
{
	uint8_t req[1+3+1+65] = { 0x4b, 0, 0, 0, 0, };
	uint8_t resp[sizeof(req)];

	w25qxx_xfer(sizeof(req), req, resp);
	hex_dump(resp, sizeof(resp));
}

void n25qxx_write_otp(uint8_t *data, uint8_t len)
{
	uint8_t req[1+3+65] = { N25QX_CMD_POTP, 0, 0, 0, 0, };
	uint8_t status;

	len = (len <= 65) ? len : 65;
	memcpy(&req[1+3], data, len);
	req[1+3+64] = 0x00;

	n25qxx_write_enable();

	w25qxx_xfer(sizeof(req), req, NULL);

	n25qxx_busy_wait();
}

/**
 * n25qxx_read_data - read @count data at address specified by @addr
 */
void n25qxx_read_data(uint32_t addr,void *buf, size_t count)
{
	size_t i;
	uint8_t *pdata = (uint8_t *) buf;

	/* cs enable */
	GPIO_ResetBits(g_w25qxx_driver->controller->cs.bank, g_w25qxx_driver->controller->cs.pin);
	spi_read_write(g_w25qxx_driver->controller, N25QX_CMD_READ, &dummy);
    spi_read_write(g_w25qxx_driver->controller, (uint8_t) ((addr) >> 16), &dummy);
	spi_read_write(g_w25qxx_driver->controller, (uint8_t) ((addr) >>  8), &dummy);
	spi_read_write(g_w25qxx_driver->controller, (uint8_t) ((addr) &0xff), &dummy);

    for (i = 0; i < count; i++) {
        spi_read_write(g_w25qxx_driver->controller, dummy, &pdata[i]);
    }

	/* cs disable */
	GPIO_SetBits(g_w25qxx_driver->controller->cs.bank, g_w25qxx_driver->controller->cs.pin);
}

static void n25qxx_erase_sector(uint32_t index)
{
	uint8_t req[1+3];
	uint32_t addr = index * N25Q128_SEC_SIZE;

	req[0] = N25QX_CMD_SECE;
	req[1] = (uint8_t) ((addr)>>16);
	req[2] = (uint8_t) ((addr)>>8);
	req[3] = (uint8_t) ((addr)&0xff);

	n25qxx_write_enable();
	w25qxx_xfer(sizeof(req), req, NULL);
	n25qxx_busy_wait();
}

static void _n25qxx_write_page(uint32_t addr, void *buf, size_t count)
{
	size_t i;
	uint8_t *pdata = (uint8_t *) buf;

	n25qxx_write_enable();
	/* cs enable */
	GPIO_ResetBits(g_w25qxx_driver->controller->cs.bank, g_w25qxx_driver->controller->cs.pin);
	spi_read_write(g_w25qxx_driver->controller, N25QX_CMD_WRITE, &dummy);
    spi_read_write(g_w25qxx_driver->controller, (uint8_t) ((addr) >> 16), &dummy);
	spi_read_write(g_w25qxx_driver->controller, (uint8_t) ((addr) >>  8), &dummy);
	spi_read_write(g_w25qxx_driver->controller, (uint8_t) ((addr) &0xff), &dummy);

    for (i = 0; i < count; i++) {
        spi_read_write(g_w25qxx_driver->controller, (uint8_t) pdata[i], &dummy);
    }

	/* cs disable */
	GPIO_SetBits(g_w25qxx_driver->controller->cs.bank, g_w25qxx_driver->controller->cs.pin);
	n25qxx_busy_wait();
}

/**
 * n25qxx_write_page - write data to as page
 * @count: count <= N25Q128_PAGE_SIZE
 */
static void n25qxx_write_page(uint32_t addr, void *buf, size_t count)
{
	_n25qxx_write_page(addr, buf, count);
}

/**
 * @count: count <= N25Q128_SEC_SIZE
 */
#if 0
static void n25qxx_write_sector(uint32_t addr, void *buf, size_t count)
{
	size_t i;
	// uint8_t buff[N25Q128_SEC_SIZE];
	uint8_t sec_index; // 0 ~ 255
	uint32_t sec_start_addr; // the start address of sector
	uint32_t sec_remain;

	sec_index = addr / N25Q128_SEC_SIZE;
	sec_start_addr = N25Q128_SEC_SIZE * sec_index;
	sec_remain = N25Q128_SEC_SIZE - (addr - sec_start_addr);
	if (count <= sec_remain) { // all operation in the same sector

	} else {

	}
}
#else
void n25qxx_write_sector(uint32_t index, void *buf, size_t count)
{
	uint32_t addr = index * N25Q128_SEC_SIZE;
	size_t page_num = count / N25Q128_PAGE_SIZE;
	size_t page_remain = count % N25Q128_PAGE_SIZE;
	uint8_t *pdata = (uint8_t *) buf;
	int i;

	n25qxx_erase_sector(index);
	for (i = 0; i < page_num; i ++) {
		n25qxx_write_page(addr, pdata, N25Q128_PAGE_SIZE);
		addr += N25Q128_PAGE_SIZE;
		pdata += N25Q128_PAGE_SIZE;
	}

	if (page_remain) {
		n25qxx_write_page(addr, pdata, page_remain);
	}
}

void n25qxx_read_sector(uint32_t index, void *buf, size_t count)
{
	uint32_t addr = index * N25Q128_SEC_SIZE;
	n25qxx_read_data(addr, buf, count);
}
#endif

static void n25qxx_write_data(uint32_t addr, void *buf, size_t count)
{
	// TODO
}


