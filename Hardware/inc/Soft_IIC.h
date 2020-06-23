#ifndef SOFT_IIC_H
#define SOFT_IIC_h

#include "stm32f4xx.h"
#include "cfg_func_f407.h"

typedef enum IIC    //DAC模块
{
    IIC,
    SCCB
} IIC_type;



#define IMU_RCC_APBx                    RCC_AHB1Periph_GPIOD
#define IMU_RCC_APBx_CMD(APBx, state)   RCC_AHB1PeriphClockCmd(APBx, state)
#define IMU_Port						GPIOD
#define IMU_SCL_GPIO_Pin                GPIO_Pin_13      //定义SCL引脚  可任意更改为其他IO
#define IMU_SDA_GPIO_Pin   	         	GPIO_Pin_15      //定义SDA引脚  可任意更改为其他IO

// gpio读写设置
#define IMU_SDA_READ        GPIO_ReadInputDataBit(IMU_Port,IMU_SDA_GPIO_Pin)  // 获取SDA引脚电平
#define IMU_SDA0()          GPIO_ResetBits(IMU_Port,IMU_SDA_GPIO_Pin)         // SDA口输出低电平
#define IMU_SDA1()          GPIO_SetBits(IMU_Port,IMU_SDA_GPIO_Pin)           // SDA口输出高电平
#define IMU_SCL0()          GPIO_ResetBits(IMU_Port,IMU_SCL_GPIO_Pin)         //IO口输出低电平
#define IMU_SCL1()          GPIO_SetBits(IMU_Port,IMU_SCL_GPIO_Pin)           //IO口输出高电平
#define IMU_DIR_OUT()       GPIO_Init_OutPP(IMU_Port,IMU_SDA_GPIO_Pin)
#define IMU_DIR_IN()        GPIO_Init_InFL(IMU_Port,IMU_SDA_GPIO_Pin)


void soft_iic_io_init(void);
uint8_t i2c_read_reg(uint8_t SlaveAddress ,uint8_t reg_addr);
void i2c_read_regs(uint8_t SlaveAddress ,uint8_t reg_addr, uint8_t *dat_add, uint8_t num);
void i2c_write_reg(uint8_t SlaveAddress ,uint8_t reg_addr, uint8_t reg_data);
#endif
