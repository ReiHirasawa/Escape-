#pragma once
#pragma once
#include "stdafx.h"
#include "DxLib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>

class CameraControler
{
public:
	float  hAngle, vAngle;
	VECTOR lookAtPosition;

	CameraControler();
	~CameraControler();
	void cameraRotationKey();
	void setCameraNearFar(float, float);
	void setCameraPosition(VECTOR);
};

