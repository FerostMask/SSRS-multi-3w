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
    if(fork_turn_direction){
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
			if(exti_lefcount) p_target[1] = (lvet_trafpoint_col[0]+rigbor[lvet_trafpoint_row[0]])>>1;
			else p_target[1] = 80;
			spd = speed.ring[0];
			folc_flag = 0;
			break;
		case 22://�뻷��
			p_target[1] = ((leftop_cut+lefbottom_cut)>>1)+(100>>(spd>>4));
			spd = speed.ring[1];
			folc_flag = 0;
			break;
		case 23://����
			folrow_f = 40;
			spd = speed.ring[2];
			break;
		case 24://����
			mp = ((((99+rcut)>>1)+rcut)>>1);
			slope_temp = (float)(rcut-found_point[2])/found_point[3];
			p_target[0] = mp, p_target[1] = ((float)(rcut-mp)/slope_temp)+42;
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
	spd = speed.bend[0];
	switch(act_flag){
		case 11://��ת����
			folrow_f = 47;
			break;
		case 12://��ת����
			folrow_f = 47;
			break;
		case 13://����
			folrow_f = 36;
			break;
		case 14://����
			folrow_f = 36;
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
