/*--------------------------------------------------------------*/
/*							ͷ�ļ�����							*/
/*==============================================================*/
#include "data.h"
/*--------------------------------------------------------------*/
/*							  �궨��							*/
/*==============================================================*/
#define u8 unsigned char                                    //8λ����
#define u16 unsigned short                                  //16λ����
#define u32 unsigned int                                    //32λ����

/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/
/*----------------------*/
/*	    ����CCDģ��		*/
/*======================*/
unsigned cjug_sta;
/*----------------------*/
/*	 	 MOTORģ��		*/
/*======================*/
//	������
short lcod = 0,rcod = 0;
short spd = 90;
short rad = 0;
//	PID
struct pidpara speed;
struct pidpara steer;
struct pidpara angle;
struct pidpara acw;
struct pidpara lmor;
struct pidpara rmor;
/*----------------------*/
/*	   �Ƕȿ���ģ��		*/
/*======================*/
short blcp = 23;
short pita;
short yawa[2];
short gy, gz;
short yfilt[4] = {0, 0, 0, 0};
short pflit[4] = {0, 0, 0, 0};
/*----------------------*/
/*	 	 �˵�ģ��		*/
/*======================*/
//	��������
unsigned char nom[128];
//	һ���˵�
unsigned char menu[ROWS];
unsigned char menu_level;
unsigned char menu_index = 0;
//	�����˵�
unsigned char menu2_index = 0;
unsigned char menu2_level = 0;
//	��־λ
unsigned char fixedflag = 0;//�̶���ʾ
unsigned char monitorflag = 0;//������
unsigned char csimenu_flag[CSIMENU_FLAG] = {0, 0};//����ͷ
unsigned char wireless_flag[WIRELESS_FLAG] = {0, 0};//��������
//	����ָ��
void(*menu_pfc[])(unsigned char) = {menu_select, menu2_select};
/*----------------------*/
/*	 	 ͨ��ģ��		*/
/*======================*/
unsigned char buff_get6, buff_get7;
/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/
/*----------------------*/
/*	 	������ʼ��		*/
/*======================*/
//	PID
void Init_para(void){
//	����ת��
	lmor.alpha = 0.15;
	lmor.Kp = 30;
	lmor.Ki = 0.45;
	lmor.Kd = 0;
	lmor.I = 0;
	lmor.e1 = 0, lmor.e2 = 0, lmor.e3 = 0;
	lmor.rs = 0;
//	����ת��
	rmor.alpha = 0.15;
	rmor.Kp = 30;
	rmor.Ki = 0.45;
	rmor.Kd = 0;
	rmor.I = 0;
	rmor.e1 = 0, rmor.e2 = 0, rmor.e3 = 0;
	rmor.rs = 0;	
//	ת��
	steer.alpha = 0.1;
	steer.Kp = 24;//��Ӧ���� | ����
	steer.Ki = 0;//��Ӧ����
	steer.Kd = 6.4;
	steer.I = 0;
	steer.e1 = 0, steer.e2 = 0, steer.e3 = 0;
	steer.rs = 0;
}
