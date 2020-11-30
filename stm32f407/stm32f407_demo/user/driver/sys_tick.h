/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/30
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _STM32F40X_SYSTICK_H
#define _STM32F40X_SYSTICK_H

#define STK_TIMEOUT_10MS  (10) /* STK timeout 10ms */

#define STK_RELOAD_MAX    (0x00FFFFFF)
#define STK_CLK           (21000000) /* HZ, (SystemCoreClock / 8) */
#define STK_CNT_USEC      (STK_CLK / 1000000) /* counts for per micro-sencond */


int sys_tick_init(void);


uint32_t sys_tick_get_reload(void);

/*
 * sys_tick_get_val - get current counter value
 * Notice: value is descrement!
 */
uint32_t sys_tick_get_val(void);

#endif /* _STM32F40X_SYSTICK_H */

