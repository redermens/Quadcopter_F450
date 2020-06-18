/*
 * @Description: 
 * @FilePath: /LQ_TC26xB_LIBtasking/src/AppSw/Tricore/MY/API/MPU9250.c
 * @Author: Wei Zhou
 * @Github: https://github.com/zromyk
 * @Date: 1970-01-01 08:00:00
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-05-22 21:00:17
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */ 

#include "dev_mpu9250.h"
#include "Soft_IIC.h"


#define Imu_Sampling_Freq  200

float Pitch,Roll,Yaw;

Butter_Parameter Gyro_Parameter;
Butter_Parameter Accel_Parameter;
Butter_BufferData accel_bpf_filter_buf[3],accel_filter_buf_bug[3],accel_filter_buf[3];
Butter_BufferData gyro_filter_buf[3],gyro_bpf_filter_buf[3],gyro_filter_buf_bug[3];


Butter_Parameter Bandstop_Filter_Parameter_30_98={
  //200hz---30hz-98hz  采样-阻带
  1,   0.627040f,  -0.290527f,
  0.354737f,   0.627040f,    0.354737f
};
Butter_Parameter Bandstop_Filter_Parameter_30_94={
  //200hz---30hz-94hz  采样-阻带
  1,   0.5334540355829,  -0.2235264828971,
  0.3882367585514,   0.5334540355829,   0.3882367585514
};




ElemNode AHRS_Param=
{
	.kp    = 1.0f,     /*比例增益*/
    .ki    = 0.02f,   /*积分增益*/
    .exInt = 0.0f,
    .eyInt = 0.0f,
    .ezInt = 0.0f,     /*积分误差累计*/

    .q0 = 1.0f,  /*四元数*/
    .q1 = 0.0f,
    .q2 = 0.0f,
    .q3 = 0.0f
};


Vector3I gyro,acc,mag,gyro_offset;
Vector3F gyroRaw,accRaw,magRaw;
Vector3F gyro_filter,acc_filter;
Vector3F gyro_bpf_filter,gyro_filter_bug,acc_filter_bug;









/***********************************************************
@函数名：Set_Cutoff_Frequency
@入口参数：float sample_frequent, float cutoff_frequent,
Butter_Parameter *LPF
@出口参数：无
功能描述：巴特沃斯低通滤波器初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Set_Cutoff_Frequency(float sample_frequent, float cutoff_frequent,Butter_Parameter *LPF)
{
  float fr = sample_frequent / cutoff_frequent;
  float ohm = tanf(M_PI_F / fr);
  float c = 1.0f + 2.0f * cosf(M_PI_F / 4.0f) * ohm + ohm * ohm;
  if (cutoff_frequent <= 0.0f) {
    // no filtering
    return;
  }
  LPF->b[0] = ohm * ohm / c;
  LPF->b[1] = 2.0f * LPF->b[0];
  LPF->b[2] = LPF->b[0];
  LPF->a[0]=1.0f;
  LPF->a[1] = 2.0f * (ohm * ohm - 1.0f) / c;
  LPF->a[2] = (1.0f - 2.0f * cosf(M_PI_F / 4.0f) * ohm + ohm * ohm) / c;
}



/*************************************************
函数名:	float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
说明:	加速度计低通滤波器
入口:	float curr_input 当前输入加速度计,滤波器参数，滤波器缓存
出口:	无
备注:	2阶Butterworth低通滤波器
*************************************************/
float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
{
  /* 加速度计Butterworth滤波 */
  /* 获取最新x(n) */
  Buffer->Input_Butter[2]=curr_input;
  /* Butterworth滤波 */
  Buffer->Output_Butter[2]=
    Parameter->b[0] * Buffer->Input_Butter[2]
      +Parameter->b[1] * Buffer->Input_Butter[1]
        +Parameter->b[2] * Buffer->Input_Butter[0]
          -Parameter->a[1] * Buffer->Output_Butter[1]
            -Parameter->a[2] * Buffer->Output_Butter[0];
  /* x(n) 序列保存 */
  Buffer->Input_Butter[0]=Buffer->Input_Butter[1];
  Buffer->Input_Butter[1]=Buffer->Input_Butter[2];
  /* y(n) 序列保存 */
  Buffer->Output_Butter[0]=Buffer->Output_Butter[1];
  Buffer->Output_Butter[1]=Buffer->Output_Butter[2];
  return Buffer->Output_Butter[2];
}



/**
 * @description: 初始化MPU9250
 * 调用该函数前，请先调用模拟IIC的初始化
 * 
 *  ACCEL_CONFIG
 *      0x00: 2g
 *      0x08: 4g
 *      0x10: 8g
 *      0x18: 16g
 *  GYRO_CONFIG:
 *      0x00: 250deg/s
 *      0x08: 500deg/s
 *      0x10: 1000deg/s 
 *      0x18: 2000deg/s
 */
#define ACC_SENSITIVITY     (2048.0f) // 32767.0f / 8.0f
#define GYRO_SENSITIVITY    (32.767f)   // 32767.0f / 2000.0f


void icm20602_self1_check(void)
{
    uint8_t dat=0;
    while(0x71 != dat)   //读取ICM20602 ID
    {
        dat = simiic_read_reg(MPU9250_DEV_ADDR,WHO_AM_I,IIC);
        Delay_ms(10);
        //卡在这里原因有以下几点
        //1 MPU6050坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
        //3 可能你需要外接上拉电阻，上拉到3.3V
    }
}


void MPU_Offset_Init()
{
	Delay_ms(500);
	
	int32_t tmp_x=0,tmp_y=0,tmp_z=0;
	
	for(int i=0;i<100;i++)
	{
		uint8_t dat[6];
		
		simiic_read_regs(MPU9250_DEV_ADDR, GYRO_XOUT_H, dat, 6, IIC);  
		tmp_x += (int16_t)((uint16_t)dat[0]<<8 | dat[1]);
		tmp_y += (int16_t)((uint16_t)dat[2]<<8 | dat[3]);
		tmp_z += (int16_t)((uint16_t)dat[4]<<8 | dat[5]);
		
		Delay_ms(10);
	}
	
	gyro_offset.x = tmp_x/100;
	gyro_offset.y = tmp_y/100;
	gyro_offset.z = tmp_z/100;
}



void MPU9250_Init(void)
{
	soft_iic_io_init();

	icm20602_self1_check();
	
	simiic_write_reg(MPU9250_DEV_ADDR, PWR_MGMT_1, 0x00);      // 解除休眠状态
    simiic_write_reg(MPU9250_DEV_ADDR, SMPLRT_DIV, 0x07);      // 采样率, 8kHz / (1 + SMPLRT_DIV)
    simiic_write_reg(MPU9250_DEV_ADDR, CONFIG, 0x02);
	simiic_write_reg(MPU9250_DEV_ADDR, GYRO_CONFIG, 0x10);
    simiic_write_reg(MPU9250_DEV_ADDR, ACCEL_CONFIG, 0x18);    
    simiic_write_reg(MPU9250_DEV_ADDR, ACCEL_CONFIG2, 0x00);    //加速度采样频率460HZ

//    simiic_write_reg(MPU9250_DEV_ADDR, USER_CTRL, 0x00);
//    simiic_write_reg(MPU9250_DEV_ADDR, INT_PIN_CFG, 0x02);
	//----------------------------------------------------------
	
	//上电的时候一定要放平
	MPU_Offset_Init();
	
	Set_Cutoff_Frequency(Imu_Sampling_Freq, 50,&Gyro_Parameter);//姿态角速度反馈滤波参数  50
	Set_Cutoff_Frequency(Imu_Sampling_Freq, 10,&Accel_Parameter);//姿态解算加计修正滤波值 30
}




/**
 * @description: 采集一次陀螺仪和加速度计的值
 */
