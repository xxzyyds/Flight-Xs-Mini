#define _MY_MAIN_
#include "hardware.h"
#include "kernel.h"
#include "include.h"
// init --> getInfo by IMU ang receive Msg form controller --> Control the motor --> getInfo

/*************************** While(1) only detect ****************************************************/
/*************************** core：传感器滤波、姿态解算、惯导、控制 run in TIME.c  **********************************/
int main()
{
    // init hardware and software
    //MCU初始化
    MCU_Init();
    
    while(true)
    {
        //核心轮训 
        KernelPolling();
    }
}