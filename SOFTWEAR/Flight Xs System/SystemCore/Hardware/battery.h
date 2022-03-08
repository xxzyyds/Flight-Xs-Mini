#ifndef _BATTERY_H
#define _BATTERY_H

#include "main.h"
#include "stdbool.h"

typedef struct
{
    bool BatteryConnected;
    uint16_t ADC_Value;
    uint8_t BatteryPercent;
}BatteryInfo_t;

extern BatteryInfo_t g_BatteryInfo;

void BatteryInit(void);
void BatteryAlarmHandle(void);
void update_battery_value(void);





#endif
