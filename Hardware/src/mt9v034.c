

#include "mt9v034.h"
#include "cfg_func_f407.h"
#include "lcd.h"

#define SCCB_READ_SDA GPIO_ReadInputDataBit(SCCB_SDA)

int mtv_frame;
unsigned char MTV_FrameBuf[120][160];

//====================SCCB===============================
void SCCB_SDA_IN(void)
{
	GPIO_Init_InPU(SCCB_SDA);
}

void SCCB_SDA_OUT(void)
{
	GPIO_Init_OutPP(SCCB_SDA);
}

void delay_uss(int n)
{
	int i;
	for(i=0;i<9000;i++);
}

void SCCB_Init(void)
{
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

void SCCB_Ack(void)
{
	delay_uss(50);
	GPIO_ResetBits(SCCB_SDA);
	GPIO_SetBits(SCCB_SCL);
	delay_uss(50);
	GPIO_ResetBits(SCCB_SCL);
	delay_uss(50);
	GPIO_ResetBits(SCCB_SDA);
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

unsigned short SCCB_RD_HalfWord(unsigned char ReadAddr)
{
	unsigned short val=0;
	SCCB_Start(); 		
	SCCB_WR_Byte(MT_WRITE_ID);	
	delay_uss(100);	 
  SCCB_WR_Byte(ReadAddr);	 
	delay_uss(100);	  
	//SCCB_Stop();   
	//delay_uss(100);	   
	SCCB_Start();
	SCCB_WR_Byte(MT_READ_ID); 
	delay_uss(100);
  val=SCCB_RD_Byte();
	SCCB_Ack();
	val<<=8;
	val|=SCCB_RD_Byte();
  SCCB_No_Ack();
  SCCB_Stop();
  return val;
}

unsigned short SCCB_WR_HalfWord(unsigned char WriteAddr,unsigned short dat)
{
	SCCB_Start();
	SCCB_WR_Byte(MT_WRITE_ID);
	delay_uss(100);
	SCCB_WR_Byte(WriteAddr);
	delay_uss(100);
	SCCB_WR_Byte(dat>>8);
	delay_uss(100);
	SCCB_WR_Byte(dat);
	delay_uss(100);
	SCCB_Stop();
	delay_uss(100);
	//Delay_ms(10);
}

int check_camera(void)
{
	int val=0;
	SCCB_Init();
	val=SCCB_RD_HalfWord(MTREG_ChipVersion);
	if(val==4900) return 1;	//MT9V034
	else return 0;
}

void MTV_DCMI_Init(uint16_t DCMI_CaptureMode,int PrePriority,int SubPriority)
{
	DCMI_InitTypeDef DCMI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI,ENABLE);//??DCMI?? 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA????
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
	GPIO_PinAFConfig(DCMI_D4_PIN_S,GPIO_AF_DCMI);				//D4???
	GPIO_PinAFConfig(DCMI_D5_PIN_S,GPIO_AF_DCMI);//
	GPIO_PinAFConfig(DCMI_D6_PIN_S,GPIO_AF_DCMI);
	GPIO_PinAFConfig(DCMI_D7_PIN_S,GPIO_AF_DCMI);
	
	DCMI_DeInit();
	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode;//DCMI_CaptureMode_Continuous
	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware; 
	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Falling;//????~??? DCMI_PCKPolarity_Rising
	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_Low;//???????
	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;//???????
	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;//8???????????
	DCMI_Init(&DCMI_InitStructure);
	DCMI_ITConfig(DCMI_IT_LINE|DCMI_IT_FRAME,ENABLE);	//?????,???
	DCMI_Cmd(ENABLE);	//DCMI??
	NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PrePriority;//?????
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;//?????
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void MTV_DMA_LineCircular(int PrePriority,int SubPriority)
{
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NIVC_GROUP_CFG;
	DMA_DeInit(DMA2_Stream1);
	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//??DMA2_Stream1???
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;  //??1 DCMI??
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DCMI->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&MTV_FrameBuf;	//???????
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//????????
	DMA_InitStructure.DMA_BufferSize = 120*160/4;//DMA????,???????,????????????DMA_PeripheralDataSize
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//??????? 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//???????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //??????32?
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word; //???????32?
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ????
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//????
	DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single; //??????????????
	DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single; //??????????????
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
	DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);//?????????
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PrePriority;//?????
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;		//?????
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);
	//DMA_Cmd(DMA2_Stream1, ENABLE);//??DMA2_Stream1
}

void MTV_Init()
{
	SCCB_Init();
	SCCB_WR_HalfWord(MTREG_ChipControl,0x0188);//Progressive scan,Sensor generates its own exposure,Stereoscopy disabled,Enable parallel output,Simultaneous mode,Context A registers are used
	SCCB_WR_HalfWord(MTREG_WindowWidthContextA,640);	//PIX_W 375
	SCCB_WR_HalfWord(MTREG_WindowHeightContextA,480);	//PIX_H 230
	SCCB_WR_HalfWord(MTREG_HorizontalBlankingContextA, 92);
	SCCB_WR_HalfWord(MTREG_VerticalBlankingContextA, 0);
	SCCB_WR_HalfWord(MTREG_ReadModeContextA,0x33A);//Row bin 4,Col bin 4,Read out rows from bottom to top ,Read out columns from right to left	
	SCCB_WR_HalfWord(MTREG_AEC_AGC_EnableA_B,3);
	SCCB_WR_HalfWord(MTREG_AEC_MInimumExposure,1);
	SCCB_WR_HalfWord(MTREG_AEC_MaximumExposure,500);
	SCCB_WR_HalfWord(MTREG_AEC_AGC_DesiredBin,30);
	SCCB_WR_HalfWord(MTREG_AEC_AGC_PixCount,19200);
} 

void DCMI_Start(void)	//DCMI????
{
	DCMI_CaptureCmd(ENABLE);//DCMI????
	DMA_Cmd(DMA2_Stream1, ENABLE);//??DMA2_Stream1
}

void DCMI_IRQHandler(void)
{
	
	if(DCMI_GetITStatus(DCMI_IT_LINE) != RESET)
	{
		//TODO:
		
		DCMI_ClearITPendingBit(DCMI_IT_LINE);
	}
	if(DCMI_GetITStatus(DCMI_IT_FRAME)!=RESET)//???????
	{
		//TODO:
		//LCD_ShowString(80,90,200,16,16,"STM32F407VET6 system TEST");	
		//DMA_Cmd(DMA2_Stream1, ENABLE);
		mtv_frame++;
		DCMI_ClearITPendingBit(DCMI_IT_FRAME);//??????
	}
}

void DMA2_Stream1_IRQHandler(void)
{
	int i;
	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1)==SET)//DMA2_Stream1????
	{
		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);//????????
		//TODO:???????

	}    											 
}








