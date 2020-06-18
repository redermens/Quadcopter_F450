
#include "cfg_func_f407.h"



FATFS SD_FATFS;
FIL fdst,fdst2;
UINT bw;
static volatile unsigned int TimingDelay;	//Systick延时计数器
int  ret = 0;            //SD卡返回参数
//======================================================================================//
//======================================================================================//
//======================================================================================//
void RCC_GPIO_Open(GPIO_TypeDef* GPIOx)	//打开GPIO时钟
{
	if(GPIOx==GPIOA)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	}
	else if(GPIOx==GPIOB)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	}
	else if(GPIOx==GPIOC)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	}
	else if(GPIOx==GPIOD)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	}
	else if(GPIOx==GPIOE)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	}
	else if(GPIOx==GPIOF)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	}
	else if(GPIOx==GPIOG)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	}
	else if(GPIOx==GPIOH)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	}
	else if(GPIOx==GPIOI)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	}
	else if(GPIOx==GPIOJ)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOJ, ENABLE);
	}
	else if(GPIOx==GPIOK)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOK, ENABLE);
	}
}

void GPIO_Init_OutPP(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)	//推挽输出
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_GPIO_Open(GPIOx);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

//======================================================================================//
//======================================================================================//
//======================================================================================//
void GPIO_Init_InPU(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)	//上拉输入
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_GPIO_Open(GPIOx);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

//======================================================================================//
//======================================================================================//
//======================================================================================//
void GPIO_Init_InFL(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)	//浮空输入
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_GPIO_Open(GPIOx);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}
//======================================================================================//
//======================================================================================//
//======================================================================================//
void GPIO_Init_AF(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_GPIO_Open(GPIOx);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_Init(GPIOx, &GPIO_InitStructure);
}

//======================================================================================//
//======================================================================================//
//======================================================================================//
void Delay_us(volatile unsigned int nTime)	//系统延时函数us
{ 
	TimingDelay = nTime;
	if(SysTick_Config(SystemCoreClock/1000000))	//1us
	{
		//错误捕获
		while(1);
	}	
	while(TimingDelay != 0);
}
//======================================================================================//
//======================================================================================//
//======================================================================================//
void Delay_ms(volatile unsigned int nTime)	//系统延时函数ms
{ 
	TimingDelay = nTime;
	if(SysTick_Config(SystemCoreClock/1000))	//1ms
	{
		//错误捕获
		while(1);
	}	
	while(TimingDelay != 0);
}


//======================================================================================//
//======================================================================================//
//======================================================================================//
void TimingDelay_Decrement()	//放在Systick中断中
{
	if(TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}	
}	


uint8_t usart_send_buff[36];
//======================================================================================//
//======================================================================================//
//======================================================================================//
void USART1_Init(int bps,int PrePriority,int SubPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PrePriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	
	DMA_DeInit(DMA2_Stream7);
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){}
	DMA_InitStructure.DMA_Channel =               DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr =    (uint32_t)(&USART1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr =       (uint32_t)usart_send_buff;
	DMA_InitStructure.DMA_DIR =                   DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize =            20;
	DMA_InitStructure.DMA_PeripheralInc =         DMA_PeripheralInc_Disable; 	//外设地址不增加
	DMA_InitStructure.DMA_MemoryInc =             DMA_MemoryInc_Enable;					//数据地址递增
	DMA_InitStructure.DMA_PeripheralDataSize =    DMA_PeripheralDataSize_Byte; 	
	DMA_InitStructure.DMA_MemoryDataSize =        DMA_MemoryDataSize_Byte;			
	DMA_InitStructure.DMA_Mode =                  DMA_Mode_Normal;//DMA_Mode_Circular//DMA_Mode_Normal
	DMA_InitStructure.DMA_Priority =              DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode=               DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold=          DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst=            DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst=        DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);	//A9,TX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);	//A10,RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;	//A9,A10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);    

	USART_InitStructure.USART_BaudRate = bps;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	USART_ClearFlag(USART1, USART_IT_RXNE);
	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	//USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	USART_Cmd(USART1,ENABLE);
}

void dma_send(uint8_t *data,uint32_t size)
{
	memcpy(usart_send_buff,data,size);
	DMA_SetCurrDataCounter(DMA2_Stream7,size);
	DMA_Cmd(DMA2_Stream7,ENABLE);
}

//static void usart_dma_init()
//{
//	DMA_InitTypeDef DMA_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE); //enable DMA1 clock
//    DMA_Cmd(DMA1_Stream1,DISABLE);                                     //close DMA Channel
//    DMA_DeInit(DMA1_Stream1);
//	
//}

	
	
