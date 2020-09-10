#include "stdafx.h"
#include "Model.h"









Model::Model(int loadedModel)
{
	modelHandle = loadedModel;
	animIndexAttached = 999;
	playTime = 0.0f;
	angle = 0.0f;
	position = VGet(5.0f, 0.0f, 0.0f);

	Model::setScale(VGet(1,1,1));



}


Model::~Model()
{
}

void Model::setScale(VECTOR size) {
	MV1SetScale(modelHandle, size);
}

void Model::setOpacityRate(float rate) {
	MV1SetOpacityRate(modelHandle,rate);
}


void Model::drawModel() {
	
	MV1SetRotationXYZ(modelHandle, VGet(0.0f, angle / 180.0f * DX_PI_F, 0.0f));
	MV1SetPosition(modelHandle, position);
	MV1DrawModel(modelHandle);
}

void Model::animChange() {
	// 今までと状態が変化した場合はアニメーションを変更
	if (animIndexToBeAttached != animIndexAttached) {

		// 今までアタッチしていたアニメーションをデタッチ, 新しいアニメーションをアタッチ
		MV1DetachAnim(modelHandle, attachedAnimHandle);
		attachedAnimHandle = MV1AttachAnim(modelHandle, animIndexToBeAttached, -1, FALSE);
			animIndexAttached = animIndexToBeAttached;
		
		totalTime = MV1GetAttachAnimTotalTime(modelHandle, attachedAnimHandle);
		//playTime = 0.0f;
	}

	// 再生時間を進める
	playTime += 1.5f;
	// 再生時間がアニメーションの総再生時間に達したらループ
	if (playTime >= totalTime) {
		playTime = playTime - totalTime;
	}
	// 再生時間をセットする
	MV1SetAttachAnimTime(modelHandle, attachedAnimHandle, playTime);

}




