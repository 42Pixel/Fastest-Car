
 
/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		isr
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/

#include "adc.h"
#include "hall.h"
#include "motor.h"
#include "pid.h"
#include "gpio.h"
#include "bldc_config.h"
#include "ccu6_pwm.h"
#include "pwm_input.h"
#include "move_filter.h"
#include "isr_config.h"
#include "SEEKFREE_FUN.h"
#include "key.h"
#include "isr.h"


//PWM�жϴ�����
IFX_INTERRUPT(ccu6_t12_pwm, 0, CCU60_T12_ISR_PRIORITY)
{
    IfxCpu_enableInterrupts();
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t12PeriodMatch);

    //��ȡadc��ֵ
    adc_read();
    //����ɨ��
    scan_hall_status();

    if(1 > commutation_delay--)
    {//��ʱʱ�䵽 ��ʼ����
        commutation_delay = 0;
        motor_commutation(next_hall_value);
    }
}

//���벶��
IFX_INTERRUPT(gtm_pwm_in, 0, GTM_PWM_IN_PRIORITY)
{

    IfxGtm_Tim_In_update(&driver);
    if(FALSE == driver.newData)
    {
        if(gpio_get(MOTOR_PWM_IN_PIN))
        {
            driver.periodTick = FPWM;                          //���� driver.periodTick;
            driver.pulseLengthTick = driver.periodTick;         //�ߵ�ƽʱ�� driver.pulseLengthTick;
        }
        else
        {
            driver.periodTick = FPWM;
            driver.pulseLengthTick = 0;
        }
    }
    pwm_in_duty = (int16)limit_ab((driver.pulseLengthTick * PWM_PRIOD_LOAD / driver.periodTick), 0, PWM_PRIOD_LOAD);
}

//PIT�жϺ���
IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
	enableInterrupts();//�����ж�Ƕ��
	PIT_CLEAR_FLAG(CCU6_0, PIT_CH0);

	key_scan();     //����ɨ��

    motor_speed_out(); //ͨ���ٶ�������������ǰ�ٶ�

    #if BLDC_CLOSE_LOOP_ENABLE
    //���ݽ��յ����źţ�ȥ�������Ҫ���õ��ٶ�ֵ
        if(model_state) motor_control.set_speed = motor_control.max_speed * adc_information.current_board/4096;
        else motor_control.set_speed = motor_control.max_speed * pwm_in_duty/PWM_PRIOD_LOAD;
        //����PI�ջ�����
        if(motor_control.dir == FORWARD)
            duty = (int16)closed_loop_pi_calc((float)(motor_control.set_speed - speed_filter.data_average));
        else
            duty = (int16)closed_loop_pi_calc((float)(motor_control.set_speed + speed_filter.data_average));
    #else
        if(model_state) duty = adc_information.current_board*PWM_PRIOD_LOAD/4096;
        else duty=pwm_in_duty;
    #endif

}


IFX_INTERRUPT(cc60_pit_ch1_isr, 0, CCU6_0_CH1_ISR_PRIORITY)
{
	enableInterrupts();//�����ж�Ƕ��
	PIT_CLEAR_FLAG(CCU6_0, PIT_CH1);
}

//CC61��Ԫ�Ѿ���ʹ�ã������޷�ʹ��CC61���������ж�
//IFX_INTERRUPT(cc61_pit_ch0_isr, 0, CCU6_1_CH0_ISR_PRIORITY)
//{
//	enableInterrupts();//�����ж�Ƕ��
//	PIT_CLEAR_FLAG(CCU6_1, PIT_CH0);
//
//}
//
//IFX_INTERRUPT(cc61_pit_ch1_isr, 0, CCU6_1_CH1_ISR_PRIORITY)
//{
//	enableInterrupts();//�����ж�Ƕ��
//	PIT_CLEAR_FLAG(CCU6_1, PIT_CH1);
//
//}




IFX_INTERRUPT(eru_ch0_ch4_isr, 0, ERU_CH0_CH4_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
	if(GET_GPIO_FLAG(ERU_CH0_REQ4_P10_7))//ͨ��0�ж�
	{
		CLEAR_GPIO_FLAG(ERU_CH0_REQ4_P10_7);
	}

	if(GET_GPIO_FLAG(ERU_CH4_REQ13_P15_5))//ͨ��4�ж�
	{
		CLEAR_GPIO_FLAG(ERU_CH4_REQ13_P15_5);
	}
}

IFX_INTERRUPT(eru_ch1_ch5_isr, 0, ERU_CH1_CH5_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
	if(GET_GPIO_FLAG(ERU_CH1_REQ5_P10_8))//ͨ��1�ж�
	{
		CLEAR_GPIO_FLAG(ERU_CH1_REQ5_P10_8);
	}

	if(GET_GPIO_FLAG(ERU_CH5_REQ1_P15_8))//ͨ��5�ж�
	{
		CLEAR_GPIO_FLAG(ERU_CH5_REQ1_P15_8);
	}
}

