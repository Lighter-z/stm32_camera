

#include  "mcusys.h"
#include "led.h"


void LED_Init(void)
{
 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOAʱ��

  //GPIOA1��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);//���øߣ�����
	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO	
	GPIO_SetBits(GPIOE,GPIO_Pin_14);//���øߣ�����	
	
}
 
