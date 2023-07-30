//
//本文件包括位置式PID（Positional PID）算法和增量式（incremental PID）PID算法
//用户可可再外部函数声明其结构体和调用本头文件函数来使用两种PID
//位置式PID的参数调节方法：先调节KP，待出现抖动再调节KI（KI一般为0.0几），最后调节KD（KD一般很大）
//增量式PID的参数调节方法：先调节KI，再调节KP（步进电机用增量式PID较好）
//注意：kp,ki,kd都必须大于0

#ifndef _PID_H_
#define _PID_H_

////PID结构体初始化说明,以位置式PID为例:
//PosiPidNode PosiPid=
//{
//    .kp = 6.0f,
//    .ki = 0.01f,
//    .kd = 10.0f,
//
//    .error_curr = 0.0f,
//    .error_last = 0.0f,
//    .sum_error = 0.0f,
//
//    .limit_out_abs = 1950
//};



//
//位置式PID参数结构体
//u(k) = Kp*e(k) + Ki*∑e + Kd*Δe(k) , 式中Δe(k)=e(k)-e(k-1)
typedef struct
{
    float kp;
    float ki;
    float kd;

    float error_curr;   //当前误差
    float error_last;   //上一次的误差
    float sum_error;    //误差和

    float limit_out_abs;    //输出限幅
    float limit_integral_abs;   //积分限幅
}PosiPidNode;


//
//增量式PID参数结构体
//Δu(k)=u(k)-u(k-1) = Kp*Δe(k)+ Ki*e(k)+ Kd*[Δe(k)-Δe(k-1)] ，式中Δe(k)=e(k)-e(k-1)
typedef struct
{
    float kp;
    float ki;
    float kd;

    float error_curr;   //当前误差（e_0）
    float error_last;   //上一次误差(e_1)
    float error_pre;    //前一次的偏差(e_2)

    float limit_out_abs;    //输出限幅
    float limit_integral_abs;

}InrcPidNode;

//位置式PID
void SetPosiPidParm(PosiPidNode *pid, float kp, float ki, float kd);//设置位置式PID参数
float CalcPosiPidOut(PosiPidNode *pid, float expc, float curr);//计算位置式PID输出

//位置式PD
void SetPosiPdParm(PosiPidNode *pid, float kp, float kd);//设置位置式PD参数
float CalcPosiPdOut(PosiPidNode *pid, float expc, float curr);//计算位置式PD输出

//增量式PID
void SetInrcPidParm(InrcPidNode *pid, float kp, float ki, float kd);//设置PID参数
float CalcInrcPidOut(InrcPidNode *pid, float expc, float curr);//计算目标增量





#endif