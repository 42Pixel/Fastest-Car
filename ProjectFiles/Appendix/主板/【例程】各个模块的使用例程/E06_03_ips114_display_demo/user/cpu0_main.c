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
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��


// ���̵��뵽���֮��Ӧ��ѡ�й���Ȼ����refreshˢ��һ��֮���ٱ���
// ����Ĭ������Ϊ�ر��Ż��������Լ��һ�����ѡ��properties->C/C++ Build->Setting
// Ȼ�����Ҳ�Ĵ������ҵ�C/C++ Compiler->Optimization->Optimization level�������Ż��ȼ�
// һ��Ĭ���½����Ĺ��̶���Ĭ�Ͽ�2���Ż�����˴��Ҳ��������Ϊ2���Ż�

// ����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ�� interrupt_global_enable(0); �������ж�Ƕ��
// �򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ������� interrupt_global_disable(); ���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ����� interrupt_global_enable(0); �������жϵ���Ӧ��

// *************************** ����Ӳ������˵�� ***************************
//      ģ��ܽ�            ��Ƭ���ܽ�
//      SCL                 �鿴 zf_device_ips114.h �� IPS114_SCL_PIN �궨�� Ĭ�� P15_3
//      SDA                 �鿴 zf_device_ips114.h �� IPS114_SDA_PIN �궨�� Ĭ�� P15_5
//      RES                 �鿴 zf_device_ips114.h �� IPS114_RST_PIN �궨�� Ĭ�� P15_1
//      DC                  �鿴 zf_device_ips114.h �� IPS114_DC_PIN  �궨�� Ĭ�� P15_0
//      CS                  �鿴 zf_device_ips114.h �� IPS114_CS_PIN  �궨�� Ĭ�� P15_2
//      BL                  �鿴 zf_device_ips114.h �� IPS114_BLK_PIN �궨�� Ĭ�� P15_4
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ



// *************************** ���̲���˵�� ***************************
// 1.���İ���¼������ ���������� 1.14��IPS ��ʾģ������������Ļ�ӿ������� ��ע�����Ŷ�Ӧ ��Ҫ���
// 2.��ع��� �ϵ�� 1.14��IPS ��Ļ���� ��ʾ�ַ����ָ������Ͳ���ͼ
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�


// **************************** �������� ****************************


int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

    uint16_t data[128];
    int16_t data_index = 0;
    for( ; data_index < 64; data_index ++)
       data[data_index] = data_index;
    for(data_index = 64; data_index < 128; data_index ++)
       data[data_index] = 128 - data_index;

    ips114_set_dir(IPS114_CROSSWISE_180);
    ips114_set_color(RGB565_RED, RGB565_BLACK);
    ips114_init();

    // �˴���д�û����� ���������ʼ�������
	cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
	while (TRUE)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���

        ips114_clear();
        ips114_show_rgb565_image(0, 27, (const uint16 *)gImage_seekfree_logo, 240, 80, 240, 80, 0); // ��ʾһ��RGB565ɫ��ͼƬ ԭͼ240*80 ��ʾ240*80 ��λ��ǰ
        system_delay_ms(1500);

        ips114_full(RGB565_GRAY);
        ips114_show_string( 0 , 16*1,   "SEEKFREE");                            // ��ʾ�ַ���
        ips114_show_chinese(80, 16*1,   16, (const uint8 *)chinese_test, 4, RGB565_BLUE);           // ��ʾ����

        // ��ʾ�� flaot ���� �����ʾ 8bit λ���� �����ʾ 6bit λС��
        ips114_show_float(  0 , 16*2,   -13.141592,     1, 6);                  // ��ʾ float ���� 1bit ���� 6bit С�� Ӧ����ʾ -3.141592 �ܹ����� 9 ���ַ�����ʾռλ
        ips114_show_float(  80, 16*2,   13.141592,      8, 4);                  // ��ʾ float ���� 8bit ���� 4bit С�� Ӧ����ʾ 13.1415 �ܹ����� 14 ���ַ�����ʾռλ ������� 5 ���ַ��Ŀհ�ռλ

        ips114_show_int(    0 , 16*3,   -127,           2);                     // ��ʾ int8 ����
        ips114_show_uint(   80, 16*3,   255,            4);                     // ��ʾ uint8 ����

        ips114_show_int(    0 , 16*4,   -32768,         4);                     // ��ʾ int16 ����
        ips114_show_uint(   80, 16*4,   65535,          6);                     // ��ʾ uint16 ����

        ips114_show_int(    0 , 16*5,   -2147483648,    8);                     // ��ʾ int32 ���� 8bit ���� Ӧ����ʾ -47483648
        ips114_show_uint(   0 , 16*6,   4294967295,     8);                     // ��ʾ uint32 ���� 10bit ���� Ӧ����ʾ 4294967295

        system_delay_ms(1000);

        ips114_full(RGB565_GRAY);
        ips114_show_wave(88, 51, data, 128, 64,  64, 32);                       // ��ʾһ�����ǲ��� ���ο�� 128 �������ֵ 64 ��ʾ��� 64 ��ʾ���ֵ 32
        system_delay_ms(1000);
        ips114_full(RGB565_GRAY);
        ips114_show_wave(56, 35, data, 128, 64, 128, 64);                       // ��ʾһ�����ǲ��� ���ο�� 128 �������ֵ 64 ��ʾ��� 128 ��ʾ���ֵ 64
        system_delay_ms(1000);

        // ʹ�û��ߺ��� �Ӷ��������ǻ�����
        ips114_clear();
        for(data_index = 0; data_index < 240; data_index += 10)
        {
           ips114_draw_line(0, 0, data_index, 135 - 1, RGB565_66CCFF);
           system_delay_ms(20);
        }
        ips114_draw_line(0, 0, 240 - 1, 135 - 1, RGB565_66CCFF);
        for(data_index = 130; data_index >= 0; data_index -= 10)
        {
           ips114_draw_line(0, 0, 240 - 1, data_index, RGB565_66CCFF);
           system_delay_ms(20);
        }

        ips114_draw_line(240 - 1, 0, 239, 135 - 1, RGB565_66CCFF);
        for(data_index = 230; data_index >= 0; data_index -= 10)
        {
           ips114_draw_line(240 - 1, 0, data_index, 135 - 1, RGB565_66CCFF);
           system_delay_ms(20);
        }
        ips114_draw_line(240 - 1, 0, 0, 135 - 1, RGB565_66CCFF);
        for(data_index = 130; data_index >= 0; data_index -= 10)
        {
           ips114_draw_line(240 - 1, 0, 0, data_index, RGB565_66CCFF);
           system_delay_ms(20);
        }
        system_delay_ms(1000);

        ips114_full(RGB565_RED);
        system_delay_ms(500);
        ips114_full(RGB565_GREEN);
        system_delay_ms(500);
        ips114_full(RGB565_BLUE);
        system_delay_ms(500);
        ips114_full(RGB565_WHITE);
        system_delay_ms(500);

        // �˴���д��Ҫѭ��ִ�еĴ���
	}
}

#pragma section all restore
// **************************** �������� ****************************

// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1����Ļ����ʾ
//      ���ʹ��������ԣ��������Ҫ�õ�ع��� �����Ļ�������ŵ�ѹ
//      �����Ļ�ǲ��ǲ��λ���� ������Ŷ�Ӧ��ϵ
//      �����Ӧ���Ŷ���ȷ ���һ���Ƿ������Ų��β��� ��Ҫ��ʾ����
//      �޷���ɲ��β�������һ��GPIO���̽���Ļ����IO��ʼ��ΪGPIO��ת��ƽ �����Ƿ��ܿ�
