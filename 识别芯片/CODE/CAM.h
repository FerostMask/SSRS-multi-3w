/*------------------------------------------------------*/
/* 					   ͷ�ļ����� 						*/
/*======================================================*/
#ifndef _CAM_H
#define _CAM_H
/*------------------------------------------------------*/
/*						 �궨��							*/
/*======================================================*/
#define grayscale 256
/*------------------------------------------------------*/ 
/* 						�������� 						*/
/*======================================================*/
void state_machine(void);
void state_machine_fork(void);
void state_machine_cross(void);
void state_machine_ring(void);
void state_machine_bend(void);
void state_machine_enter(void);
char slope_cal(char num);
void vert_slope_cal(char num);
void vetsearch_fork_support(void);
void vertsearch_frok(void);
void border_vertical_leftsearch(void);
void border_vertical_rightsearch(void);
void binary_disp(void);
void lbor_search(void);
void rbor_search(void);
void left_fop_search(void);
void right_fop_search(void);
void img_binary(void);
void otsu(void);
#endif
