#include "RUN.H"

// ���忨�����˲����ṹ��
typedef struct {
    float x;    // ����ֵ
    float P;    // �������ķ���
    float Q;    // ���������ķ���
    float R;    // ���������ķ���
} KalmanFilter;

double run_latitude[Array_SIZE];
double run_longitude[Array_SIZE];


float GpsOffset;
float azimuth_N,azimuth_E,direction_N;
float distance;
float Filter_buff[10] = {0}; // �˲�������

uint8 Point_Count=0;
uint8 Start_Speed=15;


// ��ʼ���������˲���
void kalmanFilterInit(KalmanFilter* filter, float initialValue, float initialError, float processNoise, float measurementNoise) {
    filter->x = initialValue;
    filter->P = initialError;
    filter->Q = processNoise;
    filter->R = measurementNoise;
}


// �������˲�
double kalmanFilterUpdate(KalmanFilter* filter, double measurement) {
    // Ԥ�ⲽ��
    double x_pred = filter->x;
    double P_pred = filter->P + filter->Q;

    // ���²���
    double K = P_pred / (P_pred + filter->R);
    filter->x = x_pred + K * (measurement - x_pred);
    filter->P = (1 - K) * P_pred;

    return filter->x;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����һ��ֵ����һ��ֵ������ֵȡƽ�� | �����˲�
//  @param      ���˲���ֵ
//  @return
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------

float Movingaverage_filter(float value,float Filter_buff[])
{
    int8_t i = 0;//����
    float temp = value;
    float Filter_sum = 0;
    Filter_buff[Filter_N] = temp;

    for(i = 0; i < Filter_N; i++)
    {
        Filter_buff[i] = Filter_buff[i+1];      //��������
        Filter_sum += Filter_buff[i];
    }
    temp = Filter_sum / Filter_N;
    return temp;
}

void Point_Get(void){
    if(Point_Count<=Array_SIZE-1){
        run_latitude[Point_Count]=gps_tau1201.latitude;
        run_longitude[Point_Count]=gps_tau1201.longitude;
        Point_Count++;
    }
}

//void Run(void){
//
//// // ��ʼ���������˲���
////    KalmanFilter filter;
////    kalmanFilterInit(&filter, 1.0, 1.0, 0.01, 0.1);
////    direction_N= kalmanFilterUpdate(&filter,gps_tau1201.direction)-GpsOffset;
//
//}

void Run_Start(void){
    static double startpoint1[2];
    static double startpoint2[2];
    if(Run_Start_Status){
        startpoint1[0] = gps_tau1201.latitude;
        startpoint1[1] = gps_tau1201.longitude;
        pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100));            //��һ��ֱ�߻�ȡ�ȶ���GPS����ֵ
        Speed_Duty=Start_Speed;
        system_delay_ms(3000);
        startpoint2[0] = gps_tau1201.latitude;
        startpoint2[1] = gps_tau1201.longitude;
        GpsOffset=get_two_points_azimuth(startpoint1[0], startpoint1[1], startpoint2[0], startpoint2[1])-eulerAngle.yaw;
        if (GpsOffset > 180) {
            GpsOffset -= 360;
        }
        Run_Start_Status=false;
        Servo_Status=true;                          //�����������
        Run_Status=true;
    }
    else{
//        Run();
        direction_N = Movingaverage_filter(gps_tau1201.direction, Filter_buff);

            azimuth_N= get_two_points_azimuth(run_latitude[Point_Count], run_longitude[Point_Count], run_latitude[Point_Count+1], run_longitude[Point_Count+1])-GpsOffset;
            if (azimuth_N > 180) {
                azimuth_N -= 360;
               } else if (azimuth_N < -180) {
                   azimuth_N += 360;
               }
            azimuth_E=direction_N-azimuth_N-GpsOffset;   //Ŀ�꺽��Ǽ�ȥGPS�����

            distance= get_two_points_distance(gps_tau1201.latitude, gps_tau1201.longitude, run_latitude[Point_Count+1], run_longitude[Point_Count+1]);
    }

}

