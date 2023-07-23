/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����ϵ�Ա��ͷ�
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            adc
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ3184284598)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        ADS v1.5.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-12-10
 ********************************************************************************************************************/

#include "zf_vadc.h"
#include "adc.h"


adc_struct adc_information;

void adc_collection_init(void)
{
    adc_init(ADC_NUMBER, BOARD_POTENTIOMET_PORT);   //���ص�λ��
    adc_init(ADC_NUMBER, A_PHASE_PORT);             //A�����
    adc_init(ADC_NUMBER, B_PHASE_PORT);             //B�����
    adc_init(ADC_NUMBER, C_PHASE_PORT);             //C�����
    adc_init(ADC_NUMBER, CENTER_PHASE_PORT);        //ĸ�ߵ���
}

void adc_read(void)
{
    adc_information.current_board   = adc_convert(ADC_NUMBER, BOARD_POTENTIOMET_PORT, ADC_12BIT);   //��ȡ���ص�λ����ѹ
    adc_information.current_a       = adc_convert(ADC_NUMBER, A_PHASE_PORT, ADC_12BIT);             //��ȡA�����
    adc_information.current_b       = adc_convert(ADC_NUMBER, B_PHASE_PORT, ADC_12BIT);             //��ȡB�����
    adc_information.current_c       = adc_convert(ADC_NUMBER, C_PHASE_PORT, ADC_12BIT);             //��ȡC�����
    adc_information.voltage_bus     = adc_convert(ADC_NUMBER, CENTER_PHASE_PORT, ADC_12BIT);        //��ȡĸ�ߵ���
}
