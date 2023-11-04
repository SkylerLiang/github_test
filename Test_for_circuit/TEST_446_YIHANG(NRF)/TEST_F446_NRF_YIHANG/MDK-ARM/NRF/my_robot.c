
/************************************************
joyhandleX decode lib


Modification
* + nrf_init()
* + Ack_load(uint8_t* ack_data, uint8_t lenth)
*
last modified by DQS

*使用方法：
*	if(htim->Instance == TIM6) {
*
*		if(!HAL_GPIO_ReadPin(NRF_IRQ_GPIO_Port, NRF_IRQ_Pin))
*		{
*			instruction_refresh();
*			read_keys();
*			uint8_t aaa = 1;	???
*			Ack_load(&aaa,8);
*		}
*	 }
***********************************************/

/* include --------------------------------------*/

#include "my_robot.h"
#include "nrf.h"
#include "stdlib.h"
#include "string.h"

// #include "main.h"
/* private variables --------------------------*/
uint8_t nrf_cmd[32];	  // 初始数据
int16_t nrf_trans_cmd[7]; // 转换后数据
uint8_t button[28];		  // 按键
Handle_Info handle;
bool nrf_mode = 1; // nrf工作模式 
bool No_Data = 0;

#define Pi 3.1415926f
int same_times = 0; // 计划在定时器中统计cmd 连续 不变的数目，t>10认定为nrf断开

/**********************************************************************
 *@ 	name		: instruction_refresh
 *@	function: 手柄数据刷新
 *@	input		: none
 *@	output	: none
 ***********************************************************************/
void instruction_refresh(void)
{
	if (HAL_GPIO_ReadPin(NRF_IRQ_GPIO_Port, NRF_IRQ_Pin) == 1)
		return;

	if (nRF24L01_Check())
	{
		nrf_mode = false;
		for (int i; i < 7; i++)
			nrf_trans_cmd[i] = 0; // nrf断线，cmd清零
		for (int i; i < 26; i++)
			button[i] = 0;
	}
	if (nrf_mode)
	{
		int16_t *nrf_p;
		int len = nRF24L01_RxPacket(nrf_cmd);
		if (len != 0 && nrf_cmd[14] == 123)
		{
			int i_nrf = 0;
			nrf_p = (int16_t *)nrf_cmd;
			for (i_nrf = 0; i_nrf < 7; i_nrf++)
			{
				nrf_trans_cmd[i_nrf] = nrf_p[i_nrf];
			}
		}
	}
}

//uint8_t instruction_refresh(uint8_t Check, uint16_t Clear)
//{
//	static bool nrf_mode_l = 1;

//	static uint8_t cnt = 0;
//	static uint16_t wait = 0;
//	cnt++;
//	/*每Check次检查一下nrf是否断连，重连后重新初始化*/
//	if(cnt >= Check)
//    {
//		cnt = 0;
//		if(nRF24L01_Check()){
//			nrf_mode = false;
//        }            
//		else  {
//			nrf_mode = true;
//            if(!nrf_mode_l)
//				nrf_init();
//		} 
//		nrf_mode_l = nrf_mode;        
//	}
//	
//	/*IRQ置低，FIFO有数据*/
// 	if(HAL_GPIO_ReadPin(NRF_IRQ_GPIO_Port, NRF_IRQ_Pin) == 1)
//    {
//		wait ++;      //连续Clear次无数据，认为手柄断联，切自由移动
//		if(wait > Clear)
//			No_Data = true;
//	} 
//    else 
//    {
//		wait = 0;
//        No_Data = false;
//	}
//    
//    if(nrf_mode && !No_Data)
//    {		
//        int16_t* nrf_p;
//        int len = nRF24L01_RxPacket(nrf_cmd);
//        
//        if(len != 0 && nrf_cmd[14] == 123)  
//        {
//            int i_nrf = 0;
//            nrf_p = (int16_t*)nrf_cmd;
//            for(i_nrf = 0;i_nrf < 7;i_nrf++)
//            {
//                nrf_trans_cmd[i_nrf] = nrf_p[i_nrf];
//            }
//        }	
//    }
//    else 
//    {
//        for(int i; i<7; i++)		nrf_trans_cmd[i] = 0;             //nrf断线，cmd清零
//        for(int i; i<26; i++)		button[i] = 0;
//    }
//    
//    return (nrf_mode && !No_Data);
//}
/**********************************************************************
 *@ 	name		: instruction_refresh
 *@	function: 手柄数据刷新
 *@	input		: none
 *@	output	: none
 ***********************************************************************/
