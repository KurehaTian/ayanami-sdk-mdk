//
//���ļ�����λ��ʽPID��Positional PID���㷨������ʽ��incremental PID��PID�㷨
//�û��ɿ����ⲿ����������ṹ��͵��ñ�ͷ�ļ�������ʹ������PID
//λ��ʽPID�Ĳ������ڷ������ȵ���KP�������ֶ����ٵ���KI��KIһ��Ϊ0.0������������KD��KDһ��ܴ�
//����ʽPID�Ĳ������ڷ������ȵ���KI���ٵ���KP���������������ʽPID�Ϻã�

#include "aya_common_pid.h"

//PID�ṹ���ʼ��˵��,��λ��ʽPIDΪ��:

/**********λ��ʽPID����**********/
//u(k) = Kp*e(k) + Ki*��e + Kd*��e(k) , ʽ�Ц�e(k)=e(k)-e(k-1)
//����λ��ʽPID����
void SetPosiPidParm(PosiPidNode *pid, float kp, float ki, float kd)
{
    //���Ʒ�Χ
    if (kp < 0)
    {
        kp = 0;
    }
    if (ki < 0)
    {
        ki = 0;
    }
    if (kd < 0)
    {
        kd = 0;
    }
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

//����λ��ʽPID���
//�������
//expc:����ֵ
//curr:��ǰֵ
float CalcPosiPidOut(PosiPidNode *pid, float expc, float curr)
{
    float ep = pid->kp, ei = pid->ki, ed = pid->kd;
    float posi_out = 0;

    pid->error_curr = expc - curr;
    pid->sum_error += pid->error_curr;

    //�Ի��ֽ����޷�
    if (pid->sum_error > pid->limit_integral_abs) 
    { 
        pid->sum_error=pid->limit_integral_abs; 
    } 
    else if (pid->sum_error < -pid->limit_integral_abs) 
    { 
        pid->sum_error=-pid->limit_integral_abs; 
    } 
 
    ep = pid->error_curr;
    ei = pid->sum_error;
    ed = pid->error_curr - pid->error_last;

    posi_out = pid->kp * ep + pid->ki * ei + pid->kd * ed;
    if (posi_out > pid->limit_out_abs)
    {
        posi_out = pid->limit_out_abs;
    }
    else if (posi_out < -pid->limit_out_abs)
    {
        posi_out = -pid->limit_out_abs;
    }

    pid->error_last = pid->error_curr;

    return posi_out;
}

/**********λ��ʽPD����**********/
//u(k) = Kp*e(k) + Kd*��e(k) , ʽ�Ц�e(k)=e(k)-e(k-1)
//����λ��ʽPD����
void SetPosiPdParm(PosiPidNode *pid, float kp, float kd)
{
    //���Ʒ�Χ
    if (kp < 0)
    {
        kp = 0;
    }
    if (kd < 0)
    {
        kd = 0;
    }
    pid->kp = kp;
    pid->kd = kd;
}

//����λ��ʽPD���
float CalcPosiPdOut(PosiPidNode *pid, float expc, float curr)
{
    float ep = 0, ed = 0;
    float posi_out = 0;

    pid->error_curr = expc - curr;

    ep = pid->error_curr;
    ed = pid->error_curr - pid->error_last;

    posi_out = pid->kp * ep + pid->kd * ed;
    if (posi_out > pid->limit_out_abs)
    {
        posi_out = pid->limit_out_abs;
    }
    else if (posi_out < -pid->limit_out_abs)
    {
        posi_out = -pid->limit_out_abs;
    }

    pid->error_last = pid->error_curr;

    return posi_out;
}

/**********����ʽPID����**********/
//��u(k)=u(k)-u(k-1) = Kp*��e(k)+ Ki*e(k)+ Kd*[��e(k)-��e(k-1)] ��ʽ�Ц�e(k)=e(k)-e(k-1)

//��������ʽPID����
void SetInrcPidParm(InrcPidNode *pid, float kp, float ki, float kd)
{
    //���Ʒ�Χ
    if (kp < 0)
    {
        kp = 0;
    }
    if (ki < 0)
    {
        ki = 0;
    }
    if (kd < 0)
    {
        kd = 0;
    }
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

//��������ʽPIDĿ������
//�������
//expc:����ֵ
//curr:��ǰֵ
float CalcInrcPidOut(InrcPidNode *pid, float expc, float curr)
{
    float ep = 0, ei = 0, ed = 0;
    float inrc_out = 0;

    pid->error_curr = expc - curr;

    ep = pid->error_curr - pid->error_last;
    ei = pid->error_curr;
    ed = pid->error_curr - 2 * pid->error_last + pid->error_pre;

    inrc_out = pid->kp * ep + pid->ki * ei + pid->kd * ed;
    if (inrc_out > pid->limit_out_abs)
    {
        inrc_out = pid->limit_out_abs;
    }
    else if (inrc_out < -pid->limit_out_abs)
    {
        inrc_out = -pid->limit_out_abs;
    }

    pid->error_pre = pid->error_last;
    pid->error_last = pid->error_curr;

    return inrc_out;
}