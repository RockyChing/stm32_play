/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2020/12/13
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/stm32_play.git
 */

#ifndef _STM32F40X_I2C_SW_H
#define _STM32F40X_I2C_SW_H


#define I2C_CTL_SW_AT24C  (0)
/* max number of i2c_controller */
#define I2C_CTL_SW_MAX    (5)


enum i2c_state {
	I2C_SW_DISABLE = 0,
	I2C_SW_ENABLE
};

struct i2c_pin_info {
	uint32_t clock;
	GPIO_TypeDef *bank;
	uint32_t pin;
};

struct i2c_controller {
	enum i2c_state state;
	struct i2c_pin_info sda;
	struct i2c_pin_info scl;
};


void i2c_controller_register(int index, struct i2c_controller *controller);


#endif /* _STM32F40X_I2C_SW_H */

