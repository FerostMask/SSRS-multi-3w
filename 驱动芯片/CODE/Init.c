/*--------------------------------------------------------------*/
/*							ͷ�ļ�����							*/
/*==============================================================*/
#include "menu.h"
#include "Init.h"
#include "logo.h"
#include "zf_pwm.h"
#include "zf_tim.h"
#include "zf_gpio.h"
#include "zf_exti.h"
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

/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/
/*------------------------------*/
/*		 ��������ʼ��ģ��		*/
/*==============================*/
void Init_encoder(void){
//	���������ų�ʼ��	
	tim_encoder_init(TIM_3, TIM_3_ENC1_B04, TIM_3_ENC2_B05);
	tim_encoder_init(TIM_4, TIM_4_ENC1_B06, TIM_4_ENC2_B07);
}
/*------------------------------*/
/*		  �����ʼ��ģ��		*/
/*==============================*/
void Init_motor(void){
//	�˿ڳ�ʼ��
//	������ų�ʼ�� Ĭ��Ƶ��17kHz ��ʼռ�ձ� 0%
	pwm_init(TIM_5, MOTOR_R1, 17000, 0);
	pwm_init(TIM_5, MOTOR_R0, 17000, 0);
	pwm_init(TIM_5, MOTOR_L1, 17000, 0);
	pwm_init(TIM_5, MOTOR_L0, 17000, 0);
//	PID������ʼ��
	Init_para();
}
/*------------------------------*/
/*		 ����ѡ��ģ��ģ��		*/
/*==============================*/
char core_select(void){
	gpio_init(D0, GPI, GPIO_LOW, GPI_FLOATING_IN);
	gpio_init(D2, GPI, GPIO_LOW, GPI_FLOATING_IN);
	ips200_showstr(20, 7, "Press Button to Continue ->");
	while(1){
		if(!gpio_get(D0)) return 1;
		if(!gpio_get(D2)){ 
			ips200_clear(WHITE);
			logo_display200();
			return 0;
		}
	}
}
/*------------------------------*/
/*		  ������ʼ��ģ��		*/
/*==============================*/
void Init_button(void){
//	�����жϳ�ʼ�� | ������ȼ�
	exti_interrupt_init(KEY1, EXTI_Trigger_Falling, 3, 3);
	exti_interrupt_init(KEY2, EXTI_Trigger_Falling, 3, 3);
	exti_interrupt_init(KEY3, EXTI_Trigger_Falling, 3, 3);
	exti_interrupt_init(KEY4, EXTI_Trigger_Falling, 3, 3);
	exti_interrupt_init(KEY5, EXTI_Trigger_Falling, 3, 3);
	exti_interrupt_init(KEY6, EXTI_Trigger_Falling, 3, 3);
}
