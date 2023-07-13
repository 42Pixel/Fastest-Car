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
* ��������          ADS v1.9.4
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#define LED1                    (P20_9)
#define LED2                    (P21_4)

#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"


// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��


//********************************************** �������� *************************************************

int core0_main(void)
{
    clock_init();                                           // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                                           // ��ʼ��Ĭ�ϵ��Դ���

//********************************************* Ӳ����ʼ�� *************************************************
    gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);          // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(LED2, GPO, GPIO_LOW, GPO_PUSH_PULL);          // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ

    ips200_set_dir(IPS200_PORTAIT);
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);
    ips200_init(IPS200_TYPE_PARALLEL8);

    Motor_Init();
    gps_init();
    Key_Init();
    imu660ra_init();

    system_delay_ms(500);                  //�ȴ�����Ӳ����ʼ�����

//********************************************* �����ʼ�� *************************************************
    gyroOffset_init();                      //����imu660ra��Ư

//********************************************* �жϳ�ʼ�� *************************************************
    pit_ms_init(CCU61_CH1, 19);             //IMU�жϼ�� 19����
    pit_ms_init(CCU61_CH0, 7);              //����жϼ�� 7����
    pit_ms_init(CCU60_CH0, 13);             //�����жϼ�� 13����

    uart_init(UART_2,9600,UART2_TX_P10_5,UART2_RX_P10_6);

    cpu_wait_event_ready();                // �ȴ����к��ĳ�ʼ�����

//********************************************** ����ִ�� **************************************************
    VOFA* VOFA_pt = vofa_create();         //����VOFA����
    vofa_init(VOFA_pt,                     //��ʼ����ǰ��vofa����
              vofa_ch_data,ch_sz,
              custom_buf,custom_sz,
              cmd_rxbuf,cmd_sz,
              UART_2,UART_2,UART_2);

    Run_Start();

    while (TRUE)
    {
        gpio_set_level(LED1, !gps_tau1201.state);       //�յ�GPS���ݣ�LED����
        if(gps_tau1201_flag){
            gps_tau1201_flag = 0;
            gps_data_parse();                           // ��ʼ��������
            }

        Run();
        VOFA_Sent();                             // ���ݷ���
        UI();                                    // ������ʾ

    }
}

#pragma section all restore