static void GetData()
{
    uint8_t dat[6];
    

	simiic_read_regs(MPU9250_DEV_ADDR, GYRO_XOUT_H, dat, 6, IIC);  
    gyro.x = (int16_t)((uint16_t)dat[0]<<8 | dat[1]) - gyro_offset.x;
    gyro.y = (int16_t)((uint16_t)dat[2]<<8 | dat[3]) - gyro_offset.y;
    gyro.z = (int16_t)((uint16_t)dat[4]<<8 | dat[5]) - gyro_offset.z;
//    simiic_read_regs(ICM20602_DEV_ADDR, ICM20602_GYRO_XOUT_H, dat, 6, IIC);  
//    gyro.x = (int16_t)((uint16_t)dat[0]<<8 | dat[1]) - gyro_offset.x;
//    gyro.y = (int16_t)((uint16_t)dat[2]<<8 | dat[3]) - gyro_offset.y;
//    gyro.z = (int16_t)((uint16_t)dat[4]<<8 | dat[5]) - gyro_offset.z;

    gyroRaw.x = gyro.x / GYRO_SENSITIVITY;// 翻滚角角速度
    gyroRaw.y = gyro.y / GYRO_SENSITIVITY;// 俯仰角角速度
    gyroRaw.z = gyro.z / GYRO_SENSITIVITY;// 偏航角角速度
//	
	simiic_read_regs(MPU9250_DEV_ADDR, ACCEL_XOUT_H, dat, 6, IIC);  
    acc.x = (int16_t)((uint16_t)dat[0]<<8 | dat[1]);
    acc.y = (int16_t)((uint16_t)dat[2]<<8 | dat[3]);
    acc.z = (int16_t)((uint16_t)dat[4]<<8 | dat[5]);
	
//	simiic_read_regs(ICM20602_DEV_ADDR, ICM20602_ACCEL_XOUT_H, dat, 6, IIC);  
//    acc.x = (int16_t)((uint16_t)dat[0]<<8 | dat[1]);
//    acc.y = (int16_t)((uint16_t)dat[2]<<8 | dat[3]);
//    acc.z = (int16_t)((uint16_t)dat[4]<<8 | dat[5]);

    accRaw.x  = (float)acc.x; // 向前的加速度
    accRaw.y  = (float)acc.y; // 左右的加速度
    accRaw.z  = (float)acc.z; // 向下的加速度
	
	
	//滤波
//	gyro_bpf_filter.x=LPButterworth(gyroRaw.x,&gyro_bpf_filter_buf[0],&Bandstop_Filter_Parameter_30_98);
//	gyro_bpf_filter.y=LPButterworth(gyroRaw.y,&gyro_bpf_filter_buf[1],&Bandstop_Filter_Parameter_30_98);
//	gyro_bpf_filter.z=LPButterworth(gyroRaw.z,&gyro_bpf_filter_buf[2],&Bandstop_Filter_Parameter_30_98);
	  
	gyro_filter_bug.x=LPButterworth(gyroRaw.x,&gyro_filter_buf[0],&Bandstop_Filter_Parameter_30_98);
	gyro_filter_bug.y=LPButterworth(gyroRaw.y,&gyro_filter_buf[1],&Bandstop_Filter_Parameter_30_98);
	gyro_filter_bug.z=LPButterworth(gyroRaw.z,&gyro_filter_buf[2],&Bandstop_Filter_Parameter_30_98);
  
	gyro_filter.x=LPButterworth(gyro_filter_bug.x,&gyro_filter_buf_bug[0],&Gyro_Parameter);
	gyro_filter.y=LPButterworth(gyro_filter_bug.y,&gyro_filter_buf_bug[1],&Gyro_Parameter);
	gyro_filter.z=LPButterworth(gyro_filter_bug.z,&gyro_filter_buf_bug[2],&Gyro_Parameter);

	acc_filter_bug.x=LPButterworth(accRaw.x,&accel_filter_buf[0],&Bandstop_Filter_Parameter_30_94);
	acc_filter_bug.y=LPButterworth(accRaw.y,&accel_filter_buf[1],&Bandstop_Filter_Parameter_30_94);
	acc_filter_bug.z=LPButterworth(accRaw.z,&accel_filter_buf[2],&Bandstop_Filter_Parameter_30_94);

  
	acc_filter.x=LPButterworth(acc_filter_bug.x,&accel_filter_buf_bug[0],&Accel_Parameter);
	acc_filter.y=LPButterworth(acc_filter_bug.y,&accel_filter_buf_bug[1],&Accel_Parameter);
	acc_filter.z=LPButterworth(acc_filter_bug.z,&accel_filter_buf_bug[2],&Accel_Parameter);
}

