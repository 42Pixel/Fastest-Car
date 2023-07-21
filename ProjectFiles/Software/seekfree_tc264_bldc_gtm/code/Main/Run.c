#include "RUN.H"
KFP KFP_GPSdir={0.02,0,0,0,0.001,0.543};

double run_latitude[Array_SIZE];
double run_longitude[Array_SIZE];

float GpsOffset;
float Direction_E;
float azimuth_N;
float azimuth_E;
float direction_N;
float distance;

uint8 GPS_Count=0;
uint8 Stop_Point=0;
uint8 Back_Point=0;
uint32 Start_Speed=600;

float kalmanFilter(KFP *kfp,float input)
 {
     //Ԥ��Э����̣�kʱ��ϵͳ����Э���� = k-1ʱ�̵�ϵͳЭ���� + ��������Э����
     kfp->Now_P = kfp->LastP + kfp->Q;
     //���������淽�̣����������� = kʱ��ϵͳ����Э���� / ��kʱ��ϵͳ����Э���� + �۲�����Э���
     kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
     //��������ֵ���̣�kʱ��״̬����������ֵ = ״̬������Ԥ��ֵ + ���������� * ������ֵ - ״̬������Ԥ��ֵ��
     kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//��Ϊ��һ�ε�Ԥ��ֵ������һ�ε����ֵ
     //����Э�����: ���ε�ϵͳЭ����� kfp->LastP ����һ������׼����
     kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
     return kfp->out;
 }

void Point_Get(void){
    if(GPS_Count<=Array_SIZE-1){
        run_latitude[GPS_Count]=gps_tau1201.latitude;
        run_longitude[GPS_Count]=gps_tau1201.longitude;
        GPS_Count++;
    }
}

void Run_Status_Set(void){
    if(GPS_Count==Back_Point){
        Run_Back=true;
    }
    if(GPS_Count==Stop_Point){
        Motor_Status=false;
    }
}

void Run_Start(void){
    static double StartPoint_lat,StartPoint_lon;
        if(Run_Start_Status){
            StartPoint_lat = gps_tau1201.latitude;
            StartPoint_lon = gps_tau1201.longitude;
            pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100));            //��һ��ֱ�߻�ȡ�ȶ���GPS����ֵ
            system_delay_ms(3000);
            GpsOffset=get_two_points_azimuth(StartPoint_lat, StartPoint_lon, gps_tau1201.latitude, gps_tau1201.longitude)-eulerAngle.yaw;
            Run_Start_Status=false;
            Servo_Status=true;                          //�����������
            Run_Status=true;
    }
        else if(Run_Status){
            if(GPS_Count<=Array_SIZE-2){
                direction_N = kalmanFilter(&KFP_GPSdir,gps_tau1201.direction);
                //Ŀ��ת���
                azimuth_N= get_two_points_azimuth(run_latitude[GPS_Count], run_longitude[GPS_Count], run_latitude[GPS_Count+1], run_longitude[GPS_Count+1]);
                //���Ƕ�(�������)
                azimuth_E=direction_N-azimuth_N-GpsOffset;   //GPS����Ǽ�ȥĿ�꺽���

                if (azimuth_E > 360) {
                    azimuth_E -= 360;
                    } else if (azimuth_E < 0) {
                        azimuth_E += 360;
                    }

                //��Ŀ������
                distance= get_two_points_distance(gps_tau1201.latitude, gps_tau1201.longitude, run_latitude[GPS_Count+1], run_longitude[GPS_Count+1]);
                //��Ŀ���ķ����
                Direction_E=get_two_points_azimuth(gps_tau1201.latitude, gps_tau1201.longitude, run_latitude[GPS_Count+1], run_longitude[GPS_Count+1]);


                if ((distance <= 1.5)){
                    GPS_Count++;
                }
                else if(((Direction_E >= 90) || (Direction_E <= -90)) && (distance <= 2)){
                    GPS_Count++;
                }
            }
        }
}

