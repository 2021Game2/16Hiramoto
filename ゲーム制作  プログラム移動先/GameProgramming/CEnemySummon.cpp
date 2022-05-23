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
#define OBJ "3DModel\\EnemySummon\\egg.obj"
#define MTL "3DModel\\EnemySummon\\egg.mtl"
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

void CEnemySummon::Render2D()
{
	//2D�`��J�n
	CUtil::Start2D(0, 800, 0, 600);

	CVector ret;
	CVector tpos = mPosition;
		//mpCombinedMatrix[6].GetPos();
	Camera.WorldToScreen(&ret, tpos);

	float HpRate = (float)mHp / (float)HP_MAX; //�̗͍ő�l�ɑ΂���A���݂̗̑͂̊���
	float HpGaugeWid = GAUGE_WID_MAX * HpRate; //�̗̓Q�[�W�̕�

	//��ʊO�̎��ɕ\�����Ȃ�
	if (ret.mX > 0 && ret.mX < 800) {
		//�Q�[�W�w�i
		mImageGauge.Draw(ret.mX - GAUGE_WID_MAX, ret.mX + GAUGE_WID_MAX, ret.mY + 30.0f, ret.mY + 45.0f, 210, 290, 63, 0);
		//�̗̓Q�[�W
		mImageGauge.Draw(ret.mX - GAUGE_WID_MAX, (ret.mX - GAUGE_WID_MAX) + HpGaugeWid * 2.0f, ret.mY + 30.0f, ret.mY + 45.0f, 0, 0, 0, 0);
		//�v���C���[�̍U���ΏۂɂȂ��Ă���Ƃ�
		//if (mIsTarget) {
			//�^�[�Q�b�g�摜�\��
			//mImageTarget.Draw(ret.mX - 30.0f, ret.mX + 30.0f, ret.mY - 30.0f, ret.mY + 30.0f, 0, 255, 255, 0);
		//}
	}
	CUtil::End2D();
}

