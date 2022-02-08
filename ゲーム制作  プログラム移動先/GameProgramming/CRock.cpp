
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CRock.h"
#define OBJ "3DModel\\Rock\\Rock1.obj"
#define MTL "3DModel\\Rock\\Rock1.mtl"
CModel CRock::mModel;//���f���f�[�^�쐬
CRock::CRock(const CVector& position, const CVector& rotation, const CVector& scale)
	: CRock()
{

	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	//mCollider.mTag = CCollider::EROCKCOLLIDER;
	//mColliderMesh.Set(NULL, NULL, &mModel);//���f�����R���C�_�ɂ���
	mTag = EROCK;
	CTransform::Update();//�s��̍X�V
	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}
CRock::CRock()
	//: mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 100.0f)

{
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
	mRotation.mX += 30.0f;
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
	CCharacter::Render();
}
void CRock::TaskCollision() {
	//�R���C�_�̗D��x�ύX
	//mCollider.ChangePriority();
	//�Փˏ��������s
	//CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
