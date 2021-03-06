/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/12/31
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _STM32F40X_DEMO_H
#define _STM32F40X_DEMO_H

#define DEMO_MD5_EN              (1)
#define DEMO_RNG_EN              (0)
#define DEMO_SPI_N25QXX_EN       (1)



extern void demo_md5(void);
extern void demo_rng(void);
extern void demo_n25qxx(void);


#endif /* _STM32F40X_DEMO_H */

