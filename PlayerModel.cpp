#include "stdafx.h"
#include "PlayerModel.h"



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




PlayerModel::PlayerModel(int loadedModel) : Model(loadedModel)
{
}




PlayerModel::~PlayerModel()
{
}



//�����L�[�F1�t���b�v�ł̃L�����N�^�[�̈ړ��x�N�g���ƌ�����ݒ�, �A�j���[�V�����i���o�[�𑖂�ɐݒ�
void PlayerModel::moveKey(const float cameraHAngle) {
	if (CheckHitKey(KEY_INPUT_LEFT) == 1 && CheckHitKey(KEY_INPUT_DOWN) != 1 && CheckHitKey(KEY_INPUT_UP) != 1) {
		angle = 90.0f - cameraHAngle;
		moveVector.x = -MOVESPEED;
		animIndexToBeAttached = ANIM_RUN;
	}
	else if (CheckHitKey(KEY_INPUT_LEFT) == 1 && CheckHitKey(KEY_INPUT_DOWN) == 1) {
		angle = 45.0f - cameraHAngle;
		moveVector.x = -cos(45.0 / 180.0f * DX_PI_F) * MOVESPEED;
		moveVector.z = -cos(45.0 / 180.0f * DX_PI_F) * MOVESPEED;
		animIndexToBeAttached = ANIM_RUN;
	}
	else if (CheckHitKey(KEY_INPUT_LEFT) == 1 && CheckHitKey(KEY_INPUT_UP) == 1) {
		angle = 135.0f - cameraHAngle;
		moveVector.x = -cos(45.0 / 180.0f * DX_PI_F) * MOVESPEED;
		moveVector.z = cos(45.0 / 180.0f * DX_PI_F) * MOVESPEED;
		animIndexToBeAttached = ANIM_RUN;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && CheckHitKey(KEY_INPUT_UP) != 1 && CheckHitKey(KEY_INPUT_DOWN) != 1) {
		angle = -90.0f - cameraHAngle;
		moveVector.x = MOVESPEED;
		animIndexToBeAttached = ANIM_RUN;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && CheckHitKey(KEY_INPUT_DOWN) == 1) {
		angle = -45.0f - cameraHAngle;
		moveVector.x = cos(45.0 / 180.0f * DX_PI_F) * MOVESPEED;
		moveVector.z = -cos(45.0 / 180.0f * DX_PI_F) * MOVESPEED;
		animIndexToBeAttached = ANIM_RUN;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && CheckHitKey(KEY_INPUT_UP) == 1) {
		angle = -135.0f - cameraHAngle;
		moveVector.x = cos(45.0 / 180.0f * DX_PI_F) * MOVESPEED;
		moveVector.z = cos(45.0 / 180.0f * DX_PI_F) * MOVESPEED;
		animIndexToBeAttached = ANIM_RUN;

	}
	else if (CheckHitKey(KEY_INPUT_UP) == 1 && CheckHitKey(KEY_INPUT_RIGHT) != 1 && CheckHitKey(KEY_INPUT_LEFT) != 1) {
		angle = 180.0f - cameraHAngle;
		moveVector.z = MOVESPEED;
		animIndexToBeAttached = ANIM_RUN;
	}
	else if (CheckHitKey(KEY_INPUT_DOWN) == 1 && CheckHitKey(KEY_INPUT_RIGHT) != 1 && CheckHitKey(KEY_INPUT_LEFT) != 1) {
		angle = 0.0f - cameraHAngle;
		moveVector.z = -MOVESPEED;
		animIndexToBeAttached = ANIM_RUN;
	}
	else if (CheckHitKey(KEY_INPUT_DOWN) != 1 && CheckHitKey(KEY_INPUT_RIGHT) != 1 && CheckHitKey(KEY_INPUT_LEFT) != 1 && CheckHitKey(KEY_INPUT_DOWN) != 1) {

		animIndexToBeAttached = ANIM_NEUTRAL;
	}

}

//position, movePosition, cameraHAngle���瑖��v���C���[�̈ʒu�ƌ������Z�o
void PlayerModel::calPosition(const float cameraHAngle) {
	//�Rd���f���̈ʒu�ƌ������Z�o�A�Z�b�g
	if (animIndexToBeAttached == ANIM_RUN) {
		VECTOR tempMoveVector;
		// �J�����̊p�x�ɍ��킹�Ĉړ��x�N�g������]���Ă�����Z
		float sinParam = sin(cameraHAngle / 180.0f * DX_PI_F);
		float cosParam = cos(cameraHAngle / 180.0f * DX_PI_F);
		tempMoveVector.x = moveVector.x * cosParam - moveVector.z * sinParam;
		tempMoveVector.y = 0.0f;
		tempMoveVector.z = moveVector.x * sinParam + moveVector.z * cosParam;
		position = VAdd(position, tempMoveVector);
	}
}