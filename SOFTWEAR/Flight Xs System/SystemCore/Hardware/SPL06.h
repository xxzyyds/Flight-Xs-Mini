
#ifndef SPL06_H
#define SPL06_H
//�ⲿ�ļ�����
#include "main.h"
#include "stdbool.h"

//�궨����
#define HW_ADR_L                    0x76//SDO LOW
#define HW_ADR                      HW_ADR_L<<1
#define CONTINUOUS_PRESSURE         1
#define CONTINUOUS_TEMPERATURE      2
#define CONTINUOUS_P_AND_T          3
#define PRESSURE_SENSOR             0
#define TEMPERATURE_SENSOR          1
#define OFFSET_COUNT                30



//��������
void ResetAlt(void);
void  SPL06_Init(void);
float GetSPL06Temp(void);
float GetSPL06Press(void);
void  UpdateSPL06Info(void);

#define MAX_DEVICE_NAME_LENGTH  15
typedef struct
{
    //����������
    bool (*FInit)(void);
    void (*FUpdate)(void);
    
    bool bCheck;
    bool bEnable;
    char DeviceName[MAX_DEVICE_NAME_LENGTH];
}Device_t;

typedef enum
{
	SPL06_SENSOR_PRESSURE    = 0,
	SPL06_SENSOR_TEMPERATURE = 1,	
}SPL06_SENSOR_TARG;

typedef enum
{
	SPL06_STANDBY_MODE    = 1,	/*����ģʽ*/
	SPL06_COMMAND_MODE    = 2,	/*����ģʽ*/	
	SPL06_BACKGROUND_MODE = 3,	/*��̨ģʽ*/
}SPL06_MEAS_MODE;

typedef enum
{
	/*Standby Mode*/
	SPL06_IDLE_OR_STOP_BACKGROUND_MEAS_TYPE = 0,

	/*Command Mode(�����һ�β���)*/	
	SPL06_PRESSURE_MEAS_TYPE			    = 1,
	SPL06_TEMPERATURE_MEAS_TYPE			    = 2,
	SPL06_NA_1_TYPE 						= 3,
	SPL06_NA_2_TYPE 						= 4,
	
	/*Background Mode(��̨�Զ���������)*/
	SPL06_CONTINUOUS_PRESSURE_TYPE    	    = 5,
	SPL06_CONTINUOUS_TEMPERATURE_TYPE       = 6,
	SPL06_CONTINUOUS_P_AND_T_TYPE           = 7,
}SPL06_MEAS_TYPE;


bool SPL06Init(void);
void SPL06Update(void);


#endif
