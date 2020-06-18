#ifndef SOFT_IIC_H
#define SOFT_IIC_h

#include "stm32f4xx.h"


typedef enum IIC    //DAC模块
{
    IIC,
    SCCB
} IIC_type;



#define ICM20602_RCC_APBx                    RCC_AHB1Periph_GPIOD
#define ICM20602_RCC_APBx_CMD(APBx, state)   RCC_AHB1PeriphClockCmd(APBx, state)
#define ICM20602_Port						 GPIOD
#define ICM20602_SCL_GPIO_Pin                GPIO_Pin_13      //定义SCL引脚  可任意更改为其他IO
#define ICM20602_SDA_GPIO_Pin   	         GPIO_Pin_15      //定义SDA引脚  可任意更改为其他IO

// gpio读写设置
#define SDA             GPIO_ReadInputDataBit(ICM20602_Port,ICM20602_SDA_GPIO_Pin)  // 获取SDA引脚电平
#define SDA0()          GPIO_ResetBits(ICM20602_Port,ICM20602_SDA_GPIO_Pin)         // SDA口输出低电平
#define SDA1()          GPIO_SetBits(ICM20602_Port,ICM20602_SDA_GPIO_Pin)           // SDA口输出高电平
#define SCL0()          GPIO_ResetBits(ICM20602_Port,ICM20602_SCL_GPIO_Pin)         //IO口输出低电平
#define SCL1()          GPIO_SetBits(ICM20602_Port,ICM20602_SCL_GPIO_Pin)           //IO口输出高电平
#define DIR_OUT()       ;                  //输出方向
#define DIR_IN()        ;                  //输入方向


void soft_iic_io_init(void);
void simiic_read_regs(uint8_t dev_add, uint8_t reg, uint8_t *dat_add, uint8_t num, IIC_type type);
void simiic_write_reg(uint8_t dev_add, uint8_t reg, uint8_t dat);
uint8_t simiic_read_reg(uint8_t dev_add, uint8_t reg, IIC_type type);

#endif
