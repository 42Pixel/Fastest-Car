#include "vofa.h"

//Vofaר�û�����
float vofa_ch_data[ch_sz]    = {1.2,2.4,3.6,4.8};                  //������ͨ������
uint8_t custom_buf[custom_sz]  = {0};                              //��λ������״̬���ݻ�����
uint8_t cmd_rxbuf[cmd_sz] = {0};                                   //��λ�������������


/**********************************************************************************************************
 *  @fcn    ��ʼ��VOFAЭ������ͷ�VOFA����ռ�õ���Դ
 *  @para vofa_pt:vofa����
 *              ch_data_pt:ͨ�������ݻ�����
 *********************************************************************************************************/
VOFA* vofa_create(void){
        VOFA* vofa_pt = (VOFA*)malloc(sizeof(VOFA));                //����VOFA����
                                                                    //����ͨ��������������ͬ��Ķ���
                                                                    //---
        return vofa_pt;
}

VOFA_STATE vofa_init(VOFA* vofa_pt,
                                            void*    ch_data_pt     ,uint32_t ch_size,\
                                            uint8_t* custom_buf_pt  ,uint32_t custom_size,\
                                            uint8_t* cmd_rxbuf_pt   ,uint32_t cmd_size,\
                                            uint8_t  VOFA_UART        ,uint8_t  BLE_UART,uint8_t SCOPE_UART)
{
        //vofa���ݻ�����
            //1.��ͨ���ݻ��������(��Ҫ���ڰ���λ�������ݷ��͵���λ��)
        vofa_pt->ch_data_pt = ch_data_pt;
        vofa_pt->ch_size      = ch_size;
            //2.�Զ��������ݻ��������
        vofa_pt->custom_buf_pt  = custom_buf_pt;
        vofa_pt->custom_buf_size = custom_size;
            //3.����ָ�������ݻ��������(��Ҫ���ڽ�����λ���Ŀ���ָ��)
        vofa_pt->cmd_rxbuf  = cmd_rxbuf_pt;
        vofa_pt->cmd_size       = cmd_size;

        //����ʵ�ֶ˿�
            //1.vofa��λ���Ķ˿�
        vofa_pt->VOFA_UART  = VOFA_UART;
            //2.�����������Ķ˿�
        vofa_pt->BLE_UART   = BLE_UART;
            //3.����ʾ�����Ķ˿�
        vofa_pt->SCOPE_UART = SCOPE_UART;

        //Э��涨����������
            //1.JF��FWͨ�õ�ͼ������ǰ��֡(�û��ɵ���image_property_set��vofa_preframe�����ǰ5��Ԫ�أ���ЩԪ����������������ͼƬ����Ϣ)
        vofa_pt->vofa_preframe[5] = 0x7F800000;
        vofa_pt->vofa_preframe[6] = 0x7F800000;
            //2.JFͨ�������ݵĹ̶�֡β
        vofa_pt->vofa_JF_tail[0] = 0x00;
        vofa_pt->vofa_JF_tail[1] = 0x00;
        vofa_pt->vofa_JF_tail[2] = 0x80;
        vofa_pt->vofa_JF_tail[3] = 0x7F;

        //vofa��������ķ���
        vofa_pt->vofa_release  = vofa_release;
        vofa_pt->sendzip = vofa_sendzip;
        return VOFA_OK;
}

//����ʼ��
VOFA_STATE vofa_release(VOFA* vofa_pt){
    if(!vofa_pt)    free(vofa_pt);
    else    return VOFA_ERR;
    return VOFA_OK;
}



/**********************************************************************
 *  @fcn    ���ݷ��ͺ���(˽�к���)
 *  @para vofa_pt:vofa����
 *              vofa_frame:�����������֡
 **********************************************************************/
//��������֡ͷ��֡β���������֣�
static VOFA_STATE vofa_transmit_leader(VOFA* vofa_pt,uint8_t vofa_frame_leader){
    if(vofa_pt == NULL) return VOFA_ERR;
    if(vofa_frame_leader == JF_TAIL){
        uart_write_buffer(vofa_pt->VOFA_UART,vofa_pt->vofa_JF_tail,4);                      //����λ������JFͨ�������ݵ�֡β��4byte
        return VOFA_OK;
    }
    else if(vofa_frame_leader == JF_PREFRAME){
        uart_write_buffer(vofa_pt->VOFA_UART,(uint8_t*)(vofa_pt->vofa_preframe),28);        //����λ������JFͼ��ǰ��֡��28byte
        return VOFA_OK;
    }
    return VOFA_ERR;
}


//�����û�����
static VOFA_STATE vofa_transmit(VOFA* vofa_pt,uint8_t vofa_frame_category){
    if(vofa_pt == NULL) return VOFA_ERR;
    if(vofa_frame_category == VOFA_CH_FRAME){
        uart_write_buffer(vofa_pt->VOFA_UART,(uint8_t*)(vofa_pt->ch_data_pt),vofa_pt->ch_size);
        return VOFA_OK;
    }
    else if(vofa_frame_category == VOFA_IMAGE_FRAME){
        uart_write_buffer(vofa_pt->VOFA_UART,(uint8_t*)(vofa_pt->image_pt),vofa_pt->vofa_preframe[2]);
        return VOFA_OK;
    }
    return VOFA_ERR;
}



