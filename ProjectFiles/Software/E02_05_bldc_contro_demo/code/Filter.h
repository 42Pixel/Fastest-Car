#ifndef _FILTER_H_
#define _FILTER_H_

#include "zf_common_headfile.h"

//typedef struct
//{
//    float Last_P;//�ϴι���Э���� ������Ϊ0 ! ! ! ! !
//    float Now_P;//��ǰ����Э����
//    float out;//�������˲������
//    float Kg;//����������
//    float Q;//��������Э����
//    float R;//�۲�����Э����
//}Kalman;

extern KFP_IMU;
extern float KalmanFilter(Kalman *kfp,float input);


#endif

