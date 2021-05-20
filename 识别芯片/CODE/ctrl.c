/*--------------------------------------------------------------*/
/*							头文件加载							*/
/*==============================================================*/
#include "ctrl.h"
#include "data.h"
#include "pid.h"
/*--------------------------------------------------------------*/
/* 							 变量定义 							*/
/*==============================================================*/

/*--------------------------------------------------------------*/
/* 							 函数定义 							*/
/*==============================================================*/
/*------------------------------*/
/*		   岔道控制模块 		*/
/*==============================*/
void cam_ctrl_fork(void){
//	控制
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
/*		   十字控制模块 		*/
/*==============================*/
void cam_ctrl_cross(void){
//	控制

}
/*------------------------------*/
/*		   环道控制模块 		*/
/*==============================*/
void cam_ctrl_ring(void){
//	变量定义
	unsigned char mp;
	float slope_temp;
//	控制
	switch(act_flag){
		case 21://出环口
			p_target[1] = (lefbor[90]+rigbor[90])>>1;
			p_target[1] = (p_target[1]+80)>>1;
			spd = speed.ring[0];
			folc_flag = 0;
			break;
		case 22://入环口
			p_target[1] = ((leftop_cut+lefbottom_cut)>>1)+(100>>(spd>>4))+15;
			spd = speed.ring[1];
			folc_flag = 0;
			break;
		case 23://环内
			folrow_f = 50;
			rad_temp = p_target[1];
			spd = speed.ring[2];
			break;
		case 24://出环
			p_target[1] = rad_temp;
			spd = speed.ring[3];
			folc_flag = 0;
			break;
	}
}
/*------------------------------*/
/*		   弯道控制模块 		*/
/*==============================*/
void cam_ctrl_bend(void){
//	变量定义
	unsigned char mp;
	float slope_temp;
//	控制
	switch(act_flag){
		case 11://左转丢边
			folrow_f = 55;
			spd = speed.bend[0];
			break;
		case 12://右转丢边
			folrow_f = 55;
			spd = speed.bend[0];
			break;
		case 13://左弯
			folrow_f = 50;
			spd = speed.bend[1];
			break;
		case 14://右弯
			folrow_f = 50;
			spd = speed.bend[1];
			break;
	}
}
/*------------------------------*/
/*		   直道控制模块 		*/
/*==============================*/
void cam_ctrl_direct(void){
//	变量定义
	unsigned char point_temp;
//	控制
	spd = speed.direct;
	if(exti_lefcount > 0)
		if(!exti_rigcount)
			spd = 60;
	if(exti_rigcount > 0)
		if(!exti_lefcount)
			spd = 60;
}
