#pragma once
#include "Model.h"
class EnemyModel :
	public Model
{
public:
	EnemyModel(int);
	//EnemyModel(EnemyModel&&);
	~EnemyModel();
	VECTOR enemyToPlayer;
	void renewPosition(VECTOR);
};

