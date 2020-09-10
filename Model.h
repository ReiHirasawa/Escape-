#pragma once
#include "stdafx.h"
#include "DxLib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
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
	//void drawPositionedModel
	
	


};


