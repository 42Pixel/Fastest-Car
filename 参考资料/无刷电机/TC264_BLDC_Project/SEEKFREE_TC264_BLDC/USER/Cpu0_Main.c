/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            main
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ3184284598)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        ADS v1.2.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-3-23
 ********************************************************************************************************************/


//BLDC

#include "gpio.h"
#include "adc.h"
#include "hall.h"
#include "move_filter.h"
#include "key.h"
#include "motor.h"
#include "pid.h"
#include "headfile.h"
#include "ccu6_pwm.h"
#include "pwm_input.h"
#pragma section all "cpu0_dsram"
//���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��


//�ر�˵�����ÿ�Դ��Ŀ�����ڸ�����ͬѧ������Լ���С��ʱ���вο���Ӳ������������ܹ�ֱ�ӿ���ʹ�����Լ�����Ʒ�У����Ҳο����������Ӳ������������������
//�ر�˵�����ÿ�Դ��Ŀ�����ڸ�����ͬѧ������Լ���С��ʱ���вο���Ӳ������������ܹ�ֱ�ӿ���ʹ�����Լ�����Ʒ�У����Ҳο����������Ӳ������������������
//�ر�˵�����ÿ�Դ��Ŀ�����ڸ�����ͬѧ������Լ���С��ʱ���вο���Ӳ������������ܹ�ֱ�ӿ���ʹ�����Լ�����Ʒ�У����Ҳο����������Ӳ������������������

int core0_main(void)
{
    //��ȡʱ��Ƶ��  ��ر���
    get_clk();

    //��ʼ��LED����
    led_init();

    //��ʼ����������
    key_init();

    //��ʼ��adcͨ����adc���ڲɼ���Դ��ѹ��ĸ�ߵ������������
    adc_collection_init();

    //������ʼ��
    hall_init();

    //����ƽ���˲���ʼ��
    move_filter_init(&speed_filter);

    //��ʼ������ٶ��뷽����Ϣ������
    motor_information_out_init();

    //�����źŲ����ʼ��
    pwm_input_init();

    //�����ʼ��
    motor_init();

    //PID������ʼ��
    closed_loop_pi_init();

    //��ʼ����ʱ��,�����������PWM
    ccu6_pwm_init();

    //��ʼ����ʱ��,���ڼ���ռ�ձ�
    pit_interrupt_ms(CCU6_0, PIT_CH0, 5);

    enableInterrupts();
    while(1){

        led_output();       //���ݵ�ǰ״̬��������Ϩ��LED��

        //�������ݵ�����ʾ���� ����ʾ������������ https://pan.baidu.com/s/198CMXTZsbI3HAEqNXDngBw
        data_conversion((uint16)speed_filter.data_average, pwm_in_duty, hall_value_now, duty, virtual_scope_data);
        uart_putbuff(UART_0, virtual_scope_data, sizeof(virtual_scope_data));  //����ת����ɺ�ʹ�ô��ڷ��ͽ���������ݷ��ͳ�ȥ
    }
}

#pragma section all restore


