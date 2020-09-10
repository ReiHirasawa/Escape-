#include "stdafx.h"
#include "CameraControler.h"

//�P�t���b�v������̃J�����̉�]�p�x
#define CAMERA_ANGLE_SPEED 3.0f
//�J�����̒����_�̍���
#define CAMERA_LOOK_AT_HEIGHT 15.0f
//�J�����ƒ����_�̋���
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

// �J�����̈ʒu�ƌ������Z�o�A�Z�b�g
void CameraControler::setCameraPosition(VECTOR playerPosition) {

	VECTOR TempPosition1;
	VECTOR TempPosition2;
	VECTOR CameraPosition;
	VECTOR CameraLookAtPosition;
	float sinParam, cosParam;


	// �����_�̓L�����N�^�[���f���̍��W���� CAMERA_LOOK_AT_HEIGHT �����������ʒu
	CameraLookAtPosition = playerPosition;
	CameraLookAtPosition.y += CAMERA_LOOK_AT_HEIGHT;

	// �J�����̈ʒu���J�����̐����p�x�Ɛ����p�x����Z�o
	// �ŏ��ɐ����p�x�𔽉f�����ʒu���Z�o
	sinParam = sin(vAngle / 180.0f * DX_PI_F);
	cosParam = cos(vAngle / 180.0f * DX_PI_F);
	TempPosition1.x = 0.0f;
	TempPosition1.y = sinParam * CAMERA_LOOK_AT_DISTANCE;
	TempPosition1.z = -cosParam * CAMERA_LOOK_AT_DISTANCE;

	// ���ɐ����p�x�𔽉f�����ʒu���Z�o
	sinParam = sin(hAngle / 180.0f * DX_PI_F);
	cosParam = cos(hAngle / 180.0f * DX_PI_F);
	TempPosition2.x = cosParam * TempPosition1.x - sinParam * TempPosition1.z;
	TempPosition2.y = TempPosition1.y;
	TempPosition2.z = sinParam * TempPosition1.x + cosParam * TempPosition1.z;

	// �Z�o�������W�ɒ����_�̈ʒu�����Z�������̂��J�����̈ʒu
	CameraPosition = VAdd(TempPosition2, CameraLookAtPosition);

	// �J�����̐ݒ�ɔ��f����
	SetCameraPositionAndTarget_UpVecY(CameraPosition, CameraLookAtPosition);}




void CameraControler::setCameraNearFar(float n, float f) {
	SetCameraNearFar(n,f);
}