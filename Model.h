#pragma once
#include "stdafx.h"
#include "DxLib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>

//1フリップあたりの移動量
#define MOVESPEED 10.0f
// 待機アニメーションの番号
#define ANIM_NEUTRAL 0
//走りアニメーションの番号
#define ANIM_RUN 1
//ジャンプアニメーションの番号
#define ANIM_JUMP 2
//攻撃アニメーションの番号
#define ANIM_ATTACK 3
//１フリップ当たりのカメラの回転角度
#define CAMERA_ANGLE_SPEED 2.0f
//カメラの注視点の高さ
#define CAMERA_LOOK_AT_HEIGHT 15.0f
//カメラと注視点の距離
#define CAMERA_LOOK_AT_DISTANCE 100.0f
//敵の数
#define GHOST_NUMBER  15
//敵の動く速さ
#define GHOST_MOVESPEED 9.0f
//敵の出現時距離
#define GHOST_DISTANCE 400.0f
//敵の出現間隔時間
#define GHOST_TIME 100
//敵の透過度合い
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


