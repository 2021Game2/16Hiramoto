#include "CMap.h"
#include"CCollider.h"
#include"CColliderTriangle.h"
#include"CCollisionManager.h"
#define OBJ "Resource\\3DModel\\Map\\sky.obj"
#define MTL "Resource\\3DModel\\Map\\sky.mtl"
CColliderMesh mColliderMesh;
CMap::CMap()
{
	mTag = EMAP;
	mPosition = CVector(0.0f, -3.325f, 0.0f);
	mScale = CVector(1.0f, 1.0f, 1.0f);
	CTransform::Update();

	mModel.Load(OBJ, MTL);//���f����ǂݍ���
	mpModel = &mModel;//���f���̃|�C���^��(�`�悷��Ƃ��ɕK�v)
		
	
}
void CMap::TaskCollision() {
	
}

