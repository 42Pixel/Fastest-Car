/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����ϵ�Ա��ͷ�
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            gpio
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ3184284598)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        ADS v1.5.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-12-10
 ********************************************************************************************************************/

#include "zf_gpio.h"
#include "ccu6_pwm.h"
#include "hall.h"
#include "key.h"
#include "zf_gtm_pwm.h"
#include "gpio.h"




void led_init(void)
{
    gpio_init(LED_RUN_PIN, GPO, 1, PUSHPULL);           //��ʼ������ָʾ��
    gpio_init(LED_ERR_PIN, GPO, 1, PUSHPULL);           //��ʼ������ָʾ��
    gpio_init(LED_MODEL_PIN, GPO, 1, PUSHPULL);         //��ʼ������
    gpio_init(EN_PIN, GPI, 1, PULLUP);                  //��ʼ������ָʾ��
}


void led_output(void)
{
    if(ccu61_get_trap_flag() && gpio_get(EN_PIN))
    {
        gpio_set(LED_ERR_PIN,0);    //�������ϵ�
        gpio_set(LED_RUN_PIN,1);    //�ر����е�
    }
    else
    {
        gpio_set(LED_ERR_PIN,1);    //�رչ��ϵ�
        if(motor_state==MOTOR_RUN) gpio_set(LED_RUN_PIN,0); //�������е�
        else gpio_set(LED_RUN_PIN,1);       //�ر����е�
    }
    if(model_state)
    {
        gpio_set(LED_MODEL_PIN,1);
    }
    else
    {
        gpio_set(LED_MODEL_PIN,0);
    }
}
