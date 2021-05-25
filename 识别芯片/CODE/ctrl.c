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
	register unsigned char i;
	unsigned char mp;
	short bias_temp = 0;
	unsigned char bend_bias[] = {22, 26, 16, 10, 6, 4, 2, 8};
//	����
	switch(act_flag){
		case 21://������
			p_target[1] = (lefbor[90]+rigbor[90])>>1;
			p_target[1] = (p_target[1]+77)>>1;
			spd = speed.ring[0];
			folc_flag = 0;
			break;
		case 22://�뻷��
			p_target[1] = ((leftop_cut+lefbottom_cut)>>1);
			p_target[1] = ((p_target[1]+85)>>1);
			rad_min = 159, rad_max = 0;
			spd = speed.ring[1];
			folc_flag = 0;
			break;
		case 23://����
		//	��Ȩ�㷨
			for(i = 0; i < 8; i++)
				bias_temp += (lefbor[30+(i<<2)]+rigbor[30+(i<<2)] - 160)*bend_bias[i];
			p_target[1] = (bias_temp>>7)+80;
			folc_flag = 0;
			if(p_target[1] < rad_min) rad_min = p_target[1];
			if(p_target[1] > rad_max) rad_max = p_target[1];
			spd = speed.ring[2];
			break;
		case 24://����
			p_target[1] = (rad_min+rad_max)>>1;
			p_target[1] = ((p_target[1]+55)>>1);
			spd = speed.ring[3];
			folc_flag = 0;
			break;
	//	�һ�
		case 26://������
			p_target[1] = 81;
			folc_flag = 0;
			break;
		case 27://�뻷��
			p_target[1] = ((rigtop_cut+rigbottom_cut)>>1);
			p_target[1] = ((p_target[1]+40)>>1);
			rad_min = 159, rad_max = 0;
			folc_flag = 0;
			break;
		case 28://����
			folrow_f = 63;
			if(p_target[1] < rad_min) rad_min = p_target[1];
			if(p_target[1] > rad_max) rad_max = p_target[1];
			break;
		case 29://����
			p_target[1] = (rad_min+rad_max)>>1;
			p_target[1] = ((p_target[1]+90)>>1);
			folc_flag = 0;
			break;
	}
}
/*------------------------------*/
/*		   �������ģ�� 		*/
/*==============================*/
void cam_ctrl_bend(void){
//	��������
	register unsigned char i;
	short bias_temp = 0;
	unsigned char bend_bias[] = {22, 26, 16, 10, 6, 4, 2, 8};
//	��Ȩ�㷨
	for(i = 0; i < 8; i++)
		bias_temp += (lefbor[30+(i<<2)]+rigbor[30+(i<<2)] - 160)*bend_bias[i];
	p_target[1] = (bias_temp>>7)+80;
	folc_flag = 0;
//	����
	switch(act_flag){
		case 11://��ת����
			folrow_f = 43;
			spd = speed.bend[0];
			break;
		case 12://��ת����
			folrow_f = 43;
			spd = speed.bend[0];
			break;
		case 13://����
			folrow_f = 43;
			spd = speed.bend[1];
			break;
		case 14://����
			folrow_f = 43;
			spd = speed.bend[1];
			break;
	}
}
/*------------------------------*/
/*		   ֱ������ģ�� 		*/
/*==============================*/
void cam_ctrl_direct(void){
//	��������
	register unsigned char i;
	unsigned char point_temp;
	short bias_temp;
	unsigned char bend_bias[] = {22, 26, 16, 10, 6, 4, 2, 8};
//	����
	spd = speed.direct;
	if(exti_lefcount > 0)
		if(!exti_rigcount)
			spd = 40;
	if(exti_rigcount > 0)
		if(!exti_lefcount)
			spd = 40;
//	��Ȩ�㷨
	for(i = 0; i < 8; i++)
		bias_temp += (lefbor[30+(i<<2)]+rigbor[30+(i<<2)] - 160)*bend_bias[i];
	p_target[1] = (bias_temp>>7)+80;
	folc_flag = 0;
}
