#include "zf_common_headfile.h"
#include "isr_config.h"
#pragma section all "cpu0_dsram"

#define IPS200_TYPE          (IPS200_TYPE_PARALLEL8)                            // ˫������ ���������� ����궨����д IPS200_TYPE_PARALLEL8

#define MAX_DUTY            (30 )                                              // ���ռ�ձ��������
#define DIR_CH1             (P02_6)                                            // �����������˿�
#define PWM_CH1             (ATOM0_CH7_P02_7)                                  // PWM����˿�



#define ENCODER1_TIM        (TIM5_ENCODER)                                     // ��������ʱ��
#define ENCODER1_PLUS       (TIM5_ENCODER_CH1_P10_3)                           // �����������˿�
#define ENCODER1_DIR        (TIM5_ENCODER_CH2_P10_1)                           // �����������ֵ�˿�


int8 duty = 0;                                                                 // ��ǰռ�ձ�
bool dir = true;                                                               // ��������
int16 encoder =0;                                                        // ������ֵ

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

    ips200_set_dir(IPS200_PORTAIT);
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);
    ips200_init(IPS200_TYPE);

    pwm_init(PWM_CH1, 1000, 0);                                                 // PWM ͨ��1 ��ʼ��Ƶ��1KHz ռ�ձȳ�ʼΪ0
    gpio_init(DIR_CH1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // ��ʼ����������������
    encoder_dir_init(ENCODER1_TIM, ENCODER1_PLUS, ENCODER1_DIR);                // ��ʼ����������ֵ���ż���ʱ��



    pit_ms_init(CCU60_CH0, 100);

    // �˴���д�û����� ���������ʼ�������
	cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
	while (TRUE)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���



        ips200_show_int (0,16*0,encoder,6);

        // �˴���д��Ҫѭ��ִ�еĴ���
	}
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                                                 // �����ж�Ƕ��
    pit_clear_flag(CCU60_CH0);


    if(duty >= 0)                                                               // ��ת
    {
        pwm_set_duty(PWM_CH1, duty * (PWM_DUTY_MAX / 100));                     // ����ռ�ձ�
        gpio_set_level(DIR_CH1, 1);

    }
    else                                                                        // ��ת
    {
        pwm_set_duty(PWM_CH1, -duty * (PWM_DUTY_MAX / 100));
        gpio_set_level(DIR_CH1, 0);

    }
    if(dir)                                                                     // ���ݷ����жϼ������� �����̽����ο�
    {
        duty ++;                                                                // �������
        if(duty >= MAX_DUTY)                                                    // �ﵽ���ֵ
        dir = false;                                                            // �����������
    }
    else
    {
        duty --;                                                                // �������
        if(duty <= -MAX_DUTY)                                                   // �ﵽ��Сֵ
        dir = true;                                                             // �����������
    }

    encoder = encoder_get_count(ENCODER1_TIM);                               // �ɼ���Ӧ����������

    encoder_clear_count(ENCODER1_TIM);                                          // �����Ӧ����


}


#pragma section all restore
// **************************** �������� ****************************
// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1������û������
//      �鿴�������ִ򿪵��Ƿ�����ȷ�Ĵ��ڣ����򿪵� COM ���Ƿ��Ӧ���ǵ������������� USB-TTL ģ��� COM ��
//      �����ʹ����ɿƼ� Ӣ����TriCore �������������ӣ���ô������������Ƿ��ɶ��������İ崮�������Ƿ��Ѿ����ӣ��������߲鿴���İ�ԭ��ͼ�����ҵ�
//      �����ʹ�� USB-TTL ģ�����ӣ���ô��������Ƿ������Ƿ��ɶ���ģ�� TX �Ƿ����ӵĺ��İ�� RX��ģ�� RX �Ƿ����ӵĺ��İ�� TX
// ����2��������������
//      �鿴�����������õĲ������Ƿ����������һ�£������� zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨��Ϊ debug uart ʹ�õĴ��ڲ�����
// ����3����ˢ����޷�Ӧ
//      ����ź������Ƿ�Ӷԣ��ź����Ƿ��ɶ�
//      ʹ�����ñ����ʵ��PWM��������Ƿ����������ʾ�����鿴����
// ����4����ˢ���ת����ת����ʾ���ٶ�
//      ����ź������Ƿ�Ӷԣ��ź����Ƿ��ɶ�
//      ʹ�����ñ������Ӧ����ˢ����ٶ�����Ƿ����ź�
