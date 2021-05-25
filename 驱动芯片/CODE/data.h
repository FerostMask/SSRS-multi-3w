#ifndef _DATA_H
#define _DATA_H
#include "menu.h"
#include "menu_2.h"
/*--------------------------------------------------------------*/
/*							  �궨��							*/
/*==============================================================*/
//	������־λ
#define CSIMENU_FLAG 2
#define WIRELESS_FLAG 2
//	һ���˵�
#define ROWS 4
#define menu_limit0 1
#define menu_limit1 3
#define menu_limit2 0
#define menu_limit3 0
/*------------------------------------------------------*/
/* 					  �ⲿ�������� 						*/
/*======================================================*/
/*----------------------*/
/*	    ����CCDģ��		*/
/*======================*/
extern unsigned cjug_sta;
/*----------------------*/
/*	 	 MOTORģ��		*/
/*======================*/
//	������
extern short lcod, rcod;			//����������
extern short ltcod, rtcod;          //������Ŀ��ֵ
//	���
extern short spd;
extern short rad;
/*----------------------*/
/*	 	  PIDģ��		*/
/*======================*/
//	�ṹ������
typedef struct pidpara{
	float alpha;
	float Kp;
	float Ki;
	float Kd;
	float I;
	int e1;
	int e2;
	int e3;
	short rs;
}pidpara;

//	���
extern struct pidpara speed;
extern struct pidpara steer;
extern struct pidpara lmor;
extern struct pidpara rmor;
//	�Ƕȵ�������
extern struct pidpara angle;
//	���ٶ�
extern struct pidpara acw;

/*----------------------*/
/*	   �Ƕȿ���ģ��		*/
/*======================*/
extern short blcp;
extern short pita;
extern short yawa[2];
extern short gy, gz;
extern short yfilt[4];
extern short pflit[4];

/*----------------------*/
/*	 	 �˵�ģ��		*/
/*======================*/
//	һ���˵�
extern unsigned char menu_index;
extern unsigned char menu[ROWS];
extern unsigned char nom[128];
extern unsigned char fixedflag;
extern unsigned char monitorflag;
extern unsigned char csimenu_flag[CSIMENU_FLAG];
extern unsigned char wireless_flag[WIRELESS_FLAG];
extern unsigned char menu_level;
//	�����˵�
extern unsigned char menu2_index;
extern unsigned char menu2_level;
//	����ָ��
extern void(*menu_pfc[])(unsigned char);
/*----------------------*/
/*	 	 ͨ��ģ��		*/
/*======================*/
extern unsigned char buff_get6, buff_get7;
extern unsigned char subuff_num;
extern unsigned char subuff_arr[3];
/*------------------------------------------------------*/ 
/* 						�������� 						*/
/*======================================================*/
void Init_para(void);

#endif
