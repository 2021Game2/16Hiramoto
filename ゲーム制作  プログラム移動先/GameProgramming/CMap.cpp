#include "CMap.h"
#include"CCollider.h"
#include"CColliderTriangle.h"
#include"CCollisionManager.h"
#define OBJ "sky.obj"
#define MTL "sky.mtl"
CColliderMesh mColliderMesh;
CMap::CMap()

	//:mColliderTriangle(NULL, NULL, CVector(-1000.0f, 0.0f, -1000.0f), TRIANGLE1, TRIANGLE2)
	//, mColliderTriangle2(NULL, NULL, TRIANGLE1,CVector(1000.0f, 0.0f, 1000.0f),  TRIANGLE2)
	
{

	//mBackGroundMatrix.Translate(0.0f, 0.0f, 0.0f);
	mPosition = CVector(0.0f, 1.0f, 0.0f);
	mScale = CVector(1.0f, 1.0f, 1.0f);
	CTransform::Update();

	mModel.Load(OBJ, MTL);//モデルを読み込む
	mpModel = &mModel;//モデルのポインタ化(描画するときに必要)
		mColliderMesh.Set(NULL, NULL, &mModel);//モデルをコライダにする
	//mColliderTriangle.mTag = CCollider::EMAPCOLLIDER;
	//mColliderTriangle2.mTag = CCollider::EMAPCOLLIDER;
}
void CMap::TaskCollision() {
	/*
	mColliderTriangle.ChangePriority();
	mColliderTriangle2.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mColliderTriangle, COLLISIONRANGE);

	CCollisionManager::Get()->Collision(&mColliderTriangle2, COLLISIONRANGE);
	*/
}

