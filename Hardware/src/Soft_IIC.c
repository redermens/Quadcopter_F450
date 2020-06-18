#include "Soft_IIC.h"


//�ڲ����ݶ���
uint8_t IIC_ad_main; //�����ӵ�ַ
uint8_t IIC_ad_sub;  //�����ӵ�ַ
uint8_t *IIC_buf;    //����|�������ݻ�����
uint8_t IIC_num;     //����|�������ݸ���

#define ack 1      //��Ӧ��
#define no_ack 0   //��Ӧ��

/**
 * @description: ģ��IIC��ʱ�����IICͨѶʧ�ܿ��Գ�������j��ֵ
 */
static void simiic_delay(void)
{
    //j=10ͨѶ���ʴ�ԼΪ100K ���ں�Ƶ��40M��
    //j=0 ͨѶ���ʴ�ԼΪ140K ���ں�Ƶ��40M��
    uint16_t j=30 ;
    while(j--);
}

//�ڲ�ʹ�ã��û��������
static void IIC_start(void)
{
    SDA1();
    SCL1();
    simiic_delay();
    SDA0();
    simiic_delay();
    SCL0();
}

//�ڲ�ʹ�ã��û��������
static void IIC_stop(void)
{
    SDA0();
    SCL0();
    simiic_delay();
    SCL1();
    simiic_delay();
    SDA1();
    simiic_delay();
}

//��Ӧ��(����ack:SDA=0��no_ack:SDA=0)
//�ڲ�ʹ�ã��û��������
static void I2C_SendACK(unsigned char ack_dat)
{
    SCL0();
    simiic_delay();
    if(ack_dat) SDA0();
    else        SDA1();

    SCL1();
    simiic_delay();
    SCL0();
    simiic_delay();
}

static int SCCB_WaitAck(void)
{
    SCL0();
    DIR_IN();
    simiic_delay();
    
    SCL1();
    simiic_delay();
    
    if(SDA)           //Ӧ��Ϊ�ߵ�ƽ���쳣��ͨ��ʧ��
    {
        DIR_OUT();
        SCL0();
        return 0;
    }
    DIR_OUT();
    SCL0();
    simiic_delay();
    return 1;
}

//�ֽڷ��ͳ���
//����c(����������Ҳ���ǵ�ַ)���������մ�Ӧ��
//�����Ǵ�Ӧ��λ
//�ڲ�ʹ�ã��û��������
static void send_ch(uint8_t c)
{
    uint8_t i = 8;
    while(i--)
    {
        if(c & 0x80)    SDA1();//SDA �������
        else            SDA0();
        c <<= 1;
        simiic_delay();
        SCL1();                //SCL ���ߣ��ɼ��ź�
        simiic_delay();
        SCL0();                //SCL ʱ��������
    }
    SCCB_WaitAck();
}

//�ֽڽ��ճ���
//�����������������ݣ��˳���Ӧ���|��Ӧ����|IIC_ack_main()ʹ��
//�ڲ�ʹ�ã��û��������
static uint8_t read_ch(uint8_t ack_x)
{
    uint8_t i;
    uint8_t c;
    c=0;
    SCL0();
    simiic_delay();
    SDA1();             //��������Ϊ���뷽ʽ
    DIR_IN();
    for(i=0;i<8;i++)
    {
        simiic_delay();
        SCL0();         //��ʱ����Ϊ�ͣ�׼����������λ
        simiic_delay();
        SCL1();         //��ʱ����Ϊ�ߣ�ʹ��������������Ч
        simiic_delay();
        c<<=1;
        if(SDA) c+=1;   //������λ�������յ����ݴ�c
    }
    DIR_OUT();
    SCL0();
    simiic_delay();
    I2C_SendACK(ack_x);
    
    return c;
}

/**
 * @description: ģ��IICд���ݵ��豸�Ĵ�������
 * @param      dev_add         �豸��ַ(����λ��ַ)
 * @param      reg             �Ĵ�����ַ
 * @param      dat             д�������
 */
void simiic_write_reg(uint8_t dev_add, uint8_t reg, uint8_t dat)
{
    IIC_start();
    send_ch( (dev_add<<1) | 0x00);   //����������ַ��дλ
    send_ch( reg );                  //���ʹӻ��Ĵ�����ַ
    send_ch( dat );                  //������Ҫд�������
    IIC_stop();
}

/**
 * @description: ģ��IIC���豸�Ĵ�����ȡ����
 * @param      dev_add         �豸��ַ(����λ��ַ)
 * @param      reg             �Ĵ�����ַ
 * @param      type            ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
 * @return     uint8_t         ���ؼĴ���������
 */
uint8_t simiic_read_reg(uint8_t dev_add, uint8_t reg, IIC_type type)
{
    uint8_t dat;
    IIC_start();
    send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
    send_ch( reg );                 //���ʹӻ��Ĵ�����ַ
    if(type == SCCB)IIC_stop();
    
    IIC_start();
    send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
    dat = read_ch(no_ack);                  //��ȡ����
    IIC_stop();
    
    return dat;
}

/**
 * @description: ģ��IIC��ȡ���ֽ�����
 * @param      dev_add         �豸��ַ(����λ��ַ)
 * @param      reg             �Ĵ�����ַ
 * @param      dat_add         ���ݱ���ĵ�ַָ��
 * @param      num             ��ȡ�ֽ�����
 * @param      type            ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
 */
void simiic_read_regs(uint8_t dev_add, uint8_t reg, uint8_t *dat_add, uint8_t num, IIC_type type)
{
    IIC_start();
    send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
    send_ch( reg );                 //���ʹӻ��Ĵ�����ַ
    if(type == SCCB)IIC_stop();
    
    IIC_start();
    send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
    while(--num)
    {
        *dat_add = read_ch(ack); //��ȡ����
        dat_add++;
    }
    *dat_add = read_ch(no_ack); //��ȡ����
    IIC_stop();
}


void soft_iic_io_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ICM20602_RCC_APBx_CMD(ICM20602_RCC_APBx, ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = ICM20602_SCL_GPIO_Pin | ICM20602_SDA_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ICM20602_Port, &GPIO_InitStructure);	
}
