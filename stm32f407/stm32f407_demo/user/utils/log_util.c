/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/23
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#include <cmn_hdr.h>
#include <log_util.h>


#define BUF_SIZE (512)

static const char g_log_level_char[LOG_DEBUG + 1] = {
	'F', 'E', 'W', 'I', 'D' };

static int g_log_level = LOG_DEBUG;


int _sys_debug(int level, const char *tag, const char *fmt, ...)
{
	char buff[BUF_SIZE];
	va_list ap;

	if (level > g_log_level || level < 0) {
		return -1;
	}

	va_start(ap, fmt);
	memset(buff, 0, sizeof(buff));
	vsnprintf(buff, sizeof(buff) - 1, fmt, ap);
	if (strstr(buff, "%s") != NULL) {
		printf("WARN: log_debug called with '%%s' formatted string!");
		va_end(ap);
		return -1;
	}

	printf("%c/%s %s\n", g_log_level_char[level], tag, buff);
	va_end(ap);
	return 0;
}

