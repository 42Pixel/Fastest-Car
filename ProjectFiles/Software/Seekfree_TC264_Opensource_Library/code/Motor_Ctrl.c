#include <Motor_Ctrl.h>

float Servo_pid_KP=2.0;                                                           //���PD����
float Servo_pid_KD=0.2;
uint8 servo_duty;
uint8 Speed_Duty;                                                           // �ٶ��趨ֵ
int16 Encoder;                                                             // ����������

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
    pwm_init(PWM_CH, 1000, 0);                                                 // PWM ͨ��1 ��ʼ��Ƶ��1KHz ռ�ձȳ�ʼΪ0
    gpio_init(DIR_CH, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // ��ʼ����������������

    encoder_dir_init(ENCODER_TIM, ENCODER_PLUS, ENCODER_DIR);                  // ��ʼ����������ֵ���ż���ʱ��

    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);                            // ���PWM��ʼ��
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100));              // �������
}


//----------------------------------------------------------------------------------------------------------------
// �������     ��ȡ��������ֵ
// ����˵��     encoder            ��������ֵ
// ���ز���     void
// ʹ��ʾ��     Encoder_Get();
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
void Encoder_Get(void){
    Encoder = encoder_get_count(ENCODER_TIM);                                  // �ɼ���Ӧ����������
    encoder_clear_count(ENCODER_TIM);                                          // �����Ӧ����
}


//----------------------------------------------------------------------------------------------------------------
// �������     �������
// ����˵��
// ����˵��
// ���ز���
// ʹ��ʾ��     �ڲ�����
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
void Motor_Control(void){
    gpio_set_level(DIR_CH, 0);

    if(Speed_Duty<=0)
       Speed_Duty=0;
    else
        if(Speed_Duty>=MAX_DUTY)
            Speed_Duty=MAX_DUTY;                                                 // ��ֵSpeed_Duty

    pwm_set_duty(PWM_CH, Speed_Duty * (PWM_DUTY_MAX / 100));                     // ����ռ�ձ�
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

