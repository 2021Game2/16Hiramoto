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
#include"CSceneGame.h"
#define OBJ "Resource\\3DModel\\EnemySummon\\egg.obj"
#define MTL "Resource\\3DModel\\EnemySummon\\egg.mtl"
#define HP 3
CModel CEnemySummon::mModel;
CEnemySummon::CEnemySummon(const CVector& position, const CVector& rotation, const CVector& scale)
	:CEnemySummon()
	
{
	
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

CEnemySummon::CEnemySummon()
	: mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
	, mHp(HP)
	, mEffectCount(0)
{
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}

}
void CEnemySummon::Update() {
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	if (mDamageCount > 0) {
		mDamageCount--;
	}

	mEffectCount--;
		if(mHp<=0|| tSceneGame->mBossGaugeSwitch == true){
			
		 mEnabled=false;
		}
		//CCharacter�̍X�V
		CTransform::Update();
}
void CEnemySummon::Damage() {
	if (mEffectCount % 15 == 0) {
		//�G�t�F�N�g����
		new CEffect2(mPosition, 5.0f, 5.0f, CEffect2::EFF_EXP, 4, 4, 2);
	}
}
//Collision(�R���C�_�P�A�R���C�_�Q�j
void CEnemySummon::Collision(CCollider* m, CCollider* o) {
	if (m->mType == CCollider::ESPHERE) {
		if (m->mTag == CCollider::EENEMYSUMMON) {
			if (o->mType == CCollider::ESPHERE) {
				if (o->mpParent->mTag == EPLAYER) {
					if (o->mTag == CCollider::EPLAYERSWORD) {
						//�Փ˂��Ă���Ƃ�
						if (CCollider::Collision(m, o)) {
							//�v���C���[�̓����蔻�肪�L���ȂƂ�
							if (((CXPlayer*)(o->mpParent))->mAttackHit == true) {
								if (mDamageCount <= 0) {
									//�����G�t�F�N�g�t�^
									 CEnemySummon::Damage();
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
}
void CEnemySummon::TaskCollision() {
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
