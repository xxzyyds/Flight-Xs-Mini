#ifndef __Status_CONFIG_H
#define __Status_CONFIG_H

#include "include.h"
//�궨����
#define WAITING 0
#define WAITING_1 1
#define READY 2
#define PROCESS 3
#define EXIT 4


// ����״̬��־λ������������־λ��
// to do : ʹ�� bit-field �����ڴ�ռ��
typedef struct
{
		// ����ť����
    uint8_t unlock;
    // �̿�����
		uint8_t Auto;
	
		uint8_t height_lock;
		
		// ң���� �������
    uint8_t take_off;
    uint8_t take_down;
} f_Status;

// ��Ԫ��
typedef struct {
  float q0;
  float q1;
  float q2;
  float q3;
} Quaternion;


// �Ƕ�״̬����
typedef struct{
    float roll;
    float pitch;
    float yaw;
} f_AL;

//��ǰ�Ƕ���ֵ̬
extern f_AL FlightAttitude;
extern f_Status FlightStatus;


#endif
