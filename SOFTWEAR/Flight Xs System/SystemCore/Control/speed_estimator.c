

/*==============================================================================
                         ##### How to use this driver #####
==============================================================================


*/
//�ⲿ�ļ�����
#include "speed_estimator.h"
#include "height_control.h"
#include "SPL06.h"
#include "control.h"
#include "myMath.h"
#include "fmuConfig.h"
#include "StatusConfig.h"

int32_t Terrain_following(int32_t alt,float dt);

//�궨����
#define VELOCITY_LIMIT        (130.f)    /*�ٶ��޷� ��λcm/s*/

//Extern����


//˽�к�����
float applyDeadbandf(float value, float deadband);


#define FIX_A1 0.8f
#define FIX_B1 0.5f
#define FIX_C1 0.2f

#define FIX_A2 1.6f
#define FIX_B2 0.5f
#define FIX_C2 0.2f

//wz_fus struct
float fix_rat[3];
_wz_fus_sta_enum wz_fus_sta;

int32_t obs_wz_velocity;
int32_t obs_wz_height;

int32_t obs_wz_hasl;
int32_t obs_wz_hasl_gnd;

float obs_wz_velocity_ref;
float obs_wz_height_ref;
	
float est_wz_velocity;
float est_wz_height;

float fix_wz_velocity;
float fix_wz_height;
float fix_wz_acceleration;

static uint8_t height_init_f = 10;

static int32_t height_old;
bool Select_Baro = false;

void WZ_Obs_Calcu(float dT_s)//����OBS���ݸ�������
{
	//height
	obs_wz_hasl = (int32_t)(f_SPL06.fALT *100);//cm
	
	if(height_init_f!=0)
	{
		height_init_f --;
		obs_wz_hasl_gnd = obs_wz_hasl;
	}
	else
	{
		obs_wz_height = obs_wz_hasl - obs_wz_hasl_gnd;
		//velocity
		obs_wz_velocity = (int32_t)((obs_wz_height - height_old)/dT_s);
		height_old = obs_wz_height;
		//selector
		obs_wz_velocity_ref = obs_wz_velocity;
		fix_rat[0] = FIX_A1;
		fix_rat[1] = FIX_B1;
		fix_rat[2] = FIX_C1;
	}
}

void WZ_Est_Calcu(float dT_s)//����������ݸ�������
{
    //���ٶȼƻ���
	est_wz_velocity += HeightInfo.Z_Acc *dT_s;
	
    //�ٶ�ֵ����
	est_wz_height += est_wz_velocity *dT_s;
}


void WZ_Fix_Calcu(float dT_s)//�����������
{
	//==calcu
	//�ٶ�ֵ�޷�
	obs_wz_velocity_ref = (obs_wz_velocity_ref > 500) ? 500 : ((obs_wz_velocity_ref < -500 ) ? -500 : obs_wz_velocity_ref);
	
    //����۲�߶Ȼ���ֵ
	obs_wz_height_ref += obs_wz_velocity_ref * dT_s;
	
    //�����ĸ߶�ֵ = �������� * ���۲�߶Ȼ���ֵ - ���ٶȵĻ��ָ߶ȣ�
	fix_wz_height = fix_rat[0] * ((float)obs_wz_height_ref - est_wz_height);
    
	//�������ٶ�ֵ = �������� * ���۲��ٶ�ֵ - ���ٶȵĻ����ٶȣ�
	fix_wz_velocity = fix_rat[1] * ((float)obs_wz_velocity_ref - est_wz_velocity);
    
	//�����ļ��ٶ�ֵ = �������� * ���������ٶ�ֵ��* dt;
	fix_wz_acceleration += fix_rat[2] * (fix_wz_velocity) *dT_s;
    
    //�����ļ��ٶ��޷�
	fix_wz_acceleration = (fix_wz_acceleration > 50) ? 50 : ((fix_wz_acceleration < -50 ) ? -50 : fix_wz_acceleration);

    //==fix
    //
    est_wz_height += fix_wz_height *dT_s;	
    //
    est_wz_velocity += (fix_wz_acceleration + fix_wz_velocity ) *dT_s;
    //==
    
    
    
	HeightInfo.Z_Speed = est_wz_velocity;
	HeightInfo.Z_Postion = est_wz_height;
	
	//
	if(wz_fus_sta == FIX_RESET || g_FMUflg.unlock == 0)
	{
		WZ_Fus_Reset();
		//
		wz_fus_sta = FIX_WORKING;
	}
}

void WZ_Fus_Reset()
{
	obs_wz_height_ref = 0;
	obs_wz_velocity_ref = 0;
	est_wz_height = 0;
    est_wz_velocity = 0;
}

int32_t Terrain_following(int32_t alt,float dt)
{
    static float acc = 0;
    static float velocity = 0;
    static float last_velocity = 0;
    static int32_t last_alt = 0;
    
    if(last_alt == 0)
    {
        last_alt = alt;
        return alt;
    }

    //һ����
    velocity = (alt - last_alt) / dt;
    
    //������
    acc = (velocity - last_velocity) / dt;
    
    last_velocity = velocity;
    last_alt = alt;
    
//    UpdateToGCSLine2(velocity, acc, 0,0,0,0,0,0);
    
    return alt;
}
