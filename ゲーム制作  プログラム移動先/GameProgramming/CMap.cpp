#include "CMap.h"
#include"CCollider.h"
#include"CColliderTriangle.h"
#include"CCollisionManager.h"
#define OBJ "3DModel\\Map\\sky.obj"
#define MTL "3DModel\\Map\\sky.mtl"
CColliderMesh mColliderMesh;
CMap::CMap()
{
	mTag = EMAP;
	mPosition = CVector(0.0f, 1.0f, 0.0f);
	mScale = CVector(1.0f, 1.0f, 1.0f);
	CTransform::Update();

	mModel.Load(OBJ, MTL);//モデルを読み込む
	mpModel = &mModel;//モデルのポインタ化(描画するときに必要)
		mColliderMesh.Set(this, &mMatrix, &mModel);//モデルをコライダにする
	
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

