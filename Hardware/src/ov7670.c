/*
注意:PWND接地，RESET上拉到VDD
驱动平台:STM32F407
*/
#include "ov7670.h"
#include "cfg_func_f407.h"

#define SCCB_READ_SDA GPIO_ReadInputDataBit(SCCB_SDA)

int ov_frame=0;
int ov_line=0;

unsigned int OV7670_LineBuf[160];
unsigned short OV7670_Frame[2][120][160] __attribute__((at(0X68000000))); //75KB

void Ov7670_SysClock_Init(void)
{
	GPIO_Init_AF(GPIOA,GPIO_Pin_8);
	/* HSE clock selected to output on MCO1 pin(PA8)*/
  RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
}

//====================SCCB===============================
void SCCB_SDA_IN(void)
{
	GPIO_Init_InPU(SCCB_SDA);
}

void SCCB_SDA_OUT(void)
{
	GPIO_Init_OutPP(SCCB_SDA);
}

void delay_uss(int n)	//重要！这里需要改延时参数
{
	int i;
	for(i=0;i<9000;i++);
}

void SCCB_Init(void)
{
	//PWDN,RESET
	GPIO_Init_OutPP(PWDN_PIN);
	GPIO_Init_OutPP(RESET_PIN);
	GPIO_ResetBits(PWDN_PIN);	//PWDN 低
	GPIO_SetBits(RESET_PIN);		//RESET 高
	//==========
	
	GPIO_Init_OutPP(SCCB_SDA);
	GPIO_Init_OutPP(SCCB_SCL);
	GPIO_SetBits(SCCB_SDA);
	GPIO_SetBits(SCCB_SCL);
	SCCB_SDA_OUT();	   
}	

void SCCB_Start(void)
{
    GPIO_SetBits(SCCB_SDA);
    GPIO_SetBits(SCCB_SCL);
    delay_uss(50);  
    GPIO_ResetBits(SCCB_SDA);
    delay_uss(50);	 
    GPIO_ResetBits(SCCB_SCL);
}

void SCCB_Stop(void)
{
    GPIO_ResetBits(SCCB_SDA);
    delay_uss(50);	 
    GPIO_SetBits(SCCB_SCL);
    delay_uss(50); 
    GPIO_SetBits(SCCB_SDA);
    delay_uss(50);
}  

void SCCB_No_Ack(void)
{
	delay_uss(50);
	GPIO_SetBits(SCCB_SDA);
	GPIO_SetBits(SCCB_SCL);
	delay_uss(50);
	GPIO_ResetBits(SCCB_SCL);
	delay_uss(50);
	GPIO_ResetBits(SCCB_SDA);
	delay_uss(50);
}

unsigned char SCCB_WR_Byte(unsigned char dat)
{
	unsigned char j,res;	 
	for(j=0;j<8;j++) 
	{
		if(dat&0x80) GPIO_SetBits(SCCB_SDA);
		else GPIO_ResetBits(SCCB_SDA);
		dat<<=1;
		delay_uss(50);
		GPIO_SetBits(SCCB_SCL);
		delay_uss(50);
		GPIO_ResetBits(SCCB_SCL);  
	}			 
	SCCB_SDA_IN();		
	delay_uss(50);
	GPIO_SetBits(SCCB_SCL);
	delay_uss(50);
	if(SCCB_READ_SDA)res=1;  
	else res=0;         
	GPIO_ResetBits(SCCB_SCL); 
	SCCB_SDA_OUT();	  
	return res;  
}	 

unsigned char SCCB_RD_Byte(void)
{
	unsigned char temp=0,j;
	SCCB_SDA_IN();	
	for(j=8;j>0;j--) 
	{		     	  
		delay_uss(50);
		GPIO_SetBits(SCCB_SCL); 
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;   
		delay_uss(50);
		GPIO_ResetBits(SCCB_SCL); 
	}	
	SCCB_SDA_OUT();		   
	return temp;
} 							    

unsigned char SCCB_WR_Reg(unsigned char reg,unsigned char data)
{
	unsigned char res=0;
	SCCB_Start(); 		
	if(SCCB_WR_Byte(SCCB_ID))res=1;
	delay_uss(100);
  	if(SCCB_WR_Byte(reg))res=1;	 
	delay_uss(100);
  	if(SCCB_WR_Byte(data))res=1;
  	SCCB_Stop();	  
  	return	res;
}		  					    

unsigned char SCCB_RD_Reg(unsigned char reg)
{
	unsigned char val=0;
	SCCB_Start(); 		
	SCCB_WR_Byte(SCCB_ID);	
	delay_uss(100);	 
  	SCCB_WR_Byte(reg);	 
	delay_uss(100);	  
	SCCB_Stop();   
	delay_uss(100);	   
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01); 
	delay_uss(100);
  	val=SCCB_RD_Byte();
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}

//=====================OV7670_REG===========================
int check_camera(void)
{
	unsigned char d1,d2;
	SCCB_Init();
	SCCB_WR_Reg(0x12,0x80);
	d1=SCCB_RD_Reg(0x0b);
	SCCB_WR_Reg(0x12,0x80);
	d2=SCCB_RD_Reg(0x0a);
	if(d1==0x73&&d2==0x76) return 1;
	else return 0;
}

