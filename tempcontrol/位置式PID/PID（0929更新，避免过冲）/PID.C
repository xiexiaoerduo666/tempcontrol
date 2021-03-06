#include "pid.h"

PID pid; //存放PID算法所需要的数据


void PID_Calc()  //pid计算
{
 float DelEk;
	float ti,ki;
//	float Iout;
//	float Pout;
//	float Dout;
	float td;
	float kd;
	float out;
 if(pid.C10ms<(pid.T))  //计算周期未到
 {
    return ;
 }

 pid.Ek=pid.Sv-pid.Pv;   //得到当前的偏差值
 pid.Pout=pid.Kp*pid.Ek;      //比例输出
 
 pid.SEk+=pid.Ek;        //历史偏差总和
 
 DelEk=pid.Ek-pid.Ek_1;  //最近两次偏差之差
 
 ti=pid.T/pid.Ti;
 ki=ti*pid.Kp;
 
  pid.Iout=ki*pid.SEk*pid.Kp;  //积分输出

 td=pid.Td/pid.T;
 
 kd=pid.Kp*td;
 
  pid.Dout=kd*DelEk;    //微分输出
 
 out= pid.Pout+ pid.Iout+ pid.Dout;
 
 //////////////////////////////////////////////////////////
 
 if(out>pid.pwmcycle)
 {
  pid.OUT=pid.pwmcycle;
 }
 else if(out<0)
 {
  pid.OUT=pid.OUT0; 
 }
 else 
 {
  pid.OUT=out;
 }
 //pid.OUT+=; //本次的计算结果
 pid.Ek_1=pid.Ek;  //更新偏差
 
 pid.C10ms=0;
}


void PID_Calc_overshoot()  //pid计算-过冲处理�
{
 float DelEk;
	float ti,ki;
//	float Iout;
//	float Pout;
//	float Dout;
	float td;
	float kd;
	float out;
  float SvSave;
 if(pid.C10ms<(pid.T))  //计算周期未到
 {
    return ;
 }
 
 
//判断Sv
  SvSave=pid.Sv;
 
 if(pid.Pv<(SvSave*0.55))//温度达到计数开始 66度
 {
    pid.times=0;
 }
 
  if(pid.times<=60)//3分钟 72度  100s
 {
  pid.Sv=SvSave*0.6;
 }
 
 if((pid.times>60)&&(pid.times<=100))//3分钟 96度  100s
 {
  pid.Sv=SvSave*0.8;
 }
 
 if((pid.times>100)&&(pid.times<=200))//2分钟 102度 100s
 {
  pid.Sv=SvSave*0.85;
 }
 
 if((pid.times>200)&&(pid.times<=300))//2分钟 108度 100s
 {
  pid.Sv=SvSave*0.9;
 }
 
  if((pid.times>300)&&(pid.times<=400))//2分钟 114度 100s
 {
  pid.Sv=SvSave*0.95;
 }
 
 if(pid.times>400)
 {
  pid.Sv=SvSave;
 }
//////////////////////////////////


 pid.Ek=pid.Sv-pid.Pv;   //得到当前的偏差值
 pid.Pout=pid.Kp*pid.Ek;      //比例输出
 
 pid.SEk+=pid.Ek;        //历史偏差总和
 
 DelEk=pid.Ek-pid.Ek_1;  //最近两次偏差之差
 
 ti=pid.T/pid.Ti;
 ki=ti*pid.Kp;
 
  pid.Iout=ki*pid.SEk*pid.Kp;  //积分输出

 td=pid.Td/pid.T;
 
 kd=pid.Kp*td;
 
  pid.Dout=kd*DelEk;    //微分输出
 
 out= pid.Pout+ pid.Iout+ pid.Dout;
 
 //////////////////////////////////////////////////////////
 
 if(out>pid.pwmcycle)
 {
  pid.OUT=pid.pwmcycle;
 }
 else if(out<0)
 {
  pid.OUT=pid.OUT0; 
 }
 else 
 {
  pid.OUT=out;
 }
 //pid.OUT+=; //本次的计算结果
 pid.Ek_1=pid.Ek;  //更新偏差
 
 pid.C10ms=0;
 pid.Sv=SvSave;
}












