/*
 * @Author: ��ZCM5115�� ��1902117322@qq.com��
 * @Date: 2023-05-13 20:10:02
 * @LastEditors: ��ZCM5115�� ��1902117322@qq.com��
 * @LastEditTime: 2023-05-13 20:12:00
 * @FilePath: \twod:\gitee\bb\MDK-ARM\NRF\nrf_send.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "nrf_send.h"
#include "my_robot.h"
#include "stdlib.h"

/*============================================================ 
* 
* �� �� ����check_and_sendACK()  
* �� ����
* ��������: 
*���ֱ�����ACKӦ�𣬲����ֱ�����д�˴�������ʾ
* �� �� ֵ��none
* �� �ߣ�zcm 
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

