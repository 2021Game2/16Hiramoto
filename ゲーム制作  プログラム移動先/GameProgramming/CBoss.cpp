#include"CBoss.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CXCharacter.h"
#include"CUtil.h"
#include"CText.h"
#define HP 100

#define VELOCITY 0.5f //�}�N��
#define HPCOUNT1 90 //�_���[�W���󂯂��Ƃ��ɂ̂�������s���̗͂̐��l
#define HPCOUNT2 60 //�_���[�W���󂯂��Ƃ��ɂ̂�������s���̗͂̐��l
#define HPCOUNT3 30 //�_���[�W���󂯂��Ƃ��ɂ̂�������s���̗͂̐��l
#define JUMP 5.0f
#define G 0.1f
#define PLAYERSPPOINT_MAX 30
#define GAUGE_WID_MAXHP 700.0f	//HP�Q�[�W�̕��̍ő�l
#define GAUGE_LEFT 20			//�Q�[�W�`�掞�̍��[

#define IMAGE_GAUGE "Resource\\png,tga\\Gauge.png"		//�Q�[�W�摜
int CBoss::mBossAttackCount = 0;
int CBoss::mHp = HP;

 extern CSound BossVoice;
 extern CSound BossMove;
 CBoss* CBoss::mpBossInstance;
 //�{�X�̃|�C���^��Ԃ����ƂŁA���W�Ȃǂ��Q�Ƃł���悤�ɂȂ�
 CBoss* CBoss::GetInstance()
 {
	 return mpBossInstance;
 }
CModel CBoss::mModel;//���f���f�[�^�쐬
//�f�t�H���g�R���X�g���N�^
CBoss::CBoss()
//�R���C�_�̐ݒ�
	: mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 20.0f)
	, mColSphereHead(this, &mMatrix, CVector(0.0f, 1.0f, 5.0f), 7.0f)
	, mColSphereRightFront(this, &mMatrix, CVector(0.0f, -2.0f, 0.0f), 2.0f)
	, mColSphereLeftFront(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
	, mpPlayer(0)
	, mJump2(0)
	, mEnemyDamage(60)
	, mMove(0)
	, mMove2(0)
	, mColliderCount(0.0f)
	, mGravity(0.0f)
	, mTime(0.0f)
	, mBossDamageCount(0)
	
	, mBossAttackHit(false)
	,mColSearchCount(false)
	, mBossBgm(true)
	, mBossBgmDeath(true)
{

	mImageGauge.Load(IMAGE_GAUGE);
	mGravity = 0.20f;
	mTag = EBOSS;
	mColSearch.mTag = CCollider::ESEARCH;//�^�O�ݒ�
	mColSphereHead.mTag = CCollider::EBOSSCOLLIDERHEAD;
	mColSphereRightFront.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mColSphereLeftFront.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mpBossInstance = this;
	mRotation.mY += 180.0f;
	mGravity = 0.20f;
	mState = EIDLE;
}

//CEnemy(�ʒu�A��]�A�g�k�j
CBoss::CBoss(const CVector& position, const CVector& rotation, const CVector& scale)
	:CBoss()
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position;//�ʒu�̐ݒ�
	mRotation = rotation;//��]�̐ݒ�
	mScale = scale;//�g�k�̐ݒ�
	CTransform::Update();//�s��̍X�V
	//�ڕW�n�_�̐ݒ�
	mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}

