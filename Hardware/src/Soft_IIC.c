#include "Soft_IIC.h"


// 简单延时
void iic_delay(void)
{
    uint16_t t = 10;
    while(t--);
}

// 产生IIC起始信号
void iic_start(void)
{
    IMU_DIR_OUT();
    IMU_SDA1();
    IMU_SCL1();
    iic_delay();
    IMU_SDA0();
    iic_delay();
    IMU_SCL0();
}

// 产生IIC停止信号
void iic_stop(void)
{
    IMU_DIR_OUT();
    IMU_SCL0();
    IMU_SDA0();
    iic_delay();
    IMU_SCL1();
    IMU_SDA1();
    iic_delay();
}

// 等待应答信号到来
// 返回值：1，接收应答失败
//         0，接收应答成功
uint8_t iic_wait_ack(void)
{
    uint8_t err_time = 0;
    IMU_DIR_IN();
    IMU_SDA1(); iic_delay();
    IMU_SCL1(); iic_delay();
    while (IMU_SDA_READ)
    {
        ++err_time;
        if (err_time > 250)
        {
            iic_stop();
            return 1;
        }
    }
    IMU_SCL0();
    return 0;
}

// 产生ACK应答
void iic_ack(void)
{
    IMU_SCL0();
    IMU_DIR_OUT();
    IMU_SDA0();
    iic_delay();
    IMU_SCL1();
    iic_delay();
    IMU_SCL0();
}

// 不产生ACK应答
void iic_nack(void)
{
    IMU_SCL0();
    IMU_DIR_OUT();
    IMU_SDA1();
    iic_delay();
    IMU_SCL1();
    iic_delay();
    IMU_SCL0();
}

// IIC发送一个字节
void iic_send_byte(uint8_t txd)
{
    uint8_t t;
    IMU_DIR_OUT();
    IMU_SCL0();
    for (t=0; t<8; ++t)
    {
        if ((txd&0x80) >> 7)    IMU_SDA1();
        else                    IMU_SDA0();
        txd <<= 1;
        iic_delay();
        IMU_SCL1();
        iic_delay();
        IMU_SCL0();
        iic_delay();
    }
    iic_wait_ack();
}

// IIC读一个字节
// ack=1时，发送ACK；ack=0时，发送nACK
uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t i, receive = 0;
    IMU_DIR_IN();
    for (i=0; i<8; ++i)
    {
        IMU_SCL0();
        iic_delay();
        IMU_SCL1();
        receive <<= 1;
        if (IMU_SDA_READ) ++receive;
        iic_delay();
    }
    if (!ack)
        iic_nack();
    else
        iic_ack();
    return receive;
}

// 向I2C设备寄存器写入一个字节数据
void i2c_write_reg(uint8_t SlaveAddress ,uint8_t reg_addr, uint8_t reg_data)
{
    iic_start();                    // 发送起始信号
    iic_send_byte(SlaveAddress);    // 发送设备地址+写信号
    iic_send_byte(reg_addr);        // 内部寄存器地址
    iic_send_byte(reg_data);        // 内部寄存器数据
    iic_stop();                     // 发送停止信号
}

// 向I2C设备寄存器读取一个字节数据
uint8_t i2c_read_reg(uint8_t SlaveAddress ,uint8_t reg_addr)
{
    uint8_t reg_data;
    iic_start();                    // 发送起始信号
    iic_send_byte(SlaveAddress);    // 发送设备地址+写信号
    iic_send_byte(reg_addr);        // 发送存储单元地址，从0开始
    iic_start();                    // 发送起始信号
    iic_send_byte(SlaveAddress+1);  // 发送设备地址+读信号
    reg_data = iic_read_byte(0);    // 读出寄存器数据  0   NACK   1  ACK
    iic_stop();                     // 发送停止信号
    return reg_data;
}

// 向I2C设备寄存器读取多个字节数据
// dev_add  设备地址(低七位地址)
// reg      寄存器地址
// dat_add  数据保存的地址指针
// num      读取字节数量
void i2c_read_regs(uint8_t SlaveAddress ,uint8_t reg_addr, uint8_t *dat_add, uint8_t num)
{
    iic_start();                    // 发送起始信号
    iic_send_byte(SlaveAddress);    // 发送设备地址+写信号
    iic_send_byte(reg_addr);        // 内部寄存器地址
    
    iic_start();                    // 发送起始信号
    iic_send_byte(SlaveAddress+1);  // 发送设备地址+读信号
    while(--num)
    {
        *dat_add = iic_read_byte(1);// 读出寄存器数据  0   NACK   1  ACK
        dat_add++;
    }
    *dat_add = iic_read_byte(0);    // 读出寄存器数据  0   NACK   1  ACK
    iic_stop();                     // 发送停止信号
}


void soft_iic_io_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	IMU_RCC_APBx_CMD(IMU_RCC_APBx, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = IMU_SCL_GPIO_Pin | IMU_SDA_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(IMU_Port, &GPIO_InitStructure);	
	
	
}
