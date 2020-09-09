#include "stdafx.h"
#include "Model.h"



Model::Model(int loadedModel)
{
	modelHandle = loadedModel;
	animIndexAttached = 999;
	playTime = 0.0f;
	angle = 0.0f;
	position = VGet(0.0f, 0.0f, 0.0f);

	Model::setScale(VGet(1,1,1));



}


Model::~Model()
{
}

void Model::setScale(VECTOR size) {
	MV1SetScale(modelHandle, size);
}




