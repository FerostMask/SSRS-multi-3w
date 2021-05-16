>   基准点寻找

| fop_flag | 情况     | 注释 |
| -------- | -------- | ---- |
| 1        | 突变     |      |
| 2        | 前后跳变 |      |
| 3        | 上下跳变 |      |
>   状态机

| act_flag | 情况                  | 注释 |
| -------- | --------------------- | ---- |
| 0        | 直道                  |      |
| 11       | 左弯道丢边            |      |
| 12       | 右弯道丢边            |      |
| 13       | 左弯                  |      |
| 14       | 右弯                  |      |
| 21       | 环道1 \| 检测到出环口 |      |
| 22       | 环道2 \| 检测到入环口 |      |
| 23       | 环内                  |      |
| 24       | 出环前                |      |
| 25       | 出环后                |      |
| 31       | 十字                  |      |
| 41       | 岔道                  |      |
|          |                       |      |

| state | 情况     | 注释 |
| ----- | -------- | ---- |
| 0     | 正常情况 |      |
| 11    | 左转丢边 |      |
| 12    | 右转丢边 |      |
| 13    | 左弯     |      |
| 14    | 右弯     |      |
| 21    | 入环1    |      |
| 22    | 入环2    |      |
| 23    | 出环     |      |
| 24    | 出环后   |      |
|       |          |      |
|       |          |      |
|       |          |      |
| 41    | 岔道     |      |
|       |          |      |
|       |          |      |



```c
cnt_level_change_points = 0;
    p = &binary_img[i-4][2];
    for(j = 2; j < 18; j++,p++){//换列
    //	一般情况
        if(*(uint16*)p == 0x0000) continue;//全黑
        if(*(uint16*)p == 0xffff) continue;//全白
 		if(*p != 0x00)
			if(*p != 0xff){//左突变
				for(k = 7; k > 0; k--){
                    if(((*p>>k)&0x01)==1)&&(((*p>>(k-1))&0x01)==0)){cnt_level_change_points++,flag[count++]=2;continue;}//白黑跳变
                    if(((*p>>k)&0x01)==0)&&(((*p>>(k-1))&0x01)==1)){cnt_level_change_points++,flag[count++]=1;continue;}//黑白跳变
                }
					continue;
			}
		if(*(p+1) != 0x00)
			if(*(p+1) != 0xff){//左突变
				for(k = 7; k > 0; k--){
                    if(((*p>>k)&0x01)==1)&&(((*p>>(k-1))&0x01)==0)){cnt_level_change_points++,flag[count++]=2;continue;}//白黑跳变
                    if(((*p>>k)&0x01)==0)&&(((*p>>(k-1))&0x01)==1)){cnt_level_change_points++,flag[count++]=1;continue;}//黑白跳变
                }
					continue;
			}
    //	特殊情况
        if(*(uint16*)p == 0x00FF){cnt_level_change_points++,flag[count++] = 2;continue}//左白右黑跳变
		if(*(uint16*)p == 0xFF00){cnt_level_change_points++,flag[count++] = 1;continue;}//左黑右白跳变
    }
```

