#include"CEnemySummon.h"
#include"CCharacter.h"
#include"CXCharacter.h"
#include"CCollider.h"
#include"CEnemy2.h"
#include"CModel.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CUtil.h"
#include"CCamera.h"
#define OBJ "Resource\\3DModel\\EnemySummon\\egg.obj"
#define MTL "Resource\\3DModel\\EnemySummon\\egg.mtl"
#define HP 10
#define HP_MAX 100				//�̗͍ő�l
#define GAUGE_WID_MAX 400.0f	//�Q�[�W�̕��̍ő�l
#define GAUGE_LEFT 20			//�Q�[�W�`�掞�̍��[
#define IMAGE_GAUGE "Resource\\Gauge.png"		//�Q�[�W�摜
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

	mImageGauge.Load(IMAGE_GAUGE);
}
CEnemySummon::CEnemySummon(const CVector& position, const CVector& rotation, const CVector& scale)
	:CEnemySummon()
	
{
	mHp = HP;
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
	if (mDamageCount > 0) {
		mDamageCount--;
	}
		if(mHp<=0){
		mEnabled=false;
		}
}
//Collision(�R���C�_�P�A�R���C�_�Q�j
void CEnemySummon::Collision(CCollider* m, CCollider* o) {
	if (m->mType == CCollider::ESPHERE) {
		if (o->mType == CCollider::ESPHERE) {
			if (o->mpParent->mTag ==EPLAYER) {
				if(o->mTag==CCollider::EPLAYERSWORD){
				//�Փ˂��Ă���Ƃ�
					if (CCollider::Collision(m, o)) {

						//�v���C���[�̓����蔻�肪�L���ȂƂ�
						if (((CXPlayer*)(o->mpParent))->mAttackHit == true) {
							if (mDamageCount <= 0) {

								 mHp--;
								 mDamageCount = 20;
							}
						}
					}
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