void CBoss::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSphereHead.mpMatrix = &mpCombinedMatrix[6];
    mColSphereRightFront.mpMatrix = &mpCombinedMatrix[12];//�E�O��
	mColSphereLeftFront.mpMatrix = &mpCombinedMatrix[19];//���O��
	mState = EATTACK2;


}
//�ҋ@����
void CBoss::Idle() {
	//30���܂�܂őҋ@�̃A�j���[�V����
	ChangeAnimation(8, false, 60);
	mMove++;
	if (mMove >= 120) {
		//30���܂�����ԂŃA�j���[�V�������I���ƍU�������Ɉڍs
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			if (mAttackPercent <= 5) {
			mState = EATTACK;
		    }
			else if(mAttackPercent > 5) {
				mState = EATTACK2;
			}
		}
	}
	//30���܂�O�ɃA�j���[�V�������I�������ړ������Ɉڍs
	else if (mAnimationFrame >= mAnimationFrameSize) {
		if (mColSearch.mRenderEnabled == false) {
	    	mState = EAUTOMOVE;
		}
	}


}
//�ړ�����
void CBoss::AutoMove() {
	//����

	if (CSceneGame::mVoiceSwitch == true) {
		BossMove.Play();
	}
	//CXPlayer���g�����|�C���^�Ƀv���C���[�̏���Ԃ�������������(CXPlayer�̒��̏����Ȃ̂Ń|�C���^�����K�v����j
	CXPlayer* tPlayer = CXPlayer::GetInstance();
	mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
	ChangeAnimation(4, true,180);
	//�v���C���[�Ɍ������ĉ�]���鏈��
	//�������iX���j�̃x�N�g�������߂�
	CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//������iY���j�̃x�N�g�������߂�
	CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//�O�����iZ���j�̃x�N�g�������߂�
	CVector vz = CVector(0.0f, 0.0f, 5.0f) * mMatrixRotate;
	//�ڕW�n�_�܂ł̃x�N�g�������߂�
	CVector vp = mPoint - mPosition;
	//���x�N�g���Ƃ̓��ς����߂�
	float dx = vp.Dot(vx);
	//��x�N�g���Ƃ̓��ς����߂�
	float dy = vp.Dot(vy);
	//�O�x�N�g���Ƃ̓��ς����߂�
	float dz = vp.Dot(vz);
	float margin = 0.1f;
	//���E�����։�]
	if (dx > margin) {
		mRotation.mY += 3.0f;//���։�]
	}
	else if (dx < -margin) {
		mRotation.mY -= 3.0f;//�E�։�]
	}
	CTransform::Update();//�s��X�V
	//����I�Ƀv���C���[�̍��W���L�^
	int r = rand() % 60; //rand()�͐����̗�����Ԃ�
	//%180�͂P�W�O�Ŋ������]������߂�
	if (mColSearch.mRenderEnabled == false) {
		if (r == 0) {
				//ESEARCH�ɏՓ˂��ă|�C���^�ɐݒ肵��
				//�v���C���[�̍��W���L�^
				mPoint = tPlayer->mPosition;
		
		}
	}
	
}
//�U������
void CBoss::Attack() {
	//�U���A�j���[�V����
	ChangeAnimation(5, false, 80);
	//�U���̂��Ƃ͈ړ������Ɉڍs
	if (mAnimationFrame >= mAnimationFrameSize) {
			mMove = 0;//�U���̃A�j���[�V�����̂��Ƃ͈ړ��̃A�j���[�V�����ɐ؂�ւ��
	}

}
//�U������
void CBoss::Attack2() {
	//�U���A�j���[�V����
	ChangeAnimation(6, false, 80);
	//�U���̂��Ƃ͈ړ������Ɉڍs
	if (mAnimationFrame>=mAnimationFrameSize) {
		mMove = 0;//�U���̃A�j���[�V�����̂��Ƃ͈ړ��̃A�j���[�V�����ɐ؂�ւ��
	}
}
//�_���[�W����
void CBoss::Damaged() {
	//�̗͌���
	if (mHp <= 0) {
		mState = EDEATH;
	}
	else {
    //�_���[�W�̂��Ƃ͈ړ�����
	mState = EIDLE;
	}
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition = mPosition + mCollisionEnemy * mColliderCount;
	}
}
//���S����
void CBoss::Death() {
	if (mBossBgmDeath == true) {
		CSceneGame::mBgmCountCheck = false;
		CSceneGame::mBgmCount = 4;
		mBossBgmDeath = false;
	}
	ChangeAnimation(9, false, 250);
	//�̗͂��Ȃ��Ȃ�����
	if (mHp <= 0) {
		mHp--;
		//15�t���[�����ƂɃG�t�F�N�g
		if (mHp % 15 == 0) {
			//�G�t�F�N�g����
			new CEffect2(mPosition, 1.0f, 1.0f, CEffect2::EFF_EXP, 4, 4, 2);
		}
		CTransform::Update();
	}
	//���΂炭�o���������
	if (mHp <= -250) {
		mEnabled = false;
	}
	CXCharacter::Update();
}

