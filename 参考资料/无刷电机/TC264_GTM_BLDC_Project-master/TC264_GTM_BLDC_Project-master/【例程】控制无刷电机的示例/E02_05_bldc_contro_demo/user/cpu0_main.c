/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu0_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.8.0
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "zf_device_ips200.h"
#include "isr_config.h"
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��


// ���̵��뵽���֮��Ӧ��ѡ�й���Ȼ����refreshˢ��һ��֮���ٱ���
// ����Ĭ������Ϊ�ر��Ż��������Լ��һ�����ѡ��properties->C/C++ Build->Setting
// Ȼ�����Ҳ�Ĵ������ҵ�C/C++ Compiler->Optimization->Optimization level�������Ż��ȼ�
// һ��Ĭ���½����Ĺ��̶���Ĭ�Ͽ�2���Ż�����˴��Ҳ��������Ϊ2���Ż�

// ����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ�� interrupt_global_enable(0); �������ж�Ƕ��
// �򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ������� interrupt_global_disable(); ���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ����� interrupt_global_enable(0); �������жϵ���Ӧ��

// *************************** ����Ӳ������˵�� ***************************
// ���ʹ�õĲ�������ɿƼ� tc264 ���� �밴��������ʽ���н���
//      ģ������    ��Ƭ������
//      PWM         �鿴 main.c �� PWM_CH1  �궨�� Ĭ�� P02_7 ���� P02_5
//      DIR         �鿴 main.c �� DIR_CH1  �궨�� Ĭ�� P02_6 ���� P02_4
//      SPDOUT      �鿴 main.c �� ENCODER1_PLUS �궨�� Ĭ�� P10_3 ���� P33_7
//      DIROUT      �鿴 main.c �� ENCODER1_DIR  �궨�� Ĭ�� P10_1 ���� P33_6
//      GND         ��Դ��


// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����� �����ع��� ���� drv8701 ����
// 2.������˵�� ���Կ��������������ת
// 3.�����ڴ��������Ͽ������´�����Ϣ��
//      bldc encoder counter�� xxx.
// 4.���û�нӵ�� ʹ�����ñ������������������ϲ����������ѹ�仯
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�

// **************************** �������� ****************************

#define MAX_DUTY            (25 )                                              // ���ռ�ձ��������
#define DIR_CH1             (P02_6)                                            // �����������˿�
#define PWM_CH1             (ATOM0_CH7_P02_7)                                  // PWM����˿�


#define ENCODER1_TIM        (TIM5_ENCODER)                                     // ��������ʱ��
#define ENCODER1_PLUS       (TIM5_ENCODER_CH1_P10_3)                           // �����������˿�
#define ENCODER1_DIR        (TIM5_ENCODER_CH2_P10_1)                           // �����������ֵ�˿�

int8 duty = 7;                                                                 // ��ǰռ�ձ�
bool dir = true;                                                               // ��������
int16 encoder;                                                        // ������ֵ

int core0_main(void){
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

    pwm_init(PWM_CH1, 1000, 0);                                                 // PWM ͨ��1 ��ʼ��Ƶ��1KHz ռ�ձȳ�ʼΪ0
    gpio_init(DIR_CH1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // ��ʼ����������������
    encoder_dir_init(ENCODER1_TIM, ENCODER1_PLUS, ENCODER1_DIR);                // ��ʼ����������ֵ���ż���ʱ��

    pit_ms_init(CCU60_CH0, 100);

    ips200_init(IPS200_TYPE_SPI);


    // �˴���д�û����� ���������ʼ�������
	cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����

	while (TRUE)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���


        system_delay_ms(1000);

        // �˴���д��Ҫѭ��ִ�еĴ���
	}
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                                                 // �����ж�Ƕ��
    pit_clear_flag(CCU60_CH0);

    pwm_set_duty(PWM_CH1, duty * (PWM_DUTY_MAX / 100));                     // ����ռ�ձ�
    gpio_set_level(DIR_CH1, 0);

    encoder = encoder_get_count(ENCODER1_TIM);                               // �ɼ���Ӧ����������
    encoder_clear_count(ENCODER1_TIM);                                          // �����Ӧ����

}


#pragma section all restore
// **************************** �������� ****************************

