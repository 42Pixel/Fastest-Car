#include "RUN.H"

// ���忨�����˲����ṹ��
typedef struct {
    float x;    // ����ֵ
    float P;    // �������ķ���
    float Q;    // ���������ķ���
    float R;    // ���������ķ���
} KalmanFilter;

float GpsOffset;
float azimuth_N,azimuth_E,direction_N;
float distance;
uint8 RunStart_Status=0;

float run_latitude[5]={32.885948,32.886196,32.886440};
float run_longitude[5]={117.427848,117.427833,117.427803};


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



void Run_Start(void){
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100));            //��һ��ֱ�߻�ȡ�ȶ���GPS����ֵ
    Speed_Duty=15;

    for(int i=0;i<20;i++){
        GpsOffset+=gps_tau1201.direction;
        system_delay_ms(100);
    }
    GpsOffset/=20;
    RunStart_Status=1;
}

void Run(void){
    static uint8 count=0;
 // ��ʼ���������˲���
    KalmanFilter filter;
    kalmanFilterInit(&filter, 1.0, 1.0, 0.01, 0.1);
    direction_N= kalmanFilterUpdate(&filter,gps_tau1201.direction);

    azimuth_N= get_two_points_azimuth(run_latitude[count], run_longitude[count], run_latitude[count+1], run_longitude[count+1]);

    distance= get_two_points_distance(gps_tau1201.latitude, gps_tau1201.longitude, run_latitude[count], run_longitude[count]);

    if (azimuth_N > 180) {
        azimuth_N -= 360;
       } else if (azimuth_N < -180) {
           azimuth_N += 360;
       }



    azimuth_E=direction_N-azimuth_N;   //Ŀ�꺽��Ǽ�ȥGPS�����


    if ((distance <= 1.2))
            {
                count++;
            }
    else if(((azimuth_E >= 60) || (azimuth_E <= -60)) && (distance <= 2))
            {
                count++;
            }
    else if (count==2){
        Speed_Duty=0;
    }

}

