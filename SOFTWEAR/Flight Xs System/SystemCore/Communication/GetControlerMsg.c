// get controler info to change drone controller params

void Controller_Receive(void)
{
    // todo: check the info from controler and change drone controller params
    // example:
        // if(NRF24L01_RxPacket(Data_buf)==0)
        // {
                // Controller_Channel[0]=((uint16_t)(Data_buf[6]<<8)|Data_buf[7])*3000/4095;     //横滚角ch1
                // Controller_Channel[1]=((uint16_t)(Data_buf[4]<<8)|Data_buf[5])*3000/4095;     //俯仰角ch2
                // Controller_Channel[2]=((uint16_t)(Data_buf[0]<<8)|Data_buf[1])*1000/4095+1000;//油门通道ch3
                // Controller_Channel[3]=((uint16_t)(Data_buf[2]<<8)|Data_buf[3])*3000/4095;     //偏航通道ch4
                // Controller_Channel[4]=((uint16_t)((Data_buf[12]&0x0008)>>3)*2000);              //波动开关ch5
                // Controller_Channel[5]=((uint16_t)((Data_buf[12]&0x0004)>>2)*2000);              //波动开关ch6
                // Controller_Channel[6]=((uint16_t)((Data_buf[12]&0x0002)>>1)*2000);              //波动开关ch7
                // Controller_Channel[7]=((uint16_t)((Data_buf[12]&0x0001)>>0)*2000);              //波动开关ch8
        // }
}