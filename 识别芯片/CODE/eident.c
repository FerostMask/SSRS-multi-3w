/*--------------------------------------------------------------*/
/*							ͷ�ļ�����							*/
/*==============================================================*/
#include "Init.h"
#include "math.h"
#include "stdlib.h"
#include "menu_2.h"
#include "eident.h"
#include "zf_adc.h"
#include "SEEKFREE_IPS200_PARALLEL8.h"
/*--------------------------------------------------------------*/
/* 							 �������� 							*/
/*==============================================================*/
/*----------------------*/
/*	    ��ͨ���˲�		*/
/*======================*/
void single_ch_filter(struct adcpara *para){
//	��������
	unsigned char i;
	register unsigned char j;
	unsigned short value[3];
	unsigned int sum_val = 0;
	unsigned char max_val, min_val;
//	��ֵ��ֵ�˲�
	for(i = 0; i < SAMPFRE; i++){
		//	����
			max_val = 0, min_val = 0;
		//	�ɼ���������
			for(j = 0; j < 3; j++)
				value[j] = adc_convert(ADC_MOD1, para->pin);
		//	�ж�����ֵ�Ƿ����
			if(value[0] == value[1])
				if(value[1] == value[2]){sum_val+= value[0];continue;}
		//	����ֵ
			for(j = 1; j < 3; j++){
				if(value[max_val] < value[j]) max_val = j;
				if(value[min_val] > value[j]) min_val = j;
			}
			sum_val += value[3-max_val-min_val];
		}
		para->value = (sum_val / SAMPFRE);
	//	�޷�
		if(para->value > para->max) para->value = para->max;
		if(para->value < para->min) para->value = para->min;
	//	��һ��
		para->value = 100*(para->value-para->min)/(para->max-para->min);
}
/*----------------------*/
/*	     �����ֵ		*/
/*======================*/
void adc_extreme(struct adcpara *para){
//	��������
	unsigned short adc_colltemp;
//	ADC�ɼ�����ֵ�Ƚ�
	adc_colltemp = adc_convert(ADC_MOD1, para->pin);
	if(para->max < adc_colltemp) para->max = adc_colltemp;
	if(para->min > adc_colltemp) para->min = adc_colltemp;
//	ˢ�¸�����ֵ
	ips200_pencolor = 0xFFFF;
	ips200_bgcolor = 0xB6DB;
	menu2value_hl();
	ips200_pencolor = 0xB6DB;
	ips200_bgcolor = 0xFFFF;
}
