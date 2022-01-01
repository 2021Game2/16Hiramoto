#include "CMap.h"
#include"CCollider.h"
#include"CColliderTriangle.h"
#include"CCollisionManager.h"
#define TRIANGLE1 CVector(-50.0f, 0.1f, 50.0f)
#define TRIANGLE2 CVector(50.0f, 0.1f, -50.0f)
CMap::CMap()

	:mColliderTriangle(NULL, NULL, CVector(-50.0f, 0.0f, -50.0f), TRIANGLE1, TRIANGLE2)
	, mColliderTriangle2(NULL, NULL, TRIANGLE1,CVector(50.0f, 0.0f, 50.0f),  TRIANGLE2)
	
{
	mPosition = CVector(0.0f, 1.0f, 0.0f);
	mScale = CVector(1.0f, 1.0f, 1.0f);
	CTransform::Update();

	mModel.Load("sky.obj", "sky.mtl");
	mpModel = &mModel;
	mColliderTriangle.mTag = CCollider::EMAP;
	mColliderTriangle2.mTag = CCollider::EMAP;
}
void CMap::TaskCollision() {
	mColliderTriangle.ChangePriority();
	mColliderTriangle2.ChangePriority();
	//Õ“Ëˆ—‚ðŽÀs
	CCollisionManager::Get()->Collision(&mColliderTriangle, COLLISIONRANGE);

	CCollisionManager::Get()->Collision(&mColliderTriangle2, COLLISIONRANGE);
}

