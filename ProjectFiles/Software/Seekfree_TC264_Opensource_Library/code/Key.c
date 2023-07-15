#include "Key.h"

#define KEY1_GPIO               (P20_6)
#define KEY2_GPIO               (P20_7)
#define KEY3_GPIO               (P11_2)
#define KEY4_GPIO               (P11_3)
#define SWITCH1                 (P33_12)
#define SWITCH2                 (P33_13)
#define BUZZER_PIN              (P33_10)

uint8 Key_Num=0;

bool Run_Start_Status=false;
bool Run_Status=false;
bool Servo_Status=false;
bool Gui_Page_Refersh=true;
bool Gui_Status=true;


Key_status Status={1,1,1,1};        // ��ǰ����״̬
Last_status L_status;               // ��һ�ΰ���״̬
Key_flag Flag;                      // ����������־λ
enum Set_Action set_action;

//----------------------------------------------------------------------------------------------------------------
// �������     ����&��������ʼ��
// ����˵��
// ���ز���
// ʹ��ʾ��     Key_Init();
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
void Key_Init(void){
    gpio_init(KEY1_GPIO, GPI, 0, GPI_PULL_UP);
    gpio_init(KEY2_GPIO, GPI, 0, GPI_PULL_UP);
    gpio_init(KEY3_GPIO, GPI, 0, GPI_PULL_UP);
    gpio_init(KEY4_GPIO, GPI, 0, GPI_PULL_UP);
    gpio_init(SWITCH1, GPI, GPIO_HIGH, GPI_FLOATING_IN);    // ��ʼ�� SWITCH1 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(SWITCH2, GPI, GPIO_HIGH, GPI_FLOATING_IN);    // ��ʼ�� SWITCH2 ���� Ĭ�ϸߵ�ƽ ��������

    gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);            // ��ʼ�� BUZZER_PIN ��� Ĭ�ϵ͵�ƽ �������ģʽ
    gpio_set_level(BUZZER_PIN, GPIO_LOW);
}

//----------------------------------------------------------------------------------------------------------------
// �������     ��������
// ����˵��
// ���ز���
// ʹ��ʾ��     Beep();
// ��ע��Ϣ
//----------------------------------------------------------------------------------------------------------------
void Beep(void){
    gpio_set_level(BUZZER_PIN, GPIO_HIGH);
    system_delay_ms(10);
    gpio_set_level(BUZZER_PIN, GPIO_LOW);
}


void Key_Active(void){
    switch (Key_Num){
        case 1:
            Beep();
                if(Page_Num==InfoPage){          //��һҳ��������
                    Run_Start_Status=true;       //����
                    Gui_Status=false;            //�ر�UI��ʾ
                }

                if(Page_Num==SetPage){          //�ڶ�ҳ��������
                    switch (set_action) {
                        case Servo_PD_KP : Servo_pid_KP+=0.5;break;
                        case Servo_PD_KD : Servo_pid_KD+=0.01;break;
                        case Speed : Start_Speed+=5;break;
                        default:break;
                    }
                }
                if(Page_Num==GPSPage){
                    Point_Get();
                }



                Flag.key1 = 0;                  // ʹ�ð���֮��Ӧ�������־λ
                break;
        case 2:
            Beep();
                if(Page_Num==InfoPage){        //��һҳ��������
                    Speed_Duty=0;              //�ٶ�����
                    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100));            //�������
                    Servo_Status=false;        //�����������
                    Run_Status=false;
                    Gui_Status=true;           //����UI��ʾ
                }

                if(Page_Num==SetPage){          //�ڶ�ҳ��������
                    if(Page_Num==SetPage){          //�ڶ�ҳ��������
                        switch (set_action) {
                            case Servo_PD_KP : Servo_pid_KP-=0.5;break;
                            case Servo_PD_KD : Servo_pid_KD-=0.01;break;
                            case Speed : Start_Speed-=5;break;
                            default:break;
                        }
                    }

                    if(Page_Num==GPSPage){
                        Point_Count++;
                    }

                }



                Flag.key2 = 0;                 // ʹ�ð���֮��Ӧ�������־λ
                break;
        case 3:
            Beep();
                if(Page_Num==InfoPage){        //��һҳ��������
                    Point_Count=0;             //��λ����
                    Q_info.q0=1.0f;
                    Q_info.q1=0.0f;
                    Q_info.q2=0.0f;
                    Q_info.q3=0.0f;
                    eulerAngle.yaw = 0.0f;
                    Run_Status=false;
                }

                if(Page_Num==SetPage){          //�ڶ�ҳ��������
                    set_action++;               //�л�������
                    if(set_action>2)
                        set_action=0;
                }

                if(Page_Num==GPSPage){
                    Point_Count--;
                }

                Flag.key3 = 0;                 // ʹ�ð���֮��Ӧ�������־λ
                break;
        case 4:
            Beep();
                 Page_Num++;                   //  �л�ҳ��
                 Gui_Page_Refersh=true;
                 if(Page_Num>2)
                     Page_Num=0;

                 Flag.key4 = 0;                 // ʹ�ð���֮��Ӧ�������־λ
                 break;

    }
    Key_Num=0;
}


//----------------------------------------------------------------------------------------------------------------
// �������     ����ɨ��
// ����˵��
// ���ز���
// ʹ��ʾ��     Key_scan();
// ��ע��Ϣ     ���Է��ж�;
//--------------------------------------------------------------------------------------------------------------
void Key_scan(void)
{


    L_status.key1 = Status.key1;                              // ���水��״̬
    Status.key1 = gpio_get_level(KEY1_GPIO);                  // ��ȡ��ǰ����״̬
    if(Status.key1 && !L_status.key1)                         // ����ɿ������Ķ�����   ��λ��־λ
    {
        Key_Num=1;                                            // ��־λ ��λ ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
    }

    L_status.key2 = Status.key2;                              // ���水��״̬
    Status.key2 = gpio_get_level(KEY2_GPIO);                  // ��ȡ��ǰ����״̬
    if(Status.key2 && !L_status.key2)                         // ����ɿ������Ķ�����   ��λ��־λ
    {
        Key_Num=2;                                            // ��־λ ��λ ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
    }

    L_status.key3 = Status.key3;                              // ���水��״̬
    Status.key3 = gpio_get_level(KEY3_GPIO);                  // ��ȡ��ǰ����״̬
    if(Status.key3 && !L_status.key3)                         // ����ɿ������Ķ�����   ��λ��־λ
    {
        Key_Num=3;                                            // ��־λ ��λ ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
    }

    L_status.key4 = Status.key4;                             // ���水��״̬
    Status.key4 = gpio_get_level(KEY4_GPIO);                 // ��ȡ��ǰ����״̬
    if(Status.key4 && !L_status.key4)                        // ����ɿ������Ķ�����   ��λ��־λ
    {
        Key_Num=4;                                           // ��־λ ��λ ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
    }
}


