# include "Filter.h"

int IMU_G_X=0, IMU_G_Y=0, IMU_G_Z=0;
int IMU_A_X=0, IMU_A_Y=0, IMU_A_Z=0;
float cpm_k = 0.85;             //�����˲�ϵ��
float cpm_angle;                //���׻����˲��˲�ֵ
KFP KFP_IMU_G={0.02,0,0,0,0.001,0.543};
KFP KFP_IMU_M={0.02,0,0,0,0.001,0.543};


/*******************************************************************************************************************************
// �������     �������˲�
// ����˵��     Kalman *kfp �������ṹ�����
// �������     float input ��Ҫ�˲��Ĳ����Ĳ���ֵ�����������Ĳɼ�ֵ��
// ���ز���     �˲���Ĳ���������ֵ��
// ʹ��ʾ��
// ��ע��Ϣ
********************************************************************************************************************************/
float KalmanFilter(KFP *kfp,float input){
     //Ԥ��Э����̣�kʱ��ϵͳ����Э���� = k-1ʱ�̵�ϵͳЭ���� + ��������Э����
     kfp->Now_P = kfp->LastP + kfp->Q;
     //���������淽�̣����������� = kʱ��ϵͳ����Э���� / ��kʱ��ϵͳ����Э���� + �۲�����Э���
     kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
     //��������ֵ���̣�kʱ��״̬����������ֵ = ״̬������Ԥ��ֵ + ���������� * ������ֵ - ״̬������Ԥ��ֵ��
     kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//��Ϊ��һ�ε�Ԥ��ֵ������һ�ε����ֵ
     //����Э�����: ���ε�ϵͳЭ����� kfp->LastP ����һ������׼����
     kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
     return kfp->out;
 }


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ͨ�˲�
//  @param      ���˲���ֵ
//  @return     �˲�ֵ
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
float RCFilter(float value,RC_Filter_pt Filter)
{
    Filter->temp = value;
    Filter->value = (1 - Filter->RC) * Filter->value + Filter->RC * Filter->temp;
    return Filter->value;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���׻����˲� | yaw��
//  @param      now_angle            Ӧ���ɼ��ٶȼƵõ�angle����ʹ�������ǽ��в��������Ǽ��ٶȼƵò���yaw�ǣ����ܻ��ǵ�ʹ�ô�����
//  @param      now_rate             �����ǵõ��Ľ��ٶ�
//  @param      dt                   ����ʱ��
//  @return     cpm_angle            �˲�ֵ
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
float complementary_filter2(float now_angle, float now_rate, float dt)
{
    float y1=0,x1,x2;
    x1 = (now_angle - cpm_angle) * cpm_k * cpm_k;
    y1 = y1 + x1 * dt;
    x2 = y1 + 2 * cpm_k *(now_angle - cpm_angle) + now_rate;
    cpm_angle = cpm_angle + x2 * dt;
    return cpm_angle;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����һ��ֵ����һ��ֵ������ֵȡƽ�� | �����˲�
//  @param      ���˲���ֵ
//  @return
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------

float Movingaverage_filter(float value,float Filter_buff[])
{
    int8_t i = 0;//����
    float temp = value;
    float Filter_sum = 0;
    Filter_buff[Filter_N] = temp;

    for(i = 0; i < Filter_N; i++)
    {
        Filter_buff[i] = Filter_buff[i+1];      //��������
        Filter_sum += Filter_buff[i];
    }
    temp = Filter_sum / Filter_N;
    return temp;
}


/*******************************************************************************************************************************
// �������     ��̬�������������˲�
// ����˵��
// �������
// ���ز���     �˲���Ĳ���������ֵ��
// ʹ��ʾ��
// ��ע��Ϣ     ���ж�
********************************************************************************************************************************/
void IMU_Filter(void){
    IMU_G_X=KalmanFilter(&KFP_IMU_G,(float)imu963ra_gyro_x);
    IMU_G_Y=KalmanFilter(&KFP_IMU_G,(float)imu963ra_gyro_y);
    IMU_G_Z=KalmanFilter(&KFP_IMU_G,(float)imu963ra_gyro_z);

    IMU_A_X=KalmanFilter(&KFP_IMU_M,(float)imu963ra_acc_x);
    IMU_A_Y=KalmanFilter(&KFP_IMU_M,(float)imu963ra_acc_y);
    IMU_A_Z=KalmanFilter(&KFP_IMU_M,(float)imu963ra_acc_z);
}
