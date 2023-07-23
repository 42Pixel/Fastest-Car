/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
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
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
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

// *************************** ����Ӳ������˵�� ***********************
// ���İ��������缴�� �����������


// *************************** ���̲���˵�� **************************
// 1.���İ���¼��ɱ����̣�����ϵ�
// 2.���Կ������İ������� LED ������ͬ��Ƶ�ʽ�����˸
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�

// **************************** �������� ****************************

// ��������ֿ����Ȳ�ѧϰ�����̣���������Ҫ��Ե�Ƭ�������Ϻõ�ͬѧ�����Լ��Ż�RAM����������ܵ�ͬѧ�ο�ѧϰ
// ѧϰ��ʱ����������������Ķ���

// ��TC264�е��������ģ�ʵ�������ǵ���ˮ�������ǲ�һ���ģ�CPU0��һ����ˮ�ߣ�CPU1��������ˮ�ߣ����CPU1���ʺ������㣬�ٶȻ��CPU0��һЩ��

// TC264D��Ƭ�������¼���RAM�ɹ��û�ʹ��
// CPU0 psram ����RAM ��СΪ16KB
// CPU0 dsram ����RAM ��СΪ72KB

// CPU1 psram ����RAM ��СΪ32KB
// CPU1 dsram ����RAM ��СΪ120KB

// TC264D���������Ķ����Լ��ĳ��򻺴�
// CPU0 Cache��СΪ8KB
// CPU1 Cache��СΪ16KB

// ����RAM�����Ŵ��룬���������RAM�У��������ִ���ٶȣ��ر��ǵ��������еĴ����С������cache֮��Ч���Ƚ�������
// ����RAM�����ű���������CPU�����Է���dsram0��dsram1��Ҳ����˵CPU0���Է���dsram1��������Ϊ������˷��ʣ�������������Ƿ����ӳ���������
// ��õ���CPU0����dsram0��CPU1����dsram1�������ٶ���졣
// Ĭ������¶����ȫ�ֱ������Ǳ�����dsram1�еģ���ͻᵼ����CPU0���ܵĴ����RAM��ȡ����һ�����ӳ�

// ��ô���������Ǿ�Ҫѧϰ���ָ���������λ��

// ʹ��#pragma section all "cpu0_dsram" �� #pragma section all restore���Խ���������cpu0_dsram
// ����ֻ��Ҫ��������������������仰�м伴�ɣ�ʹ��ʾ������

#pragma section all "cpu0_dsram"  // ����������cpu0_dsram������
uint8 test_value = 0;
// �����cpu0_dsram��Ϊcpu1_dsram  ���������cpu1_dsram������
// ���ǽ���������ĸ�CPUִ�У�����ʹ�õı����������ĸ������ڣ�Ĭ�ϲ�ָ��������±�����������cpu1_dsram����
#pragma section all restore

// ʹ��#pragma section all "cpu0_psram" �� #pragma section all restore���Խ��������ص�cpu0_psram
// ����ֻ��Ҫ��������������������仰�м伴�ɣ�ʹ��ʾ������
#pragma section all "cpu0_psram"  // ���������ص�cpu0_psram������ִ��
void delay_tset(void)
{
    uint32 i;
    i = 16000000;
    while(i--);
}
// �����cpu0_psram��Ϊcpu1_psram  �������ص�cpu1_psram������
// ���ǽ���������ĸ�CPUִ�У��򽫺������ص����ĸ������ڣ�Ĭ�ϲ�ָ��������£������Ǵ�flash���ز�ִ��
// ͨ�����Ƕ�����Ҫ����������RAM���У�һ��������ִ�еĳ����С������cache�󣬻���ֳ���ִ���ٶȽ��ͣ����ʱ�����ǾͿ��Խ����ִ������RAMִ�У�������������ٶ�
#pragma section all restore

#define LED1 P20_8
#define LED2 P20_9

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

	gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	gpio_init(LED2, GPO, GPIO_LOW, GPO_PUSH_PULL);

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
	while (TRUE)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���

	    delay_tset();
	    gpio_toggle_level(LED1);
	    gpio_toggle_level(LED2);

        // �˴���д��Ҫѭ��ִ�еĴ���
	}
}
// **************************** �������� ****************************

// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б����
// ����1��LED ����˸
//      �鿴�����Ƿ�������¼���Ƿ����ر�����ȷ���������¸�λ����
//      ���ñ�������Ӧ LED ���ŵ�ѹ�Ƿ�仯��������仯֤������δ���У�����仯֤�� LED ������
