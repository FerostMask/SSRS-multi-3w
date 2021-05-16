/*--------------------------------------------------------------*/
/*							头文件加载							*/
/*==============================================================*/
#include "Init.h"
#include "math.h"
#include "stdlib.h"
#include "menu_2.h"
#include "eident.h"
#include "zf_adc.h"
#include "SEEKFREE_IPS200_PARALLEL8.h"
/*--------------------------------------------------------------*/
/* 							 函数定义 							*/
/*==============================================================*/
/*----------------------*/
/*	    单通道滤波		*/
/*======================*/
void single_ch_filter(struct adcpara *para){
//	变量定义
	unsigned char i;
	register unsigned char j;
	unsigned short value[3];
	unsigned int sum_val = 0;
	unsigned char max_val, min_val;
//	中值均值滤波
	for(i = 0; i < SAMPFRE; i++){
		//	重置
			max_val = 0, min_val = 0;
		//	采集三次数据
			for(j = 0; j < 3; j++)
				value[j] = adc_convert(ADC_MOD1, para->pin);
		//	判断三个值是否相等
			if(value[0] == value[1])
				if(value[1] == value[2]){sum_val+= value[0];continue;}
		//	找中值
			for(j = 1; j < 3; j++){
				if(value[max_val] < value[j]) max_val = j;
				if(value[min_val] > value[j]) min_val = j;
			}
			sum_val += value[3-max_val-min_val];
		}
		para->value = (sum_val / SAMPFRE);
	//	限幅
		if(para->value > para->max) para->value = para->max;
		if(para->value < para->min) para->value = para->min;
	//	归一化
		para->value = 100*(para->value-para->min)/(para->max-para->min);
}
/*----------------------*/
/*	     电磁最值		*/
/*======================*/
void adc_extreme(struct adcpara *para){
//	变量定义
	unsigned short adc_colltemp;
//	ADC采集、最值比较
	adc_colltemp = adc_convert(ADC_MOD1, para->pin);
	if(para->max < adc_colltemp) para->max = adc_colltemp;
	if(para->min > adc_colltemp) para->min = adc_colltemp;
//	刷新高亮数值
	ips200_pencolor = 0xFFFF;
	ips200_bgcolor = 0xB6DB;
	menu2value_hl();
	ips200_pencolor = 0xB6DB;
	ips200_bgcolor = 0xFFFF;
}
