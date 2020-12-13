/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/27
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#include <cmn_hdr.h>
#define LOG_TAG "driver"

#include <utils.h>
#include <log_util.h>
#include "sys_int.h"
#include "sys_tick.h"
#include <led.h>
#include <button.h>
#include <ext_int.h>
#include <uart.h>


struct driver_struct {
	const char *name;
	int (*init)(void);
};

static struct driver_struct g_drivers[] = {
	{ "int",      interrupt_init     },
	{ "stk",      sys_tick_init      },
	{ "uart",     uart_init          },
	{ "led",      led_init           },
	{ "btn",      btn_init           },
	{ "eint",     eint_init          },
	{ NULL,       NULL               },
};


/*
 * driver_init - Initialize all drivers needed
 */
void driver_init(void)
{
	int i, res;
	const char *name;
	struct driver_struct *driver;

	for (i = 0; i < ARRAY_SIZE(g_drivers); i ++) {
		driver = &g_drivers[i];
		if (NULL != driver && NULL != driver->init) {
			name = driver->name ? driver->name : "null";
			res = driver->init();
			if (res < 0) {
				log_warn("driver '%s' init failed!", name);
			} else {
				log_debug("driver '%s' init done", name);
			}
		}
	}
}