//�X�V����
void CBoss::Update() {
	//�������s�����Ƃɕ���
	switch (mState) {
	case EIDLE:	//�ҋ@
		Idle();
		break;
	case EAUTOMOVE://�ړ�
		AutoMove();
		break;
	case EATTACK://�U��1
		Attack();
		break;
	case EATTACK2://�U���Q
		Attack2();
		break;
	case EDAMAGED://�_���[�W
		Damaged();
		break;
	case EDEATH://���S
		Death();
		break;
	}
	//�A�j���[�V�����̎��
	switch (mAnimationIndex) {
	case(5):
		if (mAnimationFrame == 30) {
			mBossAttackHit = true;
			if (CSceneGame::mVoiceSwitch == true) {
				BossVoice.Play();
			}
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mBossAttackHit = false;
			mState = EIDLE;
		}
		break;
	case(6):
		if (mAnimationFrame == 30) {
			mBossAttackHit = true;
			if (CSceneGame::mVoiceSwitch == true) {
			  BossVoice.Play();
			}
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mBossAttackHit = false;
			mState = EIDLE;
		}
		break;
	}
		
	if (mAttackPercent < 10) {
		mAttackPercent++;
	}
	if (mAttackPercent >= 10) {
		mAttackPercent = 0;
	}
	if (mBossDamageCount > 0) {
		mBossDamageCount--;
	}
	if (mBossDamageCount > 0) {
		if (mEffectCount % 15 == 0) {
			//�G�t�F�N�g����
			//���ɍU�����ꂽ�Ƃ�
			if (mBossColliderCheck == 1) {
				CXPlayer* tPlayer = CXPlayer::GetInstance();

				mBossEffect = new CEffect2(tPlayer->mPosition, 3.0f, 3.0f, CEffect2::EFF_EXP, 4, 4, 2);
			}
			//���ɍU�����ꂽ�Ƃ�
			else if (mBossColliderCheck == 2) {
				CXPlayer* tPlayer = CXPlayer::GetInstance();
			    mBossEffect=new CEffect2(CVector(tPlayer->mPosition.mX, tPlayer->mPosition.mY+1.0f, tPlayer->mPosition.mZ), 3.0f, 3.0f, CEffect2::EFF_EXP, 4, 4, 2);
			}
		}
	}
	if (mHp <= 0 && mState != EDEATH) {
		mState = EDEATH;
	}

	mEffectCount--;
	mGravity  -= G;
	mPosition.mY += mGravity;
	CXCharacter::Update();
}

