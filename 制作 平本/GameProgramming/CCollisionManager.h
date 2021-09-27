#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H
#include "CTaskManager.h"
#include"CCollider.h"


#define COLLISIONRANGE 30

class CCollisionManager :public CTaskManager 
{
	//マネージャのインスタンス
	static CCollisionManager* mpInstance;
public:
	static CCollisionManager* Get();
	void Collision();
	void Collision(CCollider* c, int range);

	
};
#endif