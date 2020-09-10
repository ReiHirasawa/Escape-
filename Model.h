#pragma once
#include "stdafx.h"
#include "DxLib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>

//1�t���b�v������̈ړ���
#define MOVESPEED 10.0f
// �ҋ@�A�j���[�V�����̔ԍ�
#define ANIM_NEUTRAL 0
//����A�j���[�V�����̔ԍ�
#define ANIM_RUN 1
//�W�����v�A�j���[�V�����̔ԍ�
#define ANIM_JUMP 2
//�U���A�j���[�V�����̔ԍ�
#define ANIM_ATTACK 3
//�P�t���b�v������̃J�����̉�]�p�x
#define CAMERA_ANGLE_SPEED 2.0f
//�J�����̒����_�̍���
#define CAMERA_LOOK_AT_HEIGHT 15.0f
//�J�����ƒ����_�̋���
#define CAMERA_LOOK_AT_DISTANCE 100.0f
//�G�̐�
#define GHOST_NUMBER  15
//�G�̓�������
#define GHOST_MOVESPEED 9.0f
//�G�̏o��������
#define GHOST_DISTANCE 400.0f
//�G�̏o���Ԋu����
#define GHOST_TIME 100
//�G�̓��ߓx����
#define GHOST_TRANSPARENT_RATE 0.5f

class Model
{
public:
	int modelHandle, attachIndex, animIndexToBeAttached, animIndexAttached, attachedAnimHandle;
	VECTOR position, moveVector;
	float angle, totalTime, playTime;


	Model(int);
	~Model();

	void setScale(VECTOR);
	void drawModel();
	void animChange();
	void setOpacityRate(float rate);
	//void drawPositionedModel
	
	


};


