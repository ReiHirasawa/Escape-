#include "stdafx.h"
#include "PlayerModel.h"



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




PlayerModel::PlayerModel(int loadedModel) : Model(loadedModel)
{
}




PlayerModel::~PlayerModel()
{
}



//方向キー：1フリップでのキャラクターの移動ベクトルと向きを設定, アニメーションナンバーを走りに設定
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

//position, movePosition, cameraHAngleから走るプレイヤーの位置と向きを算出
void PlayerModel::calPosition(const float cameraHAngle) {
	//３dモデルの位置と向きを算出、セット
	if (animIndexToBeAttached == ANIM_RUN) {
		VECTOR tempMoveVector;
		// カメラの角度に合わせて移動ベクトルを回転してから加算
		float sinParam = sin(cameraHAngle / 180.0f * DX_PI_F);
		float cosParam = cos(cameraHAngle / 180.0f * DX_PI_F);
		tempMoveVector.x = moveVector.x * cosParam - moveVector.z * sinParam;
		tempMoveVector.y = 0.0f;
		tempMoveVector.z = moveVector.x * sinParam + moveVector.z * cosParam;
		position = VAdd(position, tempMoveVector);
	}
}