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
// ��������ת����ģ��
//      ģ��ܽ�            ��Ƭ���ܽ�
//      RX                  �鿴 zf_device_wrieless_uart.h �� WIRELESS_UART_RX_PINx  �궨�� Ĭ�� P10_6
//      TX                  �鿴 zf_device_wrieless_uart.h �� WIRELESS_UART_TX_PINx  �궨�� Ĭ�� P10_5
//      RTS                 �鿴 zf_device_wrieless_uart.h �� WIRELESS_UART_RTS_PINx �궨�� Ĭ�� P10_2
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ

// **************************** �������� ****************************
#define LED1                    (P20_9)
uint8 data_buffer[32];
uint8 data_len;

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������
    uint8 count = 0;

    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
    if(wireless_uart_init())                                                    // �ж��Ƿ�ͨ����ʼ��
    {
        while(1)                                                                // ��ʼ��ʧ�ܾ����������ѭ��
        {
            gpio_toggle_level(LED1);                                            // ��ת LED ���������ƽ ���� LED ����
            system_delay_ms(100);                                               // ����ʱ�������Ʊ�ʾ�쳣
        }
    }
    wireless_uart_send_byte('\r');
    wireless_uart_send_byte('\n');
    wireless_uart_send_string("SEEKFREE wireless uart demo.\r\n");              // ��ʼ������ ���������Ϣ

    // �˴���д�û����� ���������ʼ�������
	cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
	while (TRUE)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���

	    data_len = (uint8)wireless_uart_read_buff(data_buffer, 32);             // �鿴�Ƿ�����Ϣ Ĭ�ϻ������� WIRELESS_UART_BUFFER_SIZE �ܹ� 64 �ֽ�
        if(data_len != 0)                                                       // �յ�����Ϣ ��ȡ�����᷵��ʵ�ʶ�ȡ�������ݸ���
        {
            wireless_uart_send_buff(data_buffer, data_len);                     // ���յ�����Ϣ���ͻ�ȥ
            memset(data_buffer, 0, 32);
            func_uint_to_str((char *)data_buffer, data_len);
            wireless_uart_send_string("\r\ndata len:");                                 // ��ʾʵ���յ���������Ϣ
            wireless_uart_send_buff(data_buffer, strlen((const char *)data_buffer));    // ��ʾ�յ������ݸ���
            wireless_uart_send_string(".\r\n");
        }
        if(count ++ > 10)
        {
            count = 0;
            gpio_toggle_level(LED1);                                            // ��ת LED ���������ƽ ���� LED ����
        }
        system_delay_ms(50);

        // �˴���д��Ҫѭ��ִ�еĴ���
	}
}

#pragma section all restore
// **************************** �������� ****************************


// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1������û������
//      �鿴�������ִ򿪵��Ƿ�����ȷ�Ĵ��ڣ����򿪵� COM ���Ƿ��Ӧ���� ����ת����ģ�� �� COM ��
//      �鿴���İ�LED��״̬ ���������˸֤����ʼ��ʧ�� ������ ���߰���˵������ģ���Ƿ�����
// ����2��������������
//      �鿴�Ƿ�ģ�鲨���ʱ��޸Ĺ� �ָ���������
// ����3����������û���յ��ظ�
//      ���Կ����Ƿ��ȡ������ data_len �Ƿ�һֱ���� ����� ��ô����˵������ģ���Ƿ�����

