#pragma once
#include "CManager.h"
#include "CEnemy.h"

#include <vector>

class CEnemyManager : public CManager<CEnemy*> {
private:
	static CEnemyManager* inst;

public:
	static CEnemyManager* GetInstance();

//	std::vector<CEnemy*>& GetArray();
	std::list<CEnemy*>& GetArray();

};
