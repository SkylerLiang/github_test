/*
 * @Author: “ZCM5115” “1902117322@qq.com”
 * @Date: 2023-05-13 20:10:02
 * @LastEditors: “ZCM5115” “1902117322@qq.com”
 * @LastEditTime: 2023-05-13 20:12:00
 * @FilePath: \twod:\gitee\bb\MDK-ARM\NRF\nrf_send.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "nrf_send.h"
#include "my_robot.h"
#include "stdlib.h"

/*============================================================ 
* 
* 函 数 名：check_and_sendACK()  
* 参 数：
* 功能描述: 
*向手柄发送ACK应答，并在手柄端书写了代码以显示
* 返 回 值：none
* 作 者：zcm 
* 
============================================================*/
extern uint8_t check3508[5];

extern ACK_PAYLOAD nRF24L01_ack_pay;

bool SendCheck[32]={1,1,1,1};

void check_and_sendACK()    
{
        

 memcpy(&nRF24L01_ack_pay.Ack_Buf[0], &SendCheck[1], 4);


 Ack_load(32);
 
}

