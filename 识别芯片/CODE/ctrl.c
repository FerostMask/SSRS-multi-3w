/*--------------------------------------------------------------*/
/*							ͷ�ļ�����							*/
/*==============================================================*/
#include "ctrl.h"
#include "data.h"
#include "pid.h"
/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/

/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/
/*------------------------------*/
/*		   �������ģ�� 		*/
/*==============================*/
void cam_ctrl_fork(void){
//	����
    p_target[0] = topbor[cut_fork_bottom];
    if(direction_fork){
        p_target[1] = (MT9V03X_W + cut_fork_bottom) >> 1;
    }else{
        p_target[1] = cut_fork_bottom >> 1;
    }
	spd = speed.fork;
	folc_flag = 0;
}
/*------------------------------*/
/*		   ʮ�ֿ���ģ�� 		*/
/*==============================*/
void cam_ctrl_cross(void){
//	����

}
/*------------------------------*/
/*		   ��������ģ�� 		*/
/*==============================*/
void cam_ctrl_ring(void){
//	��������
	unsigned char mp;
	float slope_temp;
//	����
	switch(act_flag){
		case 21://������
			p_target[1] = (lefbor[90]+rigbor[90])>>1;
			p_target[1] = (p_target[1]+80)>>1;
			spd = speed.ring[0];
			folc_flag = 0;
			break;
		case 22://�뻷��
			p_target[1] = ((leftop_cut+lefbottom_cut)>>1)+(100>>(spd>>4))+15;
			spd = speed.ring[1];
			folc_flag = 0;
			break;
		case 23://����
			folrow_f = 50;
			rad_temp = p_target[1];
			spd = speed.ring[2];
			break;
		case 24://����
			p_target[1] = rad_temp;
			spd = speed.ring[3];
			folc_flag = 0;
			break;
	}
}
/*------------------------------*/
/*		   �������ģ�� 		*/
/*==============================*/
void cam_ctrl_bend(void){
//	��������
	unsigned char mp;
	float slope_temp;
//	����
	switch(act_flag){
		case 11://��ת����
			folrow_f = 55;
			spd = speed.bend[0];
			break;
		case 12://��ת����
			folrow_f = 55;
			spd = speed.bend[0];
			break;
		case 13://����
			folrow_f = 50;
			spd = speed.bend[1];
			break;
		case 14://����
			folrow_f = 50;
			spd = speed.bend[1];
			break;
	}
}
/*------------------------------*/
/*		   ֱ������ģ�� 		*/
/*==============================*/
void cam_ctrl_direct(void){
//	��������
	unsigned char point_temp;
//	����
	spd = speed.direct;
	if(exti_lefcount > 0)
		if(!exti_rigcount)
			spd = 60;
	if(exti_rigcount > 0)
		if(!exti_lefcount)
			spd = 60;
}
