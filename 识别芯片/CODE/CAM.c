/*--------------------------------------------------------------*/
/*							ͷ�ļ�����							*/
/*==============================================================*/
#include "CAM.h"
#include "pid.h"
#include "data.h"
#include "zf_tim.h"
#include "SEEKFREE_MT9V03X.h"
#include "SEEKFREE_IPS200_PARALLEL8.h"
/*--------------------------------------------------------------*/
/* 							  �궨�� 							*/
/*==============================================================*/
#define uint16 unsigned short
/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/
float show_flvalue[5];
short show_value[5];
short traf_slope[4];
short ave_slope[4];
short line_slope_diff;
unsigned char show_point;
unsigned char ac_flag[4];
unsigned char exti_leftop, exti_rigtop;
unsigned char lefhigh, righigh;
short lef_widrate, lef_toprate, lef_botrate, rig_widrate, rig_toprate, rig_botrate;
/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/
/*------------------------------*/
/*		   ͼ����ʾģ�� 		*/
/*==============================*/
void binary_disp(void){
//	��������
	register unsigned char i, j, k;
	unsigned char column = (MT9V03X_W-4)>>3;
	unsigned char binary_temp;
//	ͼ����ʾ
	ips200_address_set(0,0,MT9V03X_W-5,MT9V03X_H-1);
	for(i = 0; i < MT9V03X_H; i++){
		for(j = 0; j < column; j++){
			for(k = 8; k > 0; k--){
				binary_temp = (binary_img[i][j]>>(k-1)) & 0x01;
				if(binary_temp) ips200_wr_data16(WHITE);
				else ips200_wr_data16(img_color);
			}
		}
	}
//	��ʾ�߽�
	for(i = MT9V03X_H - 1; i > lcut; i--) ips200_drawpoint(lefbor[i], i, 0x00);
	for(i = MT9V03X_H - 1; i > rcut; i--) ips200_drawpoint(rigbor[i], i, 0x00);
//	��ʾ����
	for(i = 0; i < exti_lefcount; i++){
		ips200_drawpoint(0, exti_lefp[i], 0xB20E);
		ips200_drawpoint(1, exti_lefp[i], 0xB20E);
		ips200_drawpoint(2, exti_lefp[i], 0xB20E);
	}
	for(i = 0; i < exti_rigcount; i++){
		ips200_drawpoint(157, exti_rigp[i], 0xB20E);
		ips200_drawpoint(158, exti_rigp[i], 0xB20E);
		ips200_drawpoint(159, exti_rigp[i], 0xB20E);
	}
//	��ʾ��ֱ����
	for(i = 0; i < lefbottom_cut; i++)
		ips200_drawpoint(i, bottombor[i], 0xFD10);
	for(i = 0; i < leftop_cut; i++)
		ips200_drawpoint(i,topbor[i], 0xFD10);
	for(i = 159; i > rigbottom_cut; i--)
		ips200_drawpoint(i, bottombor[i], 0xFD10);
	for(i = 159; i > rigtop_cut; i--)
		ips200_drawpoint(i, topbor[i], 0xFD10);
//	��ʾ�������
	for(i = 17; i < cut_fork; i++)
		ips200_drawpoint(i, topbor[i], 0xA759);
//	��ʾ״̬
	ips200_showstr(160, 0, "state");
	ips200_showint16(160, 1, state);
	ips200_showstr(160, 2, "act_flag");
	ips200_showint16(160, 3, act_flag);
//	��ʾĿ���
	ips200_drawpoint(p_target[1]+2, p_target[0], 0xED2A);
	ips200_drawpoint(p_target[1]-2, p_target[0], 0xED2A);
	ips200_drawpoint(p_target[1], p_target[0], 0xED2A);
	ips200_drawpoint(p_target[1], p_target[0]+2, 0xED2A);
	ips200_drawpoint(p_target[1], p_target[0]-2, 0xED2A);
//	��ʾֵ
//	ips200_showstr(170, 4, "show_value");
//	ips200_showint16(170, 5, show_value[0]);
//	ips200_showint16(170, 6, show_value[1]);
//	ips200_showint16(170, 7, show_value[2]);
//	ips200_showint16(170, 8, show_value[3]);
//	ips200_showint16(170, 9, show_value[4]);
//	��ʾֵ | ������
//	ips200_showstr(0, 12, "show_flvalue");	
//	ips200_showfloat(0, 13, show_flvalue[0], 2, 2);
//	ips200_showfloat(150, 13, show_flvalue[1], 2, 2);
//	ips200_showfloat(0, 14, show_flvalue[2], 2, 2);
//	ips200_showfloat(150, 14, show_flvalue[3], 2, 2);
//	ips200_showfloat(0, 15, show_flvalue[4], 2, 2);
	
//	ips200_drawpoint(0, show_point+1, RED);
//	ips200_drawpoint(0, show_point+2, RED);
//	ips200_drawpoint(0, show_point+1, RED);
//	ips200_drawpoint(0, show_point+2, RED);
}
/*------------------------------*/
/*	    	״̬��ģ��			*/
/*==============================*/
void state_machine(void){
//	��ʼ��
	state_temp = state, state = 0;
	cut_fork = 0;
	show_value[0] = yawa;
	switch(act_flag){
	//	�������
		case 13:
			if(exti_rigcount > 0) {state = 23; return;}
			break;
		case 14:
			if(yawa < -70) {state = 24; return;}
			break;
	}
	vertsearch_frok();
	if(state == 41) return;
//	�����������
	if(!exti_lefcount)
		if(!exti_rigcount){//û�г��� | ֱ�� | ������� | ������
			if(abs(found_point[0]-rcut)<5)
				if(abs(found_point[0] - found_point[2])>45) {state = 11;return;}
			if(abs(found_point[2]-lcut)<5)
				if(abs(found_point[0] - found_point[2])>45) {state = 12;return;}
		}
	if(exti_lefcount)
		if(!exti_rigcount){//ֻ������г��� | ���� | �� | ʮ�ֶ���
		//	û��ֱ������
			if(abs(ltraf_point_row[exti_leftop] - rcut) < 5)
				{state = 13;return;}//����
		//	��ֱ������
			if(lefhigh > 4)//9
				if(!cooling_flag)
					if(lefhigh < 23){//�����ж�
						if(found_point[2] > exti_lefp[0]+10)
							slope_cal(3);
							if(abs(line_slope_diff) < 120){//�ж��ұ���ֱ��
								if(lef_botrate < -260 && lef_toprate < 70)
									if(lef_widrate > 46)
										{state = 22;return;}
							//	δ��⵽�뻷�ڣ��ж��Ƿ�Ϊ������
								lef_toprate = (topbor[0]-topbor[leftop_cut>>1]);//���ñ����洢
								lef_botrate = (topbor[leftop_cut>>1]-topbor[leftop_cut]);
								if(lef_toprate*lef_toprate-lef_botrate*lef_botrate < -24)
									{state = 21;return;}
							}
					}
	}
	if(exti_rigcount)
		if(!exti_lefcount){//ֻ���ұ��г��� | ���� | �һ� | ʮ�ֶ��� | ��������
			if(abs(rtraf_point_row[exti_rigtop] - lcut) < 5)//����
				{state = 14; return;}
		}
	if(exti_lefcount)
		if(exti_rigcount){//���߶��г��� | ʮ�� | ���
		
		}
}
/*------------------------------*/
/*	    	���״̬��			*/
/*==============================*/
void state_machine_fork(void){
	switch(act_flag){
		case 41:
			if(state == 0 && cut_fork_bottom<20 || state != 31)
				act_flag = 0, state_flag = 0, img_color = 0xAE9C;
			break;
	}
}


