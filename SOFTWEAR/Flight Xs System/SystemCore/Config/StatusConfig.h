#include "include.h"


//�궨����
#define WAITING 1
#define READY 2
#define PROCESS 3
#define EXIT 4

// ���״̬����
#define MOTOR_MAX_VALUE 200


// ����״̬��־λ������������־λ��
typedef struct
{
		// ����ť����
    uint8_t unlock:0;
    // �̿�����
		uint8_t Auto:0;
	
		uint8_t height_lock:0;
		
		// ң���� �������
    uint8_t take_off:0;
    uint8_t take_down:0;
}FS;

FS FlightStatus;


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
}FA;

//��ǰ�Ƕ���ֵ̬
FA FlightAttitude;





