#ifndef _VOFA_H_
#define _VOFA_H_

/*********************************************************************************************************************
 * @file            main
 * @author          HHF
 * @version         2022/02/25
 * @Software          MDK 5.32
 * @brief               ͨ��vofa��justfloat����firewaterЭ�鴫������
 * @note                    ��λ�����ֽڽ�����ѡ��Ҫ��ѡʮ�����ƣ���ʮ�����Ʒ�ʽ��ӡ�ַ�������ֻ�ܴ�ӡ���롣
 * @note                    arm�ĵ�Ƭ������С����ʽ�ģ�little Eidian��
 ********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "zf_common_typedef.h"                     //��ɵ�common.h�а���uint8_t���͵Ķ���ȱ�Ҫ����
#include "zf_driver_uart.h"                        //vofa��������ʵ��
#include "zf_common_function.h"

//Vofaר�û������ӿ�
#define ch_sz     4
//#define image_sz  100
#define custom_sz 4*7
#define cmd_sz    3+2*5
extern float    vofa_ch_data[ch_sz];
//extern uint8_t vofa_image[image_sz];
extern uint8_t custom_buf[custom_sz];      //��λ�����ͻ�����
//tx:float yaw��accl(preprocessed)x,y,z��gyro,x,y,z
extern uint8_t cmd_rxbuf[cmd_sz];          //��λ�����ջ�����
//rx:char start/stop,LED,reserved,short forward/backward+leftshift/rightshift��yaw_adj��cam_rotate��rod_rotate



//Vofaר�õ�״̬��
typedef uint8_t VOFA_STATE;
enum VOFA_STATE{
        VOFA_ERR = 0,
        VOFA_OK,
};


//vofa֧�ֵ�Э�飨����EXT�ı�ʾ��vofa��Э�飩
enum vofa_protocol{
        VOFA_PROTOCOL_FIREWATER = 0,
        VOFA_PROTOCOL_JUSTFLOAT,        //��Э����С�˸���������ʽ���ֽ���Э�飬��ʮ�����Ƹ��㴫�䣬��ʡ������\
                                                                //--��Э��ǳ��ʺ�����ͨ�������ࡢ����Ƶ�ʸߵ�ʱ��
        VOFA_PROTOCOL_RAWDATA,          //���ֽ���
        EXT_PROTOCOL_BLEDEBUGGER,       //�����ֻ��˵������������ļ�Э��
        EXT_PROTOCOL_DATASCOPE,         //���Զ�ʮͨ��ʾ����Э��
};



enum vofa_frame_category{                                   //����֡������//
        VOFA_CH_FRAME = 0,                                      //ͨ�����ͣ�����ͨ��������������һ�£�
        VOFA_IMAGE_FRAME,                                           //ͼ������
        VOFA_RAMDOM_FRAME,                                      //ͨ������
};

enum vofa_frame_leader{                                     //��ͬ��������֡����������//
        JF_TAIL = 0,                                                    //JFЭ���ͨ������
        JF_PREFRAME,                                                    //JFЭ���ͼ������
        EXT_BLE
};

#define VOFA_SINGLE_IMAGE_TRANSMIT 0            //����ͼƬʱĬ��Ϊ��ͨ��
//#define VOFA_MULTI_IMAGE_TRANSMIT 1           //Ҳ��������Ϊ��ͨ��
#ifdef VOFA_SINGLE_IMAGE_TRANSMIT
    #define IMG_ID  0                                               //Ĭ����ͨ��0����ͼƬ����
#endif



typedef struct vofa{
        //Э��涨����������
        uint32_t vofa_preframe[7];  //JF��FWЭ��ͨ�õ�ͼƬǰ��֡
        uint8_t vofa_JF_tail[4];        //JFЭ�����е�֡β

        //vofa���ݻ�����
        void*       ch_data_pt;             //ͨ��������ר��
        uint32_t    ch_size;                    //ͨ�������ݵ��ֽ���
        uint8_t*    image_pt;                   //ͼ��������ר��
        uint8_t*  custom_buf_pt;        //�Զ���������ר��
        uint32_t    custom_buf_size; //�Զ��������ݵ��ֽ���
        uint8_t*    cmd_rxbuf;              //����ָ��������ר��
        uint32_t    cmd_size;                   //����ָ�������ݵ��ֽ���

        //����ʵ�ֶ˿�
        uint8_t VOFA_UART;                  //��Vofa��λ��ͨ�ŵĶ˿�
        uint8_t BLE_UART;                       //������/WIFIģ��ͨ�ŵĶ˿�
        uint8_t SCOPE_UART;                 //��Datascope��λ��ͨ�ŵĶ˿�

        //vofa��������ķ���
        VOFA_STATE   (*vofa_release)                        (struct vofa* vofa_pt);
        VOFA_STATE   (*sendzip)                     (struct vofa* vofa_pt,uint8_t vofa_protocol,uint8_t vofa_frame_category);       //�����Ѵ��������
        VOFA_STATE   (*unzip)                       (struct vofa* vofa_pt,uint8_t vofa_protocol,uint8_t msg_catagory);                                                              //������λ�����͵�����
        VOFA_STATE   (*image_property_set)(struct vofa* vofa_pt,uint32_t id,uint32_t size,uint32_t width,uint32_t height,uint32_t format);  //����ͼƬ����
        VOFA_STATE   (*image_property_get)(struct vofa* vofa_pt,uint32_t id);                                                                                                                               //��ȡͼƬ����
} VOFA;



/**********************************************************************
 *  @fcn    ��ʼ��VOFAЭ������ͷ�VOFA����ռ�õ���Դ
 *  @para vofa_pt:vofa����
 *              ch_data_pt:ͨ�������ݻ�����
 **********************************************************************/
VOFA* vofa_create(void);
VOFA_STATE vofa_release(VOFA* vofa_pt);
VOFA_STATE vofa_init(VOFA* vofa_pt,
                                            void*    ch_data_pt     ,uint32_t ch_size,\
                                            uint8_t* custom_buf_pt  ,uint32_t custom_buf_size,\
                                            uint8_t* cmd_rxbuf_pt   ,uint32_t cmd_size,\
                                            uint8_t  VOFA_UART        ,uint8_t  BLE_UART,uint8_t SCOPE_UART);

/**********************************************************************
 *  @fcn    ���ݷ��ͺ���(˽�к���)
 *  @para vofa_pt                           :vofa����
 *              vofa_frame_leader       :��ǰЭ��涨��������������
 *              vofa_frame_category :���������ݵ�����
 **********************************************************************/
//��������������
static VOFA_STATE vofa_transmit_leader(VOFA* vofa_pt,uint8_t vofa_frame_leader);
//�����û�����
static VOFA_STATE vofa_transmit(VOFA* vofa_pt,uint8_t vofa_frame_category);



/**********************************************************************
 *  @fcn    ���ݴ�����û�ָ�������Ͳ�����
 *  @para vofa_pt                         :VOFA����
 *              vofa_protocal               :����λ��ͨ��Э��
 *              vofa_frame_category :���������ݵ�����
 *              rawdataΪԭʼ����
 **********************************************************************/
VOFA_STATE vofa_sendzip(VOFA* vofa_pt,uint8_t vofa_protocol,uint8_t vofa_frame_category);





extern void VOFA_Sent(void);

#endif /* CODE_VOFA_H_ */