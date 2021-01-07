/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/12/29
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _STM32F40X_SPI_CONTROLLER_H
#define _STM32F40X_SPI_CONTROLLER_H


extern void spi_controller_init(void);

extern const struct spi_controller *spi_controller_get(int index);

inline void spi_cs_active(const struct spi_controller *controller)
{
	GPIO_ResetBits(controller->cs.bank, controller->cs.pin);
}

inline void spi_cs_deactive(const struct spi_controller *controller)
{
	GPIO_SetBits(controller->cs.bank, controller->cs.pin);
}

extern int spi_read_write(const struct spi_controller *controller, uint8_t wb, uint8_t *rb);


#endif /* _STM32F40X_SPI_CONTROLLER_H */

