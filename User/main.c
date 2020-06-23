/*
 *    Copyright 赛北智能车
 *    All rights reserved.
 *    除注明出处外，以下所有内容版权均中国计量大学赛北智能车所有，未经允许，不得用于商业用途，
 *    修改内容时必须保留赛北智能车的版权声明。
 *
 * @file       F-450'main.c
 * @brief      调试模块
 * @version    v1.0
 * @date       2018-10-17
 * @author     YuTao Xu, HuanCheng Wu
 * @edit       DD
 */
#include <stdio.h>
#include "arm_math.h"
#include "stm32f4xx.h"
#include "cfg_func_f407.h"


uint8_t ms20_flag=0,ms5_flag=0,motor_tmp_flag=0;
extern uint16_t pwm_tmp;

static void sys_init()
{
	NVIC_GROUP_CFG;
	//SD_FATFS_INIT();
	OLED_Init();
	//LCD_P6x8Str(0,0,"MPU_Init");
	Delay_ms(500);
	MPU9250_Init();
	pwm_init();

	LED_INIT;
	KEY_INIT;
	GPIO_Init_OutPP(GPIOE,GPIO_Pin_1);
	USART1_Init(115200,3,1);
	LED_ON;
	//Delay_ms(5000);
	//motor_tmp_flag=1;
	LED_OFF;
	
	
	
	//定时器最后启动
	TIM3_Int_Init(5000-1,84-1);//ms 中断
}

float f_data[8];
int16_t c_data[8];

char sd_write[10];
uint8_t sd_size;

static void Param_Show(int i)
{
	switch (i)
		{
		case 0:	
		{			
			LCD_PrintValueF(0,2,gyro_filter.x,2);
			LCD_PrintValueF(0,3,gyro_filter.y,2);
			LCD_PrintValueF(0,4,gyro_filter.z,2);
			LCD_PrintValueF(0,5,acc_filter.x,2);
			LCD_PrintValueF(0,6,acc_filter.y,2);
			LCD_PrintValueF(0,7,acc_filter.z,2);
			LCD_PrintValueI(0,0,pwm_tmp);
			LCD_PrintValueF(64,2,Pitch,2);
			LCD_PrintValueF(64,3,Roll,2);
			LCD_PrintValueF(64,4,Yaw,2);
		}break;
		case 1:
		{
		
		}break;
		case 2:
		{
		
		}
			break;
		case 3:
		{
		
		}break;
		default:
			LCD_P6x8Str(0,0,"Show Error");
	}	
	
}

static void Param_Send()
{
		f_data[0] = gyro_filter.x;
		f_data[1] = gyro_filter.y;
		f_data[2] = gyro_filter.z;
//		f_data[0] = gyroRaw.x;
//		f_data[1] = gyroRaw.y;
//		f_data[2] = gyroRaw.z;
	
		
		f_data[3] = acc_filter.x;
		f_data[4] = acc_filter.y;
		f_data[5] = acc_filter.z;
//		f_data[3] = accRaw.x;
//		f_data[4] = accRaw.y;
//		f_data[5] = accRaw.z;
		
		f_data[6] = Pitch;
		f_data[7] = Roll;
	
		c_data[0] = acc.x;
		c_data[1] = acc.y;
		c_data[2] = acc.z;
		c_data[3] = gyro.x;
		c_data[4] = gyro.y;
		c_data[5] = gyro.z;
		
	
		//vcan_send_wave((uint8_t*)c_data,12);
		vcan_send_wave((uint8_t*)f_data,32);
}

uint8_t key_flag=0;
static void NV_Operation()
{
	
	if(KEY_GET==1&&key_flag==0)
	{
		key_flag=1;
		motor_tmp_flag=!motor_tmp_flag;
		pwm_tmp=1020;
		accelerator_motor_set(pwm_tmp,pwm_tmp,pwm_tmp,pwm_tmp);
		Delay_ms(50);
		//MPU_Offset_Init();
	}
	else if(KEY_GET==0&&key_flag==1)
	{
		key_flag=0;
	}
	
	
	if(ms5_flag)
	{
		MPU9250_GetInfo();
//		if(motor_tmp_flag)
//		{

//			sprintf(sd_write, "%.2f,",accRaw.x);
//			sd_size = strlen(sd_write);
//			f_write(&fdst2,sd_write,sd_size,&bw);
//			f_sync(&fdst2);
//		}
		ms5_flag=0;
	}
	
	if(ms20_flag)
	{
		
		Param_Show(0);
		Param_Send();

//		sprintf(sd_write, "%f,",accRaw.z);
//		sd_size = strlen(sd_write);
//		f_write(&fdst,sd_write,sd_size,&bw);
//		f_sync(&fdst);
		ms20_flag=0;
	}
	
}

int main(void)
{
	sys_init();
	//f_open(&fdst,"acc_30.csv",FA_WRITE | FA_CREATE_ALWAYS);
	//f_open(&fdst2,"acc_33.csv",FA_WRITE | FA_CREATE_ALWAYS);
	//LED_OFF; 
	while(1)
	{
		NV_Operation();
	}
}
