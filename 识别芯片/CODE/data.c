/*--------------------------------------------------------------*/
/*							头文件加载							*/
/*==============================================================*/
#include "data.h"
#include "eident.h"
#include "CAM.h"
#include "ctrl.h"
/*--------------------------------------------------------------*/
/*							  宏定义							*/
/*==============================================================*/
#define u8 unsigned char                                    //8位数据
#define u16 unsigned short                                  //16位数据
#define u32 unsigned int                                    //32位数据
/*--------------------------------------------------------------*/
/* 							 变量定义 							*/
/*==============================================================*/
/*----------------------*/
/*	 	摄像头模块		*/
/*======================*/
//	大津法二值化
float P[256] = {0}, PK[256] = {0}, MK[256] = {0};
unsigned char binary_img[MT9V03X_H][(MT9V03X_W-4)/8];
unsigned char img_thrsod;
//	基准点寻找
unsigned char found_point[4];
unsigned char fop_flag;
//	水平边线寻找
unsigned char lefbor[MT9V03X_H], rigbor[MT9V03X_H], mid_point[MT9V03X_H];
unsigned char ltraf_point_row[10], rtraf_point_row[10], ltraf_point_col[10], rtraf_point_col[10];
unsigned char ltraf_flag[10], rtraf_flag[10];
unsigned char ltraf_count, rtraf_count;
unsigned char lcut, rcut;
//	垂直边线寻找
unsigned char topbor[MT9V03X_W-4], bottombor[MT9V03X_W-4];//上下边线
unsigned char lvet_trafpoint_row[5], rvet_trafpoint_row[5], lvet_trafpoint_col[5], rvet_trafpoint_col[5];//转变点
unsigned char lvet_trafcount, rvet_trafcount;//转变点计数
unsigned char leftop_cut, lefbottom_cut, rigtop_cut, rigbottom_cut;//截止列
unsigned char exti_lefp[4], exti_rigp[4], exti_lefcount, exti_rigcount;//出口
//	状态机
unsigned char act_flag, act_flag_temp, fragile_flag;
unsigned char state, state_temp;
unsigned char state_flag;
unsigned short img_color = 0xAE9C;
void(*state_pfc[])(void) = {state_machine_enter, state_machine_bend, state_machine_ring, state_machine_cross, state_machine_fork};
//  岔道相关
unsigned char fork_turn_direction; //0 左 1 右
/*----------------------*/
/*	 	 控制模块		*/
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
/*	 	 菜单模块		*/
/*======================*/
//	汉字数组
unsigned char nom[128];
//	一级菜单
unsigned char menu[ROWS] = {0, 0, 0, 0};
unsigned char menu_level;
unsigned char menu_index = 0;
//	二级菜单
unsigned char menu2_index = 0;
unsigned char menu2_level = 0;
//	标志位
unsigned char excollflag = 0;//电磁极值采集标志位
unsigned char fixedflag = 0;//固定显示
unsigned char monitorflag = 0;//监视器
unsigned char csimenu_flag[CSIMENU_FLAG] = {0, 0};//摄像头
unsigned char wireless_flag[WIRELESS_FLAG] = {0, 0};//无线数据
//	指针函数
void(*menu_pfc[])(unsigned char) = {menu_select, menu2_select};
/*----------------------*/
/*	 	 有来有去		*/
/*======================*/
unsigned char subuff_num = 0;
unsigned char subuff_arr[3];
unsigned short subuff_ranging;
/*----------------------*/
/*	 	 串口通信		*/
/*======================*/
unsigned char yawa_flag;
short yawa;
char buff_get6, buff_get7;
/*----------------------*/
/*	 	 电磁模块		*/
/*======================*/
//	结构体定义
struct adcpara adc2;
/*--------------------------------------------------------------*/
/* 							 函数定义 							*/
/*==============================================================*/
/*----------------------*/
/*	 	参数初始化		*/
/*======================*/
void Init_para(void){
//	速度控制
	speed.direct = 40;
	speed.bend[0] = 40, speed.bend[1] = 40;
	speed.ring[0] = 40, speed.ring[1] = 40;
	speed.ring[2] = 40, speed.ring[3] = 40, speed.ring[4] = 40;
	speed.cross = 40;
	speed.fork = 40;
//	CAM转向
	cam_steering.Kp = 0.9;
	cam_steering.Kd = 1.1;	
//	电磁模块
	adc2.max = 4095, adc2.min = 0;

}
