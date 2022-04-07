
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CRock.h"
#include"CCollider.h"
#define OBJ "3DModel\\Rock\\Rock.obj"
#define MTL "3DModel\\Rock\\Rock.mtl"

CModel CRock::mModel;//���f���f�[�^�쐬

CRock::CRock(const CVector& position, const CVector& rotation, const CVector& scale)
	: CRock()
{
	
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mTag = EROCK;
	 CTransform::Update();//�s��̍X�V
	//�D��x��2�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}
CRock::CRock()

{
	//mPosition = CVector(-100.0f, 0.0f, 50.0f);
	//mPosition = CVector(0.0f, 0.0f, 0.0f);
	//mScale = CVector(1.0f, 1.0f, 1.0f);
   
	

	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;

		//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}

	mColliderMesh.Set(this, NULL, &mModel);//���f�����R���C�_�ɂ���
	
}
void CRock::Update() {
	
	//CCharacter�̍X�V
	CTransform::Update();
}
void CRock::Collision(CCollider* m, CCollider* o) {
	switch (m->mType) {
	case CCollider::ESPHERE:
		if (o->mType == CCollider::ESPHERE) {
			if (o->mpParent->mTag == EPLAYER) {

				//�Փ˂��Ă���Ƃ�
				if (CCollider::Collision(m, o)) {
					
					
				}

			}
		}
	}

}
void CRock::Render() {
	//�e�̕`�揈��
	//CCharacter::Render();
}
void CRock::TaskCollision() {
	//�R���C�_�̗D��x�ύX
	//mCollider.ChangePriority();
	//�Փˏ��������s
	//CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
