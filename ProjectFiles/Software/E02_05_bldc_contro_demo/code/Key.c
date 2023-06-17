#include "Key.h"

#define KEY1_GPIO               (P20_6)
#define KEY2_GPIO               (P20_7)
#define KEY3_GPIO               (P11_2)
#define KEY4_GPIO               (P11_3)
#define BUZZER_PIN              (P33_10)

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

//----------------------------------------------------------------------------------------------------------------
// �������     ����ɨ��
// ����˵��
// ���ز���
// ʹ��ʾ��     Check_Key();
// ��ע��Ϣ     ��������;
//---------------------------------------------------------------------------------------------------------------
void Scan_Key(void){
    static uint8 K[4];

    if (gpio_get_level(KEY1_GPIO)==0){
        system_delay_ms(15);
        if (gpio_get_level(KEY1_GPIO)==0)                   //����
            K[Key1] = TRUE;
    }

    if (gpio_get_level(KEY2_GPIO)==0){
        system_delay_ms(15);
        if (gpio_get_level(KEY2_GPIO)==0)                   //����
            K[Key2] = TRUE;
    }

    if (gpio_get_level(KEY3_GPIO)==0){
        system_delay_ms(15);
        if (gpio_get_level(KEY3_GPIO)==0)                   //����
            K[Key3] = TRUE;
    }

    if (gpio_get_level(KEY4_GPIO)==0){
        system_delay_ms(15);
        if (gpio_get_level(KEY4_GPIO)==0)                   //����
            K[Key4] = TRUE;
    }


    if (K[Key1]){
        Beep();
        s_pid_KP+=0.1;
        K[Key1] = FALSE;
    }
    if (K[Key2]){
        Beep();
        s_pid_KP-=0.1;
        K[Key2] = FALSE;
    }
    if (K[Key3]){
        Beep();
        s_pid_KD+=0.1;
        K[Key3] = FALSE;
    }
    if (K[Key4]){
        Beep();
        s_pid_KD-=0.1;
        K[Key4] = FALSE;
    }
}
