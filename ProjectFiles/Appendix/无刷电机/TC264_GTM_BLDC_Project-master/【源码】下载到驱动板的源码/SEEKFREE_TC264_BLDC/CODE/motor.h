#ifndef _MOTOR_H
#define _MOTOR_H

#include "common.h"

#define MOTOR_SPEED_OUT_PIN ATOM2_CH1_P33_5 //电机速度输出引脚
#define MOTOR_DIR_OUT_PIN   P33_11          //电机运行方向输出引脚
#define MOTOR_SWITCH        P11_6           //电机使能开关


typedef enum
{
    FORWARD,    //正转
    REVERSE,    //反转
}MOTOR_DIR_enum;

typedef enum
{
    MOTOR_DISABLE,  //驱动关闭
    MOTOR_ENABLE,   //驱动使能
}MOTOR_EN_STATUS_enum;

typedef struct
{
    MOTOR_EN_STATUS_enum en_status; //指示电机使能状态
    uint8 brake_flag;   //指示当前刹车是否有效    1：正在刹车  0：正常运行
    MOTOR_DIR_enum  dir;//电机旋转方向 FORWARD：正转  REVERSE：反转     BRAKE：刹车
    int32 set_speed;    //设置的速度
    int32 max_speed;    //速度最大值
    int32 min_speed;    //速度最小值
}motor_struct;

extern motor_struct motor_control;
extern int16    duty;           //PWM占空比初值


void motor_init(void);
void motor_information_out_init(void);
void motor_set_dir(void);
void motor_dir_out(void);
void motor_speed_out(void);
void motor_commutation(int8 except_hall);

#endif
