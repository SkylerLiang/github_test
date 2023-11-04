#ifndef __NRF_SEND__
#define __NRF_SEND__

/*From Cubemx*/
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

/*From Mine*/
#include "my_robot.h"

/*��NRF��ʾxy*/
void NRF_xy_display(float x, float y);
/*��NRF����ʾ�ǶȺͽ��ٶ�*/
void NRF_angle_display(float angle,float spd);
/*NRF��ʾ��־λ*/
void NRF_Flag_display(uint8_t num, int Pitch_Err, int Yaw_Err);
void check_and_sendACK(void);
#endif
