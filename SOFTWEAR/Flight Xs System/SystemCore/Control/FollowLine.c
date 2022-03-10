#include "FollowLine.h"
#include "stdbool.h"
#include "pid.h"
#include "timer_drv.h"
#include "myMath.h"
#include "program_ctrl.h"
#include "fmuConfig.h"
#include "StatusConfig.h"
#include "SDK.h"
#include "pidConfig.h"

extern PID PIDGroup[Num_Of_PIDList];
extern u16 val, spd;
extern _program_ctrl_st program_ctrl;
bool FollowLine = false;
FollowManager_t FollowManager;

void UpdateStatus(void);
void UpdateAction(float dt);
void UpdateButton(void);
void ActionHoldPoint(int8_t Err, int16_t HoldTime, FSMList_t NextAction);

/*
        |+X
        |
        |
+Y------------- -Y
        |
        |
        |-X
        
        
        
*/

//以100hz的速度轮询 10ms
void UpdateCentControl(float dt)
{
    //更新按钮控制实践
    UpdateButton();

    //更新程控状态线
    UpdateStatus();

    //更新程控动作线
    UpdateAction(dt);
}

//此函数只做状态判断和状态更新
void UpdateStatus()
{
    //根据ActionList的内容，进入不同的状态
    switch (FollowManager.ActionList)
    {
    //判断
    case ActionWaitting:
        //Do nothing;
        break;
    case ActionCountdown:
        FollowManager.CountDownNumMs--;

        if (FollowManager.CountDownNumMs <= 0)
        {
            FollowManager.ActionList = ActionTakeOff;
        }
        break;
    case ActionTakeOff:
    {
				//自动起飞动作持续时间为5s（500 * 10ms = 5000ms = 5s），然后跳到ActionHoverStartPoint动作；
				ActionHoldPoint(MAX_HOVER_ERR, 500, ActionHoverStartPoint);
    }
    break;
    case ActionHoverStartPoint:
        ActionHoldPoint(MAX_HOVER_ERR, 10000, ActionLand);
        break;

    case ActionLand:
    {
        static int Cnt = MAX_TIMEOUT1;

        if (Cnt-- < 0)
        {
            FollowManager.ActionList = ActionLock;
        }
    }
    break;
    case ActionLock:
        FollowManager.ActionList = ActionWaitting;
        break;
    default:
        break;
    }
}

//只执行动作
void UpdateAction(float dt)
{
    switch (FollowManager.ActionList)
    {
    case ActionWaitting:
        //Do nothing
        break;
    case ActionTakeOff:
        sdk_takeoff(80);
        break;
    //悬停命令
    case ActionHoverStartPoint:
        sdk_velocity_reset();
        break;
    //自动降落
    case ActionLand:
        //降落命令
        sdk_land();
        sdk_velocity_reset();
        break;
    //上锁动作
    case ActionLock:
        FlightStatus.unlock = 0;
        break;
    default:
        break;
    }
}

void ActionHoldPoint(int8_t Err, int16_t HoldTime, FSMList_t NextAction)
{
    static bool Enter = true;
    static uint16_t CountDown = 0;

    if (Enter)
    {
        CountDown = HoldTime;
        Enter = false;
    }
    else
    {
        CountDown--;
    }

    if (CountDown == 0)
    {
        Enter = true;
        FollowManager.ActionList = NextAction;
    }
}


void UpdateButton()
{
    // 判断按键
    volatile static uint8_t input = 0;
	
    if (input)
    {
        static bool CloseGate = true;

        // 动作线进入倒计时状态
				// 防止重复进入
        if (CloseGate)
        {
            CloseGate = false;
            FollowManager.ActionList = ActionCountdown;
        }
    }
    else
    {
        return;
    }
}
