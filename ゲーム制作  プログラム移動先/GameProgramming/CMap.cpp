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

	mModel.Load(OBJ, MTL);//���f����ǂݍ���
	mpModel = &mModel;//���f���̃|�C���^��(�`�悷��Ƃ��ɕK�v)
		mColliderMesh.Set(this, &mMatrix, &mModel);//���f�����R���C�_�ɂ���
	
}
void CMap::TaskCollision() {
	/*
	mColliderTriangle.ChangePriority();
	mColliderTriangle2.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mColliderTriangle, COLLISIONRANGE);

	CCollisionManager::Get()->Collision(&mColliderTriangle2, COLLISIONRANGE);
	*/
}

