/*--------------------------------------------------------------*/
/*							ͷ�ļ�����							*/
/*==============================================================*/
#include "data.h"
#include "eident.h"
#include "CAM.h"
#include "ctrl.h"
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
/*	 	����ͷģ��		*/
/*======================*/
//	��򷨶�ֵ��
float P[256] = {0}, PK[256] = {0}, MK[256] = {0};
unsigned char binary_img[MT9V03X_H][(MT9V03X_W-4)/8];
unsigned char img_thrsod;
//	��׼��Ѱ��
unsigned char found_point[4];
unsigned char fop_flag;
//	ˮƽ����Ѱ��
unsigned char lefbor[MT9V03X_H], rigbor[MT9V03X_H], mid_point[MT9V03X_H];
unsigned char ltraf_point_row[10], rtraf_point_row[10], ltraf_point_col[10], rtraf_point_col[10];
unsigned char ltraf_flag[10], rtraf_flag[10];
unsigned char ltraf_count, rtraf_count;
unsigned char lcut, rcut;
//	��ֱ����Ѱ��
unsigned char topbor[MT9V03X_W-4], bottombor[MT9V03X_W-4];//���±���
unsigned char lvet_trafpoint_row[5], rvet_trafpoint_row[5], lvet_trafpoint_col[5], rvet_trafpoint_col[5];//ת���
unsigned char lvet_trafcount, rvet_trafcount;//ת������
unsigned char leftop_cut, lefbottom_cut, rigtop_cut, rigbottom_cut;//��ֹ��
unsigned char exti_lefp[4], exti_rigp[4], exti_lefcount, exti_rigcount;//����
//	״̬��
unsigned char act_flag, act_flag_temp, fragile_flag;
unsigned char state, state_temp;
unsigned char state_flag;
unsigned short img_color = 0xAE9C;
void(*state_pfc[])(void) = {state_machine_enter, state_machine_bend, state_machine_ring, state_machine_cross, state_machine_fork};
//  ������
unsigned char fork_turn_direction; //0 �� 1 ��
/*----------------------*/
/*	 	 ����ģ��		*/
/*======================*/
struct spdpara speed;
struct pidpara cam_steering;
unsigned short cut_fork, cut_fork_bottom;
short p_target[2];
short error_flit[8], ctrl_error1, ctrl_error2;
short spd_slow;
short spd, rad;
unsigned char folrow_f = 63;
char folc_flag, cooling_flag;
void(*ctrl_pfc[])(void) = {cam_ctrl_direct, cam_ctrl_bend, cam_ctrl_ring, cam_ctrl_cross, cam_ctrl_fork};
/*----------------------*/
/*	 	 �˵�ģ��		*/
/*======================*/
//	��������
unsigned char nom[128];
//	һ���˵�
unsigned char menu[ROWS] = {0, 0, 0, 0};
unsigned char menu_level;
unsigned char menu_index = 0;
//	�����˵�
unsigned char menu2_index = 0;
unsigned char menu2_level = 0;
//	��־λ
unsigned char excollflag = 0;//��ż�ֵ�ɼ���־λ
unsigned char fixedflag = 0;//�̶���ʾ
unsigned char monitorflag = 0;//������
unsigned char csimenu_flag[CSIMENU_FLAG] = {0, 0};//����ͷ
unsigned char wireless_flag[WIRELESS_FLAG] = {0, 0};//��������
//	ָ�뺯��
void(*menu_pfc[])(unsigned char) = {menu_select, menu2_select};
/*----------------------*/
/*	 	 ������ȥ		*/
/*======================*/
unsigned char subuff_num = 0;
unsigned char subuff_arr[3];
unsigned short subuff_ranging;
/*----------------------*/
/*	 	 ����ͨ��		*/
/*======================*/
unsigned char yawa_flag;
short yawa;
char buff_get6, buff_get7;
/*----------------------*/
/*	 	 ���ģ��		*/
/*======================*/
//	�ṹ�嶨��
struct adcpara adc2;
/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/
/*----------------------*/
/*	 	������ʼ��		*/
/*======================*/
void Init_para(void){
//	�ٶȿ���
	speed.direct = 40;
	speed.bend[0] = 40, speed.bend[1] = 40;
	speed.ring[0] = 40, speed.ring[1] = 40;
	speed.ring[2] = 40, speed.ring[3] = 40, speed.ring[4] = 40;
	speed.cross = 40;
	speed.fork = 40;
//	CAMת��
	cam_steering.Kp = 0.9;
	cam_steering.Kd = 1.1;	
//	���ģ��
	adc2.max = 4095, adc2.min = 0;

}
