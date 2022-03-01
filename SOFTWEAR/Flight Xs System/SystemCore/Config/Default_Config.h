// init default params

// sysStatus flag 0: loced  1: locked 2: reset to land
uint16_t SysStatus = 0; 



// sensor info 

// poseture info
    // current altitude
float CurrentAltitude;
    // current speed x, y, z
float CurrentSpeedX;
float CurrentSpeedY;
float CurrentSpeedZ;
    // current accelerated speed x, y, z
float CurrentAccX;
float CurrentAccY;
float CurrentAccZ;
    // current angle roll, yoll, yaw, pitch
float CurrentRoll;
float CurrentYaw;
float CurrentPitch;

// controller params
u8 Data_Buf[32]={0,0};



// drone controller params
uint16_t ControllerChannel[10]={0};