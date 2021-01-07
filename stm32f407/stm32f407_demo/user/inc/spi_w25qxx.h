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


#define SPI_FLASH_CHIPID_N25Q128    (0x5217)


extern int w25qxx_init(void);
extern uint16_t w25qxx_chipid(void);


#endif /* _W25QXX_H */

