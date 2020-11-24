/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/11/24
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#include <cmn_hdr.h>

#ifndef __I
#define __I volatile const /*!< Defines 'read only' permissions */
#endif


typedef struct {
	__I uint32_t u_id0;
	__I uint32_t u_id1;
	__I uint32_t u_id2;
} CPUID_TypeDef;

#define STF4_CPUID_BASE    (0x1FFF7A10)
#define STF4_CPUID         ((CPUID_TypeDef *) STF4_CPUID_BASE)

void stf4_get_cpuid(uint32_t u_id[3])
{
	u_id[0] = (uint32_t) STF4_CPUID->u_id0;
	u_id[1] = (uint32_t) STF4_CPUID->u_id1;
	u_id[2] = (uint32_t) STF4_CPUID->u_id2;
}