//��������ͷpclk����Ĭ��ռ���� 2ͨ�������ڴ���DMA��������ﲻ�ٶ����жϺ���
//IFX_INTERRUPT(eru_ch2_ch6_isr, 0, ERU_CH2_CH6_INT_PRIO)
//{
//	enableInterrupts();//�����ж�Ƕ��
//	if(GET_GPIO_FLAG(ERU_CH2_REQ7_P00_4))//ͨ��2�ж�
//	{
//		CLEAR_GPIO_FLAG(ERU_CH2_REQ7_P00_4);
//
//	}
//	if(GET_GPIO_FLAG(ERU_CH6_REQ9_P20_0))//ͨ��6�ж�
//	{
//		CLEAR_GPIO_FLAG(ERU_CH6_REQ9_P20_0);
//
//	}
//}



IFX_INTERRUPT(eru_ch3_ch7_isr, 0, ERU_CH3_CH7_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
	if(GET_GPIO_FLAG(ERU_CH3_REQ6_P02_0))//ͨ��3�ж�
	{
		CLEAR_GPIO_FLAG(ERU_CH3_REQ6_P02_0);
		if		(CAMERA_GRAYSCALE == camera_type)	mt9v03x_vsync();
		else if (CAMERA_BIN_UART  == camera_type)	ov7725_uart_vsync();
		else if	(CAMERA_BIN       == camera_type)	ov7725_vsync();

	}
	if(GET_GPIO_FLAG(ERU_CH7_REQ16_P15_1))//ͨ��7�ж�
	{
		CLEAR_GPIO_FLAG(ERU_CH7_REQ16_P15_1);

	}
}



IFX_INTERRUPT(dma_ch5_isr, 0, ERU_DMA_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��

	if		(CAMERA_GRAYSCALE == camera_type)	mt9v03x_dma();
	else if (CAMERA_BIN_UART  == camera_type)	ov7725_uart_dma();
	else if	(CAMERA_BIN       == camera_type)	ov7725_dma();
}


//�����жϺ���  ʾ��
IFX_INTERRUPT(uart0_tx_isr, 0, UART0_TX_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
    IfxAsclin_Asc_isrTransmit(&uart0_handle);
}
IFX_INTERRUPT(uart0_rx_isr, 0, UART0_RX_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
    IfxAsclin_Asc_isrReceive(&uart0_handle);
}
IFX_INTERRUPT(uart0_er_isr, 0, UART0_ER_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
    IfxAsclin_Asc_isrError(&uart0_handle);
}

//����1Ĭ�����ӵ�����ͷ���ô���
IFX_INTERRUPT(uart1_tx_isr, 0, UART1_TX_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
    IfxAsclin_Asc_isrTransmit(&uart1_handle);
}
IFX_INTERRUPT(uart1_rx_isr, 0, UART1_RX_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
    IfxAsclin_Asc_isrReceive(&uart1_handle);
    if		(CAMERA_GRAYSCALE == camera_type)	mt9v03x_uart_callback();
    else if (CAMERA_BIN_UART  == camera_type)	ov7725_uart_callback();
}
IFX_INTERRUPT(uart1_er_isr, 0, UART1_ER_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
    IfxAsclin_Asc_isrError(&uart1_handle);
}


//����2Ĭ�����ӵ�����ת����ģ��
IFX_INTERRUPT(uart2_tx_isr, 0, UART2_TX_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
    IfxAsclin_Asc_isrTransmit(&uart2_handle);
}
IFX_INTERRUPT(uart2_rx_isr, 0, UART2_RX_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
    IfxAsclin_Asc_isrReceive(&uart2_handle);
    switch(wireless_type)
    {
    	case WIRELESS_SI24R1:
    	{
    		wireless_uart_callback();
    	}break;

    	case WIRELESS_CH9141:
		{
			bluetooth_ch9141_uart_callback();
		}break;
    	default:break;
    }

}
IFX_INTERRUPT(uart2_er_isr, 0, UART2_ER_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
    IfxAsclin_Asc_isrError(&uart2_handle);
}



IFX_INTERRUPT(uart3_tx_isr, 0, UART3_TX_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
    IfxAsclin_Asc_isrTransmit(&uart3_handle);
}
IFX_INTERRUPT(uart3_rx_isr, 0, UART3_RX_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
    IfxAsclin_Asc_isrReceive(&uart3_handle);
}
IFX_INTERRUPT(uart3_er_isr, 0, UART3_ER_INT_PRIO)
{
	enableInterrupts();//�����ж�Ƕ��
    IfxAsclin_Asc_isrError(&uart3_handle);
}
