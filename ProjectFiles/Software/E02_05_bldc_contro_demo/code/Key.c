#include "Key.h"

#define BUZZER_PIN              (P33_10)

void Key_Init(void){
    gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);                        // ��������ʼ�� BUZZER_PIN ��� Ĭ�ϵ͵�ƽ �������ģʽ
    key_init(10);
}