/**
 * @description: 快速开平方求倒 ( Fast inverse square-root )
 * http://en.wikipedia.org/wiki/Fast_inverse_square_root
 */
static float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

/**
 * @description: 计算旋转矩阵
 */
static void imuComputeRotationMatrix()
{
    float q1q1 = AHRS_Param.q1 * AHRS_Param.q1;
    float q2q2 = AHRS_Param.q2 * AHRS_Param.q2;
    float q3q3 = AHRS_Param.q3 * AHRS_Param.q3;
    float q0q1 = AHRS_Param.q0 * AHRS_Param.q1;
    float q0q2 = AHRS_Param.q0 * AHRS_Param.q2;
    float q0q3 = AHRS_Param.q0 * AHRS_Param.q3;
    float q1q2 = AHRS_Param.q1 * AHRS_Param.q2;
    float q1q3 = AHRS_Param.q1 * AHRS_Param.q3;
    float q2q3 = AHRS_Param.q2 * AHRS_Param.q3;

    AHRS_Param.rMat[0][0] = 1.0f - 2.0f * q2q2 - 2.0f * q3q3;
    AHRS_Param.rMat[0][1] = 2.0f * (q1q2 + -q0q3);
    AHRS_Param.rMat[0][2] = 2.0f * (q1q3 - -q0q2);

    AHRS_Param.rMat[1][0] = 2.0f * (q1q2 - -q0q3);
    AHRS_Param.rMat[1][1] = 1.0f - 2.0f * q1q1 - 2.0f * q3q3;
    AHRS_Param.rMat[1][2] = 2.0f * (q2q3 + -q0q1);

    AHRS_Param.rMat[2][0] = 2.0f * (q1q3 + -q0q2);
    AHRS_Param.rMat[2][1] = 2.0f * (q2q3 - -q0q1);
    AHRS_Param.rMat[2][2] = 1.0f - 2.0f * q1q1 - 2.0f * q2q2;
}

/**
 * @description: 四元素求解欧拉角
 * @param {type} 
 * @return: 
 */

#define DEG2RAD     0.017453293f    /* 度转弧度 π/180 */
#define RAD2DEG     57.29578f       /* 弧度转度 180/π */