void Ack_load(uint8_t lenth)
{

	nRF24L01_ack_pay.Ack_Len = lenth;
#if (DYNPD_ACK_DATA) // 回送反向的ACK数据
	nRF24L01_Rx_AckPayload(nRF24L01_ack_pay);
#endif
}

/**********************************************************************
 *@ 	name		: read_rocker
 *@	functio	: 获得摇杆数据		编号0~3
 *@	input		: none
 *	@	output	: none
 ***********************************************************************/
int16_t read_rocker(int id)
{
	if (id == 1 || id == 0)
		return -nrf_trans_cmd[2 + id];
	else
		return nrf_trans_cmd[2 + id];
}
/**********************************************************************
*@ 	name		: read_keys
*@	functio	: 获得按键数据，放在 boll button[26]中
*@	input		: none
*@	output	: none

	20        |		  21
 16	 17	 06    | 04  08  09
 18	 19	 07    | 05  10  11
 |22| |23|     |     12  13
   00 |24|     |          |25|
02    03       |     15  14
   01          |
***********************************************************************/
  void read_keys(void)
{
	button[0] = (nrf_trans_cmd[0] & 0x0001);
	button[1] = (nrf_trans_cmd[0] & 0x0002) >> 1;
	button[2] = (nrf_trans_cmd[0] & 0x0004) >> 2;
	button[3] = (nrf_trans_cmd[0] & 0x0008) >> 3;
	button[4] = (nrf_trans_cmd[0] & 0x0010) >> 4;
	button[5] = (nrf_trans_cmd[0] & 0x0020) >> 5;
	button[6] = (nrf_trans_cmd[0] & 0x0040) >> 6;
	button[7] = (nrf_trans_cmd[0] & 0x0080) >> 7;
	button[8] = (nrf_trans_cmd[1] & 0x0001);
	button[9] = (nrf_trans_cmd[1] & 0x0002) >> 1;
	button[10] = (nrf_trans_cmd[1] & 0x0004) >> 2;
	button[11] = (nrf_trans_cmd[1] & 0x0008) >> 3;
	button[12] = (nrf_trans_cmd[1] & 0x0010) >> 4;
	button[13] = (nrf_trans_cmd[1] & 0x0020) >> 5;
	button[14] = (nrf_trans_cmd[1] & 0x0040) >> 6;
	button[15] = (nrf_trans_cmd[1] & 0x0080) >> 7;
	button[16] = (nrf_trans_cmd[0] & 0x0400) >> 10;
	button[17] = (nrf_trans_cmd[0] & 0x0800) >> 11;
	button[18] = (nrf_trans_cmd[0] & 0x1000) >> 12;
	button[19] = (nrf_trans_cmd[0] & 0x2000) >> 13;
	button[20] = (nrf_trans_cmd[0] & 0x4000) >> 14;
	button[21] = (nrf_trans_cmd[0] & 0x8000) >> 15;
	button[22] = (nrf_trans_cmd[6] & 0x0008) >> 3;
	button[23] = (nrf_trans_cmd[6] & 0x0004) >> 2;
	button[24] = (nrf_trans_cmd[6] & 0x0002) >> 1;
	button[25] = (nrf_trans_cmd[6] & 0x0001);
	/*左右摇杆按压按键*/
	button[26] = (nrf_trans_cmd[0] & 0x0100) >> 8;
	button[27] = (nrf_trans_cmd[0] & 0x0200) >> 9;
    
    memcpy(handle.button, button, 28);
    /*提取摇杆值*/
    
        if (abs(read_rocker(1)) > 25 && abs(read_rocker(1)) <= 190)
            handle.ly = (int)read_rocker(1);
        else
            handle.ly = 0;
    
        if (abs(read_rocker(0)) > 25 && abs(read_rocker(0)) <= 190)
            handle.lx = (int)read_rocker(0);
        else
            handle.lx = 0;
        if (abs(read_rocker(2)) > 25 && abs(read_rocker(2)) <= 190){
            handle.rx = (int)read_rocker(2);
        }
        else
            handle.rx = 0;
        if (abs(read_rocker(3)) > 25 && abs(read_rocker(3)) <= 190)
            handle.ry = (int)read_rocker(3);
        else
            handle.ry = 0;

}

//将按键摇杆的信息作为遥控信息传递给底盘数据
Handle_Info* get_remote_control_point(void)
{
	return &handle;
}
