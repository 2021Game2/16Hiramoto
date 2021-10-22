#include"CEnemySummon.h"
#include"CCharacter.h"
#include"CCollider.h"
#include"CEnemy2.h"
#include"CModel.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#define OBJ "egg.obj"
#define MTL "egg.mtl"
CModel CEnemySummon::mModel;
CEnemySummon::CEnemySummon()
    :mHp(0)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)

{
   

	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
}
CEnemySummon::CEnemySummon(const CVector& position, const CVector& rotation, const CVector& scale)
	:CEnemySummon()
	
{
	mHp = 10;
 mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mCollider.mTag = CCollider:: EENEMYSUMMON;
	mTag = EENEMYSUMMON;
	CTransform::Update();//�s��̍X�V
	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}
void CEnemySummon::Update() {
		if(mHp<=0){
		mEnabled=false;
		}
}
//Collision(�R���C�_�P�A�R���C�_�Q�j
void CEnemySummon::Collision(CCollider* m, CCollider* o) {
	if (m->mType == CCollider::ESPHERE) {
		if (o->mType == CCollider::ESPHERE) {
			if (o->mpParent->mTag ==EPLAYER) {
				if(o->mTag==CCollider::ESWORD)
				//�Փ˂��Ă���Ƃ�
				if (CCollider::Collision(m, o)) {
				//	mHp--;
				}
			}
		}
	}
}
void CEnemySummon::Render() {
	//�e�̕`�揈��
	CCharacter::Render();
}
void CEnemySummon::TaskCollision() {
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}