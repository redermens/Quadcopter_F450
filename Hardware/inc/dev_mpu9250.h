/*
 * @Description: 
 * @FilePath: /LQ_TC26xB_LIBtasking/src/AppSw/Tricore/MY/API/MPU9250.h
 * @Author: Wei Zhou
 * @Github: https://github.com/zromyk
 * @Date: 1970-01-01 08:00:00
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-05-22 19:46:09
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */ 

#ifndef _DEV_MPU9250_H
#define _DEV_MPU9250_H

#include <math.h>

// MPU9250端口配置
#include "stm32f4xx.h"
#include "cfg_func_f407.h"
#include "stm32f4xx_gpio.h"



#ifndef M_PI_F
 #define M_PI_F 3.141592653589793f
#endif

#define     ICM20602_DEV_ADDR           0x68 //SA0接地：0x68   SA0上拉：0x69  模块默认上拉


#define     ICM20602_SPI_W              0x00
#define     ICM20602_SPI_R              0x80


#define     ICM20602_XG_OFFS_TC_H       0x04
#define     ICM20602_XG_OFFS_TC_L       0x05
#define     ICM20602_YG_OFFS_TC_H       0x07
#define     ICM20602_YG_OFFS_TC_L       0x08
#define     ICM20602_ZG_OFFS_TC_H       0x0A
#define     ICM20602_ZG_OFFS_TC_L       0x0B
#define     ICM20602_SELF_TEST_X_ACCEL  0x0D
#define     ICM20602_SELF_TEST_Y_ACCEL  0x0E
#define     ICM20602_SELF_TEST_Z_ACCEL  0x0F
#define     ICM20602_XG_OFFS_USRH       0x13
#define     ICM20602_XG_OFFS_USRL       0x14
#define     ICM20602_YG_OFFS_USRH       0x15
#define     ICM20602_YG_OFFS_USRL       0x16
#define     ICM20602_ZG_OFFS_USRH       0x17
#define     ICM20602_ZG_OFFS_USRL       0x18
#define     ICM20602_SMPLRT_DIV         0x19
#define     ICM20602_CONFIG             0x1A
#define     ICM20602_GYRO_CONFIG        0x1B
#define     ICM20602_ACCEL_CONFIG       0x1C
#define     ICM20602_ACCEL_CONFIG_2     0x1D
#define     ICM20602_LP_MODE_CFG        0x1E
#define     ICM20602_ACCEL_WOM_X_THR    0x20
#define     ICM20602_ACCEL_WOM_Y_THR    0x21
#define     ICM20602_ACCEL_WOM_Z_THR    0x22
#define     ICM20602_FIFO_EN            0x23
#define     ICM20602_FSYNC_INT          0x36
#define     ICM20602_INT_PIN_CFG        0x37
#define     ICM20602_INT_ENABLE         0x38
#define     ICM20602_FIFO_WM_INT_STATUS 0x39 
#define     ICM20602_INT_STATUS         0x3A
#define     ICM20602_ACCEL_XOUT_H       0x3B
#define     ICM20602_ACCEL_XOUT_L       0x3C
#define     ICM20602_ACCEL_YOUT_H       0x3D
#define     ICM20602_ACCEL_YOUT_L       0x3E
#define     ICM20602_ACCEL_ZOUT_H       0x3F
#define     ICM20602_ACCEL_ZOUT_L       0x40
#define     ICM20602_TEMP_OUT_H         0x41
#define     ICM20602_TEMP_OUT_L         0x42
#define     ICM20602_GYRO_XOUT_H        0x43
#define     ICM20602_GYRO_XOUT_L        0x44
#define     ICM20602_GYRO_YOUT_H        0x45
#define     ICM20602_GYRO_YOUT_L        0x46
#define     ICM20602_GYRO_ZOUT_H        0x47
#define     ICM20602_GYRO_ZOUT_L        0x48
#define     ICM20602_SELF_TEST_X_GYRO   0x50
#define     ICM20602_SELF_TEST_Y_GYRO   0x51
#define     ICM20602_SELF_TEST_Z_GYRO   0x52
#define     ICM20602_FIFO_WM_TH1        0x60
#define     ICM20602_FIFO_WM_TH2        0x61
#define     ICM20602_SIGNAL_PATH_RESET  0x68
#define     ICM20602_ACCEL_INTEL_CTRL   0x69
#define     ICM20602_USER_CTRL          0x6A
#define     ICM20602_PWR_MGMT_1         0x6B
#define     ICM20602_PWR_MGMT_2         0x6C
#define     ICM20602_I2C_IF             0x70
#define     ICM20602_FIFO_COUNTH        0x72
#define     ICM20602_FIFO_COUNTL        0x73
#define     ICM20602_FIFO_R_W           0x74
#define     ICM20602_WHO_AM_I           0x75
#define     ICM20602_XA_OFFSET_H        0x77
#define     ICM20602_XA_OFFSET_L        0x78
#define     ICM20602_YA_OFFSET_H        0x7A
#define     ICM20602_YA_OFFSET_L        0x7B
#define     ICM20602_ZA_OFFSET_H        0x7D
#define     ICM20602_ZA_OFFSET_L        0x7E




