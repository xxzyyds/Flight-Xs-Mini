/**
  ******************************************************************************
  * Copyright (c) 2018,�����пƺƵ�Ƽ����޹�˾
  * All rights reserved.
  * �ļ����ƣ�pid.h
  * ժ    Ҫ��
  *
  * ��ǰ�汾��V1.0
  * ��    �ߣ������пƺƵ�Ƽ����޹�˾�з��� 
  * ������ڣ�    
  * �޸�˵����
  * 
  *
  * ��ʷ�汾��
  *
  *
  *******************************************************************************/
#ifndef __PID_H
#define __PID_H
//�ⲿ�ļ�����
#include <stdbool.h>
#include "stdint.h"
#include "pidConfig.h"

//�궨����


//��������
void reset_i(PID* pid);
void ResetPID(void);
void ClacCascadePID(PID* pidRate, PID* pidAngE, const float dt);  //����PID
void UpdatePID(PID* pid, const float dt);  //PID
#endif

/******************* (C) ��Ȩ���� 2018 �����пƺƵ�Ƽ����޹�˾ *******************/
