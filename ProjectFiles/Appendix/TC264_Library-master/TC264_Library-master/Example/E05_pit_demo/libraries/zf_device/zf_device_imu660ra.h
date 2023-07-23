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
* �ļ�����          zf_device_imu660ra
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.4
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
* 2023-04-28       pudding            ��������ע��˵��
********************************************************************************************************************/
/*********************************************************************************************************************
* ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   // Ӳ�� SPI ����
*                   SCL/SPC           �鿴 zf_device_imu660ra.h �� IMU660RA_SPC_PIN �궨��
*                   SDA/DSI           �鿴 zf_device_imu660ra.h �� IMU660RA_SDI_PIN �궨��
*                   SA0/SDO           �鿴 zf_device_imu660ra.h �� IMU660RA_SDO_PIN �궨��
*                   CS                �鿴 zf_device_imu660ra.h �� IMU660RA_CS_PIN �궨��
*                   VCC               3.3V��Դ
*                   GND               ��Դ��
*                   ������������
*
*                   // ���� IIC ����
*                   SCL/SPC           �鿴 zf_device_imu660ra.h �� IMU660RA_SCL_PIN �궨��
*                   SDA/DSI           �鿴 zf_device_imu660ra.h �� IMU660RA_SDA_PIN �궨��
*                   VCC               3.3V��Դ
*                   GND               ��Դ��
*                   ������������
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_imu660ra_h_
#define _zf_device_imu660ra_h_

#include "zf_common_typedef.h"

//================================================���� IMU660RA ��������================================================
// IMU660RA_USE_SOFT_IIC����Ϊ0��ʾʹ��Ӳ��SPI���� ����Ϊ1��ʾʹ������IIC����
// ������IMU660RA_USE_SOFT_IIC�������Ҫ�ȱ��벢���س��򣬵�Ƭ����ģ����Ҫ�ϵ�������������ͨѶ
#define IMU660RA_USE_SOFT_IIC         (0)                                       // Ĭ��ʹ��Ӳ�� SPI ��ʽ����
#if IMU660RA_USE_SOFT_IIC                                                       // ������ ��ɫ�����Ĳ�����ȷ�� ��ɫ�ҵľ���û���õ�
//====================================================���� IIC ����====================================================
#define IMU660RA_SOFT_IIC_DELAY       (59)                                      // ���� IIC ��ʱ����ʱ���� ��ֵԽС IIC ͨ������Խ��
#define IMU660RA_SCL_PIN              (P20_11)                                  // ���� IIC SCL ���� ���� IMU660RA �� SCL ����
#define IMU660RA_SDA_PIN              (P20_14)                                  // ���� IIC SDA ���� ���� IMU660RA �� SDA ����
//====================================================���� IIC ����====================================================
#else
//====================================================Ӳ�� SPI ����====================================================
#define IMU660RA_SPI_SPEED            (10 * 1000 * 1000)                        // Ӳ�� SPI ����
#define IMU660RA_SPI                  (SPI_0)                                   // Ӳ�� SPI ��
#define IMU660RA_SPC_PIN              (SPI0_SCLK_P20_11)                        // Ӳ�� SPI SCK ����
#define IMU660RA_SDI_PIN              (SPI0_MOSI_P20_14)                        // Ӳ�� SPI MOSI ����
#define IMU660RA_SDO_PIN              (SPI0_MISO_P20_12)                        // Ӳ�� SPI MISO ����
//====================================================Ӳ�� SPI ����====================================================
#endif
#define IMU660RA_CS_PIN               (P20_13)                                  // CS Ƭѡ����
#define IMU660RA_CS(x)                ((x) ? (gpio_high(IMU660RA_CS_PIN)) : (gpio_low(IMU660RA_CS_PIN)))
typedef enum
{
    IMU660RA_ACC_SAMPLE_SGN_2G ,                                                // ���ٶȼ����� ��2G  (ACC = Accelerometer ���ٶȼ�) (SGN = signum �������� ��ʾ������Χ) (G = g �������ٶ� g��9.80 m/s^2)
    IMU660RA_ACC_SAMPLE_SGN_4G ,                                                // ���ٶȼ����� ��4G  (ACC = Accelerometer ���ٶȼ�) (SGN = signum �������� ��ʾ������Χ) (G = g �������ٶ� g��9.80 m/s^2)
    IMU660RA_ACC_SAMPLE_SGN_8G ,                                                // ���ٶȼ����� ��8G  (ACC = Accelerometer ���ٶȼ�) (SGN = signum �������� ��ʾ������Χ) (G = g �������ٶ� g��9.80 m/s^2)
    IMU660RA_ACC_SAMPLE_SGN_16G,                                                // ���ٶȼ����� ��16G (ACC = Accelerometer ���ٶȼ�) (SGN = signum �������� ��ʾ������Χ) (G = g �������ٶ� g��9.80 m/s^2)
}imu660ra_acc_sample_config;

typedef enum
{
    IMU660RA_GYRO_SAMPLE_SGN_125DPS ,                                           // ���������� ��125DPS  (GYRO = Gyroscope ������) (SGN = signum �������� ��ʾ������Χ) (DPS = Degree Per Second ���ٶȵ�λ ��/S)
    IMU660RA_GYRO_SAMPLE_SGN_250DPS ,                                           // ���������� ��250DPS  (GYRO = Gyroscope ������) (SGN = signum �������� ��ʾ������Χ) (DPS = Degree Per Second ���ٶȵ�λ ��/S)
    IMU660RA_GYRO_SAMPLE_SGN_500DPS ,                                           // ���������� ��500DPS  (GYRO = Gyroscope ������) (SGN = signum �������� ��ʾ������Χ) (DPS = Degree Per Second ���ٶȵ�λ ��/S)
    IMU660RA_GYRO_SAMPLE_SGN_1000DPS,                                           // ���������� ��1000DPS (GYRO = Gyroscope ������) (SGN = signum �������� ��ʾ������Χ) (DPS = Degree Per Second ���ٶȵ�λ ��/S)
    IMU660RA_GYRO_SAMPLE_SGN_2000DPS,                                           // ���������� ��2000DPS (GYRO = Gyroscope ������) (SGN = signum �������� ��ʾ������Χ) (DPS = Degree Per Second ���ٶȵ�λ ��/S)
}imu660ra_gyro_sample_config;

#define IMU660RA_ACC_SAMPLE_DEFAULT   ( IMU660RA_ACC_SAMPLE_SGN_8G )            // ��������Ĭ�ϵ� ���ٶȼ� ��ʼ������
#define IMU660RA_GYRO_SAMPLE_DEFAULT  ( IMU660RA_GYRO_SAMPLE_SGN_2000DPS )      // ��������Ĭ�ϵ� ������   ��ʼ������
#define IMU660RA_TIMEOUT_COUNT        (0x00FF)                                  // IMU660 ��ʱ����
//================================================���� IMU660RA ��������================================================


//================================================���� IMU660RA �ڲ���ַ================================================
#define IMU660RA_DEV_ADDR             (0x69)                                    // SA0�ӵأ�0x68 SA0������0x69 ģ��Ĭ������
#define IMU660RA_SPI_W                (0x00)
#define IMU660RA_SPI_R                (0x80)

#define IMU660RA_CHIP_ID              (0x00)
#define IMU660RA_PWR_CONF             (0x7C)
#define IMU660RA_PWR_CTRL             (0x7D)
#define IMU660RA_INIT_CTRL            (0x59)
#define IMU660RA_INIT_DATA            (0x5E)
#define IMU660RA_INT_STA              (0x21)
#define IMU660RA_ACC_ADDRESS          (0x0C)
#define IMU660RA_GYRO_ADDRESS         (0x12)
#define IMU660RA_ACC_CONF             (0x40)
#define IMU660RA_ACC_RANGE            (0x41)
#define IMU660RA_GYR_CONF             (0x42)
#define IMU660RA_GYR_RANGE            (0x43)
#define IMU660RA_ACC_SAMPLE           (0x02)                                    // ���ٶȼ�����
// ����Ϊ:0x00 ���ٶȼ�����Ϊ:��2g         ��ȡ���ļ��ٶȼ����� ���� 16384   ����ת��Ϊ��������λ������ ��λ��g(m/s^2)
// ����Ϊ:0x01 ���ٶȼ�����Ϊ:��4g         ��ȡ���ļ��ٶȼ����� ���� 8192    ����ת��Ϊ��������λ������ ��λ��g(m/s^2)
// ����Ϊ:0x02 ���ٶȼ�����Ϊ:��8g         ��ȡ���ļ��ٶȼ����� ���� 4096    ����ת��Ϊ��������λ������ ��λ��g(m/s^2)
// ����Ϊ:0x03 ���ٶȼ�����Ϊ:��16g        ��ȡ���ļ��ٶȼ����� ���� 2048    ����ת��Ϊ��������λ������ ��λ��g(m/s^2)
#define IMU660RA_GYR_SAMPLE           (0x00)                                    // ����������
// ����Ϊ:0x00 ����������Ϊ:��2000dps     ��ȡ�������������� ���� 16.4       ����ת��Ϊ��������λ������ ��λΪ����/s
// ����Ϊ:0x01 ����������Ϊ:��1000dps     ��ȡ�������������� ���� 32.8       ����ת��Ϊ��������λ������ ��λΪ����/s
// ����Ϊ:0x02 ����������Ϊ:��500 dps     ��ȡ�������������� ���� 65.6       ����ת��Ϊ��������λ������ ��λΪ����/s
// ����Ϊ:0x03 ����������Ϊ:��250 dps     ��ȡ�������������� ���� 131.2      ����ת��Ϊ��������λ������ ��λΪ����/s
// ����Ϊ:0x04 ����������Ϊ:��125 dps     ��ȡ�������������� ���� 262.4      ����ת��Ϊ��������λ������ ��λΪ����/s
//================================================���� IMU660RA �ڲ���ַ================================================


//================================================���� IMU660RA ȫ�ֱ���================================================
extern int16 imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z;                 // ��������������      GYRO (������)
extern int16 imu660ra_acc_x, imu660ra_acc_y, imu660ra_acc_z;                    // ������ٶȼ�����     ACC  (accelerometer ���ٶȼ�)
extern float imu660ra_transition_factor[2];                                     // ת��ʵ��ֵ�ı���
//================================================���� IMU660RA ȫ�ֱ���================================================


//================================================���� IMU660RA ��������================================================
void  imu660ra_get_acc              (void);                                     // ��ȡ IMU660RA ���ٶȼ�����
void  imu660ra_get_gyro             (void);                                     // ��ȡ IMU660RA ����������
uint8 imu660ra_init                 (void);                                     // ��ʼ�� IMU660RA
//================================================���� IMU660RA ��������================================================


//================================================���� IMU660RA ��չ����================================================
//-------------------------------------------------------------------------------------------------------------------
// �������     �� IMU660RA ���ٶȼ�����ת��Ϊʵ����������
// ����˵��     acc_value       ������ļ��ٶȼ�����
// ���ز���     void
// ʹ��ʾ��     float data = imu660ra_acc_transition(imu660ra_acc_x);           // ��λΪ g(m/s^2)
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
#define imu660ra_acc_transition(acc_value)      ((float)acc_value / imu660ra_transition_factor[0])

//-------------------------------------------------------------------------------------------------------------------
// �������     �� IMU660RA ����������ת��Ϊʵ����������
// ����˵��     gyro_value      �����������������
// ���ز���     void
// ʹ��ʾ��     float data = imu660ra_gyro_transition(imu660ra_gyro_x);         // ��λΪ ��/s
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
#define imu660ra_gyro_transition(gyro_value)    ((float)gyro_value / imu660ra_transition_factor[1])
//================================================���� IMU660RA ��չ����================================================

#endif
