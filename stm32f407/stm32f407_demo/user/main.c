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
#include <led.h>
#include <uart.h>


extern void driver_init(void);
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

static void delay(__IO uint32_t nCount)
{
	while(nCount--)
	{}
}

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

	memset(uid, 0, sizeof(uid));
	stf4_get_cpuid(uid);
	log_info("cpuid: %08x:%08x:%08x", uid[2], uid[1], uid[0]);

	memset(&clk, 0, sizeof(stf4_clock_t));
	stf4_get_sysclk(&clk);
	log_info("SYSCLK_Frequency: %u", clk.SYSCLK_Frequency);
	log_info("HCLK_Frequency: %u", clk.HCLK_Frequency);
	log_info("PCLK1_Frequency: %u", clk.PCLK1_Frequency);
	log_info("PCLK2_Frequency: %u", clk.PCLK2_Frequency);

	while(1) {
		log_info("open led...");
		led_ctl(LED_RED, LED_CTL_ON);
		delay(0x8FFFFF);
		led_ctl(LED_GREEN, LED_CTL_ON);
		delay(0x8FFFFF);

		log_info("close led...");
		led_ctl(LED_RED, LED_CTL_OFF);
		delay(0x8FFFFF);
		led_ctl(LED_GREEN, LED_CTL_OFF);
		delay(0x8FFFFF);
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
