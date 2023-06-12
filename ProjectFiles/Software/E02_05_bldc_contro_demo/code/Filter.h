#ifndef _FILTER_H_
#define _FILTER_H_

#include "zf_common_headfile.h"

typedef struct
{
    float LastP;//�ϴι���Э���� ��ʼ��ֵΪ0.02
    float Now_P;//��ǰ����Э���� ��ʼ��ֵΪ0
    float out;//�������˲������ ��ʼ��ֵΪ0
    float Kg;//���������� ��ʼ��ֵΪ0
    float Q;//��������Э���� ��ʼ��ֵΪ0.001
    float R;//�۲�����Э���� ��ʼ��ֵΪ0.543
}KFP;//Kalman Filter parameter

extern float KalmanFilter(KFP *kfp,float input);
extern void IMU_Filter(void);
extern int IMU_G,IMU_M;
#endif

