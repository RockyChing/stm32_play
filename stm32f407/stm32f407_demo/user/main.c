/**
  ******************************************************************************
  * @file    main.c
  * @author  RockyChing <zdh1021680772@163.com>
  * @version V0.0.1
  * @date    20-November-2020
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * You may clone this project at:
  *
  *        https://github.com/RockyChing/stm32_play.git
  *
  ******************************************************************************
  */
#include "stm32f4xx.h"
#define LOG_TAG "main"

#include <cmn_hdr.h>
#include <log_util.h>
#include <misc_util.h>
#include <version.h>
#include <led.h>
#include <uart.h>
#include <delay.h>
#include <button.h>
#include <ext_int.h>
#include <demo.h>


extern void driver_init(void);

struct stm32_version {
	/** major version. */
	const uint16_t major;

	/** minor version. */
	const uint16_t minor;

	/** micro version. */
	const uint16_t micro;

	/** nano version. */
	const uint16_t nano;

	/** release candidate suffix string, e.g. "-rc4". */
	const char *rc;

	/** For ABI compatibility only. */
	const char *describe;
};

static const struct stm32_version g_version = {
	STM32_MAJOR, STM32_MINOR, STM32_MICRO, STM32_NANO,
	STM32_RC, "https://github.com/RockyChing/stm32_play.git"
};


int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files before to branch to application main.
       To reconfigure the default setting of SystemInit() function, 
       refer to system_stm32f4xx.c file */

	uint32_t uid[3];
	stf4_clock_t clk;

	driver_init();
	log_info("libusb v%u.%u.%u.%u%s", g_version.major, g_version.minor,
		g_version.micro, g_version.nano, g_version.rc);

	memset(uid, 0, sizeof(uid));
	stf4_get_cpuid(uid);
	log_info("cpuid: %08x:%08x:%08x", uid[2], uid[1], uid[0]);

	memset(&clk, 0, sizeof(stf4_clock_t));
	stf4_get_sysclk(&clk);
	log_info("SYSCLK_Frequency: %u", clk.SYSCLK_Frequency);
	log_info("HCLK_Frequency: %u", clk.HCLK_Frequency);
	log_info("PCLK1_Frequency: %u", clk.PCLK1_Frequency);
	log_info("PCLK2_Frequency: %u", clk.PCLK2_Frequency);

#if DEMO_MD5_EN
	demo_md5();
#endif

	while(1) {
#if BTN_INT
		if (EINT_ACTIVE == eint_state_get()) {
			led_ctl(LED_RED, LED_CTL_ON);
		} else {
			led_ctl(LED_RED, LED_CTL_OFF);
		}
#else
		if (is_btn_active(BTN_UP)) {
			log_info("BTN_UP active");
			led_ctl(LED_RED, LED_CTL_ON);
		} else {
			led_ctl(LED_RED, LED_CTL_OFF);
		}

		if (is_btn_active(BTN_DOWN)) {
			log_info("BTN_DOWN active");
			led_ctl(LED_GREEN, LED_CTL_ON);
		} else {
			led_ctl(LED_GREEN, LED_CTL_OFF);
		}
#endif

#if 0
		log_info("open led...");
		led_ctl(LED_RED, LED_CTL_ON);
		delay_ms(500);
		led_ctl(LED_GREEN, LED_CTL_ON);
		delay_ms(500);

		log_info("close led...");
		led_ctl(LED_RED, LED_CTL_OFF);
		delay_ms(500);
		led_ctl(LED_GREEN, LED_CTL_OFF);
		delay_ms(500);
#endif
	}
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
