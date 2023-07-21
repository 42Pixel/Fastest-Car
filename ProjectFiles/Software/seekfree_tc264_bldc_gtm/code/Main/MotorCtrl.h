#ifndef _MOTORCTRL_H_
#define _MOTORCTRL_H_

#include "zf_common_headfile.h"

#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                         // ���������϶����Ӧ����
#define SERVO_MOTOR_FREQ            (100 )                                    // ���������϶��Ƶ��  �����ע�ⷶΧ 50-300

//#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))
#define SERVO_MOTOR_DUTY(x)           (1000*(0.5+(float)(x)/90.0))

#define SERVO_MOTOR_L_MAX           (70 )                                     // ���������϶�����Χ �Ƕ�
#define SERVO_MOTOR_R_MAX           (130)                                     // ���������϶�����Χ �Ƕ�

#if (SERVO_MOTOR_FREQ<50 || SERVO_MOTOR_FREQ>300)
    #error "SERVO_MOTOR_FREQ ERROE!"
#endif

extern float Servo_pid_KP;                                                           //���PD����
extern float Servo_pid_KD;
extern uint8 servo_duty;

extern void Servo_Motor_Control(void);
extern void Encoder_Get(void);
extern void MotorCtrl_Init(void);
extern void Motor_Control(void);

#endif /* CODE_MAIN_MOTORCTRL_H_ */