/**********************************************************************
 *  @fcn    ���ݴ�����û�ָ�������Ͳ�����
 *  @para vofa_pt:VOFA����;
 *              vofa_protocal:���ݴ���Э��
 *              vofa_frame_category:���������ݵ�����
 *              rawdataΪԭʼ����
 **********************************************************************/
//��λ�����ݴ��
VOFA_STATE vofa_sendzip(VOFA* vofa_pt,uint8_t vofa_protocol,uint8_t vofa_frame_category){
    if(vofa_pt == NULL) return VOFA_ERR;
        //JFЭ��//
        if(vofa_protocol == VOFA_PROTOCOL_JUSTFLOAT){
            if(vofa_frame_category == VOFA_CH_FRAME){                                        //С����ͨ��ģʽ
                        if(vofa_transmit(vofa_pt,VOFA_CH_FRAME)){                            //�����û�����(ͨ������)
                                vofa_transmit_leader(vofa_pt,JF_TAIL);                       //����֡β
                                return VOFA_OK;
                        }
                        else return VOFA_ERR;
             }

//            else if(vofa_frame_category == VOFA_IMAGE_FRAME){                           //ͼ��ģʽ
//                vofa_transmit_leader(vofa_pt,JF_PREFRAME);                              //����ǰ��֡
//                if(vofa_transmit(vofa_pt,VOFA_IMAGE_FRAME)){                            //�����û����ݣ�ͼ�����ͣ�
//                    return VOFA_OK;
//                }
//            }
        }

        //FWЭ��//
        else if(vofa_protocol == VOFA_PROTOCOL_FIREWATER){
            if(vofa_frame_category == VOFA_CH_FRAME){
                uint32_t ch_size = vofa_pt->ch_size/sizeof(float);
                uint8_t i = 0;
                for(;i < ch_size;i++){
                    VOFA_Printf("%f",((float*)(vofa_pt->ch_data_pt))[i]);              //С����ͨ��ģʽ����ǰ��֡��
                    if(i < ch_size-1){
                        VOFA_Printf(",");
                    }
                }
                VOFA_Printf("\n");                                                    //���з���FWЭ���֡β��
                return VOFA_OK;
            }
        }
        else if(vofa_protocol == EXT_PROTOCOL_BLEDEBUGGER){
            uint16_t checksum = 0;
            uart_write_byte(vofa_pt->BLE_UART,0xA5);                                     //����֡ͷ
            uart_write_buffer(vofa_pt->BLE_UART,custom_buf,sizeof(custom_buf));          //�����û�����
            for (int i = 1;i < sizeof(custom_buf)-2;i++){
                checksum += custom_buf[i];
            }
            uart_write_byte(vofa_pt->BLE_UART,(uint8_t)(checksum&0xff));                 //����У��λ(ps.ǿ��ת����������λ)
            uart_write_byte(vofa_pt->BLE_UART,0x5A);                                             //����֡β
            return VOFA_OK;
        }

        //��������ʾ����Э��(v1.0)
        //��ע����Э�������byte���͵ı�����Ҳ����˵��ʹ��ǰ��Ҫ��float_to_byte֮��Ĳ���(��fast_algorithm��)
        else if(vofa_protocol == EXT_PROTOCOL_DATASCOPE){
            uart_write_byte(vofa_pt->SCOPE_UART,'$');//����֡ͷ
            uart_write_buffer(vofa_pt->SCOPE_UART,custom_buf,28);//����7��ͨ�������ݣ�Ҫ����λ�����͵���������Ϊfloat��
            return VOFA_OK;
        }
        return VOFA_ERR;
}

void VOFA_Sent(void){
//    VOFA_Printf("%f,%f,%f,%f,%f,%f\n",eulerAngle.yaw,gps_tau1201.direction,Direction_E,azimuth_N,azimuth_E,distance);
//        vofa_ch_data[1]=eulerAngle.yaw;
//        vofa_ch_data[2]=gps_tau1201.direction;
//        vofa_ch_data[3]=Direction_E;
//        vofa_ch_data[4]=azimuth_N;
//        vofa_ch_data[5]=azimuth_E;
//        vofa_ch_data[6]=distance;

        VOFA_Printf("%f,%f,%f,%f,%f,%f\n",eulerAngle.yaw,gps_tau1201.direction,direction_N,azimuth_N,azimuth_E,GpsOffset);
               vofa_ch_data[1]=eulerAngle.yaw;
               vofa_ch_data[2]=gps_tau1201.direction;
               vofa_ch_data[3]=direction_N;
               vofa_ch_data[4]=azimuth_N;
               vofa_ch_data[5]=azimuth_E;
               vofa_ch_data[6]=GpsOffset;
}
