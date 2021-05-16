/*--------------------------------------------------------------*/
/*							ͷ�ļ�����							*/
/*==============================================================*/
#include "IMU.h"
#include "pid.h"
#include "math.h"
#include "motor.h"
#include "SEEKFREE_VIRSCO.h"
#include "SEEKFREE_TSL1401.h"
#include "SEEKFREE_WIRELESS.h"
#include "SEEKFREE_ICM20602.h"
#include "SEEKFREE_IPS200_PARALLEL8.h"
/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/
//unsigned char temp[4] = {0, 0, 0, 0};
/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/
/*----------------------*/
/*	   �Ƕȿ���ģ��		*/
/*======================*/
void angle_ctrl(void){
//	��������
	register char i ;
	static unsigned char imu_count;
//	������ѭ��
	imu_count = (imu_count+1)&3;
//	���ٶ��˲�
	for(i = 2; i >= 0; i--) yfilt[i+1] = yfilt[i];
	yfilt[0] = icm_gyro_y;
	gy = (yfilt[0] + yfilt[1] + yfilt[2] + yfilt[3])/65.6;
//	����PID
	if(imu_count == 3){
	//	ƫת���ٶȡ��ٶ�PID
		gz = icm_gyro_z/(16.4*5.73);
		pos_pid(&steer, rad, gz, 1200, -1200);
		inc_pid(&speed, spd, (lcod+rcod)>>1, 50);
	}
//	�Ƕ�
	if(imu_count == 1 || imu_count == 3){
	//	���㸩���ǡ�������ٶ�	
		for(i = 2; i >= 0; i--) pflit[i+1] = pflit[i];
	}
//	���ٶȡ��������
	inc_pid(&acw, angle.rs, gy, 5000);
	motor_act();
}
