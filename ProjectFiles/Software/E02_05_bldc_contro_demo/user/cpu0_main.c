#include "zf_common_headfile.h"
#include "isr_config.h"
#pragma section all "cpu0_dsram"

//********************************************************�궨��***********************************************************************
#define IPS200_TYPE          (IPS200_TYPE_PARALLEL8)                            // ˫������ ���������� ����궨����д IPS200_TYPE_PARALLEL8

//********************************************************�궨��***********************************************************************


//******************************************************** ���� ***********************************************************************

//******************************************************** ���� ***********************************************************************


//********************************************************������***********************************************************************
int core0_main(void){
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���

    Motor_Init();
    gps_init();
    Key_Init();

    ips200_set_dir(IPS200_PORTAIT);
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);
    ips200_init(IPS200_TYPE);

    while(1){
        if(imu963ra_init()){
            ips200_show_string(0,16*0,"IMU_Init_Erro");                                  // IMU963RA ��ʼ��ʧ��
        }
        else{
           break;
        }
    }

    gyroOffset_init();

    system_delay_ms(750);           //�ȴ�����Ӳ����ʼ�����

    pit_ms_init(CCU60_CH0, 10);      //IMU�жϼ�� ����
	pit_ms_init(CCU60_CH1, 2);      //����жϼ�� ����

	uart_init(UART_2,115200,UART2_TX_P10_5,UART2_RX_P10_6);

	cpu_wait_event_ready();        // �ȴ����к��ĳ�ʼ�����

	    // VOFA+
	        VOFA* VOFA_pt = vofa_create();       //����VOFA����
	        vofa_init(VOFA_pt,                   //��ʼ����ǰ��vofa����
	                    vofa_ch_data,ch_sz,
	           custom_buf,custom_sz,
	           cmd_rxbuf,cmd_sz,
	           UART_2,UART_2,UART_2);

	while (TRUE)
//******************************************************** ע�� ***********************************************************************
	    //��Ҫ�����޸Ķ��PD���ƵĲ�����������
	    //��Ҫ�����޸Ķ��PD���ƵĲ�����������
	    //��Ҫ�����޸Ķ��PD���ƵĲ�����������
//******************************************************** ע�� ***********************************************************************
	{

	    if(gps_tau1201_flag){
	        gps_tau1201_flag = 0;
	        gps_data_parse();           //��ʼ��������
	      }

	    Scan_Key();
	    VOFA_Sent();

	    ips200_show_string  (0,     16*0,   "Encoder");
	    ips200_show_int     (120,   16*0,   Encoder,            6);
	    ips200_show_string  (0,     16*1,   "Speed");
	    ips200_show_int     (120,   16*1,   Speed_Duty,         6);

	    ips200_draw_line    (0,16*3,239,16*3,RGB565_WHITE);

	    ips200_show_string  (0,     16*4,   "state");
        ips200_show_uint    (120,   16*4,   gps_tau1201.state,    5);
        ips200_show_string  (0,     16*5,   "latitude");
	    ips200_show_float   (120,   16*5,   gps_tau1201.latitude, 4,     6);
	    ips200_show_string  (0,     16*6,   "longitude");
	    ips200_show_float   (120,   16*6,   gps_tau1201.longitude,4,     6);
	    ips200_show_string  (0,     16*7,   "direction");
	    ips200_show_float   (120,   16*7,   gps_tau1201.direction,4,     6);
	    ips200_show_string  (0,     16*8,   "satellite_used");
	    ips200_show_uint    (120,   16*8,   gps_tau1201.satellite_used,  5);
        ips200_draw_line    (0,16*10,239,16*10,RGB565_WHITE);

        ips200_show_string  (0,     16*11,  "Gyro_Z");


        ips200_show_float   (120,   16*14,   eulerAngle.pitch,4,     6);
        ips200_show_float   (120,   16*15,   eulerAngle.roll,4,     6);
        ips200_show_float   (120,   16*16,   eulerAngle.yaw,4,     6);
	}
}
//********************************************************������***********************************************************************


#pragma section all restore
// **************************** �������� ****************************
