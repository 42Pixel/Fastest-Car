#include "Key.h"

#define KEY1_GPIO               (P20_6)
#define KEY2_GPIO               (P20_7)
#define KEY3_GPIO               (P11_2)
#define KEY4_GPIO               (P11_3)
#define SWITCH1                 (P33_12)
#define SWITCH2                 (P33_13)
#define BUZZER_PIN              (P33_10)

uint8 Key_Num=0;
uint8 Run_Status=0;

Key_status Status={1,1,1,1};        // ��ǰ����״̬
Last_status L_status;               // ��һ�ΰ���״̬
Key_flag Flag;                      // ����������־λ

//----------------------------------------------------------------------------------------------------------------
// �������     ����&��������ʼ��
// ����˵��
// ���ز���
// ʹ��ʾ��     Key_Init();
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
void Key_Init(void){
    gpio_init(KEY1_GPIO, GPI, 0, GPI_PULL_UP);
    gpio_init(KEY2_GPIO, GPI, 0, GPI_PULL_UP);
    gpio_init(KEY3_GPIO, GPI, 0, GPI_PULL_UP);
    gpio_init(KEY4_GPIO, GPI, 0, GPI_PULL_UP);
    gpio_init(SWITCH1, GPI, GPIO_HIGH, GPI_FLOATING_IN);    // ��ʼ�� SWITCH1 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(SWITCH2, GPI, GPIO_HIGH, GPI_FLOATING_IN);    // ��ʼ�� SWITCH2 ���� Ĭ�ϸߵ�ƽ ��������

    gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);            // ��ʼ�� BUZZER_PIN ��� Ĭ�ϵ͵�ƽ �������ģʽ
    gpio_set_level(BUZZER_PIN, GPIO_LOW);
}

//----------------------------------------------------------------------------------------------------------------
// �������     ��������
// ����˵��
// ���ز���
// ʹ��ʾ��     Beep();
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
void Beep(void){
    gpio_set_level(BUZZER_PIN, GPIO_HIGH);
    system_delay_ms(10);
    gpio_set_level(BUZZER_PIN, GPIO_LOW);
}


void Key_Active(uint8 num){



    switch (num){
        case 1:
                Beep();
                s_pid_KP+=0.5;

                Flag.key1 = 0;                  // ʹ�ð���֮��Ӧ�������־λ

                break;
        case 2:
                Beep();
                s_pid_KD+=0.1;
                Flag.key2 = 0;                 // ʹ�ð���֮��Ӧ�������־λ

                break;
        case 3:
                Beep();
//                Flsh_Wite();
                Flag.key2 = 0;                 // ʹ�ð���֮��Ӧ�������־λ

                break;
        case 4:
                 Beep();


                 Flag.key4 = 0;                 // ʹ�ð���֮��Ӧ�������־λ

    }
    Key_Num=0;
}


//----------------------------------------------------------------------------------------------------------------
// �������     ����ɨ��
// ����˵��
// ���ز���
// ʹ��ʾ��     Key_scan();
// ��ע��Ϣ     ���Է��ж�;
//--------------------------------------------------------------------------------------------------------------
void Key_scan(void)
{

    if(gpio_get_level(SWITCH1))Run_Status=1;
//    if(gpio_get_level(SWITCH2))

    L_status.key1 = Status.key1;                              // ���水��״̬
    Status.key1 = gpio_get_level(KEY1_GPIO);                  // ��ȡ��ǰ����״̬
    if(Status.key1 && !L_status.key1)                         // ����ɿ������Ķ�����   ��λ��־λ
    {
        Key_Num=1;                                            // ��־λ ��λ ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
    }

    L_status.key2 = Status.key2;                              // ���水��״̬
    Status.key2 = gpio_get_level(KEY2_GPIO);                  // ��ȡ��ǰ����״̬
    if(Status.key2 && !L_status.key2)                         // ����ɿ������Ķ�����   ��λ��־λ
    {
        Key_Num=2;                                            // ��־λ ��λ ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
    }

    L_status.key3 = Status.key3;                              // ���水��״̬
    Status.key3 = gpio_get_level(KEY3_GPIO);                  // ��ȡ��ǰ����״̬
    if(Status.key3 && !L_status.key3)                         // ����ɿ������Ķ�����   ��λ��־λ
    {
        Key_Num=3;                                            // ��־λ ��λ ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
    }

    L_status.key4 = Status.key4;                             // ���水��״̬
    Status.key4 = gpio_get_level(KEY4_GPIO);                 // ��ȡ��ǰ����״̬
    if(Status.key4 && !L_status.key4)                        // ����ɿ������Ķ�����   ��λ��־λ
    {
        Key_Num=4;                                           // ��־λ ��λ ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
    }

    Key_Active(Key_Num);
}


