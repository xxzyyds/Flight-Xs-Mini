#include <fmuConfig.h>
#include <pidConfig.h>
#include <StatusConfig.h>

f_AL FlightAttitude;
f_Status FlightStatus;
bool InitComplete;

SPL f_SPL06;
SPL06 f_SPL06Params;
SPL06_t device_SPL06;

f_MPU MPU6050;
f_HeightInfo HeightInfo;


