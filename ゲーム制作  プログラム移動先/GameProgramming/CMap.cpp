#include "CMap.h"
#include"CCollider.h"
#include"CColliderTriangle.h"
#include"CCollisionManager.h"
#define OBJ "Resource\\3DModel\\Map\\sky.obj"
#define MTL "Resource\\3DModel\\Map\\sky.mtl"
//CColliderMesh mColliderMesh;
CModel CMap::mModel;//���f���f�[�^�쐬


CMap::CMap(const CVector& position, const CVector& rotation, const CVector& scale)
:CMap()
{
	mTag = EMAP;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
	CTransform::Update();//�s��̍X�V

  
	//mColliderMesh.Set(this, &mMatrix, &mModel);//���f�����R���C�_�ɂ���
	
	
	
}
 CMap::CMap() {
	 //���f���̃|�C���^�ݒ�
	 mpModel = &mModel;
	 //���f���������Ƃ��͓ǂݍ���
	 if (mModel.mTriangles.size() == 0) {
		 mModel.Load(OBJ, MTL);
	 }
}
 void CMap::Update() {
	 //CCharacter�̍X�V
	 CTransform::Update();
 }

void CMap::TaskCollision() {
	
}

