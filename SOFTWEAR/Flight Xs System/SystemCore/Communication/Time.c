// the core. 
// control the system by timer interrupt


//  collocate the system timer interrupt
void Timer_Configuration(void)
{
    // todo: collocate the system interrupt
}

// System scheduling timer
void TIM_UP_IRQHandler(void)//5ms刷新一次
{
    if(GetStatus() == 1)
    {
        // check controler msg and update the drone controller params
        Controller_Receive(); 
        // update all sensor info
        SensorUpdate();
        // poseture algorithm
        PoseUpdate();
    }
}