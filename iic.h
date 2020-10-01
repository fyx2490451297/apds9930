#ifndef __IIC_H
#define __IIC_H

#include <inttypes.h>

#define IIC_SCL_Pin GPIO_PIN_6
#define IIC_SCL_GPIO_Port GPIOA
#define IIC_SDA_Pin GPIO_PIN_7
#define IIC_SDA_GPIO_Port GPIOA

#define I2C_SCL_0()     HAL_GPIO_WritePin(IIC_SCL_GPIO_Port,IIC_SCL_Pin,GPIO_PIN_RESET);
#define I2C_SCL_1()     HAL_GPIO_WritePin(IIC_SCL_GPIO_Port,IIC_SCL_Pin,GPIO_PIN_SET);

#define I2C_SDA_0()     HAL_GPIO_WritePin(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_PIN_RESET);
#define I2C_SDA_1()     HAL_GPIO_WritePin(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_PIN_SET);

#define I2C_SCL_READ()  HAL_GPIO_ReadPin(IIC_SCL_GPIO_Port,IIC_SCL_Pin)
#define I2C_SDA_READ()  HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port,IIC_SDA_Pin)


#define I2C_WR	0		/* Ð´¿ØÖÆbit */
#define I2C_RD	1		/* ¶Á¿ØÖÆbit */

void iic_gpio_init(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(uint8_t ack);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);


#endif