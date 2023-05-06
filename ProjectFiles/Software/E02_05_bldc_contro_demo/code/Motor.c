#include "Motor.h"

#define MAX_DUTY           (30 )                                              // ���ռ�ձ��������
#define DIR_CH             (P02_6)                                            // �����������˿�
#define PWM_CH             (ATOM0_CH7_P02_7)                                  // PWM����˿�

#define ENCODER_TIM        (TIM5_ENCODER)                                     // ��������ʱ��
#define ENCODER_PLUS       (TIM5_ENCODER_CH1_P10_3)                           // �����������˿�
#define ENCODER_DIR        (TIM5_ENCODER_CH2_P10_1)                           // �����������ֵ�˿�

int8 Motor_Duty;
int8 Speed_Target;
int16 encoder;
bool dir = true;



float Position_KP =10;
float Position_KI =0;
float Position_KD =5;


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
    encoder_dir_init(ENCODER_TIM, ENCODER_PLUS, ENCODER_DIR);                // ��ʼ����������ֵ���ż���ʱ��
}


//----------------------------------------------------------------------------------------------------------------
// �������     ���PID����
// ����˵��     Encoder            ��������ֵ
// ����˵��     Target             Ŀ����ֵ
// ���ز���     duty
// ʹ��ʾ��     Motor_PID(float Encoder,float Target);
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
int Motor_PID(int Encoder,int Target){

        static int duty,Bias,Integral_bias,Last_Bias;
        Bias=Target - Encoder;
        Integral_bias+=Bias;
        duty=Position_KP*Bias+Position_KD*(Bias-Last_Bias);
        Last_Bias=Bias;
        return duty;
 }


//----------------------------------------------------------------------------------------------------------------
// �������     ��ȡ��������ֵ
// ����˵��     encoder            ��������ֵ
// ���ز���     encoder
// ʹ��ʾ��     Encoder_Get();
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
int16 Encoder_Get(void){
    encoder = encoder_get_count(ENCODER_TIM);                               // �ɼ���Ӧ����������
    encoder_clear_count(ENCODER_TIM);                                          // �����Ӧ����
    return  encoder;
}


//----------------------------------------------------------------------------------------------------------------
// �������     �������
// ����˵��     Encoder            ��������ֵ
// ����˵��     Target             Ŀ����ֵ
// ���ز���     duty
// ʹ��ʾ��     Motor_PID(float Encoder,float Target);
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
void Motor_Control(void){
    Motor_Duty=Motor_PID(Encoder_Get(),Speed_Target);

    pwm_set_duty(PWM_CH, Motor_Duty * (PWM_DUTY_MAX / 100));                     // ����ռ�ձ�
    gpio_set_level(DIR_CH, 0);
}



