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
uint8 Point_Count=0;
uint8 Point_Get_Count=0;
uint8 Start_Speed=20;


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

void Run(void){

 // ��ʼ���������˲���
    KalmanFilter filter;
    kalmanFilterInit(&filter, 1.0, 1.0, 0.01, 0.1);
    direction_N= kalmanFilterUpdate(&filter,gps_tau1201.direction)-GpsOffset;

    azimuth_N= get_two_points_azimuth(run_latitude[Point_Count], run_longitude[Point_Count], run_latitude[Point_Count+1], run_longitude[Point_Count+1])-GpsOffset;
    if (azimuth_N > 180) {
        azimuth_N -= 360;
       } else if (azimuth_N < -180) {
           azimuth_N += 360;
       }
    distance= get_two_points_distance(gps_tau1201.latitude, gps_tau1201.longitude, run_latitude[Point_Count+1], run_longitude[Point_Count+1]);

    azimuth_E=direction_N-azimuth_N;   //Ŀ�꺽��Ǽ�ȥGPS�����


    if ((distance <= 1.2))
            {
        Point_Count++;
            }
    else if(((azimuth_E >= 60) || (azimuth_E <= -60)) && (distance <= 2))
            {
        Point_Count++;
            }
    else if (Point_Count>=2){
        Speed_Duty=0;
    }

}

void Point_Get(void){
    if(Point_Get_Count<=Array_SIZE-1){
        run_latitude[Point_Get_Count]=gps_tau1201.latitude;
        run_longitude[Point_Get_Count]=gps_tau1201.longitude;
        Point_Get_Count++;
    }
}

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
//
//        for(int i=0;i<50;i++){
//            GpsOffset+=gps_tau1201.direction;
//            VOFA_Sent();
//            system_delay_ms(100);
//        }
//        GpsOffset/=50;


        Run_Start_Status=false;
        Servo_Status=true;                          //�����������

    }
    else{
        Run();
    }

}

