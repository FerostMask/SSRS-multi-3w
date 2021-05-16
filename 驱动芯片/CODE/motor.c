/*--------------------------------------------------------------*/
/*							ͷ�ļ�����							*/
/*==============================================================*/
#include "motor.h"
#include "pid.h"
#include "Init.h"
#include "zf_pwm.h"
#include "zf_tim.h"
#include "SEEKFREE_VIRSCO.h"
#include "SEEKFREE_WIRELESS.h"
#include "SEEKFREE_ICM20602.h"
#include "SEEKFREE_IPS200_PARALLEL8.h"
/*--------------------------------------------------------------*/
/*							  �궨��							*/
/*==============================================================*/
#define u8 unsigned char                                    //8λ����
#define u16 unsigned short                                  //16λ����
#define u32 unsigned int                                    //32λ����
/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/
//unsigned char motor_temp[4] = {0, 0, 0, 0};
/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/
/*------------------------------*/
/*	     ������ֵ��ȡģ��		*/
/*==============================*/
void encoder_get(void){
//	��ȡ����������ֵ������
	lcod = -tim_encoder_get_count(TIM_3);
	rcod = tim_encoder_get_count(TIM_4);
//	��ʾ����ֵ
//	ips200_showint16(0, 0, lcod);
//	ips200_showint16(0, 1, rcod);
}
/*------------------------------*/
/*		   �������ģ��			*/
/*==============================*/
void motor_act(void){
//	��������
	static unsigned char motor_count;
	motor_count = (motor_count+1)&0x01;
//	���ټ���
	if(motor_count){
		gz = icm_gyro_z/(16.4*5.73);
		pos_pid(&steer, rad, gz, 80, -80);
	}
//	ת�ٿ���
	inc_pid(&lmor, spd+steer.rs, lcod, 5000);
	inc_pid(&rmor, spd-steer.rs, rcod, 5000);	
//	����ռ�ձ�����
	if(lmor.rs >= 0){	//��ת
		pwm_duty_updata(TIM_5, MOTOR_L1, lmor.rs);     		
		pwm_duty_updata(TIM_5, MOTOR_L0, 0);                     		
	}
	else{	//��ת
		pwm_duty_updata(TIM_5, MOTOR_L1, 0);                      		
		pwm_duty_updata(TIM_5, MOTOR_L0, -lmor.rs);    		
	}	
//	�ҵ��ռ�ձ�����
	if(rmor.rs >= 0){	//��ת	
		pwm_duty_updata(TIM_5, MOTOR_R1, rmor.rs);     		
		pwm_duty_updata(TIM_5, MOTOR_R0, 0);                      		
	}
	else{	//��ת
		pwm_duty_updata(TIM_5, MOTOR_R1, 0);                      		
		pwm_duty_updata(TIM_5, MOTOR_R0, -rmor.rs);    		
	}
//	���ݷ���
//	data_conversion(lcod, rcod, spd, 0, motor_temp);
}
