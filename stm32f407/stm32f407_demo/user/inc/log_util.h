/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/23
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _LOG_UTIL_H
#define _LOG_UTIL_H


#define LOG_FATAL    (0)
#define LOG_ERROR    (1)
#define LOG_WARN     (2)
#define LOG_INFO     (3)
#define LOG_DEBUG    (4)


int _sys_debug(int level, const char *tag, const char *fmt, ...);

#define log_fatal(x...) do { _sys_debug(LOG_FATAL, LOG_TAG, x); abort(); } while (0)
#define log_error(x...) _sys_debug(LOG_ERROR, LOG_TAG, x)
#define log_warn(x...)  _sys_debug(LOG_WARN,  LOG_TAG, x)
#define log_info(x...)  _sys_debug(LOG_INFO,  LOG_TAG, x)
#define log_debug(x...) _sys_debug(LOG_DEBUG, LOG_TAG, x)


#define hex_dump(buf, len)	\
	do { \
		int i; \
		char *p = (char*) buf; \
		for(i = 0; i < len; i++) { \
			if(0 == (i % 32) && 0 != i) \
				printf("\n"); \
			printf("%02x ", (p[i]&0xff)); \
		} \
		printf("\n"); \
	} while(0)


#endif /* _LOG_UTIL_H */

