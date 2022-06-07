
#include"CXPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CItem.h"
#define OBJ "Resource\\3DModel\\Weapon\\Weapon.obj"
#define MTL "Resource\\3DModel\\Weapon\\Weapon.mtl"
int CItem::mItemCount = 0;
bool CItem::mItemAttackHit = false;
CModel CItem::mModel;//���f���f�[�^�쐬
CItem::CItem(const CVector& position, const CVector& rotation, const CVector& scale)
	: CItem()
{
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mCollider.mTag = CCollider::EITEMCOLLIDER;
	mCollider2.mTag = CCollider::EITEMCOLLIDER2;
	mTag = EITEM;
	CTransform::Update();//�s��̍X�V
	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}
CItem::CItem() 
	: mCollider(this, &mMatrix, CVector(0.0f, 3.0f, 0.0f), 6.0f)
	, mCollider2(this, &mMatrix, CVector(0.0f, 3.0f, 0.0f), 6.0f)
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
	//�擾����ƕ���ɂȂ��ăv���C���[�̎�������
	if (mItemCount > 0) {
	     mItemCount--;
		 mItemAttackHit = true;
		if (mRotation.mX < 90.0f) {
			mRotation = CVector(90.0f, 0.0f, 0.0f);
		}
	  mRotation.mY += 10.0f;
	  CCollider mCollider2;
	  mPosition.mX = tPlayer->mPosition.mX;
	  mPosition.mY = tPlayer->mPosition.mY+1.0f;
	  mPosition.mZ = tPlayer->mPosition.mZ;
	}
	//�A�C�e�����擾���Ă��Ȃ��Ƃ�
	if (mItemAttackHit == true&&mItemCount <= 0) {
		mItemAttackHit = false;
		mEnabled = false;
	}
	//�A�C�e�����擾���Ă��Ȃ��Ƃ�
	if (mItemAttackHit == false) {
       mRotation.mY++;
	}
    //CCharacter�̍X�V
    CTransform::Update();
    //�����̍s��ɐe�̍s����|���āA�ŏI�I�ȍ����s��ɂ���
}
void CItem::Collision(CCollider* m, CCollider* o) {
	switch (m->mType) {
	case CCollider::ESPHERE:
		if (m->mpParent->mTag == EITEM) {
			if (m->mTag == CCollider::EITEMCOLLIDER2) {
				if (o->mType == CCollider::ESPHERE) {
					if (o->mpParent->mTag == EPLAYER) {
						//�Փ˂��Ă���Ƃ�
						if (CCollider::Collision(m, o)) {

							if (mCollider2.mRenderEnabled == true) {
								mCollider2.mRenderEnabled = false;
								mItemCount = 1800;
							}
						}
					}
				}
			}
		}
	}
}
void CItem::TaskCollision() {
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	mCollider2.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider2, COLLISIONRANGE);
}
