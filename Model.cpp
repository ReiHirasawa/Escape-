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
	// ���܂łƏ�Ԃ��ω������ꍇ�̓A�j���[�V������ύX
	if (animIndexToBeAttached != animIndexAttached) {

		// ���܂ŃA�^�b�`���Ă����A�j���[�V�������f�^�b�`, �V�����A�j���[�V�������A�^�b�`
		MV1DetachAnim(modelHandle, attachedAnimHandle);
		attachedAnimHandle = MV1AttachAnim(modelHandle, animIndexToBeAttached, -1, FALSE);
			animIndexAttached = animIndexToBeAttached;
		
		totalTime = MV1GetAttachAnimTotalTime(modelHandle, attachedAnimHandle);
		//playTime = 0.0f;
	}

	// �Đ����Ԃ�i�߂�
	playTime += 1.5f;
	// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB�����烋�[�v
	if (playTime >= totalTime) {
		playTime = playTime - totalTime;
	}
	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(modelHandle, attachedAnimHandle, playTime);

}




