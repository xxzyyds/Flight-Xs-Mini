// init --> getInfo by IMU ang receive Msg form controller --> Control the motor --> getInfo

// 硬件资源 + 相关外设初始化
void NCQ_Init(void)
{
    Key_Init();
}



/*************************** While(1) only detect ****************************************************/
/*************************** core：传感器滤波、姿态解算、惯导、控制 run in TIME.c  **********************************/
int main()
{
    // init hardware and software
    NCQ_Init();
    delay_ms(500);
    while(1)
    {
        Key_Scan();
    }
}