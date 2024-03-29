

#include  "mcusys.h"
#include "led.h"


void LED_Init(void)
{
 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA时钟

  //GPIOA1初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);//设置高，灯灭
	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIO	
	GPIO_SetBits(GPIOE,GPIO_Pin_14);//设置高，灯灭	
	
}
 