void OV7670_Reg_Init(void)
{
	SCCB_Init();
	  {
		SCCB_WR_Reg(0x3a, 0x0c);
	  SCCB_WR_Reg(0x12, 0x14);//QVGA,RGB
  }
	//else if(mode==GRAY_240X320)
	{
		/*SCCB_WR_Reg(0x3a, 0x0c);
		SCCB_WR_Reg(0x3d, 0x89);
		SCCB_WR_Reg(0x12, 0x10);//QVGA,YUV
		SCCB_WR_Reg(0x67, 0x11);
		SCCB_WR_Reg(0x68, 0x11);*/
	}
	SCCB_WR_Reg(0x40, 0xd0);//565
	SCCB_WR_Reg(0x32, 0x80);//HREF control	bit[2:0] HREF start 3 LSB
	//bit[5:3] HSTOP HREF end 3LSB
	SCCB_WR_Reg(0x17, 0x16);//HSTART start high 8-bit MSB
	SCCB_WR_Reg(0x18, 0x04);//5 HSTOP end high 8-bit
	SCCB_WR_Reg(0x19, 0x02);
	SCCB_WR_Reg(0x1a, 0x7b);
	SCCB_WR_Reg(0x03, 0x06);//0x0a,???????
	SCCB_WR_Reg(0x0c, 0x00);
	SCCB_WR_Reg(0x15, 0x00);													///00
	SCCB_WR_Reg(0x3e, 0x00);
	SCCB_WR_Reg(0x70, 0x3a);
	SCCB_WR_Reg(0x71, 0x35);
	SCCB_WR_Reg(0x72, 0x11);
	SCCB_WR_Reg(0x73, 0x00);
	SCCB_WR_Reg(0xa2, 0x02);
	SCCB_WR_Reg(0x11, 0x81);//??????,0,???.		//81
	SCCB_WR_Reg(0x7a, 0x20);
	SCCB_WR_Reg(0x7b, 0x1c);
	SCCB_WR_Reg(0x7c, 0x28);
	SCCB_WR_Reg(0x7d, 0x3c);
	SCCB_WR_Reg(0x7e, 0x55);
	SCCB_WR_Reg(0x7f, 0x68);
	SCCB_WR_Reg(0x80, 0x76);
	SCCB_WR_Reg(0x81, 0x80);
	SCCB_WR_Reg(0x82, 0x88);
	SCCB_WR_Reg(0x83, 0x8f);
	SCCB_WR_Reg(0x84, 0x96);
	SCCB_WR_Reg(0x85, 0xa3);
	SCCB_WR_Reg(0x86, 0xaf);
	SCCB_WR_Reg(0x87, 0xc4);
	SCCB_WR_Reg(0x88, 0xd7);
	SCCB_WR_Reg(0x89, 0xe8);
	SCCB_WR_Reg(0x13, 0xe0);
	SCCB_WR_Reg(0x00, 0x00);//AGC
	SCCB_WR_Reg(0x10, 0x00);
	SCCB_WR_Reg(0x0d, 0x00);//???, ?[5:4]: 01 ???,10 1/4??,11 1/4??
	SCCB_WR_Reg(0x14, 0x28);//0x38, limit the max gain
	SCCB_WR_Reg(0xa5, 0x05);
	SCCB_WR_Reg(0xab, 0x07);
	SCCB_WR_Reg(0x24, 0x75);
	SCCB_WR_Reg(0x25, 0x63);
	SCCB_WR_Reg(0x26, 0xA5);
	SCCB_WR_Reg(0x9f, 0x78);
	SCCB_WR_Reg(0xa0, 0x68);
	SCCB_WR_Reg(0xa1, 0x03);
	SCCB_WR_Reg(0xa6, 0xdf);
	SCCB_WR_Reg(0xa7, 0xdf);
	SCCB_WR_Reg(0xa8, 0xf0);
	SCCB_WR_Reg(0xa9, 0x90);
	SCCB_WR_Reg(0xaa, 0x94);
	SCCB_WR_Reg(0x13, 0xe5);
	SCCB_WR_Reg(0x0e, 0x61);
	SCCB_WR_Reg(0x0f, 0x4b);
	SCCB_WR_Reg(0x16, 0x02);
	SCCB_WR_Reg(0x1e, 0x27);//镜像控制
	SCCB_WR_Reg(0x21, 0x02);
	SCCB_WR_Reg(0x22, 0x91);
	SCCB_WR_Reg(0x29, 0x07);
	SCCB_WR_Reg(0x33, 0x0b);
	SCCB_WR_Reg(0x35, 0x0b);
	SCCB_WR_Reg(0x37, 0x1d);
	SCCB_WR_Reg(0x38, 0x71);
	SCCB_WR_Reg(0x39, 0x2a);
	SCCB_WR_Reg(0x3c, 0x78);
	SCCB_WR_Reg(0x4d, 0x40);
	SCCB_WR_Reg(0x4e, 0x20);
	SCCB_WR_Reg(0x69, 0x00);
	SCCB_WR_Reg(0x6b, 0x80);//8MHz*6=48Mhz			//80
	SCCB_WR_Reg(0x74, 0x19);
	SCCB_WR_Reg(0x8d, 0x4f);
	SCCB_WR_Reg(0x8e, 0x00);
	SCCB_WR_Reg(0x8f, 0x00);
	SCCB_WR_Reg(0x90, 0x00);
	SCCB_WR_Reg(0x91, 0x00);
	SCCB_WR_Reg(0x92, 0x00);
	SCCB_WR_Reg(0x96, 0x00);
	SCCB_WR_Reg(0x9a, 0x80);
	SCCB_WR_Reg(0xb0, 0x84);
	SCCB_WR_Reg(0xb1, 0x0c);
	SCCB_WR_Reg(0xb2, 0x0e);
	SCCB_WR_Reg(0xb3, 0x82);
	SCCB_WR_Reg(0xb8, 0x0a);
	SCCB_WR_Reg(0x43, 0x14);
	SCCB_WR_Reg(0x44, 0xf0);
	SCCB_WR_Reg(0x45, 0x34);
	SCCB_WR_Reg(0x46, 0x58);
	SCCB_WR_Reg(0x47, 0x28);
	SCCB_WR_Reg(0x48, 0x3a);
	SCCB_WR_Reg(0x59, 0x88);
	SCCB_WR_Reg(0x5a, 0x88);
	SCCB_WR_Reg(0x5b, 0x44);
	SCCB_WR_Reg(0x5c, 0x67);
	SCCB_WR_Reg(0x5d, 0x49);
	SCCB_WR_Reg(0x5e, 0x0e);
	SCCB_WR_Reg(0x64, 0x04);
	SCCB_WR_Reg(0x65, 0x20);
	SCCB_WR_Reg(0x66, 0x05);
	SCCB_WR_Reg(0x94, 0x04);
	SCCB_WR_Reg(0x95, 0x08);
	SCCB_WR_Reg(0x6c, 0x0a);
	SCCB_WR_Reg(0x6d, 0x55);
	SCCB_WR_Reg(0x4f, 0x80);
	SCCB_WR_Reg(0x50, 0x80);
	SCCB_WR_Reg(0x51, 0x00);
	SCCB_WR_Reg(0x52, 0x22);
	SCCB_WR_Reg(0x53, 0x5e);
	SCCB_WR_Reg(0x54, 0x80);
	SCCB_WR_Reg(0x09, 0x03);//??????
	SCCB_WR_Reg(0x6e, 0x11);
	SCCB_WR_Reg(0x6f, 0x9f);//0x9e for advance AWB
	SCCB_WR_Reg(0x55, 0x00);//??
	SCCB_WR_Reg(0x56, 0x40);//??? 0x40
	SCCB_WR_Reg(0x57, 0x40);//0x40,  change according to Jim's request
	//SCCB_WR_Reg(0x71, 0x80);	//彩条
}

//==========================DCMI============================================
void OV7670_DCMI_Init(uint16_t DCMI_CaptureMode,int PrePriority,int SubPriority)
{
	DCMI_InitTypeDef DCMI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI,ENABLE);//使能DCMI时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA时钟使能
	NIVC_GROUP_CFG;
	GPIO_Init_AF(DCMI_HSYNC_PIN);
	GPIO_Init_AF(DCMI_PIXCLK_PIN);
	GPIO_Init_AF(DCMI_VSYNC_PIN);
	GPIO_Init_AF(DCMI_D0_PIN);
	GPIO_Init_AF(DCMI_D1_PIN);
	GPIO_Init_AF(DCMI_D2_PIN);
	GPIO_Init_AF(DCMI_D3_PIN);
	GPIO_Init_AF(DCMI_D4_PIN);
	GPIO_Init_AF(DCMI_D5_PIN);
	GPIO_Init_AF(DCMI_D6_PIN);
	GPIO_Init_AF(DCMI_D7_PIN);
	GPIO_PinAFConfig(DCMI_HSYNC_PIN_S,GPIO_AF_DCMI);
	GPIO_PinAFConfig(DCMI_PIXCLK_PIN_S,GPIO_AF_DCMI);
	GPIO_PinAFConfig(DCMI_VSYNC_PIN_S,GPIO_AF_DCMI);
	
	GPIO_PinAFConfig(DCMI_D0_PIN_S,GPIO_AF_DCMI);
	GPIO_PinAFConfig(DCMI_D1_PIN_S,GPIO_AF_DCMI);
	GPIO_PinAFConfig(DCMI_D2_PIN_S,GPIO_AF_DCMI);
	GPIO_PinAFConfig(DCMI_D3_PIN_S,GPIO_AF_DCMI);//
	GPIO_PinAFConfig(DCMI_D4_PIN_S,GPIO_AF_DCMI);				//D4没接好
	GPIO_PinAFConfig(DCMI_D5_PIN_S,GPIO_AF_DCMI);//
	GPIO_PinAFConfig(DCMI_D6_PIN_S,GPIO_AF_DCMI);
	GPIO_PinAFConfig(DCMI_D7_PIN_S,GPIO_AF_DCMI);
	
	DCMI_DeInit();
	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode;//DCMI_CaptureMode_Continuous
	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;//像素时钟~沿有效
	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;//垂直同步低有效
	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;//水平同步低有效
	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;//8位数据宽度一个像素时钟
	DCMI_Init(&DCMI_InitStructure);
	DCMI_ITConfig(DCMI_IT_LINE|DCMI_IT_FRAME,ENABLE);	//开启行中断，帧中断
	DCMI_Cmd(ENABLE);	//DCMI使能
	NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PrePriority;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//==========================DMA_Circular============================================
