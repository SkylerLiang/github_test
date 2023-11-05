#ifndef __NRF_SEND__
#define __NRF_SEND__

/*From Cubemx*/
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

/*From Mine*/
#include "my_robot.h"

/*在NRF显示xy*/
void NRF_xy_display(float x, float y);
/*在NRF上显示角度和角速度*/
void NRF_angle_display(float angle,float spd);
/*NRF显示标志位*/
void NRF_Flag_display(uint8_t num, int Pitch_Err, int Yaw_Err);
void check_and_sendACK(void);
#endif
