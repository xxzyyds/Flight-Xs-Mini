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

//��100hz���ٶ���ѯ 10ms
void UpdateCentControl(float dt)
{
    //���°�ť����ʵ��
    UpdateButton();

    //���³̿�״̬��
    UpdateStatus();

    //���³̿ض�����
    UpdateAction(dt);
}

//�˺���ֻ��״̬�жϺ�״̬����
void UpdateStatus()
{
    //����ActionList�����ݣ����벻ͬ��״̬
    switch (FollowManager.ActionList)
    {
    //�ж�
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
				//�Զ���ɶ�������ʱ��Ϊ5s��500 * 10ms = 5000ms = 5s����Ȼ������ActionHoverStartPoint������
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

//ִֻ�ж���
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
    //��ͣ����
    case ActionHoverStartPoint:
        sdk_velocity_reset();
        break;
    //�Զ�����
    case ActionLand:
        //��������
        sdk_land();
        sdk_velocity_reset();
        break;
    //��������
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
    // �жϰ���
    volatile static uint8_t input = 0;
	
    if (input)
    {
        static bool CloseGate = true;

        // �����߽��뵹��ʱ״̬
				// ��ֹ�ظ�����
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