// 定义MPU9250内部地址
#define MPU9250_DEV_ADDR        0xD0

//0x68    //IIC写入时的地址字节数据，+1为读取

#define SMPLRT_DIV              0x19    //陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIG                  0x1A    //低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG             0x1B    //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG            0x1C    //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_CONFIG2           0X1D    //加速度计低通滤波器 0x06 5hz
#define INT_PIN_CFG             0x37    //设置9250辅助I2C为直通模式寄存器
#define ACCEL_XOUT_H            0x3B
#define ACCEL_XOUT_L            0x3C
#define ACCEL_YOUT_H            0x3D
#define ACCEL_YOUT_L            0x3E
#define ACCEL_ZOUT_H            0x3F
#define ACCEL_ZOUT_L            0x40
#define TEMP_OUT_H              0X41    //温度计输出数据
#define TEMP_OUT_L              0X42
#define GYRO_XOUT_H             0x43
#define GYRO_XOUT_L             0x44    
#define GYRO_YOUT_H             0x45
#define GYRO_YOUT_L             0x46
#define GYRO_ZOUT_H             0x47
#define GYRO_ZOUT_L             0x48
#define USER_CTRL               0x6A    //关闭9250对辅助I2C设备的控制,用户配置当为0X10时使用SPI模式
#define PWR_MGMT_1              0x6B    //电源管理，典型值：0x00(正常启用)
#define WHO_AM_I                0x75    //器件ID MPU9250默认ID为0X71




#define MPU_TIM_COUNT_CYCLE     (0.005) // 定时器中断以秒为单位的周期，如1ms中断，MPU_TIM_COUNT_CYCLE = 0.001
#define _abs(x)                 ((x) >= 0 ? (x) : -(x))

// 四元数数据结构体
typedef struct
{
    float kp;           /*比例增益*/
    float ki;           /*积分增益*/
    float exInt;
    float eyInt;
    float ezInt;        /*积分误差累计*/

    float q0;           /*四元数*/
    float q1;
    float q2;
    float q3;	
    float rMat[3][3];   /*旋转矩阵*/
} ElemNode;


typedef struct 
{
	int16_t   	x;
	int16_t		y;
	int16_t		z;
}Vector3I;

typedef struct
{
	float		x;
	float		y;
	float 		z;
}Vector3F;


typedef struct
{
 float Input_Butter[3];
 float Output_Butter[3];
}Butter_BufferData;


typedef struct
{
  float a[3];
  float b[3];
}Butter_Parameter;


extern float Pitch,Roll,Yaw;
extern Vector3I gyro,acc,mag,gyro_offset;
extern Vector3F gyroRaw,accRaw,magRaw;
extern Vector3F gyro_filter,acc_filter;

void GetData(void);
void MPU9250_Init(void);        // 初始化MPU9250
void MPU9250_GetInfo(void);     // 放在定时器中断中执行
void MPU_Offset_Init(void);
//void imuUpdate(MPUNode *mpu, float dt,ElemNode* ahrs);

#endif
