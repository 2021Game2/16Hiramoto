
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CRock.h"
#include"CCollider.h"
#define OBJ "3DModel\\Ground\\Ground.obj"
#define MTL "3DModel\\Ground\\Ground.mtl"

CModel CRock::mModel;//���f���f�[�^�쐬

CRock::CRock(const CVector& position, const CVector& rotation, const CVector& scale)
	
{
	
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	
	//�D��x��2�ɕύX����
	mPriority = 2;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
	 CTransform::Update();//�s��̍X�V
	 
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
		//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0) {
			mModel.Load(OBJ, MTL);
	}
	mColliderMesh.Set(this, &mMatrix, &mModel);//���f�����R���C�_�ɂ���
    mTag = EROCK;
}
void CRock::Collision(CCollider* m, CCollider* o) {
	m->mpParent->mTag = EROCK;

}
void CRock::Update() {
	//CCharacter�̍X�V
	CTransform::Update();
}

void CRock::Render() {
	//�e�̕`�揈��
	CCharacter::Render();
}
void CRock::TaskCollision() {
	
}