/*------------------------------*/
/*	    	ʮ��״̬��			*/
/*==============================*/
void state_machine_cross(void){
}
/*------------------------------*/
/*	    	����״̬��			*/
/*==============================*/
void state_machine_ring(void){
	switch(act_flag){
		case 21://������ -> �뻷
			if(state == 22)
				act_flag = 22, img_color = 0x8CF6;
		case 22://�뻷 -> ����
			if(state == 11)
				act_flag = 23, img_color = 0x46D0;
				return;
		case 23://���� -> ����
			if(state == 23)
				act_flag = 24, yawa_flag = 1, yawa = 0, img_color = 0xB6DB;
			return;
		case 24://���� -> ����
			if(state == 24)
				act_flag = 0, yawa_flag = 0, state_flag = 0, img_color = 0xAE9C;
				cooling_flag = 1;
				tim_interrupt_init_ms(TIM_3, 3000, 0, 0);
			return;
	}
}
/*-
/*------------------------------*/
/*	    	���״̬��			*/
/*==============================*/
void state_machine_bend(void){
	switch(act_flag){
		case 11:
			if(state!=11)
				act_flag = 0, state_flag = 0, img_color = 0xAE9C;
			return;
		case 12:
			if(state!=12)
				act_flag = 0, state_flag = 0, img_color = 0xAE9C;
			return;
		case 13:
			if(state!=13)
				act_flag = 0, state_flag = 0, img_color = 0xAE9C;
			return;
		case 14:
			if(state!=14)
				act_flag = 0, state_flag = 0, img_color = 0xAE9C;
			return;
	}
}
/*------------------------------*/
/*	    	״̬�����			*/
/*==============================*/
void state_machine_enter(void){
	switch(state){
	//	������������
		case 11:
			act_flag = 11, state_flag = 1, img_color = 0x6DDD;
			return;
		case 12:
			act_flag = 12, state_flag = 1, img_color = 0x6DDD;
			return;
		case 13:
			act_flag = 13, state_flag = 1, img_color = 0x7EFE;
			return;
		case 14:
			act_flag = 14, state_flag = 1, img_color = 0x7EFE;
			return;
	//	Բ��
		case 21://��⵽Բ������ | ����״̬
			act_flag = 21, state_flag = 2, img_color = 0x0250;
			act_flag_temp = act_flag;
			tim_interrupt_init_ms(TIM_3, 2000, 0, 0);//״̬��ʱ
			return;
		case 22://��⵽Բ�����
			act_flag = 22, state_flag = 2, img_color = 0x8CF6;
			return;
	//	ʮ�֡�ʮ�ֶ���
	//	���
		case 41:
			act_flag = 41, state_flag = 4, img_color = 0xEFBE;
			return;
	}
}
/*------------------------------*/
/*	   	 ����б�ʷ���ģ��		*/
/*==============================*/
char slope_cal(char num){
//	��������
	register char i;
	unsigned char mp[4];
	short slope[2];
	short angle;
//	����б��
	switch(num){
	//	Բ������ | ֱ�����
		case 3:
			mp[0] = ltraf_point_row[exti_leftop];
			mp[2] = mp[0]+found_point[2] >> 1;
			mp[1] = (mp[0]+mp[2]) >> 1, mp[3] = mp[2]>>1;
			slope[0] = (float)(rigbor[mp[0]]-rigbor[mp[2]])/(float)(mp[0]-mp[2])*1000;
			slope[1] = (float)(rigbor[mp[1]]-rigbor[mp[3]])/(float)(mp[1]-mp[3])*1000;
			line_slope_diff = slope[0]-slope[1];
			break;
	}
}
/*------------------------------*/
/*	   ��ֱ���߿�ȷ���ģ��		*/
/*==============================*/
void vert_width_analysis(char num, unsigned char end_set){
//	��������
	register char i;
	char mp[5];
	short width[5], top_bias[4], bottom_bias[4];
	short wic[4]; 
	short topwic[4], bottomwic[4];
//	������ȱ仯
	switch(num){
		case 1://��
			mp[0] = 0, mp[4] = end_set;
			mp[2] = mp[4]>>1;
			mp[1] = mp[2]>>1, mp[3] = (mp[2]+mp[4])>>1;
		//	������ȱ仯
			for(i = 0; i < 5; i++) width[i] = bottombor[mp[i]]-topbor[mp[i]];
			for(i = 0; i < 4; i++){
				wic[i] = width[i+1]*width[i+1]-width[i]*width[i];
				top_bias[i] = topbor[0]*topbor[0]-topbor[mp[i+1]]*topbor[mp[i+1]];
				bottom_bias[i] = bottombor[0]*bottombor[0]-bottombor[mp[i+1]]*bottombor[mp[i+1]];
			}
			lef_widrate = (wic[0]+wic[1]+wic[2]+wic[3])>>2;
			lef_toprate = (top_bias[0]+top_bias[2]+top_bias[3]+top_bias[4])>>2;
			lef_botrate = (bottom_bias[0]+bottom_bias[2]+bottom_bias[3]+bottom_bias[4])>>2;
//			show_value[0] = lef_widrate;
//			show_value[1] = lef_toprate;
//			show_value[2] = lef_botrate;
			break;
		case 2://��
			break;
	}
}
/*------------------------------*/
/*	  	����߽��Ѱ��ģ��		*/
/*==============================*/
void vertsearch_frok(void){
//	��������
	register unsigned char i;
	register char j, k;
	unsigned char col = (MT9V03X_W-4)>>3;//����
	unsigned char *p;
	unsigned char found_flag, view_temp;
	unsigned char bottom_point, bottom_cut;
    unsigned char cnt_left=0, cnt_right=0;//��������б
    uint16 sum_left=0,sum_right=0;
    unsigned char flag[2]={0};
    unsigned char cnt_level_change_points;
    //  б�����
    char x1, x2, y1, y2;
    float k1, k2, lef_k, rig_k, error;    

//	**Ѱ�ұ߽����**
	cut_fork_bottom = 0, bottom_cut = 0;
	p = &binary_img[MT9V03X_H-1][8];
    
	for(i = MT9V03X_H-1; i > 0; i--, p-=col){
		if(*(unsigned long *)p == 0xffffffff) continue;
		bottom_point = i;
		break;
	}
	if(i < 15 || i > 90) return;
//  **������������������**
    cnt_level_change_points = 0;
    p = &binary_img[i-4][2];
    for(j = 2; j < 18; j++,p++){//����
    //	һ�����
        if(*(uint16*)p == 0x0000) continue;//ȫ��
        if(*(uint16*)p == 0xffff) continue;//ȫ��
 		if(*p != 0x00)
			if(*p != 0xff){//��ͻ��
				for(k = 7; k > 0; k--){
//                    if( ( ((*p>>k)&0x01)==1 ) && ( ((*p>>(k-1))&0x01)==0 ) ){flag[0]++;continue;}//�׺�����
                    if((((*p>>k)&0x01)==0)&&(((*p>>(k-1))&0x01)==1)){flag[1]++;continue;}//�ڰ�����
                }
					continue;
			}
		if(*(p+1) != 0x00)
			if(*(p+1) != 0xff){//��ͻ��
				for(k = 7; k > 0; k--){
//                    if((((*(p+1)>>k)&0x01)==1)&&(((*(p+1)>>(k-1))&0x01)==0)){flag[0]++;continue;}//�׺�����
                    if((((*(p+1)>>k)&0x01)==0)&&(((*(p+1)>>(k-1))&0x01)==1)){flag[1]++;continue;}//�ڰ�����
                }
					continue;
			}
    //	�������
//        if(*(uint16*)p == 0x00FF){flag[0]++;continue;}//����Һ�����
		if(*(uint16*)p == 0xFF00){flag[1]++;continue;}//����Ұ�����
    }



	for(j = 2; j < 18; j++){
		found_flag = 0, p = &binary_img[bottom_point][j];
		for(i = bottom_point+2; i > 0; i--, p-=col ){
			view_temp = *(p)^*(p+col);
			for(k = 7; k > -1; k--){
				if(!((found_flag>>k)&0x01))
					if((view_temp>>k)&0x01){       
     
						topbor[(j<<3)+7-k] = i;
                        if(i > bottom_cut) bottom_cut = i, cut_fork_bottom = (j<<3)+7-k;
						if(cut_fork < (j<<3)+7-k) cut_fork =(j<<3)+7-k;
					}
			}
			found_flag |= view_temp;
			if(found_flag == 0xFF) break;
			if(*(p+col) == 0x00) break;
		}
		if(found_flag != 0xFF) break;	
	}
    
    for(i = cut_fork_bottom ; i < 144; i++){//�ұ�
    //  ���С & ������ҵ������
        j = abs(topbor[i+1] - topbor[i]);
        if(j < 3 && (topbor[i+1]  < topbor[i]) ) 
            {cnt_right++;sum_right+=j*j ;}
    }
    for(i = 16; i < cut_fork_bottom; i++){
        j = abs(topbor[i+1] - topbor[i]);
        if(j < 3 && (topbor[i+1] > topbor[i]) )//���С & ������ҵ������
            {cnt_left++;sum_left+=j*j ;}
             
    }
//    show_value[1]=cnt_left;
//    show_value[3]=cnt_right;
//    show_value[2]=sum_left;
//    show_value[4]=sum_right;
/********** �յ㡢����жϿ�ʼ **********/    
    if(abs(cnt_left-cnt_right) < 9 && cnt_left > 5 && cnt_right <25 && cnt_right > 5 && cnt_right <25){
            state = 41;
    }
}
/*------------------------------*/
/*	  ��ֱ�߽��Ѱ��ģ�飨��	*/
/*==============================*/
void vert_leftsearch(unsigned char top, unsigned char bottom){
//	��������
	register unsigned char i;
	register char j, k;
	unsigned char col = (MT9V03X_W-4)>>3;//����
	unsigned char *p;
	unsigned char found_flag, view_temp;
//	Ѱ���±߽�
	lefbottom_cut = 0;
	for(j = 0; j < 19; j++){
		found_flag = 0, p = &binary_img[exti_lefp[0]][j];
		for(i = exti_lefp[0]; i < bottom; i++, p+=col){
			view_temp = *(p)^*(p+col);
			for(k = 7; k > -1; k--){
				if(!((found_flag>>k)&0x01))
					if((view_temp>>k)&0x01){
						bottombor[(j<<3)+7-k] = i;
						if(lefbottom_cut < (j<<3)+7-k) lefbottom_cut =(j<<3)+7-k;
					}
			}
			found_flag |= view_temp;
			if(found_flag == 0xFF) break;
			if(*(p+col) == 0x00) break;
		}
		if(found_flag != 0xFF) break;
	}
//	Ѱ���ϱ߽�
	leftop_cut = 0;
	for(j = 0; j < 19; j++){
		found_flag = 0, p = &binary_img[exti_lefp[0]][j];
		for(i = exti_lefp[0]; i > top; i--, p-=col){
			view_temp = *(p)^*(p-col);
			for(k = 7; k > -1; k--){
				if(!((found_flag>>k)&0x01))
					if((view_temp>>k)&0x01){
						topbor[(j<<3)+7-k] = i;
						if(leftop_cut < (j<<3)+7-k) leftop_cut =(j<<3)+7-k;
					}
			}
			found_flag |= view_temp;
			if(found_flag == 0xFF) break;
			if(*(p+col) == 0x00) break;
		}
		if(found_flag != 0xFF) break;
	}
	if(leftop_cut > 5) 
		if(lefbottom_cut > 5){
			if(lefbottom_cut < rigtop_cut) vert_width_analysis(1, lefbottom_cut);
			else vert_width_analysis(2, leftop_cut);
		}
}
/*------------------------------*/
/*	  ��ֱ�߽��Ѱ��ģ�飨�ң�	*/
/*==============================*/
void vert_rightsearch(unsigned char top, unsigned char bottom){
//	��������
	register unsigned char i;
	register char j, k;
	unsigned char col = (MT9V03X_W-4)>>3;//����
	unsigned char *p;
	unsigned char found_flag, view_temp;
//	Ѱ���±߽�
	rigbottom_cut = 159;
	for(j = 19; j > -1; j--){
		found_flag = 0, p = &binary_img[exti_rigp[0]][j];
		for(i = exti_rigp[0]; i < bottom; i++, p+=col){
			view_temp = *(p)^*(p+col);
			for(k = 7; k > -1; k--){
				if(!((found_flag>>k)&0x01))
					if((view_temp>>k)&0x01){
						bottombor[(j<<3)+7-k] = i;
						if(rigbottom_cut > (j<<3)+7-k) rigbottom_cut =(j<<3)+7-k;
					}
			}
			found_flag |= view_temp;
			if(found_flag == 0xFF) break;
			if(*(p+col) == 0x00) break;
		}
		if(found_flag != 0xFF) break;
	}
//	Ѱ���ϱ߽�
	rigtop_cut = 159;
	for(j = 19; j > -1; j--){
		found_flag = 0, p = &binary_img[exti_rigp[0]][j];
		for(i = exti_rigp[0]; i > top; i--, p-=col){
			view_temp = *(p)^*(p-col);
			for(k = 7; k > -1; k--){
				if(!((found_flag>>k)&0x01))
					if((view_temp>>k)&0x01){
						topbor[(j<<3)+7-k] = i;
						if(rigtop_cut > (j<<3)+7-k) rigtop_cut =(j<<3)+7-k;
					}
			}
			found_flag |= view_temp;
			if(found_flag == 0xFF) break;
			if(*(p+col) == 0x00) break;
		}
		if(found_flag != 0xFF) break;
	}
//	�������
	if(rigbottom_cut > 5)
		if(rigtop_cut > 5){
			if(rigbottom_cut < rigtop_cut) vert_width_analysis(2, rigbottom_cut);
			else vert_width_analysis(2, rigtop_cut);
		}
}
/*------------------------------*/
/*	  �߽������Ѱ��ģ�飨��	*/
/*==============================*/
void border_vertical_leftsearch(void){
//	��������
	register unsigned char i, k;
	unsigned char vetflag = 0;
	unsigned char vet_colmax, vet_rowmax;
//	��ֱ�߽�Ѱ��
	if(ltraf_count > 1)
		for(i = 1; i < ltraf_count; i++){
		//	����͹
			if(ltraf_flag[i] == 0)
				if(ltraf_flag[i-1] == 1){
					for(k = ltraf_point_row[i], vet_colmax = 0; k < ltraf_point_row[i-1]; k++) 
						if(lefbor[k] > vet_colmax) vet_colmax = lefbor[k], vet_rowmax = k; 
					lvet_trafpoint_row[lvet_trafcount] = vet_rowmax, lvet_trafpoint_col[lvet_trafcount] = vet_colmax, lvet_trafcount++;
				}
		//	����
			if(ltraf_flag[i] == 1)
				if(ltraf_flag[i-1] == 0){ 
					exti_lefp[exti_lefcount] = (ltraf_point_row[i]+ltraf_point_row[i-1])>>1, exti_lefcount++;
					if(!vetflag){
						if(i+1 < ltraf_count) {vetflag = 1, exti_leftop = i, lefhigh = ltraf_point_row[i-1] - ltraf_point_row[i];vert_leftsearch(ltraf_point_row[i+1], ltraf_point_row[i-1]+10);}
						else {vetflag = 1, exti_leftop = i, lefhigh = ltraf_point_row[i-1] - ltraf_point_row[i];vert_leftsearch(ltraf_point_row[i]-7, ltraf_point_row[i-1]+10);}
					}
				}
		}
}
/*------------------------------*/
/*	  �߽������Ѱ��ģ�飨�ң�	*/
/*==============================*/
void border_vertical_rightsearch(void){
//	��������
	register unsigned char i, k;
	unsigned char vetflag = 0;
	unsigned char vet_colmax, vet_rowmax;
//	Ѱ������
	if(rtraf_count > 1)
		for(i = 1; i < rtraf_count; i++){
		//	����͹
			if(rtraf_flag[i] == 0)
				if(rtraf_flag[i-1] == 1){
					for(k = rtraf_point_row[i], vet_colmax = 159; k < rtraf_point_row[i-1]; k++) 
						if(rigbor[k] < vet_colmax) vet_colmax = rigbor[k], vet_rowmax = k; 
					rvet_trafpoint_row[rvet_trafcount] = vet_rowmax, rvet_trafpoint_col[rvet_trafcount] = vet_colmax, rvet_trafcount++;
				}
		//	����
			if(rtraf_flag[i] == 1){
				if(rtraf_flag[i-1] == 0) exti_rigp[exti_rigcount] = (rtraf_point_row[i]+rtraf_point_row[i-1])>>1, exti_rigcount++;
				if(!vetflag){
					if(i+1 < rtraf_count){vetflag = 1, exti_rigtop = i, righigh = rtraf_point_row[i-1] - rtraf_point_row[i];vert_rightsearch(rtraf_point_row[i+1], rtraf_point_row[i-1]+10);}
					else {vetflag = 1, exti_rigtop = i, righigh = rtraf_point_row[i-1] - rtraf_point_row[i];vert_rightsearch(rtraf_point_row[i]-7, rtraf_point_row[i-1]+10);}
				}
			}
		}
}
/*------------------------------*/
/*		 ��߽��Ѱ��ģ��		*/
/*==============================*/
void lbor_search(void){
//	��������
	short traf_slope, traf_slope_temp;
	register unsigned char i = found_point[0], k;
	register char j = found_point[1];
	unsigned char col = (MT9V03X_W-4)>>3;//����
	unsigned char *p;
	unsigned char traf_flag, traf_flag_temp;
	char sloptraf, lcmax = 0, lcmin = 0, local_flag1, local_flag1_temp, local_flag2, local_flag2_temp;
//	��ʼ��
	lcut = 0, ltraf_count = 0, lefbottom_cut = 0, leftop_cut = 0;
	lvet_trafcount = 0, exti_lefcount = 0, leftop_cut = 0, lefbottom_cut = 0;
	for(k = 0; k < 4; k++) ac_flag[k] = 0;
//	�ӻ�׼�㿪ʼѰ��
	p = &binary_img[i][j];
	switch(fop_flag){
		case 1://��׼���������
			for(k = 0; k < 7; k++)
				if(((*p>>(k+1))&0x01)^((*p>>k)&0x01)){lefbor[i] = (j<<3)+6-k;break;}
			break;
		case 2:
			lefbor[i] = (j<<3)+7;
			break;
		case 3://��׼�㲻�ڵ���
			for(k = 0; k < 7; k++)
				if(((*p>>(k+1))&0x01)^((*p>>k)&0x01)){lefbor[i] = (j<<3)+6-k;break;}
			for(k = MT9V03X_H - 1; k > i; k--) lefbor[k] = 0;
			break;
	}
//	���ϼ��
	for(i--; i > 0; i--){
	//	��ʼ�����С�������
		p -= col;
		if(traf_flag!=traf_flag_temp) 
			if(i < 90)//���������µ�ת���
				if(ltraf_point_row[ltraf_count-1]-i>3 || ltraf_count == 0){//����ת���߶�
					if(traf_flag == 1) ltraf_flag[ltraf_count] = 0;//���ҵ���ת��
					else ltraf_flag[ltraf_count] = 1;//������ת��
					ltraf_point_row[ltraf_count] = i+1, ltraf_point_col[ltraf_count] = lefbor[i+1], ltraf_count++;
				}
		traf_flag_temp = traf_flag;
		traf_flag = 0;
		if(*(p+1) != 0x00)
			if(*(p+1) != 0xff){//��ͻ��
				for(k = 0; k < 7; k++)
					if(((*(p+1)>>(k+1))&0x01)^((*(p+1)>>k)&0x01)){lefbor[i] = ((j+1)<<3)+6-k;break;}
					continue;
			}
		if(*p != 0x00)
			if(*p != 0xff){//��ͻ��
				for(k = 0; k < 7; k++)
					if(((*p>>(k+1))&0x01)^((*p>>k)&0x01)){lefbor[i] = (j<<3)+6-k;break;}
					continue;
			}
		if(*(uint16*)p == 0xFF00){lefbor[i] = (j<<3)+7;continue;}//����Ұ�����
		if(*(uint16*)p == 0x00FF){lefbor[i] = (j+1)<<3;continue;}//����Һ�����
	//	ȫ��
		if(*(uint16*)p == 0x0000){
			for(p++, j++; j < 19; p++, j++){//����Ѱ�������
				if(*(uint16*)p == 0x0000) continue;
				else break;
			}
			if(j == 19){//�ִ�߽磬ֹͣ����
				lcut = i;
				if(traf_flag_temp)
					if(ltraf_point_row[ltraf_count-1]-i>5 || ltraf_count == 0)
						ltraf_flag[ltraf_count] = 1, ltraf_point_row[ltraf_count] = i+1, ltraf_point_col[ltraf_count] = lefbor[i+1], ltraf_count++;
				return;
			}
			if(*(uint16*)p == 0xFF00){lefbor[i] = (j<<3)+7;continue;}//����Ұ�����
			if(*(uint16*)p == 0x00FF){lefbor[i] = (j+1)<<3;continue;}//����Һ�����
			if(*(p+1) != 0x00)
				if(*(p+1) != 0xff){//��ͻ��
					for(k = 0; k < 7; k++)
						if(((*(p+1)>>(k+1))&0x01)^((*(p+1)>>k)&0x01)){lefbor[i] = ((j+1)<<3)+6-k;break;}
						continue;
				}
			if(*p != 0x00)
				if(*p != 0xff){//��ͻ��
					for(k = 0; k < 7; k++)
						if(((*p>>(k+1))&0x01)^((*p>>k)&0x01)){lefbor[i] = (j<<3)+6-k;break;}
						continue;
				}
		}
	//	ȫ��
		if(*(uint16*)p == 0xFFFF){
			for(p--, j--; j > -1; p--, j--){//����Ѱ�������
				if(*(uint16*)p == 0xFFFF) continue;
				else break;
			}
			if(j == -1){lcut = i;traf_flag = 1,j+=6,p+=6,lefbor[i] = 0;continue;}//�ִ�ȫ�ױ߽�
			if(*(uint16*)p == 0xFF00){lefbor[i] = (j<<3)+7;continue;}//����Ұ�����
			if(*(uint16*)p == 0x00FF){lefbor[i] = (j+1)<<3;continue;}//����Һ�����
			if(*p != 0x00)
				if(*p != 0xff){//��ͻ��
					for(k = 0; k < 7; k++)
						if(((*p>>(k+1))&0x01)^((*p>>k)&0x01)){lefbor[i] = (j<<3)+6-k;break;}
						continue;
				}
			if(*(p+1) != 0x00)
				if(*(p+1) != 0xff){//��ͻ��
					for(k = 0; k < 7; k++)
						if(((*(p+1)>>(k+1))&0x01)^((*(p+1)>>k)&0x01)){lefbor[i] = ((j+1)<<3)+6-k;break;}
						continue;
				}
		}
	}
	lcut = i;//�˳�ǰ����������
}
/*------------------------------*/
/*		 �ұ߽��Ѱ��ģ��		*/
/*==============================*/
void rbor_search(void){
//	��������
	register unsigned char i = found_point[2], k;
	register char j = found_point[3];
	unsigned char col = (MT9V03X_W-4)>>3;//����
	unsigned char traf_flag, traf_flag_temp;
	unsigned char *p;
//	��ʼ��
	rcut = 0, rtraf_count = 0, rigbottom_cut = 159, rigtop_cut = 159;
	rvet_trafcount = 0, exti_rigcount = 0, rigtop_cut = 159, rigbottom_cut = 159;
//	�ӻ�׼�㿪ʼѰ��
	p = &binary_img[i][j];
	switch(fop_flag){
		case 1://��׼���������
			for(k = 7; k > 0; k--)
				if(((*p>>k)&0x01)^((*p>>(k-1))&0x01)){rigbor[i] = (j<<3)+8-k;break;}
			break;
		case 2:
			rigbor[i] = (j<<3);
			break;
		case 3:
			for(k = 7; k > 0; k--)
				if(((*p>>k)&0x01)^((*p>>(k-1))&0x01)){rigbor[i] = (j<<3)+8-k;break;}
			for(k = MT9V03X_H - 1; k > i; k--) rigbor[k] = 159;
			break;
	}
//	���Ʊ߽�
	if(j == 19) j--, p--;
//	���ϼ��
	for(i--; i > 0; i--){
	//	��ʼ�����С�������
		p -= col;
		if(traf_flag!=traf_flag_temp) 
			if(i < 80)//���������µ�ת���
				if(rtraf_point_row[rtraf_count-1]-i>5 || rtraf_count == 0){//����ת���߶�
					if(traf_flag == 1) rtraf_flag[rtraf_count] = 0;//������ת��
					else rtraf_flag[rtraf_count] = 1;//���ҵ���ת��
					rtraf_point_row[rtraf_count] = i+1, rtraf_point_col[rtraf_count] = rigbor[i+1], rtraf_count++;
				}
		traf_flag_temp = traf_flag;
		traf_flag = 0;
		if(*p != 0x00)
			if(*p != 0xff){//��ͻ��
				for(k = 7; k > 0; k--)
					if(((*p>>k)&0x01)^((*p>>(k-1))&0x01)){rigbor[i] = (j<<3)+8-k;break;}
					continue;
			}
		if(*(p+1) != 0x00)
			if(*(p+1) != 0xff){//��ͻ��
				for(k = 7; k > 0; k--)
					if(((*(p+1)>>k)&0x01)^((*(p+1)>>(k-1))&0x01)){rigbor[i] = ((j+1)<<3)+8-k;break;}
					continue;
			}
		if(*(uint16*)p == 0x00FF){rigbor[i] = ((j+1)<<3);continue;}//����Һ�����
		if(*(uint16*)p == 0xFF00){rigbor[i] = (j<<3)+7;continue;}//����Ұ�����
	//	ȫ��
		if(*(uint16*)p == 0x0000){
			for(p--, j--; j >  -1; p--, j--){//����Ѱ�������
				if(*(uint16*)p == 0x0000) continue;
				else break;
			}
			if(j == -1){//�ִ�߽磬ֹͣ����
				rcut = i;
				if(traf_flag_temp)//ת�����
					if(rtraf_point_row[rtraf_count-1]-i>5 || rtraf_count == 0)//����ת���߶�
						rtraf_point_row[rtraf_count] = i+1, rtraf_point_col[rtraf_count] = rigbor[i+1], rtraf_count++;
				return;
			}
			if(*(uint16*)p == 0x00FF){rigbor[i] = ((j+1)<<3);continue;}//����Һ�����
			if(*(uint16*)p == 0xFF00){rigbor[i] = (j<<3)+7;continue;}//����Ұ�����
			if(*p != 0x00)
				if(*p != 0xff){//��ͻ��
					for(k = 7; k > 0; k--)
						if(((*p>>k)&0x01)^((*p>>(k-1))&0x01)){rigbor[i] = (j<<3)+8-k;break;}
						continue;
				}
			if(*(p+1) != 0x00)
				if(*(p+1) != 0xff){//��ͻ��
					for(k = 7; k > 0; k--)
						if(((*(p+1)>>k)&0x01)^((*(p+1)>>(k-1))&0x01)){rigbor[i] = ((j+1)<<3)+8-k;break;}
						continue;
				}
		}
	//	ȫ��
		if(*(uint16*)p == 0xFFFF){
			for(p++, j++; j < 19; p++, j++){//����Ѱ�������
				if(*(uint16*)p == 0xFFFF) continue;
				else break;
			}
			if(j == 19){rcut = i;traf_flag = 1;j-=6, p-=6,rigbor[i] = 159;continue;}
			if(*(uint16*)p == 0x00FF){rigbor[i] = ((j+1)<<3);continue;}//����Һ�����
			if(*(uint16*)p == 0xFF00){rigbor[i] = (j<<3)+7;continue;}//����Ұ�����
			if(*(p+1) != 0x00)
				if(*(p+1) != 0xff){//��ͻ��
					for(k = 7; k > 0; k--)
						if(((*(p+1)>>k)&0x01)^((*(p+1)>>(k-1))&0x01)){rigbor[i] = ((j+1)<<3)+8-k;break;}
						continue;
				}
			if(*p != 0x00)
				if(*p != 0xff){//��ͻ��
					for(k = 7; k > 0; k--)
						if(((*p>>k)&0x01)^((*p>>(k-1))&0x01)){rigbor[i] = (j<<3)+8-k;break;}
						continue;
				}
		}
	}
	rcut = i;//�˳�ǰ����������
}
/*------------------------------*/
/*		��׼�㸨��ģ�飨��	*/
/*==============================*/
static void left_fop_search_sup(unsigned char* p, char j){
//	һ�����
	if(*p != 0xFF)
		if(*p != 0x00){//���ӳ�ͻ��
			fop_flag = 1, found_point[0] = MT9V03X_H - 1, found_point[1] = j;
			return;
		}
	if(*(p+1) != 0xFF)
		if(*(p+1) != 0x00){//���ӳ�ͻ��
			fop_flag = 1, found_point[0] = MT9V03X_H - 1, found_point[1] = j+1;
			return;
		}
//	����Ұ�
	fop_flag = 2, found_point[0] = MT9V03X_H - 1, found_point[1] = j;
}
/*------------------------------*/
/*		��׼�㸨��ģ�飨�ң�	*/
/*==============================*/
static void right_fop_search_sup(unsigned char* p, char j){
//	һ�����
	if(*p != 0xFF)
		if(*p != 0x00){//���ӳ�ͻ��
			fop_flag = 1, found_point[2] = MT9V03X_H - 1, found_point[3] = j;
			return;
		}
	if(*(p+1) != 0xFF)
		if(*(p+1) != 0x00){//���ӳ�ͻ��
			fop_flag = 1, found_point[2] = MT9V03X_H - 1, found_point[3] = j+1;
			return;
		}
//	����Һ�
	fop_flag = 2, found_point[2] = MT9V03X_H - 1, found_point[3] = j+1;
}
/*------------------------------*/
/*		 ��׼��Ѱ��ģ�飨��	*/
/*==============================*/
void left_fop_search(void){
//	��������
	register unsigned char i = MT9V03X_H - 1;
	register char j;
	unsigned char* p;
//	�����Ѱ�� 
	p = &binary_img[i][8];
	for(j = 8; j > -1; j--){
		if(*(uint16*)p == 0xFFFF){p--;continue;}
		left_fop_search_sup(p, j);
		return;
	}
//	δ�ҵ����㣬����Ѱ��
	for(i = MT9V03X_H - 1; i > 0; i--){
		if(!(binary_img[i][0]^binary_img[i-1][0])) continue;
		fop_flag = 3, found_point[0] = i, found_point[1] = 0;
		return;
	}
}
/*------------------------------*/
/*		 ��׼��Ѱ��ģ�飨�ң�	*/
/*==============================*/
void right_fop_search(void){
//	��������
	register unsigned char i = MT9V03X_H - 1;
	register char j;
	unsigned char* p;
//	�һ���Ѱ�� 
	p = &binary_img[i][10];
	for(j = 10; j < 19; j++){
		if(*(uint16*)p == 0xFFFF){p++;continue;}
		right_fop_search_sup(p, j);
		return;
	}
//	δ�ҵ����㣬����Ѱ��
	for(i = MT9V03X_H - 1; i > 0; i--){
		if(!(binary_img[i][19]^binary_img[i-1][19])) continue;
		fop_flag = 3, found_point[2] = i, found_point[3] = 19;
		return;
	}
}
/*------------------------------*/
/*		  ͼ���ֵ��ģ��		*/
/*==============================*/
void img_binary(void){
//	��������
	register unsigned char i, j, k;
	unsigned char column = (MT9V03X_W-4)>>3;
	unsigned char binary_temp;
//	ͼ���ֵ��
	for(i = 0; i < MT9V03X_H; i++){
		for(j = 0; j < column; j++){
		//	����ͼ��
			binary_img[i][j] = 0x00;
		//	��ֵ��
			for(k = 0; k < 8; k++){
				binary_img[i][j] <<= 1;
				if(mt9v03x_image[i][(j<<3)+k+2] > img_thrsod) binary_img[i][j] |= 0x01;
			}
		}
	}
}
/*------------------------------*/
/*		���ȫ����ֵģ��		*/
/*==============================*/
void otsu(void){
//	��������
	float var = 0, vartmp = 0;
	float imgsize = MT9V03X_H*MT9V03X_W, sumPK = 0, sumMK = 0;
	register short i, j;
	unsigned short hist[grayscale] = {0};	
//	��ȡֱ��ͼ
	for(i = 0; i < MT9V03X_H; i++){
		for(j = 0; j < MT9V03X_W; j++)
			hist[mt9v03x_image[i][j]]++;
	}
//	����䷽��
	for(i = 0; i < grayscale; i++){
		P[i] = (float)hist[i]/imgsize;//����ÿ���Ҷȼ����ֵĸ���
		PK[i] = sumPK+P[i];//�����ۼƺ�
		sumPK = PK[i];
		MK[i] = sumMK+i*P[i];//�Ҷ�ֵ�ۼӾ�ֵ
		sumMK = MK[i];
	}
	for(i = 115; i < 200; i++){
		vartmp = ((MK[grayscale-1]*PK[i] - MK[i])*(MK[grayscale-1]*PK[i] - MK[i])) / (PK[i]*(1 - PK[i]));
		if(vartmp > var){
			var = vartmp;
			img_thrsod = i;//�����ֵ
		}
	}
}
