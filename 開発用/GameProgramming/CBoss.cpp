#include"CBoss.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CXCharacter.h"
#include"CUtil.h"
#include"CText.h"
#define HP 20

#define VELOCITY 0.5f //�}�N��
#define HPCOUNT1 15 //�_���[�W���󂯂��Ƃ��ɂ̂�������s���̗͂̐��l
#define HPCOUNT2 10 //�_���[�W���󂯂��Ƃ��ɂ̂�������s���̗͂̐��l
#define HPCOUNT3 5 //�_���[�W���󂯂��Ƃ��ɂ̂�������s���̗͂̐��l
#define JUMP 5.0f //�W�����v�U�����̃W�����v��
#define JUMP2 2.5f //�Ђ������U�����̃W�����v��
#define G 0.1f //�d��
#define G2  0.2f //�Ђ������U�����̏d��
#define GAUGE_WID_MAXHP 700.0f	//HP�Q�[�W�̕��̍ő�l
#define GAUGE_LEFT 20			//�Q�[�W�`�掞�̍��[
#define ATTACKSELECT 4  //�U���̎��
#define ROTATIONBASE 14400.0f//��]�U���̑��t���[��
#define ROTATIONCOUNT 36.0f//��]�U���̍ہA�A�j���[�V�����̏I���Ɖ�]�̏I�������킹��
#define ROTATIONCOUNTM -36.0f
#define ROTATION 3.6f//�U�����ɉ�]�����
#define ROTATIONMIN -0.36f
#define ROTATIONMIN2 -0.1f
#define ROTATIONBEFOREATTACK -3.6f
#define IMAGE_GAUGE "Resource\\png,tga\\Gauge.png"		//�Q�[�W�摜


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
	, mColSphereHead(this, &mMatrix, CVector(0.0f, 3.0f, 5.0f), 5.0f)
	, mColSphereRightFront(this, &mMatrix, CVector(0.0f, -2.0f, 0.0f), 2.0f)
	, mColSphereLeftFront(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
	, mColSphereAttack(this, &mMatrix, CVector(0.0f, 0.0f, 10.0f), 10.0f)
	, mpPlayer(0)
	, mMove(0)
	, mMove2(0)
	, mBossDamageCount(0)
	, mBossJumpCount(0)
	, mBossAttackMove(0)
	, mEnemyDamage(60)
	, mJump(0.0f)
	, mColliderCount(0.0f)
	, mTime(0.0f)
	, mHp(HP)
	, mJumpStopper(true)
	, mBossBgm(true)
	, mBossBgmDeath(true)
	, mBossAttackHit(false)
	, mColSearchCount(false)
	
{

	mImageGauge.Load(IMAGE_GAUGE);//�̗̓Q�[�W�̃e�N�X�`��
	
	mTag = EBOSS;
	mColSearch.mTag = CCollider::ESEARCH;//�^�O�ݒ�
	mColSphereHead.mTag = CCollider::EBOSSCOLLIDERHEAD;
	//mColSphereRightFront.mTag = CCollider::EBOSSCOLLIDERATTACK;
	//mColSphereLeftFront.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mColSphereAttack.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mpBossInstance = this;
	mRotation.mY += 180.0f;//���������Ă���
	mState = EIDLE;//�ŏ��͑ҋ@���
	mColSphereAttack.mRenderEnabled = false;//�ŏ��͕\�������Ȃ�
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
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
	//���̃R���C�_
	mColSphereHead.mpMatrix = &mpCombinedMatrix[6];
	//�W�����v�U��������Ƃ��̃R���C�_
	mColSphereAttack.mpMatrix = &mpCombinedMatrix[6];
    mColSphereRightFront.mpMatrix = &mpCombinedMatrix[12];//�E�O��
	mColSphereLeftFront.mpMatrix = &mpCombinedMatrix[19];//���O��
	

}
//�ҋ@���
void CBoss::Idle() {
	mPosition.mY -= G;//�d�͂�������
	//mMove��120���܂�܂őҋ@�̃A�j���[�V����
	ChangeAnimation(8, true, 60);
	mMove++;

	if (mMove >= 120) {
		//120���܂�����ԂŃA�j���[�V�������I���ƍU�������Ɉڍs
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			int num = rand() % ATTACKSELECT + 1;
			 
				mAttackPercent = num;
				switch (mAttackPercent) {
				case(0):
					if (mColSearch.mRenderEnabled == false) {
						mState = EIDLE;
					}
					else {
						mState = EAUTOMOVE;
					}
					break;
				case(1):
					if (mColSearch.mRenderEnabled == false) {
						mState = EATTACK;
						mBossAttackMove = 0;
					}
					else {
						mState = EIDLE;
					}
					break;
				case(2):
					if (mColSearch.mRenderEnabled == false) {
						mState = EATTACK2;
						mBossAttackMove = 0;
					}
					else {
						mState = EIDLE;
					}
					break;
				case(3):
					if (mColSearch.mRenderEnabled == false) {
					mJumpStopper = false;
					mJump = JUMP;
					mState = EATTACK3;
					}
					else {
					 mState = EIDLE;

					}
					break;
				case(4):
					if (mColSearch.mRenderEnabled == false) {
						mRotationCount = ROTATIONBASE;
						mAttack4Count = 1;
						mAttack4RotationCount = 0.0f;
						mState = EATTACK4;
					}
					else {
						mState = EIDLE;
					}
					break;
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
	mPosition.mY -= G;
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	//����
	if (tSceneGame->mVoiceSwitch == true) {
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
//�U������(�Ђ������U���j
void CBoss::Attack() {
	//�U���A�j���[�V����
	ChangeAnimation(5, false, 80);
	switch (mBossAttackMove) {
	case(0):
		if (mAnimationFrame < 30) {

			mPosition.mZ -= 0.1f;
		}
		else {
			mJump = JUMP2;
			mJumpZ = JUMP2;
			mBossAttackMove = 1;	
		}
		break;
	case(1):
		if (mAnimationFrame < 60) {
			mJumpCount = CVector(0.0f, mJump, mJumpZ) * mMatrixRotate;
			mBossAttackHit = true;
			mPosition += mJumpCount;
			mJump -= G2;
			if (mJumpZ > 0) {
				mJumpZ -= G2;
			}
		}
		else {
			mBossAttackHit = false;
			mJump = 0.0f;
		    mBossAttackMove = 2;

		}
		break;
	case(2):
			mPosition.mZ -= 0.1f;
		
		break;
	}
	//�U���̂��Ƃ͑ҋ@��ԂɈڍs
	if (mAnimationFrame >= mAnimationFrameSize) {
			mMove = 0;//�U���̃A�j���[�V�����̂��Ƃ͑ҋ@�̃A�j���[�V�����ɐ؂�ւ��
			mState = EAUTOMOVE;
	}

}
//�U������2�i�Ȃ��΂炢�j
void CBoss::Attack2() {
	//�U���A�j���[�V����
	ChangeAnimation(6, false, 80);

	switch (mBossAttackMove) {
	case(0):
		if (mAnimationFrame < 30) {
			mRotation.mY += 4.5f;
		}
		else {
			mBossAttackMove = 1;
		}
		break;
	case(1):
		mBossAttackHit = true;
		if (mAnimationFrame < 50) {
			mRotation.mY -= 9.0f;
	     }
		else {
			mBossAttackMove = 2;
		}
		break;
	case(2):
		if (mAnimationFrame < 80) {
			mRotation.mY += 2.0f;
		}
		
			
		break;
	}
	if (mAnimationFrame>=mAnimationFrameSize) {
             mBossAttackHit = false;		
		     mMove = 0;//�U���̃A�j���[�V�����̂��Ƃ͑ҋ@�̃A�j���[�V�����ɐ؂�ւ��
			 mState = EAUTOMOVE;
	}
}
//�U�������R�i�W�����v�U���j
void CBoss::Attack3() {
	ChangeAnimation(8, false, 120);
	mPosition.mY += mJump;//�W�����v
	mRotation.mX+=6.0f;//��]
	mColSphereHead.mRadius = 6.0f;//�n�ʂ��痎���Ȃ��悤�ɃR���C�_�[��傫��
	if (mJump > -0.5f) {
				mJump -= G;//�d��
	}
    else if (mJump >= -2.5f) {
		mColSphereAttack.mRenderEnabled = true;//�R���C�_�[��\��
		mBossAttackHit = true;//�U�������L��
			mJump -= G2;//��яオ��Ƃ������d�͂�傫��
	}
	//���n������U��������������A�ҋ@������
	if (mJumpStopper == true) {
		if (mAnimationFrame >= mAnimationFrameSize) {	
			mColSphereAttack.mRenderEnabled = false;
			mBossAttackHit = false;
			mRotation.mX = 0.0f;
			mColSphereHead.mRadius = 3.0f;//�R���C�_�[�̔��a�����Ƃɖ߂�
				mState = EAUTOMOVE;
		}
	}
	
}
//�U�������S�i��]�U���j
void CBoss::Attack4() {
	
		mPosition.mY -= G;//�d�͂�������
	
	ChangeAnimation(6, false, 2000);
	switch (mAttack4Count) {
	case(1):
		if (mAttack4RotationCount > -90.0f) {
			mAttackRotation = ROTATIONBEFOREATTACK;
			mRotation.mY += mAttackRotation;
			mAttack4RotationCount += mAttackRotation;
		}
		else if (mAttack4RotationCount <= -90.0f) {
            mAttackRotation = 0.0f;
			mAttack4directionCount = 1;
			mAttack4MoveX = 0.5f;
			mAttack4MoveZ = 0.5f;
			mAttack4Count = 2;
		}
		break;
	case(2):
		mBossAttackHit = true;//�U�������L��
		mAttack4MoveCount++;
		mPosition += CVector(mAttack4MoveX, 0.0f, mAttack4MoveZ) ;
			switch (mAttack4directionCount) {
			case(1)://X+Z+
				
				if (mAttack4MoveCount >= 60) {
					mAttack4MoveCount = 0;
					mAttack4MoveZ *= -1;
					mAttack4directionCount = 2;
				}
				break;
			case(2)://X-Z+
				
				if (mAttack4MoveCount >= 60) {
					mAttack4MoveCount = 0;
					mAttack4MoveX *= -1;
					mAttack4directionCount = 3;
				}

				break;
			case(3)://X-Z-
				
				if (mAttack4MoveCount >= 60) {
					mAttack4MoveCount = 0;
					mAttack4MoveZ *= -1;
					mAttack4directionCount = 4;
				}
				break;
			case(4)://X+Z-
				
				if (mAttack4MoveCount >= 60) {
					mAttack4MoveCount = 0;
					
					mAttack4MoveZ *= -1;
					mAttack4directionCount = 1;
				}
				break;
			}
		//��]
		mRotation.mY += mAttackRotation;
		//��]�lY�������邲�Ƃɑ���
		mAttack4RotationCount += mAttackRotation;
		//���񂾂񑁂��Ȃ�
		if (mAttackRotation < ROTATIONCOUNT) {
			mAttackRotation += ROTATION;
		}
		//�����J�n
		 if (mAttack4RotationCount >= ROTATIONBASE) {
			 //�ړ����Ȃ�
			//mAttack4directionCount = 0;
			//��������
			mAttack4Count = 3;
		}
		break;
	case(3):
		//����
		if (mAttackRotation > 0.0f) {
			mAttackRotation += ROTATIONMIN;
			mRotation.mY += mAttackRotation;
		}
		 if (mAttackRotation <= 0.0f) {
			mAnimationFrame = mAnimationFrameSize;
			mAttack4RotationCount = 0.0f;
			mAttackRotation = 0.0f;
			mAttack4Count = 0;
			mBossAttackHit = false;
			mState = EAUTOMOVE;
		}

		break;
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
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	mColSphereHead.mRadius = 2.0f;                                    
	if (mBossBgmDeath == true) {
		tSceneGame->mBgmCountCheck = false;
		tSceneGame->mBgmCount = 4;
		mBossBgmDeath = false;
	}
	ChangeAnimation(9, false, 250);
	//�̗͂��Ȃ��Ȃ�����
	if (mHp <= 0) {
		mHp--;
		//15�t���[�����ƂɃG�t�F�N�g
		if (mHp % 15 == 0) {
			//�G�t�F�N�g����
			new CEffect2(mPosition, 10.0f, 10.0f, CEffect2::EFF_EXP, 4, 4, 2);
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
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
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
	case EATTACK3:
		Attack3();
		break;
	case EATTACK4:
		Attack4();
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
			if (tSceneGame->mVoiceSwitch == true) {
				BossVoice.Play();
			}
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mBossAttackHit = false;
			if (mState != EATTACK3) {
			
			}
		}
		break;
	case(6):
		if (mAnimationFrame == 30) {
			if (tSceneGame->mVoiceSwitch == true) {
			  BossVoice.Play();
			}
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mBossAttackHit = false;
			
		}
		break;
	}
	if (mAttackPercent >= 10) {
		mAttackPercent = 0;
	}
	if (mBossDamageCount > 0) {
		mBossDamageCount--;
	}
	if (mBossDamageCount > 0) {
		if (mEffectCount % 10 == 0) {
			//�G�t�F�N�g����
				CXPlayer* tPlayer = CXPlayer::GetInstance();
				mBossEffect = new CEffect2(tPlayer->GetSwordColPos(), 3.0f, 3.0f, CEffect2::EFF_EXP, 4, 4, 2,false, &mRotation);
		}
	}
	if (mHp <= 0 && mState != EDEATH) {
		mState = EDEATH;
	}
	if (mColSearch.mRenderEnabled == false) {
		if (mBossBgm == true) {
			tSceneGame->mBgmCountCheck = false;
			tSceneGame->mBossBattleStage = true;
			tSceneGame->mBgmCount = 3;
			mBossBgm = false;
		}
	}
	mEffectCount--;
	CXCharacter::Update();
}

//Collision(�R���C�_�P�C�R���C�_�Q�C�j
void CBoss::Collision(CCollider* m, CCollider* o) {
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
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
							if (mColSearch.mRenderEnabled == true) mColSearch.mRenderEnabled = false;
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
					switch(o->mTag) {
					case CCollider::EPLAYERSWORD:
                            //�Փ˂��Ă���Ƃ�
							if (CCollider::Collision(m, o)) {
								if (mBossDamageCount <= 0) {
									//�e��CXPlayer�����Ƀ|�C���^�����A�ϐ����Q��
									if (((CXPlayer*)(o->mpParent))->GetAttackHit()==true)
									{
										if (mColSearch.mRenderEnabled == true) mColSearch.mRenderEnabled = false;
										//�����G�t�F�N�g�b���t�^
										mEffectCount = 5;
										if (mHp > 0) {
											((CXPlayer*)(o->mpParent))->CXPlayer::SpAttackPoint2();
											//30�����邲�Ƃɂ̂�����
											if (mHp == HPCOUNT1 || mHp == HPCOUNT2 || mHp == HPCOUNT3) {
												mColliderCount = 10;
												mCollisionEnemy = mPosition - o->mpParent->mPosition;
												mCollisionEnemy.mY = 0;
												mCollisionEnemy = mCollisionEnemy.Normalize();
												mState = EDAMAGED;
											}
											mHp--;
											if (((CXPlayer*)(o->mpParent))->GetAttackSp() == true) {
												mBossDamageCount = 3;
											}
											else {
											mBossDamageCount = 30;
											}
										}
									}
								}
							}
						break;
					case CCollider::EITEMCOLLIDER:
						//�Փ˂��Ă���Ƃ�
						if (((CXPlayer*)(o->mpParent))->GetAttackHit() == false) {
							if (CCollider::Collision(m, o)) {
								if (mBossDamageCount <= 0) {
									if (mColSearch.mRenderEnabled == true) mColSearch.mRenderEnabled = false;
									//�����G�t�F�N�g�b���t�^
									mEffectCount = 5;
									if (mHp > 0) {
										((CXPlayer*)(o->mpParent))->CXPlayer::SpAttackPoint2();
										//30�����邲�Ƃɂ̂�����
										if (mHp == HPCOUNT1 || mHp == HPCOUNT2 || mHp == HPCOUNT3) {
											mColliderCount = 10;
											mCollisionEnemy = mPosition - o->mpParent->mPosition;
											mCollisionEnemy.mY = 0;
											mCollisionEnemy = mCollisionEnemy.Normalize();
											mState = EDAMAGED;

										}
										mHp--;
										mBossDamageCount = 30;

									}
								}
							}
						}
						break;
					case CCollider::EPLAYERBODY:
                        if (CCollider::Collision(m, o)) {
							
									if (mState ==EAUTOMOVE) {
										if (mHp > 0) {
											mState = EIDLE;
										}
									}
							mColliderCount = 1.5f;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy = mCollisionEnemy.Normalize();
						}
						break;
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
					if (mState == EATTACK3) {

						if (mAnimationFrame >= mAnimationFrameSize) {
							mJumpStopper = true;
						}
				    }	
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
	mColSphereAttack.ChangePriority();
	//�Փˏ��������s

	CCollisionManager::Get()->Collision(&mColSphereRightFront, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereLeftFront, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereHead, COLLISIONRANGEFIELD);
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereAttack, COLLISIONRANGE);
}

void CBoss::Render2D()
{
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	//2D�`��J�n
	CUtil::Start2D(0, 800, 0, 600);

	//�̗͂̊���
	float hpRate = (float)mHp / (float)HP;
	//�̗̓Q�[�W�̕�
	float hpGaugeWid = GAUGE_WID_MAXHP * hpRate;
	if (tSceneGame->mBossGaugeSwitch == true&&mHp>0) {
		mImageGauge.Draw(50, GAUGE_WID_MAXHP, 510, 550, 201, 300, 63, 0);//�Q�[�W�w�i
		mImageGauge.Draw(50, hpGaugeWid, 510, 550, 487, 572, 63, 0);//�̗̓Q�[�W
	}

	CUtil::End2D();
}
