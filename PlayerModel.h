#pragma once
#include "Model.h"

class PlayerModel :
	public Model
{
public:
	PlayerModel(int);
	~PlayerModel();
	
	void moveKey(const float);
	void calPosition( const float );
};

