#ifndef _RUN_H_
#define _RUN_H_

#include "zf_common_headfile.h"
#include <stdint.h>
#include <stdio.h>

#define                 Array_SIZE                  (64)

typedef struct
{
    float LastP;//�ϴι���Э���� ��ʼ��ֵΪ0.02
    float Now_P;//��ǰ����Э���� ��ʼ��ֵΪ0
    float out;//�������˲������ ��ʼ��ֵΪ0
    float Kg;//���������� ��ʼ��ֵΪ0
    float Q;//��������Э���� ��ʼ��ֵΪ0.001
    float R;//�۲�����Э���� ��ʼ��ֵΪ0.543
}KFP;//Kalman Filter parameter

extern uint8 GPS_Count;
extern uint8 Stop_Point;
extern uint8 Back_Point;
extern uint32 Start_Speed;

extern float Direction_E;
extern float direction_N;
extern float GpsOffset;
extern float azimuth_E;
extern float azimuth_N;
extern float distance;

extern double run_latitude[Array_SIZE];
extern double run_longitude[Array_SIZE];

extern void Run_Start(void);
extern void Point_Get(void);
extern void Run_Status_Set(void);

#endif
