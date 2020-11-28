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

	driver_init();

	log_info("test\n");
	memset(uid, 0, sizeof(uid));
	stf4_get_cpuid(uid);

	while(1) {
		led_ctl(LED_RED, LED_CTL_ON);
		delay(0x8FFFFF);
		led_ctl(LED_GREEN, LED_CTL_ON);
		delay(0x8FFFFF);

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
