/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/12/29
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _W25QXX_H
#define _W25QXX_H


#define N25QX_CMD_RDCID        (0x90)

#define N25QX_CMD_RDID         (0x9F)

#define N25QX_CMD_READ         (0x03)
#define N25QX_CMD_WRITE        (0x02)

#define N25QX_CMD_RDSR         (0x05)
#define N25QX_CMD_WRSR         (0x01)

#define N25QX_CMD_WREN         (0x06)
#define N25QX_CMD_WRDI         (0x04)

#define N25QX_CMD_ROTP         (0x4B)
#define N25QX_CMD_POTP         (0x42)

#define N25QX_CMD_SECE         (0xD8)



#define SPI_FLASH_CHIPID_N25Q128    (0x5217)


extern int w25qxx_init(void);
extern uint16_t w25qxx_chipid(void);

extern void n25qxx_read_id(void);

extern void n25qxx_read_otp(void);
extern void n25qxx_write_otp(uint8_t *data, uint8_t len);

extern void n25qxx_read_data(uint32_t addr,void *buf, size_t count);

extern void n25qxx_erase_sector(uint32_t addr);


#endif /* _W25QXX_H */

