#ifndef _FILTER_H_
#define _FILTER_H_

#include "zf_common_headfile.h"
#include <stdint.h>

#define Filter_N 10 //������ȡ10����������ֵ

typedef struct
{
    float LastP;                //�ϴι���Э���� ��ʼ��ֵΪ0.02
    float Now_P;                //��ǰ����Э���� ��ʼ��ֵΪ0
    float out;                  //�������˲������ ��ʼ��ֵΪ0
    float Kg;                   //���������� ��ʼ��ֵΪ0
    float Q;                    //��������Э���� ��ʼ��ֵΪ0.001
    float R;                    //�۲�����Э���� ��ʼ��ֵΪ0.543
}KFP;                           //Kalman Filter parameter

struct RC_Para
{
    float temp;                 //�ݴ�ֵ,�洢RC_baro
    float value;                //�˲�ֵ
    float RC;                   //��ͨ�˲�����
};
typedef struct RC_Para *RC_Filter_pt;



float Kalmen_getAngle(float now_angle, float now_rate,float dt);
float complementary_filter2(float now_angle, float now_rate, float dt);
float Movingaverage_filter(float value,float Filter_buff[]);
float RCFilter(float value,RC_Filter_pt Filter);


extern float KalmanFilter(KFP *kfp,float input);
extern void IMU_Filter(void);
extern int IMU_G,IMU_M;
extern float yaw_Filter;

#endif

