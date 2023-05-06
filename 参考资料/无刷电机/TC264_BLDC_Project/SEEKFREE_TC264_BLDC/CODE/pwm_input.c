/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����ϵ�Ա��ͷ�
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            pwm_input
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ3184284598)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        ADS v1.5.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-12-10
 ********************************************************************************************************************/

#include "ccu6_pwm.h"
#include "zf_gpio.h"
#include "pwm_input.h"

uint16 pwm_in_duty;

IfxGtm_Tim_In driver;

//���벶���ʼ��
void pwm_input_init(void)
{
    IfxGtm_enable(&MODULE_GTM);
    if(!(MODULE_GTM.CMU.CLK_EN.U & 0x2))
    {
        IfxGtm_Cmu_setClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Clk_0, 100000000);
        IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_CLK0);
    }

    IfxGtm_Tim_In_Config config;
    IfxGtm_Tim_In_initConfig(&config, &MODULE_GTM);
    config.timIndex                     = IfxGtm_Tim_1;
    config.channelIndex                 = IfxGtm_Tim_Ch_1;
    config.isrPriority                  = GTM_PWM_IN_PRIORITY;
    config.capture.irqOnNewVal          = TRUE;
    config.capture.irqOnCntOverflow     = TRUE;
    config.timeout.clock                = IfxGtm_Cmu_Clk_0;
    config.filter.inputPin              = &IfxGtm_TIM1_1_TIN95_P11_2_IN;
    config.filter.inputPinMode          = IfxPort_InputMode_pullDown;


    IfxGtm_Tim_In_init(&driver, &config);
    driver.periodTick = FPWM;


    gpio_init(MOTOR_DIR_IN_PIN, GPI, 0, PULLDOWN);      //��ʼ��������������
}
