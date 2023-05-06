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
// ʹ����ɿƼ� Ӣ����TriCore ��������������
//      ֱ�ӽ���������ȷ�����ں��İ�ĵ������ؽӿڼ���
// ʹ�� USB-TTL ģ������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      USB-TTL-RX          �鿴 zf_common_debug.h �ļ��� DEBUG_UART_TX_PIN �궨������� Ĭ�� P14_0
//      USB-TTL-TX          �鿴 zf_common_debug.h �ļ��� DEBUG_UART_RX_PIN �궨������� Ĭ�� P14_1
//      USB-TTL-GND         ���İ��Դ�� GND
//      USB-TTL-3V3         ���İ� 3V3 ��Դ
// ������������������ 1
//      ģ��ܽ�            ��Ƭ���ܽ�
//      A                   ENCODER_1_A �궨������� Ĭ�� P33_7
//      B                   ENCODER_1_B �궨������� Ĭ�� P33_6
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ
// ������������������ 2
//      ģ��ܽ�            ��Ƭ���ܽ�
//      A                   ENCODER_2_A �궨������� Ĭ�� P10_3
//      B                   ENCODER_2_B �궨������� Ĭ�� P10_1
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ
// ������������������ 3
//      ģ��ܽ�            ��Ƭ���ܽ�
//      A                   ENCODER_3_A �궨������� Ĭ�� P02_8
//      B                   ENCODER_3_B �궨������� Ĭ�� P00_9
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ
// ������������������ 4
//      ģ��ܽ�            ��Ƭ���ܽ�
//      A                   ENCODER_4_A �궨������� Ĭ�� P20_3
//      B                   ENCODER_4_B �궨������� Ĭ�� P20_0
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ


// *************************** ���̲���˵�� ***************************
// 1.���İ���¼������ ���������� �����������������ӵ�����ı������ӿ�
// 2.��ع��� �ϵ���Ӵ�������������ɼ���ת����Ϣ ����
//      ENCODER_x counter   xxxx.
// 3.ת�����������ݻ��б仯
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�

// **************************** �������� ****************************

#define ENCODER_1                   (TIM2_ENCODER)
#define ENCODER_1_A                 (TIM2_ENCODER_CH1_P33_7)
#define ENCODER_1_B                 (TIM2_ENCODER_CH2_P33_6)

#define ENCODER_2                   (TIM5_ENCODER)
#define ENCODER_2_A                 (TIM5_ENCODER_CH1_P10_3)
#define ENCODER_2_B                 (TIM5_ENCODER_CH2_P10_1)

#define ENCODER_3                   (TIM4_ENCODER)
#define ENCODER_3_A                 (TIM4_ENCODER_CH1_P02_8)
#define ENCODER_3_B                 (TIM4_ENCODER_CH2_P00_9)

#define ENCODER_4                   (TIM6_ENCODER)
#define ENCODER_4_A                 (TIM6_ENCODER_CH1_P20_3)
#define ENCODER_4_B                 (TIM6_ENCODER_CH2_P20_0)

int16 encoder_data_1 = 0;
int16 encoder_data_2 = 0;
int16 encoder_data_3 = 0;
int16 encoder_data_4 = 0;

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

    encoder_quad_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);                     // ��ʼ��������ģ�������� �������������ģʽ
    encoder_quad_init(ENCODER_2, ENCODER_2_A, ENCODER_2_B);                     // ��ʼ��������ģ�������� �������������ģʽ
    encoder_quad_init(ENCODER_3, ENCODER_3_A, ENCODER_3_B);                     // ��ʼ��������ģ�������� �������������ģʽ
    encoder_quad_init(ENCODER_4, ENCODER_4_A, ENCODER_4_B);                     // ��ʼ��������ģ�������� �������������ģʽ
    pit_ms_init(CCU60_CH0, 100);

    // �˴���д�û����� ���������ʼ�������
	cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
	while (TRUE)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���

	    printf("ENCODER_1 :%d .ENCODER_2 :%d .ENCODER_3 :%d .ENCODER_4 :%d .\r\n", encoder_data_1, encoder_data_2, encoder_data_3, encoder_data_4);                 // ���������������Ϣ
        system_delay_ms(100);

        // �˴���д��Ҫѭ��ִ�еĴ���
	}
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    pit_clear_flag(CCU60_CH0);

    encoder_data_1 = encoder_get_count(ENCODER_1);                              // ��ȡ����������
    encoder_clear_count(ENCODER_1);                                             // ��ձ���������

    encoder_data_2 = encoder_get_count(ENCODER_2);                              // ��ȡ����������
    encoder_clear_count(ENCODER_2);                                             // ��ձ���������

    encoder_data_3 = encoder_get_count(ENCODER_3);                              // ��ȡ����������
    encoder_clear_count(ENCODER_3);                                             // ��ձ���������

    encoder_data_4 = encoder_get_count(ENCODER_4);                              // ��ȡ����������
    encoder_clear_count(ENCODER_4);                                             // ��ձ���������

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
// ����3����ֵһֱ������һ��ת
//      ��Ҫ�ѷ����������������������ģʽ�Ľӿ���
// ����4����ֵ�������ת���仯
//      ���ʹ��������ԣ��������Ҫ�õ�ع���
//      ���������Ƿ��������ģ����Ƿ��ɶ����������Ƿ������ˣ��Ƿ�Ӵ���


