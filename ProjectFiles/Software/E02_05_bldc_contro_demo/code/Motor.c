#include "Motor.h"

#define MAX_DUTY           (30 )                                              // ���ռ�ձ��������
#define DIR_CH             (P02_6)                                            // �����������˿�
#define PWM_CH             (ATOM0_CH7_P02_7)                                  // PWM����˿�

#define ENCODER_TIM        (TIM5_ENCODER)                                     // ��������ʱ��
#define ENCODER_PLUS       (TIM5_ENCODER_CH1_P10_3)                           // �����������˿�
#define ENCODER_DIR        (TIM5_ENCODER_CH2_P10_1)                           // �����������ֵ�˿�

#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                         // ���������϶����Ӧ����
#define SERVO_MOTOR_FREQ            (50 )                                     // ���������϶��Ƶ��  �����ע�ⷶΧ 50-300

#define SERVO_MOTOR_L_MAX           (50 )                                     // ���������϶�����Χ �Ƕ�
#define SERVO_MOTOR_R_MAX           (150)                                     // ���������϶�����Χ �Ƕ�

#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))        //���PWM����

#if (SERVO_MOTOR_FREQ<50 || SERVO_MOTOR_FREQ>300)
    #error "SERVO_MOTOR_FREQ ERROE!"
#endif

float s_pid_KP=0.0;                                                           //���PD����
float s_pid_KD=0.0;

int8 Speed_Duty=20;                                                           // �ٶ��趨ֵ
int16 Encoder;                                                                // ����������



//----------------------------------------------------------------------------------------------------------------
// �������     ���&��������ʼ��
// ����˵��
// ���ز���
// ʹ��ʾ��     Motor_Init();
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
void Motor_Init(void){
    pwm_init(PWM_CH, 1000, 0);                                                 // PWM ͨ��1 ��ʼ��Ƶ��1KHz ռ�ձȳ�ʼΪ0
    gpio_init(DIR_CH, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // ��ʼ����������������

    encoder_dir_init(ENCODER_TIM, ENCODER_PLUS, ENCODER_DIR);                  // ��ʼ����������ֵ���ż���ʱ��

    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);                            // ���PWM��ʼ��
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100.0));            // �������
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
// �������     ���PID
// ����˵��
// �������     set                    Ŀ��ֵ
// �������     NowPoint               ��ǰֵ
// ���ز���     output                 PD���ֵ
// ʹ��ʾ��     �ڲ�����
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
int8 PID_Servo_Contrl(float SetPoint,float NowPoint){
    static float iError,LastError,PrevError;                            //iError:���,LastError:�ϴ������ϴ����
    float output;                                                       //���
    iError = SetPoint - NowPoint;                                       //��ǰ���  �趨��Ŀ��ֵ��ʵ��ֵ��ƫ��
    output = s_pid_KP * iError+ s_pid_KD * (iError - LastError);        //��������

    /*�洢���  �����´μ���*/
    PrevError = LastError;
    LastError = iError;
    return (int)output;                                                 //����λ��ֵ
}


//----------------------------------------------------------------------------------------------------------------
// �������     �������
// ����˵��
// ���ز���
// ʹ��ʾ��     Servo_Motor_Control();
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
void Servo_Motor_Control(void){
    float servo_duty;                                                    // ��������Ƕ�
    servo_duty=100+PID_Servo_Contrl(100,101);                            // PD����
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(servo_duty)); // ���ö��PWM
}

