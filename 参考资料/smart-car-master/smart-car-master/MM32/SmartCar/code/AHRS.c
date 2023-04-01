//
// Created by FLY on 2022/5/7.
//

#include "ahrs.h"

//Q4_t Q4 = {1, 0, 0, 0};  //��Ԫ��


float I_ex, I_ey, I_ez;  // ������
Q4_t Q_info = {1, 0, 0, 0};  // ��Ԫ����ʼ��
float icm_kp= 0.17;    // ���ٶȼƵ��������ʱ�������
float icm_ki= 0.004;   // �������������ʵĻ�������
/***********************************************************************************************************************
 * @brief �û����˲��㷨������������̬(�����ü��ٶȼ����������ǵĻ������)
 * ���ٶȼƶ���֮��������Ƚ����У��������ݼ��������̬���ţ������Ƕ������������У��������ݿ��ţ�������ʹ�û����������(�ڲ������㷨�Ŵ�̬���)��
 * ���ʹ����̬�����˲����������������ǣ��������ż��ٶȼơ�
 * @tips: n - ��������ϵ�� b - ��������ϵ
 **********************************************************************************************************************/
void icmAHRSupdate()
{
    icmConvert();
    float halfT = 0.5 * delta_T;    // ��������һ��
    float vx, vy, vz;               // ��ǰ��̬��������������������ϵķ���
    float ex, ey, ez;               // ��ǰ���ټƲ�õ��������ٶ��������ϵķ������õ�ǰ��̬��������������������ϵķ��������

    float q0 = Q_info.q0;  //��Ԫ��
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;

    float q0q0 = q0 * q0;  //����ˣ������������
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    // ������ֹ״̬Ϊ-g ����������
    if(icm_data.acc_x * icm_data.acc_y * icm_data.acc_z == 0) // �Ӽƴ�����������״̬ʱ(��ʱg = 0)��������̬���㣬��Ϊ�������ĸ���������
        return;

    // �Լ��ٶ����ݽ��й�һ�� �õ���λ���ٶ� (a^b -> ��������ϵ�µļ��ٶ�)
    float norm = InvSqrt(icm_data.acc_x * icm_data.acc_x + icm_data.acc_y * icm_data.acc_y + icm_data.acc_z * icm_data.acc_z);
    icm_data.acc_x = icm_data.acc_x * norm;
    icm_data.acc_y = icm_data.acc_y * norm;
    icm_data.acc_z = icm_data.acc_z * norm;

    // ��������ϵ���������������ϵķ���
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    // g^b �� a^b ��������ˣ��õ������ǵ�У����������e��ϵ��
    ex = icm_data.acc_y * vz - icm_data.acc_z * vy;
    ey = icm_data.acc_z * vx - icm_data.acc_x * vz;
    ez = icm_data.acc_x * vy - icm_data.acc_y * vx;




    // ����ۼ�
    I_ex += halfT * ex;
    I_ey += halfT * ey;
    I_ez += halfT * ez;

    // ʹ��PI�������������������(������Ư�����)
    icm_data.gyro_x = icm_data.gyro_x + icm_kp* ex + icm_ki* I_ex;
    icm_data.gyro_y = icm_data.gyro_y + icm_kp* ey + icm_ki* I_ey;
    icm_data.gyro_z = icm_data.gyro_z + icm_kp* ez + icm_ki* I_ez;

    // һ����������������Ԫ��΢�ַ��̣�����halfTΪ�������ڵ�1/2��gx gy gzΪbϵ�����ǽ��ٶȡ�
    q0 = q0 + (-q1 * icm_data.gyro_x - q2 * icm_data.gyro_y - q3 * icm_data.gyro_z) * halfT;
    q1 = q1 + (q0 * icm_data.gyro_x + q2 * icm_data.gyro_z - q3 * icm_data.gyro_y) * halfT;
    q2 = q2 + (q0 * icm_data.gyro_y - q1 * icm_data.gyro_z + q3 * icm_data.gyro_x) * halfT;
    q3 = q3 + (q0 * icm_data.gyro_z + q1 * icm_data.gyro_y - q2 * icm_data.gyro_x) * halfT;

    // ��λ����Ԫ���ڿռ���תʱ�������죬������ת�Ƕȣ������㷨�������Դ�����������任
    norm = InvSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    Q_info.q0 = q0 * norm;
    Q_info.q1 = q1 * norm;
    Q_info.q2 = q2 * norm;
    Q_info.q3 = q3 * norm;  // ��ȫ�ֱ�����¼��һ�μ������Ԫ��ֵ

    // atan2�������������������ԭ��������X�������μнǵĻ���ֵ
    eulerAngle.pitch = asin(2 * q0 * q2 - 2 * q1 * q3) * 180 / PI;
    eulerAngle.roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 180 / PI;
    eulerAngle.yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 180 / PI;
}
