
#include "stm32f4xx_it.h"
#include "cfg_func_f407.h"

extern uint8_t ms20_flag,ms5_flag,motor_tmp_flag;


void HardFault_Handler(void)
{
  while (1);//Sys_Soft_Reset();
}

void MemManage_Handler(void)
{
  while (1);
}

void BusFault_Handler(void)
{
  while (1);
}

void UsageFault_Handler(void)
{
  while (1);
}

void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}

//
//
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		//TODO:
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
	}
}

//
//
void USART1_IRQHandler(void)
{
	unsigned char ch;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		ch = USART1->DR;
		USART1_SendChar(ch);
	} 
}

uint16_t pwm_tmp=1000;
//
//
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		static uint8_t ms20_count=0,ms100_count=0;
		ms20_count++;
		ms5_flag=1;
		
		if(ms20_count==4)
		{
			ms20_count=0;
			ms20_flag=1;
			motor_pwm2tim();
			ms100_count++;
		}
		
		if(ms100_count==5)
		{
			if(pwm_tmp<1400&&motor_tmp_flag)
				accelerator_motor_set(pwm_tmp++,pwm_tmp,pwm_tmp,pwm_tmp);
			ms100_count=0;
		}
		TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);

	}
}


void DMA2_Stream7_IRQHandler()
{
		if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7) != RESET) 
	{
		//清除标志位
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
		//关闭DMA
		DMA_Cmd(DMA2_Stream7,DISABLE);
		//打开发送完成中断,发送最后两个字节
		//USART_ITConfig(USART2,USART_IT_TC,ENABLE);
	}
}





