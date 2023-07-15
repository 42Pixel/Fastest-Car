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
* �ļ�����          zf_common_function
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

#ifndef _zf_common_function_h_
#define _zf_common_function_h_

#include "zf_common_typedef.h"

//====================================================�궨�庯����====================================================
//-------------------------------------------------------------------------------------------------------------------
// �������     ����ֵ���� ���ݷ�Χ�� [-32767,32767]
// ����˵��     dat             ��Ҫ�����ֵ����
// ���ز���     int             ���ؾ���ֵ
// ʹ��ʾ��     dat = func_abs(dat);                            // ��dat�������
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
#define     func_abs(x)             ((x) >= 0 ? (x): -(x))

//-------------------------------------------------------------------------------------------------------------------
// �������     �޷� ���ݷ�Χ�� [-32768,32767]
// ����˵��     x               ���޷�������
// ����˵��     y               �޷���Χ(���ݻᱻ������-y��+y֮��)
// ���ز���     int             �޷�֮�������
// ʹ��ʾ��     int dat = func_limit(500, 300);                 // ���ݱ�������-300��+300֮��  ��˷��صĽ����300
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
#define     func_limit(x, y)        ((x) > (y) ? (y) : ((x) < -(y) ? -(y) : (x)))

//-------------------------------------------------------------------------------------------------------------------
// �������     ˫���޷� ���ݷ�Χ�� [-32768,32767]
// ����˵��     x               ���޷�������
// ����˵��     a               �޷���Χ��߽�
// ����˵��     b               �޷���Χ�ұ߽�
// ���ز���     int             �޷�֮�������
// ʹ��ʾ��     int dat = func_limit_ab(500, -300, 400);        //���ݱ�������-300��+400֮��  ��˷��صĽ����400
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
#define     func_limit_ab(x, a, b)  ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

//====================================================�궨�庯����====================================================

//=====================================================���溯����=====================================================
uint32      func_get_greatest_common_divisor    (uint32 num1, uint32 num2);

void        func_soft_delay                     (volatile long t);

int32       func_str_to_int                     (char *str);
void        func_int_to_str                     (char *str, int32 number);
uint32      func_str_to_uint                    (char *str);
void        func_uint_to_str                    (char *str, uint32 number);
float       func_str_to_float                   (char *str);
void        func_float_to_str                   (char *str, float number, uint8 point_bit);
double      func_str_to_double                  (char *str);
void        func_double_to_str                  (char *str, double number, uint8 point_bit);
uint32      func_str_to_hex                     (char *str);
void        func_hex_to_str                     (char *str, uint32 number);

uint32      zf_sprintf                          (int8 *buff, const int8 *format, ...);
//=====================================================���溯����=====================================================
#define  PRINTF     1
extern void VOFA_Printf(const int8 *format, ...);

#endif


