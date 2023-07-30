//
//���ļ�����λ��ʽPID��Positional PID���㷨������ʽ��incremental PID��PID�㷨
//�û��ɿ����ⲿ����������ṹ��͵��ñ�ͷ�ļ�������ʹ������PID
//λ��ʽPID�Ĳ������ڷ������ȵ���KP�������ֶ����ٵ���KI��KIһ��Ϊ0.0������������KD��KDһ��ܴ�
//����ʽPID�Ĳ������ڷ������ȵ���KI���ٵ���KP���������������ʽPID�Ϻã�
//ע�⣺kp,ki,kd���������0

#ifndef _PID_H_
#define _PID_H_

////PID�ṹ���ʼ��˵��,��λ��ʽPIDΪ��:
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
//λ��ʽPID�����ṹ��
//u(k) = Kp*e(k) + Ki*��e + Kd*��e(k) , ʽ�Ц�e(k)=e(k)-e(k-1)
typedef struct
{
    float kp;
    float ki;
    float kd;

    float error_curr;   //��ǰ���
    float error_last;   //��һ�ε����
    float sum_error;    //����

    float limit_out_abs;    //����޷�
    float limit_integral_abs;   //�����޷�
}PosiPidNode;


//
//����ʽPID�����ṹ��
//��u(k)=u(k)-u(k-1) = Kp*��e(k)+ Ki*e(k)+ Kd*[��e(k)-��e(k-1)] ��ʽ�Ц�e(k)=e(k)-e(k-1)
typedef struct
{
    float kp;
    float ki;
    float kd;

    float error_curr;   //��ǰ��e_0��
    float error_last;   //��һ�����(e_1)
    float error_pre;    //ǰһ�ε�ƫ��(e_2)

    float limit_out_abs;    //����޷�
    float limit_integral_abs;

}InrcPidNode;

//λ��ʽPID
void SetPosiPidParm(PosiPidNode *pid, float kp, float ki, float kd);//����λ��ʽPID����
float CalcPosiPidOut(PosiPidNode *pid, float expc, float curr);//����λ��ʽPID���

//λ��ʽPD
void SetPosiPdParm(PosiPidNode *pid, float kp, float kd);//����λ��ʽPD����
float CalcPosiPdOut(PosiPidNode *pid, float expc, float curr);//����λ��ʽPD���

//����ʽPID
void SetInrcPidParm(InrcPidNode *pid, float kp, float ki, float kd);//����PID����
float CalcInrcPidOut(InrcPidNode *pid, float expc, float curr);//����Ŀ������





#endif