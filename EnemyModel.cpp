#include "stdafx.h"
#include "EnemyModel.h"


EnemyModel::EnemyModel(int loadedModel) : Model(loadedModel)
{
	position = VGet(0.0f, 0.0f, 1000.0f);
	
	int size = rand() % 3 + 3 ;
	Model::setScale(VGet(size, size, size));
	setOpacityRate(GHOST_TRANSPARENT_RATE);
	angle = 270.0;
}
//EnemyModel::EnemyModel(EnemyModel&& obj) noexcept : Model(obj.modelHandle)
//{
//	position = VGet(0.0f, 0.0f, 1000.0f);
//	int size = rand() % 4 + 2 ;
//	Model::setScale(VGet(size, size, size));
//	setOpacityRate(GHOST_TRANSPARENT_RATE);
//	angle = 270.0;
//}


EnemyModel::~EnemyModel()
{
}

//position, movePosition, cameraHAngle���瑖��v���C���[�̈ʒu�ƌ������Z�o
void EnemyModel::renewPosition(VECTOR playerPosition) {
	//�Rd���f���̈ʒu�ƌ������Z�o�A�Z�b�g



	moveVector = VGet(VNorm(enemyToPlayer).x * GHOST_MOVESPEED, 0.0f, VNorm(enemyToPlayer).z*GHOST_MOVESPEED);
	position = VAdd(position, moveVector);
	angle = atan2(position.x - playerPosition.x, position.z - playerPosition.z) / DX_PI_F * 180.0f;

}


