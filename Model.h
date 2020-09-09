#pragma once
#include "stdafx.h"
#include "DxLib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
class Model
{
public:
	int modelHandle, attachIndex, animIndexToBeAttached, animIndexAttached;
	VECTOR position, moveVector;
	float angle, totalTime, playTime;


	Model(int);
	~Model();

	void setScale(VECTOR);
	


};


