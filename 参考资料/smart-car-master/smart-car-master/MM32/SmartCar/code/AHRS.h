//
// Created by FLY on 2022/5/7.
//

#ifndef _AHRS_H
#define _AHRS_H

#include "zf_common_headfile.h"
#include "math.h"
#define delta_T     0.001f  // ��������1ms ��Ƶ��1KHZ

typedef struct
{
    float q0;
    float q1;
    float q2;
    float q3;
}Q4_t;	//��Ԫ��

typedef struct vectorxyz
{
    float x;
    float y;
    float z;
}vectorxyz;	//��Ȼ����ϵʸ���ṹ��

typedef struct
{
    vectorxyz a;
    vectorxyz b;
    vectorxyz c;
}Matrix_t;	//�����������

typedef struct
{
    void (* update)(void);

}AHRS_t;

void icmAHRSupdate();
#endif //MM32_AHRS_H
