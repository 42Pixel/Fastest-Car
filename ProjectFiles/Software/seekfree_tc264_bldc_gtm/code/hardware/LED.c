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
* �ļ�����          gpio
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.8.0
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2023-02-01       pudding             first version
********************************************************************************************************************/

#include <LED.h>
#include "zf_driver_pwm.h"
#include "zf_driver_delay.h"
#include "motor.h"
#include "pwm_input.h"


void led_init(void)
{
    gpio_init(LED_RUN_PIN   , GPO, 1, GPO_PUSH_PULL);       // ��ʼ������ָʾ��
    gpio_init(LED_ERR_PIN   , GPO, 1, GPO_PUSH_PULL);       // ��ʼ������ָʾ��
    gpio_init(EN_PIN        , GPI, 1, GPI_PULL_UP);         // ��ʼ��ʹ�ܿ���

}

void led_output(void)
{
    if(!gpio_get_level(EN_FSOI_PIN) && !gpio_get_level(EN_PIN))
    {
        if(motor_control.run_state == MOTOR_RUN)
        {
            gpio_set_level(LED_RUN_PIN, 0);                 // �������е�
        }
        else
        {
            gpio_set_level(LED_RUN_PIN, 1);                 // �ر����е�
        }
    }
    else
    {
        gpio_set_level(LED_RUN_PIN, 1);                     // �ر����е�
    }

    if((gpio_get_level(EN_FSOI_PIN) && !gpio_get_level(EN_PIN)) || motor_control.battery_state == LOW_VOLTAGE || motor_control.locked_rotor)
    {
        gpio_set_level(LED_ERR_PIN, 0);                     // �������ϵ�
    }
    else
    {
        gpio_set_level(LED_ERR_PIN, 1);                     // �رչ��ϵ�
    }
}