void USART1_SendChar(unsigned char ch)
{
	USART1->DR = (ch & (uint16_t)0x01FF);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
}

void Sys_Soft_Reset(void)
{   
    SCB->AIRCR =0X05FA0000|(u32)0x04;      
}


void DMA_P2M_Word_Init( DMA_Stream_TypeDef* DMAy_Streamx,        uint32_t DMA_Channel,
	                            uint32_t DMA_PeripheralBaseAddr,         uint32_t DMA_Memory0BaseAddr,
                              uint32_t DMA_BufferSize,                 uint8_t NVIC_IRQChannel,
                              uint32_t DMA_Mode,
                              int PrePriority,int SubPriority )
{
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_GROUP_CFG;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(DMAy_Streamx);
	while (DMA_GetCmdStatus(DMAy_Streamx) != DISABLE){}
	DMA_InitStructure.DMA_Channel =               DMA_Channel;
	DMA_InitStructure.DMA_PeripheralBaseAddr =    DMA_PeripheralBaseAddr;
	DMA_InitStructure.DMA_Memory0BaseAddr =       DMA_Memory0BaseAddr;
	DMA_InitStructure.DMA_DIR =                   DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize =            DMA_BufferSize;
	DMA_InitStructure.DMA_PeripheralInc =         DMA_PeripheralInc_Disable; 	//外设地址不增加
	DMA_InitStructure.DMA_MemoryInc =             DMA_MemoryInc_Enable;					//数据地址递增
	DMA_InitStructure.DMA_PeripheralDataSize =    DMA_PeripheralDataSize_Word; 	
	DMA_InitStructure.DMA_MemoryDataSize =        DMA_MemoryDataSize_Word;			
	DMA_InitStructure.DMA_Mode =                  DMA_Mode;//DMA_Mode_Circular//DMA_Mode_Normal
	DMA_InitStructure.DMA_Priority =              DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode=               DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold=          DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst=            DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst=        DMA_PeripheralBurst_Single;
	DMA_Init(DMAy_Streamx, &DMA_InitStructure);
	DMA_ITConfig(DMAy_Streamx,DMA_IT_TC,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PrePriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI_INT_IRQ_Init( uint32_t RCC_AHB1Periph,       GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin, 
	                      uint8_t EXTI_PortSourceGPIOx,  uint8_t EXTI_PinSourcex,
										    uint32_t EXTI_Line,            EXTITrigger_TypeDef EXTI_Trigger,
                        uint8_t NVIC_IRQChannel,
                        int PrePriority,int SubPriority )
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
  GPIO_Init(GPIOx, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourcex);
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PrePriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}






//初始化TIM3定时器，并且开启中断
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
							 
}



//初始化SD卡Fatfs
void SD_FATFS_INIT(void)
{
	unsigned int total = 0, free = 0;
	
	while(SD_Init(2,2))
	{
		Delay_ms(500);
		LED_TURN;
	}
	Delay_ms(100);
	LED_OFF;
	ret = f_mount(&SD_FATFS, "0:", 1);
	exf_ifmkfs(ret); //if format invalid than change file-system
	
	ret = exf_getfree("0:", &total, &free);
//	LED_PrintValueI(0,1,total/1024);				LED_PrintValueI(60,1,free/1024);

//	f_open(&fdst, "Log.txt", FA_WRITE | FA_CREATE_ALWAYS); //新建一个txt文件
//	f_write(&fdst, "Program Run ...\n", sizeof("Program Run...\n"), &bw);                     //写入字符串
//	f_sync(&fdst);
	LED_ON;
}


void uart_putbuff(uint8_t *data,uint32_t length)
{
	while(length--)
	{
		USART1_SendChar(*(data++));
	}
	
}

#define CMD_WARE 	3
void vcan_send_wave(uint8_t* dat,uint32_t dat_size)
{
    static uint8_t cmdf[2] = {CMD_WARE,~CMD_WARE};
    static uint8_t cmdr[2] = {~CMD_WARE,CMD_WARE};
    
	uint8_t uart_send[36];
	memcpy(uart_send,cmdf,2);
	memcpy(uart_send+2,dat,dat_size);
	memcpy(uart_send+2+dat_size,cmdr,2);
	dma_send(uart_send,4+dat_size);
//    uart_putbuff(cmdf,sizeof(cmdf));
//    uart_putbuff(dat,dat_size);
//    uart_putbuff(cmdr,sizeof(cmdr));
}
