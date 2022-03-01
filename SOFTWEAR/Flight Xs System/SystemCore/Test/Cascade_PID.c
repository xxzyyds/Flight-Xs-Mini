#include <stdio.h>
#include <stdlib.h>
#include<string.h>              //strlen函数的头文件
//g++ main.c -o main.exe
//./mian.exe 

typedef struct{
    float Set;           //定义设定值
    float voltage;         //定义电压值（控制执行器的变量）
    float integral;           //定义积分值
    float Actual;        //定义实际值
    float err;               //定义偏差值
    float err_last;           //定义上一个偏差值
    float Kp,Ki,Kd;           //定义比例、积分、微分系数
    
}PID;
PID Extern_PID,Inside_PID; 
float Speed = 0;
float Location = 0;
float Exp_Speed = 0;
float dt = 0.2;
char buf[10];

//外环PID初始化
void Extern_PID_Init(){
    printf("Extern_PID_Init begin \n");
    Extern_PID.Set=0.0;
    Extern_PID.Actual=0.0;
    Extern_PID.err=0.0;
    Extern_PID.err_last=0.0;
    Extern_PID.voltage=0.0;
    Extern_PID.integral=0.0;
    Extern_PID.Kp = 0.2f;
    Extern_PID.Ki = 0;
    Extern_PID.Kd = 0;
    printf("Extern_PID_Init end \n");
}
//内环PID初始化
void Inside_PID_Init(){
    printf("Inside_PID_Init begin \n");
    Inside_PID.Set=0.0;
    Inside_PID.Actual=0.0;
    Inside_PID.err=0.0;
    Inside_PID.err_last=0.0;
    Inside_PID.voltage=0.0;
    Inside_PID.integral=0.0;
    Inside_PID.Kp = 0.2f;
    Inside_PID.Ki = 0.015f;
    Inside_PID.Kd = 0.2f;
    printf("Inside_PID_Init end \n");
}

//传入期望位置、返回当前位置、计算期望速度给到内环
float Extern_PID_Realize(float Location , float Speed){
    Extern_PID.Set = Location;//定义设定值
    Extern_PID.err = Extern_PID.Set - Extern_PID.Actual;//定义偏差值
    Extern_PID.err_last = Extern_PID.err;//定义上一个偏差值
    Extern_PID.integral += Extern_PID.err; //定义积分值
    Extern_PID.voltage = Extern_PID.Kp*Extern_PID.err + Extern_PID.Ki*Extern_PID.integral + Extern_PID.Kd*(Extern_PID.err-Extern_PID.err_last);//定义电压值（控制执行器的变量）
    Extern_PID.Actual += Speed * dt;//定义实际值
    return Extern_PID.Actual;
}

//传入期望速度、返回当前速度、当前位置
float Inside_PID_Realize(float Speed){
    Inside_PID.Set = Speed;//定义设定值
    Inside_PID.err = Inside_PID.Set - Inside_PID.Actual;//定义偏差值
    Inside_PID.err_last = Inside_PID.err;//定义上一个偏差值
    Inside_PID.integral += Inside_PID.err; //定义积分值
    Inside_PID.voltage = Inside_PID.Kp*Inside_PID.err + Inside_PID.Ki*Inside_PID.integral + Inside_PID.Kd*(Inside_PID.err-Inside_PID.err_last);//定义电压值（控制执行器的变量）
    Inside_PID.Actual = Inside_PID.voltage*1.0;//定义实际值
    return Inside_PID.Actual;
}
void main(){
    FILE *fp = fopen("PID.csv","w");
    Extern_PID_Init();
    Inside_PID_Init();
    for(int i = 0 ;i<=200;i++)
    {
        Location = Extern_PID_Realize(100 , Speed);
        //计算期望速度给到内环
        Exp_Speed = 10 * Extern_PID.err;
        Speed = Inside_PID_Realize(Exp_Speed);
        printf("%f , %f\n" , Speed , Location);
        //(0,100)
        sprintf(buf,"%.3f,",Location);
		fwrite(buf,sizeof(char),sizeof(buf),fp);
    }
    system("pause");
}
