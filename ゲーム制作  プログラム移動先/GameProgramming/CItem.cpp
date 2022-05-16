
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CItem.h"
#define OBJ "3DModel\\Weapon\\Weapon.obj"
#define MTL "3DModel\\Weapon\\Weapon.mtl"


int CItem::mItemCount = 0;
CModel CItem::mModel;//���f���f�[�^�쐬
CItem::CItem(const CVector& position, const CVector& rotation, const CVector& scale)
	: CItem()
{

	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mCollider.mTag = CCollider::EITEMCOLLIDER;
	mTag = EITEM;
	CTransform::Update();//�s��̍X�V

	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}
CItem::CItem() 
	: mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 4.0f)
	
{
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}

}
void CItem::Update() {
	CXPlayer* tPlayer = CXPlayer::GetInstance();
	if (mItemCount > 0) {
	     mItemCount--;
		if (mRotation.mX < 90.0f) {
			mRotation = CVector(90.0f, 0.0f, 0.0f);
		}
	  mRotation.mY += 10.0f;
	  CCollider mCollider2;

	  mPosition.mX = tPlayer->mPosition.mX;
	  mPosition.mY = tPlayer->mPosition.mY;
	  mPosition.mZ = tPlayer->mPosition.mZ;

	}
    mRotation.mY++;
//CCharacter�̍X�V
CTransform::Update();

//�����̍s��ɐe�̍s����|���āA�ŏI�I�ȍ����s��ɂ���

}
void CItem::Collision(CCollider* m, CCollider* o) {
	switch (m->mType) {
	case CCollider::ESPHERE:
		
		if (o->mType == CCollider::ESPHERE) {
			if (o->mpParent->mTag == EPLAYER) {

				//�Փ˂��Ă���Ƃ�
				if (CCollider::Collision(m, o)) {
					mItemCount = 1800;
					mCollider.mTag = CCollider::EPLAYERSWORD;
					//mEnabled = false;
				}

			}
		}
	}
	
}
void CItem::TaskCollision() {
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
