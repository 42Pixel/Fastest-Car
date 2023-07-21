#include "MOTORCTRL.H"

float Servo_pid_KP=2.5;                                                           //���PD����
float Servo_pid_KD=0.2;
uint8 servo_duty;

//----------------------------------------------------------------------------------------------------------------
// �������     ���PID
// ����˵��
// �������     SetPoint               Ŀ��ֵ
// �������     NowPoint               ��ǰֵ
// ���ز���     output                 PD���ֵ
// ʹ��ʾ��     �ڲ�����
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
int8 PID_Servo(float SetPoint,float NowPoint){
    static float iError,LastError,PrevError;                            //iError:���,LastError:�ϴ������ϴ����
    float output;                                                       //���
    iError = SetPoint - NowPoint;                                       //��ǰ���  �趨��Ŀ��ֵ��ʵ��ֵ��ƫ��
    output = Servo_pid_KP * iError+ Servo_pid_KD * (iError - LastError);        //��������

    /*�洢���  �����´μ���*/
    PrevError = LastError;
    LastError = iError;
    return (int)output;                                                 //����λ��ֵ
}

//----------------------------------------------------------------------------------------------------------------
// �������     ���&��������ʼ��
// ����˵��
// ���ز���
// ʹ��ʾ��     Motor_Init();
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
void MotorCtrl_Init(void){
//    motor_control.set_dir == REVERSE;
    motor_control.brake_flag=0;                                                // �ر�ɲ��
    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);                            // ���PWM��ʼ��
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100));              // �������
}

//----------------------------------------------------------------------------------------------------------------
// �������     �������
// ����˵��
// ���ز���
// ʹ��ʾ��     Servo_Motor_Control();
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
void Servo_Motor_Control(void){

    if(Servo_Status){
//        float servo_duty;                                                    // ��������Ƕ�
        servo_duty=100+PID_Servo(azimuth_E,eulerAngle.yaw);                          // PD����
        if(servo_duty>=SERVO_MOTOR_R_MAX)
            servo_duty=SERVO_MOTOR_R_MAX;
        else if(servo_duty<=SERVO_MOTOR_L_MAX)
            servo_duty=SERVO_MOTOR_L_MAX;
        pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(servo_duty)); // ���ö��PWM
    }
}