//Collision(�R���C�_�P�C�R���C�_�Q�C�j
void CBoss::Collision(CCollider* m, CCollider* o) {
	//�R���C�_�̂Ƃ�
	m->mType = CCollider::ESPHERE;
		//�������T�[�`�p�̂Ƃ�
		if (m->mTag == CCollider::ESEARCH) {
			//���肪�e�R���C�_�̂Ƃ�
			if (o->mType == CCollider::ESPHERE) {
				//���肪�v���C���[�̂Ƃ�
				if (o->mpParent->mTag == EPLAYER) {
					if (o->mTag == CCollider::EPLAYERBODY) {
						//�Փ˂��Ă���Ƃ�
						if (CCollider::Collision(m, o)) {
							//�|�C���^���v���C���[�ɐݒ�
							mColSearchCount = true;
							if (mColSearch.mRenderEnabled == true) {
								if (mBossBgm == true) {
									CSceneGame::mBgmCountCheck = false;
									CSceneGame::mBgmCount = 3;
									mBossBgm = false;
								}
								mColSearch.mRenderEnabled = false;
							}
						}
					}
				}
			}
			return;
		}
		//�{�X�̃R���C�_�[
		if (m->mTag== CCollider::EBOSSCOLLIDERATTACK|| CCollider::EBOSSCOLLIDERHEAD) {
			if (o->mType == CCollider::ESPHERE) {
				//�v���C���[
				if (o->mpParent->mTag == EPLAYER) {
					//���肪����̂Ƃ�
					//���ƃn���}�[�i�A�C�e���j
					if (o->mTag == CCollider::EPLAYERSWORD || o->mpParent->mTag == EITEM) {
						//�Փ˂��Ă���Ƃ�
							if (CCollider::Collision(m, o)) {
								
								//�e��CXPlayer�����Ƀ|�C���^�����A�ϐ����Q��
								if (((CXPlayer*)(o->mpParent))->mAttackHit == true)
								{
									if (m->mTag == CCollider::EBOSSCOLLIDERATTACK) mBossColliderCheck = 1;
									else if (m->mTag == CCollider::EBOSSCOLLIDERHEAD) mBossColliderCheck = 2;
									//�����G�t�F�N�g�b���t�^
									mEffectCount = 60;
									if (mHp > 0) {
										if (((CXPlayer*)(o->mpParent))->mSpAttack < PLAYERSPPOINT_MAX) {

											((CXPlayer*)(o->mpParent))->mSpAttack++;
										}
										//30�����邲�Ƃɂ̂�����
										if (mHp ==HPCOUNT1|| mHp == HPCOUNT2|| mHp == HPCOUNT3) {
											mColliderCount = 10;
											mCollisionEnemy = mPosition - o->mpParent->mPosition;
											mCollisionEnemy.mY = 0;
											mCollisionEnemy = mCollisionEnemy.Normalize();
											mState = EDAMAGED;
										}
										if (mBossDamageCount <= 0) {
											mHp--;
											mBossDamageCount = 10;
										}
								    }
										
								}
							}
						
					}
					//���肪ESTOPPER�̎�
					if (o->mTag == CCollider::EPLAYERBODY) {

						if (CCollider::Collision(m, o)) {
							
							if (mState != EATTACK) {
								if (mState != EATTACK2) {
									if (mState != EIDLE) {
										if (mHp > 0) {
											mState = EIDLE;
										}
									}
								}

							}
							mColliderCount = 1.5f;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy = mCollisionEnemy.Normalize();
						}
					}
				}
			}
		}
		if (m->mTag == CCollider::EBOSSCOLLIDERHEAD) {
			if (o->mType == CCollider::ETRIANGLE) {
				CVector adjust;//�����l
				//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
				//adjust�A�A�A�����l	
				if (CCollider::CollisionTriangleSphere(o, m, &adjust))
				{
					mPosition = mPosition + adjust;
					mGravity = 0;
				}
			}
		}
		return;
}
void CBoss::TaskCollision() {
	//�R���C�_�̗D��x�ύX
	mColSearch.ChangePriority();
	mColSphereHead.ChangePriority();
	mColSphereRightFront.ChangePriority();
	mColSphereLeftFront.ChangePriority();
	//�Փˏ��������s

	CCollisionManager::Get()->Collision(&mColSphereRightFront, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereLeftFront, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereHead, COLLISIONRANGEFIELD);
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
}

void CBoss::Render2D()
{
	//2D�`��J�n
	CUtil::Start2D(0, 800, 0, 600);
	//�̗͂̊���
	float hpRate = (float)mHp / (float)HP;
	//�̗̓Q�[�W�̕�
	float hpGaugeWid = GAUGE_WID_MAXHP * hpRate;
	if (CSceneGame::mBossGaugeSwitch == true) {
		mImageGauge.Draw(50, GAUGE_WID_MAXHP, 510, 550, 201, 300, 63, 0);//�Q�[�W�w�i
		mImageGauge.Draw(50, hpGaugeWid, 510, 550, 487, 572, 63, 0);//�̗̓Q�[�W
	}

	CUtil::End2D();
}
