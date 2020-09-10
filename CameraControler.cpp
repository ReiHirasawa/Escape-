#include "stdafx.h"
#include "CameraControler.h"

//１フリップ当たりのカメラの回転角度
#define CAMERA_ANGLE_SPEED 3.0f
//カメラの注視点の高さ
#define CAMERA_LOOK_AT_HEIGHT 15.0f
//カメラと注視点の距離
#define CAMERA_LOOK_AT_DISTANCE 100.0f


CameraControler::CameraControler()
{
	hAngle = 0.0f;
	vAngle = -3.0f;
	SetCameraNearFar(15.0f, 3500.0f);

}


CameraControler::~CameraControler()
{
}


void CameraControler::cameraRotationKey() {
	if (CheckHitKey(KEY_INPUT_R) == 1) {
		hAngle -= CAMERA_ANGLE_SPEED;
		if (hAngle <= -180) {
			hAngle += 360;
		}
	}
	if (CheckHitKey(KEY_INPUT_Q) == 1) {
		hAngle += CAMERA_ANGLE_SPEED;
		if (hAngle >= 180) {
			hAngle -= 360;
		}
	}
	if (CheckHitKey(KEY_INPUT_E) == 1) {
		vAngle -= CAMERA_ANGLE_SPEED;
		if (vAngle <= -5.0f) {
			vAngle = -5.0f;
		}
	}
	if (CheckHitKey(KEY_INPUT_W) == 1) {
		vAngle += CAMERA_ANGLE_SPEED;
		if (vAngle >= 89.0f) {
			vAngle = 89.0f;

		}
	}
}

// カメラの位置と向きを算出、セット
void CameraControler::setCameraPosition(VECTOR playerPosition) {

	VECTOR TempPosition1;
	VECTOR TempPosition2;
	VECTOR CameraPosition;
	VECTOR CameraLookAtPosition;
	float sinParam, cosParam;


	// 注視点はキャラクターモデルの座標から CAMERA_LOOK_AT_HEIGHT 分だけ高い位置
	CameraLookAtPosition = playerPosition;
	CameraLookAtPosition.y += CAMERA_LOOK_AT_HEIGHT;

	// カメラの位置をカメラの水平角度と垂直角度から算出
	// 最初に垂直角度を反映した位置を算出
	sinParam = sin(vAngle / 180.0f * DX_PI_F);
	cosParam = cos(vAngle / 180.0f * DX_PI_F);
	TempPosition1.x = 0.0f;
	TempPosition1.y = sinParam * CAMERA_LOOK_AT_DISTANCE;
	TempPosition1.z = -cosParam * CAMERA_LOOK_AT_DISTANCE;

	// 次に水平角度を反映した位置を算出
	sinParam = sin(hAngle / 180.0f * DX_PI_F);
	cosParam = cos(hAngle / 180.0f * DX_PI_F);
	TempPosition2.x = cosParam * TempPosition1.x - sinParam * TempPosition1.z;
	TempPosition2.y = TempPosition1.y;
	TempPosition2.z = sinParam * TempPosition1.x + cosParam * TempPosition1.z;

	// 算出した座標に注視点の位置を加算したものがカメラの位置
	CameraPosition = VAdd(TempPosition2, CameraLookAtPosition);

	// カメラの設定に反映する
	SetCameraPositionAndTarget_UpVecY(CameraPosition, CameraLookAtPosition);}




void CameraControler::setCameraNearFar(float n, float f) {
	SetCameraNearFar(n,f);
}