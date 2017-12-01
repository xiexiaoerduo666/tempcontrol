
#include "led.h"
#include "12864.h"
#include "time_init.h"
#include "max6675.h"
#include "pid.h"
#include "pidout.h"

u16 Kms10;
void delay(u32 x)
{
  while(x--);
}

void delay_us(u16 us)
{
	 u8 i;
  while(us--)
	{
	  for(i=0;i<6;i++)
		{
		
		}
	}
}

void Isr_Init()//中断初始化
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;// TIM4_IRQChannel; 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
NVIC_Init (&NVIC_InitStructure); 


NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;// TIM3_IRQChannel; 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
NVIC_Init (&NVIC_InitStructure);
	
}

 void TIM4_IRQHandler() //10ms 1次
{					  
  
	static u8 tsec; u8 st;
	st=TIM_GetFlagStatus(TIM4, TIM_IT_Update);//中断标志
	if(st!=0)
	{  TIM_ClearFlag(TIM4, TIM_IT_Update); //更新时间
		Kms10++;//max6675的200ms
//    pid.C10ms++;
		if(tsec++>=100)
		{
		  tsec=0;

		}
	}
}

 void TIM3_IRQHandler() //1ms 1次
{					  
   u8 st;
	st=TIM_GetFlagStatus(TIM3, TIM_IT_Update);
	if(st!=0)
	{  pid.C10ms++;//pid计算周期的
		 TIM_ClearFlag(TIM3, TIM_IT_Update); 
     PID_out(); //输出PID运算结果到负载
	}
}

void PID_Init()
{
  pid.Sv=120;//用户设定温度120'C
	pid.Kp=30;//比例系数
	pid.T=500;//PID计算周期500ms
  pid.Ti=5000000;//积分时间5000s
	pid.Td=1000;//微分时间1s
	pid.pwmcycle=200;//pwm周期1000
	pid.OUT0=1;
}


void Display()
{
	Show_string1(0x90,"设定值:");
	Show_number(0X94,pid.Sv);	
	
	Show_string1(0x88,"当前值:");
	Show_number(0X8c,pid.Pv);
	
	Show_string1(0x98,"PIDOUT:");
	Show_number(0X9c,pid.OUT);	
}


int main()
{
   LCD_Init();//12864初始化
	 Show_string1(0x80,"位置式PID");//液晶第一行地址显示字符串
	 Timer4_init();	//T4 10ms时钟	更新
	 Isr_Init();//	T4 中断打开
	 Max6675_Init();//6675初始化
	 PID_Init();
	 PIDOUT_init();
	 Timer3_init();	//T3 1ms时钟
   while(1)
   {
    read_temper();//读取当前温度
		PID_Calc(); //pid计算 
		Display(); 
   }
}












