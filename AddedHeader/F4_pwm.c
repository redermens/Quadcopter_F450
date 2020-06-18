#include "F4_pwm.h"

static uint16_t accelerator_motor[4]={1000,1000,1000,1000};

void pwm_init()//初始化4路pwm波
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM9时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能PORTA时钟	

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PA6
	

	TIM_TimeBaseStructure.TIM_Prescaler=83;  //定时器分频	//1Mhz
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=19999;   //自动重装载值 50hz
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器4
	
		//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3
	
	TIM_SetCompare1(TIM4,3000);
	TIM_SetCompare2(TIM4,3000);
	TIM_SetCompare3(TIM4,3000);
	TIM_SetCompare4(TIM4,3000);
	
	Delay_ms(2000);
	
	TIM_SetCompare1(TIM4,1000);
	TIM_SetCompare2(TIM4,1000);
	TIM_SetCompare3(TIM4,1000);
	TIM_SetCompare4(TIM4,1000);
}


void accelerator_motor_set(uint16_t motor_0,uint16_t motor_1,uint16_t motor_2,uint16_t motor_3)
{
	motor_0 = (1000<motor_0 && 2000>motor_0) ? motor_0 : accelerator_motor[0];
	motor_1 = (1000<motor_1 && 2000>motor_1) ? motor_1 : accelerator_motor[1];
	motor_2 = (1000<motor_2 && 2000>motor_2) ? motor_2 : accelerator_motor[2];
	motor_3 = (1000<motor_3 && 2000>motor_3) ? motor_3 : accelerator_motor[3];
	
	accelerator_motor[0] = motor_0;
	accelerator_motor[1] = motor_1;
	accelerator_motor[2] = motor_2;
	accelerator_motor[3] = motor_3;
}

void motor_pwm2tim()
{
	TIM4->CCR1 = accelerator_motor[0];
	TIM4->CCR2 = accelerator_motor[1];
	TIM4->CCR3 = accelerator_motor[2];
	TIM4->CCR4 = accelerator_motor[3];
}
