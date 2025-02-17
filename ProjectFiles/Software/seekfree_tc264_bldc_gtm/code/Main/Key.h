#ifndef _KEY_H_
#define _KEY_H_

#include "zf_common_headfile.h"
#include "stdbool.h"

#define KEY1_GPIO               (P20_6)
#define KEY2_GPIO               (P20_7)
#define KEY3_GPIO               (P20_2)
#define KEY4_GPIO               (P20_0)
#define BUZZER_PIN              (P33_10)

enum Set_Action{
    Servo_PD_KP,
    Servo_PD_KD,
    Speed,
    Stop,
    Back
};

typedef struct {
    uint8 key1;
    uint8 key2;
    uint8 key3;
    uint8 key4;
} Key_status;

typedef struct {
    uint8 key1;
    uint8 key2;
    uint8 key3;
    uint8 key4;
} Last_status;

typedef struct {
    uint8 key1;
    uint8 key2;
    uint8 key3;
    uint8 key4;
} Key_flag;

extern bool Run_Start_Status;
extern bool Run_Status;
extern bool Run_Back;

extern bool Servo_Status;
extern bool Gui_Page_Refersh;
extern bool Gui_Status;
extern bool Motor_Status;
extern uint8 Key_Num;

extern void Key_Init(void);
extern void Key_scan(void);
extern void Key_Active(void);

#endif