//https://blog.csdn.net/qq_27114397/article/details/53247023
//分析得挺详细的
//一阶龙格库塔公式里n为什么消失了不太理解
static void imuUpdate(Vector3F gyro_raw, Vector3F acc_raw/*, Vector3F mag_raw*/,float dt)
{
    float normalise;
    float ex, ey, ez;
//	float hx, hy, hz, bx, bz;
//	float wx, wz, wy;
    float halfT = 0.5f * dt;

    gyro_raw.x = gyro_raw.x * DEG2RAD;  /* 度转弧度 */
    gyro_raw.y = gyro_raw.y * DEG2RAD;
    gyro_raw.z = gyro_raw.z * DEG2RAD;
	
    /* 加速度计输出有效时,利用加速度计补偿陀螺仪*/
    if((acc_raw.x != 0.0f) || (acc_raw.y != 0.0f) || (acc_raw.z != 0.0f))
    {
        /*单位化加速计测量值*/
        normalise = invSqrt(acc_raw.x * acc_raw.x + acc_raw.y * acc_raw.y + acc_raw.z * acc_raw.z);
        acc_raw.x *= normalise;
        acc_raw.y *= normalise;
        acc_raw.z *= normalise;
		
		/*单位化磁力计测量值*/
//		normalise = invSqrt(mag_raw.x * mag_raw.x + mag_raw.y * mag_raw.y + mag_raw.z * mag_raw.z);
//		mag_raw.x *= normalise;
//		mag_raw.y *= normalise;
//		mag_raw.z *= normalise;
//		
//		
//		hx = mag_raw.x * AHRS_Param.rMat[0][1] + mag_raw.y*AHRS_Param.rMat[0][1] + mag_raw.z*AHRS_Param[0][2];
//		hy = mag_raw.x * AHRS_Param.rMat[1][1] + mag_raw.y*AHRS_Param.rMat[1][1] + mag_raw.z*AHRS_Param[1][2];
//		hz = mag_raw.x * AHRS_Param.rMat[2][1] + mag_raw.y*AHRS_Param.rMat[2][1] + mag_raw.z*AHRS_Param[2][2];
//		bx = sqrt((hx*hx) + (hy*hy));
//		bz = hz;
//		
//		wx = 
//		wy = 
//		wz = 
		

		/*通过G和旋转矩阵的外积得到载体坐标系下的重力加速度表示*/
        /*加速计读取的方向与重力加速计方向的差值，用向量叉乘计算*/
        ex = (acc_raw.y * AHRS_Param.rMat[2][2] - acc_raw.z * AHRS_Param.rMat[2][1]);
        ey = (acc_raw.z * AHRS_Param.rMat[2][0] - acc_raw.x * AHRS_Param.rMat[2][2]);
        ez = (acc_raw.x * AHRS_Param.rMat[2][1] - acc_raw.y * AHRS_Param.rMat[2][0]);

        /*误差累计，与积分常数相乘*/
        AHRS_Param.exInt += AHRS_Param.ki * ex * dt ;
        AHRS_Param.eyInt += AHRS_Param.ki * ey * dt ;
        AHRS_Param.ezInt += AHRS_Param.ki * ez * dt ;

        /*用叉积误差来做PI修正陀螺零偏，即抵消陀螺读数中的偏移量*/
        gyro_raw.x += AHRS_Param.kp * ex + AHRS_Param.exInt;
        gyro_raw.y += AHRS_Param.kp * ey + AHRS_Param.eyInt;
        gyro_raw.z += AHRS_Param.kp * ez + AHRS_Param.ezInt;
    }
	
	//一阶龙格库塔公式的四元数更新公式
    float q0Last = AHRS_Param.q0;
    float q1Last = AHRS_Param.q1;
    float q2Last = AHRS_Param.q2;
    float q3Last = AHRS_Param.q3;
    AHRS_Param.q0 += (-q1Last * gyro_raw.x - q2Last * gyro_raw.y - q3Last * gyro_raw.z) * halfT;
    AHRS_Param.q1 += ( q0Last * gyro_raw.x + q2Last * gyro_raw.z - q3Last * gyro_raw.y) * halfT;
    AHRS_Param.q2 += ( q0Last * gyro_raw.y - q1Last * gyro_raw.z + q3Last * gyro_raw.x) * halfT;
    AHRS_Param.q3 += ( q0Last * gyro_raw.z + q1Last * gyro_raw.y - q2Last * gyro_raw.x) * halfT;

	
    /*单位化四元数*/
    normalise = invSqrt(AHRS_Param.q0 * AHRS_Param.q0 + AHRS_Param.q1 * AHRS_Param.q1 + AHRS_Param.q2 * AHRS_Param.q2 + AHRS_Param.q3 * AHRS_Param.q3);
    AHRS_Param.q0 *= normalise;
    AHRS_Param.q1 *= normalise;
    AHRS_Param.q2 *= normalise;
    AHRS_Param.q3 *= normalise;


}



void MPU9250_GetInfo(void)
{
	GetData();

	//imuUpdate(gyroRaw, accRaw, MPU_TIM_COUNT_CYCLE);
	
    imuUpdate(gyro_filter, acc_filter, MPU_TIM_COUNT_CYCLE);
	
	imuComputeRotationMatrix();  /*计算旋转矩阵*/

    /*计算roll pitch yaw 欧拉角*/
    Pitch = asinf(AHRS_Param.rMat[2][0]) * RAD2DEG;
    Roll  = atan2f(AHRS_Param.rMat[2][1], AHRS_Param.rMat[2][2]) * RAD2DEG;
    Yaw   = atan2f(AHRS_Param.rMat[1][0], AHRS_Param.rMat[0][0]) * RAD2DEG;
}
