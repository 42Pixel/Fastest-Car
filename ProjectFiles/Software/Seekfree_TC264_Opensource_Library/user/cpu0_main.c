#include "zf_common_headfile.h"
#include "isr_config.h"
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// ����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ�� interrupt_global_enable(0); �������ж�Ƕ��
// �򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ������� interrupt_global_disable(); ���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ����� interrupt_global_enable(0); �������жϵ���Ӧ��


// **************************** �궨�� ****************************
#define IPS200_TYPE          (IPS200_TYPE_PARALLEL8)                            // ˫������ ���������� ����궨����д IPS200_TYPE_PARALLEL8
                                                                               // �������� SPI ������ ����궨����д IPS200_TYPE_SPI
#define MAX_DUTY             (30 )                                              // ���ռ�ձ��������
#define DIR_CH              (P02_6)                                            // �����������˿�
#define PWM_CH              (ATOM0_CH7_P02_7)                                  // PWM����˿�

#define ENCODER_TIM        (TIM5_ENCODER)                                     // ��������ʱ��
#define ENCODER_PLUS       (TIM5_ENCODER_CH1_P10_3)                           // �����������˿�
#define ENCODER_DIR        (TIM5_ENCODER_CH2_P10_1)                           // �����������ֵ�˿�

// **************************** �궨�� ****************************


// **************************** �������� ****************************
int8 duty = 0;                                                                 // ��ǰռ�ձ�
bool dir = true;                                                               // ��������
int32 encoder = 0;
// **************************** �������� ****************************


// **************************** �������� ****************************
int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
//****************************** ��ʼ�� *****************************

    ips200_set_dir(IPS200_PORTAIT);
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);
    ips200_init(IPS200_TYPE);

    pwm_init(PWM_CH, 1000, 0);                                                 // PWM ͨ��1 ��ʼ��Ƶ��1KHz ռ�ձȳ�ʼΪ0
    gpio_init(DIR_CH, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // ��ʼ����������������
    encoder_dir_init(ENCODER_TIM, ENCODER_PLUS, ENCODER_DIR);                // ��ʼ����������ֵ���ż���ʱ��

    pit_ms_init(CCU60_CH0, 100);

//****************************** ��ʼ�� *****************************
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����

    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        encoder = encoder_get_count(ENCODER_TIM);                               // �ɼ���Ӧ����������
        encoder_clear_count(ENCODER_TIM);                                          // �����Ӧ����
        ips200_show_int (0,16*0,encoder,6);


        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                                                 // �����ж�Ƕ��
    pit_clear_flag(CCU60_CH0);

    pwm_set_duty(PWM_CH, duty * (PWM_DUTY_MAX / 100));                     // ����ռ�ձ�
    gpio_set_level(DIR_CH, 1);
}

#pragma section all restore
// **************************** �������� ****************************
