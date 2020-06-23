#include "Soft_IIC.h"


// ����ʱ
void iic_delay(void)
{
    uint16_t t = 10;
    while(t--);
}

// ����IIC��ʼ�ź�
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

// ����IICֹͣ�ź�
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

// �ȴ�Ӧ���źŵ���
// ����ֵ��1������Ӧ��ʧ��
//         0������Ӧ��ɹ�
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

// ����ACKӦ��
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

// ������ACKӦ��
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

// IIC����һ���ֽ�
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

// IIC��һ���ֽ�
// ack=1ʱ������ACK��ack=0ʱ������nACK
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

// ��I2C�豸�Ĵ���д��һ���ֽ�����
void i2c_write_reg(uint8_t SlaveAddress ,uint8_t reg_addr, uint8_t reg_data)
{
    iic_start();                    // ������ʼ�ź�
    iic_send_byte(SlaveAddress);    // �����豸��ַ+д�ź�
    iic_send_byte(reg_addr);        // �ڲ��Ĵ�����ַ
    iic_send_byte(reg_data);        // �ڲ��Ĵ�������
    iic_stop();                     // ����ֹͣ�ź�
}

// ��I2C�豸�Ĵ�����ȡһ���ֽ�����
uint8_t i2c_read_reg(uint8_t SlaveAddress ,uint8_t reg_addr)
{
    uint8_t reg_data;
    iic_start();                    // ������ʼ�ź�
    iic_send_byte(SlaveAddress);    // �����豸��ַ+д�ź�
    iic_send_byte(reg_addr);        // ���ʹ洢��Ԫ��ַ����0��ʼ
    iic_start();                    // ������ʼ�ź�
    iic_send_byte(SlaveAddress+1);  // �����豸��ַ+���ź�
    reg_data = iic_read_byte(0);    // �����Ĵ�������  0   NACK   1  ACK
    iic_stop();                     // ����ֹͣ�ź�
    return reg_data;
}

// ��I2C�豸�Ĵ�����ȡ����ֽ�����
// dev_add  �豸��ַ(����λ��ַ)
// reg      �Ĵ�����ַ
// dat_add  ���ݱ���ĵ�ַָ��
// num      ��ȡ�ֽ�����
void i2c_read_regs(uint8_t SlaveAddress ,uint8_t reg_addr, uint8_t *dat_add, uint8_t num)
{
    iic_start();                    // ������ʼ�ź�
    iic_send_byte(SlaveAddress);    // �����豸��ַ+д�ź�
    iic_send_byte(reg_addr);        // �ڲ��Ĵ�����ַ
    
    iic_start();                    // ������ʼ�ź�
    iic_send_byte(SlaveAddress+1);  // �����豸��ַ+���ź�
    while(--num)
    {
        *dat_add = iic_read_byte(1);// �����Ĵ�������  0   NACK   1  ACK
        dat_add++;
    }
    *dat_add = iic_read_byte(0);    // �����Ĵ�������  0   NACK   1  ACK
    iic_stop();                     // ����ֹͣ�ź�
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
