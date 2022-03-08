#ifndef __FOLLOW_LINE_H
#define __FOLLOW_LINE_H

#include "stdint.h"
#include "stdbool.h"
#include "pid.h"
#include "fmuConfig.h"
#include "StatusConfig.h"

//单位:10ms
#define MAX_COUNTDOWN 100 * 8
#define TARGETALTITUDECM 50
#define MAX_HOVER_ERR 10
#define MAX_FORMCHANGE_TIME 3
#define MAX_HOVER_TIME 300
#define MAX_TIMEOUT1 500
#define MAX_TIMEOUT2 1500
#define MAX_ALT_ERR 10

/*

*/

typedef enum
{
    CmdNone = 0,
    CmdTakeOff,
    CmdLand,
    CmdUp,
    CmdDown,
    CmdForward,
    CmdBack,
    CmdLeft,
    CmdRight,
    CmdCCW,
    CmdCW,
    CmdEmergencyShutDown,
    CmdSpeeedControl,

    NumofCmd,
} FMUCmd_t;

typedef enum
{
    Vertical = 0,        //竖线
    Horizontal,          //横线
    Cross,               //十字
    Ttype,               //T字形
    TurnTtype,           //倒T字形
    Ltype,               //L字形
    MirrorFlipLtype,     //镜像翻转L字形
    TurnLtype,           //倒L字形
    MirrorFlipTurnLtype, //镜像翻转倒L字形
    LeftTtype,
    ApriTag = 100,
    Pole = 200,                //绿色200   红色204
    Cirle = 202,
    FirstOrdCode = 203,
    SecondOrdCode,

    NumofForm,
} FormType_t;

typedef enum
{
    ActionWaitting = 0,
    ActionCountdown,
    ActionTakeOff,
    ActionHoverStartPoint,
	
		HeightDec,
		HeightInc,
    //悬停动作
    ActionHoldLtype,
    ActionHoldMirrorFlipLtype,
    ActionHoldTurnLtype,
    ActionHoldMirrorFlipTurnLtype,
    ActionHoldCross,
    ActionHoldTtype,
    ActionHoldTurnTtype,
    ActionHoldFeaturePoint,
    ActionHoldLeftTtype,
    ActionHoldApriTag,

    //飞行动作
    ActionGoForward,
    ActionGoRight,
    ActionGoLeft,
    ActionGoBack,
    ActionGoRound,
    ActionGoForward2,
    ActionFindPole,
    ActionCloseToPole,
    ActionTurnRound,
    ActionFindLandSpace,
    ActionResetAngle,
    ActionPreLand,
    ActionStop,

    //寻杆步骤
    ActionFindPoleStep1,
    ActionFindPoleStep2,
    ActionFindPoleStep3,
    ActionFindPoleStep4,
    ActionFindPoleStep5,
    ActionFindPoleStep6,

    //给黄色一维码拍照
    ActionFindoneDimensionalCodeStep1,
    ActionFindoneDimensionalCodeStep2,
    ActionFindoneDimensionalCodeStep3,

    ActionRotateServer,
    ActionHoverStopPoint,
    ActionFollowTarget,
    ActionLostTargetInfo,
    ActionLand,
    ActionLock,
    ActionTest,
    ActionSonar,
		
		ActionFind_A,
		ActionFind_Ten,
		ActionAlignment_A,
		ActionAlignment_Ten,
	
		ActionAlignment_rightpoint,
		ActionFind_rightpoint,
		
	
		ActionHoverStartPoint0,
		ActionHoverStartPoint1,
		ActionHoverStartPoint2,
		ActionHoverStartPoint3,
		ActionHoverStartPoint4,
		ActionHoverStartPoint5,
		
		Point1,
		Point2,
		Point3,
		Point4,
		Point5,
		Point6,
		Point7,
		Point8,
		Point9,
		Point10,
		Point11,
		Point12,
		Point13,
		Point14,
		Point15,
		Point16,
		Point17,
		Point18,
		Point19,
		Point20,
		Point21,
		Point22,
		Point23,
		Point24,
		Point25,
		Point26,
		Point27,
		Point28,
		Point29,
		Point30,
		Point31,
		Point32,
		Point33,
		Point34,
		Point35,
		Point36,
		Point37,
		Point38,
		
		sPoint1,
		sPoint2,
		sPoint3,
		sPoint4,
		sPoint5,
		sPoint6,
		sPoint7,
		sPoint8,
		sPoint9,
		sPoint10,
		sPoint11,
		sPoint12,
		sPoint13,
		sPoint14,
		sPoint15,
		sPoint16,
		sPoint17,
		sPoint18,
		sPoint19,
		sPoint20,
		sPoint21,
		sPoint22,
		sPoint23,
		sPoint24,
		sPoint25,
		sPoint26,
		sPoint27,
		sPoint28,
		sPoint29,
		sPoint30,
		sPoint31,
		sPoint32,
		sPoint33,
		sPoint34,
		sPoint35,
		sPoint36,
		sPoint37,
		sPoint38,
		
		GoHomeY,
		GoHomeX,
		
		FindA,
		
		FindPoleLeft,
		FindPoleRight,
		FindPole,
		NearPole,
		
		
		
    ActionEmergencyLand,

    NumofActionList,
} FSMList_t;

typedef enum
{
    ActionTimes1 = 0,
    ActionTimes2,
    ActionTimes3,
    ActionTimes4,
} ActionTimes_t;

typedef struct
{
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
} Line_t;

typedef struct
{
    int16_t x1;
    int16_t y1;
} Point_t;

//数据结构声明
#pragma pack(1)
typedef struct
{
    int16_t Start;
    //uint16_t Cnt;
    uint8_t cnt1;
    uint8_t Target;
    FormType_t FormType;
    Point_t CentPoint;
    int16_t End;
} OpenMVFrame_t;

typedef struct
{
    int16_t Start;
    uint8_t cnt1;
    uint8_t Target;
    FormType_t FormType;
    Point_t CentPoint;
    uint16_t Area;
    uint8_t prame2;
    uint8_t prame3;
    uint8_t prame4;
    int16_t End;
} OpenMVFrame2_t;
#pragma pack()

typedef struct
{
    uint16_t SonarF;
    uint16_t SonarB;
    uint16_t SonarL;
    uint16_t SonarR;
} SonarManager_t;

typedef struct
{
    //OpenMV数据帧   图像的形状、位置
    OpenMVFrame_t *GroundOpenmvFramePtr;
    OpenMVFrame_t *FrontOpenmvFramePtr;
    OpenMVFrame2_t *FrontOpenmvFramePtr2;
    OpenMVFrame_t *ptrFrame;
    //飞机的状态
    UAVInfo *ptrUAVInfo;

    //动作序列
    FSMList_t ActionList;

    //CountDownNumMs倒计时数据
    int16_t CountDownNumMs;
    int16_t TargetAltitudeCM;
    int16_t WatchDogCnt;

    //3个PID控制器
    PIDInfo_t *ptrPIDInfoV;
    PIDInfo_t *ptrPIDInfoH;
    PIDInfo_t *ptrPIDInfoY;

    float distance;

    bool ActionComplete;
} FollowManager_t;

extern FollowManager_t FollowManager;
extern SonarManager_t SonarManager;

void UpdateCentControl(float dt);
void Follow_Init(void);
#endif
