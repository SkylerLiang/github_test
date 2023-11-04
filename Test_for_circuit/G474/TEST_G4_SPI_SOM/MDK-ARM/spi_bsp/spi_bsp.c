#include "spi_bsp.h"

/*���Ķ�ͷ�ļ��е�˵������ø������ʹ��˵��*/

int32_t Pos_Base[5] = {0};//�ۻ�Ȧ��
int32_t Pos_Now[5] = {0};//���ڵ��ۻ�����λ��
int16_t Pos_Init[5] = {0};//��ʼ�ı�����λ�ã�����ֵ���������籣�棩
uint16_t tmp[5] = {0};//��������Ȧ��ǰλ��
uint16_t tmp_last[5] = {0};//��������Ȧ��һ��λ��

void Encoder_Init(void)
{
	SPI_Set_Speed(&hspi2, PCLK_128);
	SPI_Set_Speed(&hspi3, PCLK_128);
	for(uint8_t i=0; i<50; i++)//�˲�
	{
		Encoder1_Read(&hspi2);
		Encoder2_Read(&hspi3);
		HAL_Delay(0);
	}
	Pos_Init[2] = Encoder1_Read(&hspi2);
	Pos_Init[3] = Encoder2_Read(&hspi3);
	tmp_last[2] = Pos_Init[2];
	tmp_last[3] = Pos_Init[3];
}
void SPI_Set_Speed(SPI_HandleTypeDef* hspi, uint8_t SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));   //�ж���Ч��

    __HAL_SPI_DISABLE(hspi);  //�ر�SPI
    hspi->Instance->CR1 &= 0XFFC7;  //λ3-5���㣬�������ò�����
    hspi->Instance->CR1 |= SPI_BaudRatePrescaler;	 //����SPI�ٶ�
    __HAL_SPI_ENABLE(hspi);  //ʹ��SPI
}
uint16_t Encoder1_Read(SPI_HandleTypeDef* hspi)
{
	static uint16_t PreData = 0;
	uint16_t RawData = 0;
	uint16_t Data = 0;
	
	SPI2_CS_L;
	HAL_SPI_Receive(hspi, (uint8_t*)&RawData, 1, 10);
	SPI2_CS_H;
	
	Data = (RawData>>3)&0xfff;
	if(RawData>>15 == 1)  
		Data = PreData;
	PreData = Data;
	
	return Data;
}
uint16_t Encoder2_Read(SPI_HandleTypeDef* hspi)
{
	static uint16_t PreData = 0;
	uint16_t RawData = 0;
	uint16_t Data = 0;
	
	SPI3_CS_L;
	HAL_SPI_Receive(hspi, (uint8_t*)&RawData, 1, 10);
	SPI3_CS_H;
	
	Data = (RawData>>3)&0xfff;
	if(RawData>>15 == 1)  
		Data = PreData;
	PreData = Data;
	
	return Data;
}
void Encoder_Cal(void)
{
//	tmp[1] = Encoder1_Read(&hspi1);
//	if(tmp[1] - tmp_last[1] > 2048)  
//	Pos_Base[1]-=4096;
//	else if(tmp[1] - tmp_last[1] < -2048)  
//	Pos_Base[1]+=4096;
//	Pos_Now[1] = Pos_Base[1] + tmp[1] - Pos_Init[1];
//	tmp_last[1] = tmp[1];
	tmp[2] = Encoder1_Read(&hspi2);
	if(tmp[2] - tmp_last[2] > 2048)  
	Pos_Base[2]-=4096;
	else if(tmp[2] - tmp_last[2] < -2048)  
	Pos_Base[2]+=4096;
	Pos_Now[2] = Pos_Base[2] + tmp[2] - Pos_Init[2];
	tmp_last[2] = tmp[2];
	
	tmp[3] = Encoder2_Read(&hspi3);
	if(tmp[3] - tmp_last[3] > 2048)  
	Pos_Base[3]-=4096;
	else if(tmp[3] - tmp_last[3] < -2048)  
	Pos_Base[3]+=4096;
	Pos_Now[3] = Pos_Base[3] + tmp[3] - Pos_Init[3];
	tmp_last[3] = tmp[3];
}
