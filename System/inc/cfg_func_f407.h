#ifndef CFG_FUNC_F407_H
#define CFG_FUNC_F407_H

#include <stdio.h>
#include "arm_math.h"
#include "stm32f4xx.h"
#include "sd_card.h"
#include "fat_func.h"
#include "oledSPI.h"
#include "sram.h"
#include "fast.h"
#include "dev_mpu9250.h"
#include "F4_pwm.h"



#define NVIC_GROUP_CFG NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2)
#define LED_PIN      GPIOA,GPIO_Pin_1              //LED PIN
#define LED_INIT     GPIO_Init_OutPP(LED_PIN)       //do not modify
#define LED_ON       GPIO_ResetBits(LED_PIN)        //do not modify
#define LED_OFF      GPIO_SetBits(LED_PIN)          //do not modify
#define LED_TURN     GPIO_ToggleBits(LED_PIN)
#define KEY_PIN      GPIOA,GPIO_Pin_0
#define KEY_INIT     GPIO_Init_InFL(KEY_PIN)
#define KEY_GET		 GPIO_ReadInputDataBit(KEY_PIN)


void GPIO_Init_OutPP(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void GPIO_Init_InPU(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void GPIO_Init_InFL(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void GPIO_Init_AF(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void Delay_us(volatile unsigned int nTime);
void Delay_ms(volatile unsigned int nTime);
void TimingDelay_Decrement(void);
void TIM_IRQ_Init(TIM_TypeDef* TIMx,int nms,int PrePriority,int SubPriority);
void USART1_Init(int bps,int PrePriority,int SubPriority);
void USART1_SendChar(unsigned char ch);
void Sys_Soft_Reset(void);

void DMA_P2M_Word_Init( DMA_Stream_TypeDef* DMAy_Streamx,        uint32_t DMA_Channel,
	                            uint32_t DMA_PeripheralBaseAddr,         uint32_t DMA_Memory0BaseAddr,
                              uint32_t DMA_BufferSize,                 uint8_t NVIC_IRQChannel,
                              uint32_t DMA_Mode,
                              int PrePriority,int SubPriority );

void EXTI_INT_IRQ_Init( uint32_t RCC_AHB1Periph,       GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin, 
	                      uint8_t EXTI_PortSourceGPIOx,  uint8_t EXTI_PinSourcex,
										    uint32_t EXTI_Line,            EXTITrigger_TypeDef EXTI_Trigger,
                        uint8_t NVIC_IRQChannel,
                        int PrePriority,int SubPriority );

void TIM3_Int_Init(u16 arr,u16 psc);
void SD_FATFS_INIT(void);
void vcan_send_wave(uint8_t* dat,uint32_t dat_size);
void dma_send(uint8_t *data,uint32_t size);


extern FATFS SD_FATFS;
extern FIL fdst,fdst2;
extern UINT bw;

#endif