void OV7670_DMA_LineCircular(int PrePriority,int SubPriority)
{
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NIVC_GROUP_CFG;
	DMA_DeInit(DMA2_Stream1);
	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//等待DMA2_Stream1可配置
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;  //通道1 DCMI通道
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DCMI->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&OV7670_LineBuf;	//目标内存零地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储器方向
	DMA_InitStructure.DMA_BufferSize = 160;//DMA缓存大小，单位为数据单位，当外设传内存时数据单位为DMA_PeripheralDataSize
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //外设数据宽度32位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word; //存储器数据宽度32位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// 循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//高优先级
	DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single; //内存突发传输每次转移一个数据
	DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single; //外设突发传输每次转移一个数据
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
	DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);//开启传输完成终中断
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PrePriority;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;		//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
	//DMA_Cmd(DMA2_Stream1, ENABLE);//开启DMA2_Stream1
}


void DCMI_Start(void)	//DCMI启动传输
{
	DCMI_CaptureCmd(ENABLE);//DCMI捕获使能
	DMA_Cmd(DMA2_Stream1, ENABLE);//开启DMA2_Stream1
}

void DCMI_Stop(void)	//DCMI关闭传输
{
  DCMI_CaptureCmd(DISABLE);//DCMI捕获关闭
	while(DCMI->CR&0X01);		//等待传输结束
	DMA_Cmd(DMA2_Stream1,DISABLE);//关闭DMA2,Stream1
} 

int CamState=CAMST_WaitStart;
int CamNowFrame=0;

void CameraStart(void)
{
	//DMA_Cmd(DMA2_Stream1,DISABLE);
	//DMA_Cmd(DMA2_Stream1, ENABLE);
	//DMA_MemoryTargetConfig(DMA2_Stream1,100,DMA_Memory_1);
	CamState=CAMST_Start;
	CamNowFrame=!CamNowFrame;
}

int IsCameraReady(void)
{
	return (CamState==CAMST_ImgReady);
}

void DCMI_IRQHandler(void)
{
	if(DCMI_GetITStatus(DCMI_IT_LINE) != RESET)
	{
		switch(CamState)
		{
			case CAMST_WaitStart:
				break;
			case CAMST_Start:
				break;
			case CAMST_WaitImg:
				ov_line++;
				if(ov_line>=240)
				{
					CamState=CAMST_ImgReady;
					ov_frame++;
				}
				break;
			case CAMST_ImgReady:
				break;
			default:
				break;
		}
		DCMI_ClearITPendingBit(DCMI_IT_LINE);
	}
	if(DCMI_GetITStatus(DCMI_IT_FRAME)!=RESET)//捕获到一帧图像
	{
		switch(CamState)
		{
			case CAMST_WaitStart:
				break;
			case CAMST_Start:
				ov_line=0;
				CamState=CAMST_WaitImg;
				break;
			case CAMST_WaitImg:	//error
				CamState=CAMST_ImgError;
				break;
			case CAMST_ImgReady:
				break;
			case CAMST_ImgError:	//when error
				//TODO:
				Sys_Soft_Reset();	//system reset
				break;
		}
		DCMI_ClearITPendingBit(DCMI_IT_FRAME);//清除中断标记
	}
}


void DMA2_Stream1_IRQHandler(void)
{
	int i;
	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1)==SET)//DMA2_Stream1传输完成
	{
		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);//清除传输完成中断
		//TODO:读取数据等操作
		if((CamState==CAMST_WaitImg)&&(ov_line&0x01))
		{
			for(i=0;i<160;i++)
				OV7670_Frame[CamNowFrame][ov_line>>1][i]=(u16)(OV7670_LineBuf[i]&0xffff);
		}
	}
}
