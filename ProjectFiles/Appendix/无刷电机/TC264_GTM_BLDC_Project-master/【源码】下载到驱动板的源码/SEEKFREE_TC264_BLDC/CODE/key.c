/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����ϵ�Ա��ͷ�
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            key
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ3184284598)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        ADS v1.5.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-12-10
 ********************************************************************************************************************/

#include "zf_gpio.h"
#include "motor.h"
#include "hall.h"
#include "key.h"

//����״̬����
uint8 key1_status=1;

//��һ�ο���״̬����
uint8 key1_last_status;

//���ر�־λ
uint8 key1_flag;

uint8 model_state;

void key_init(void)
{
    gpio_init(KEY1, GPI, 1, PULLUP);
}



void key_scan(void)
{
    //ʹ�ô˷����ŵ����ڣ�����Ҫʹ��while(1) �ȴ������⴦������Դ�˷�
    //���水��״̬
    key1_last_status = key1_status;
    //��ȡ��ǰ����״̬
    key1_status = gpio_get(KEY1);

    //��⵽��������֮��  ���ſ���λ��־λ
    if(key1_status && !key1_last_status)    key1_flag = 1;

    //��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
    if(key1_flag)
    {
      key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
      model_state=!model_state;         //��������ģʽ
    }
}
