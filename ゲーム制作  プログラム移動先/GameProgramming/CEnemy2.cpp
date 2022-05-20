#include"CEnemy2.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CSceneGame.h"
#include"CXCharacter.h"
#include"CUtil.h"
#include"CText.h"
#include"CItem.h"
#define DAMAGEEFFECT "Resource\\png,tga\\exp.tga"
#define HP 30
#define VELOCITY 0.2f //�}�N��
#define ROTATION 180.0f
#define JUMP 2.0f
#define G 0.1f
 extern CSound Enemy2Voice;
CModel CEnemy2::mModel;//���f���f�[�^�쐬
//�f�t�H���g�R���X�g���N�^
//�G�i�T�\���j
CEnemy2::CEnemy2()
//�R���C�_�̐ݒ�
	: mColSphereRight(this,&mMatrix, CVector(1.5f, 3.0f, 0.5f), 2.0f)
	, mColSphereLeft(this,&mMatrix,  CVector(-1.0f, 0.5f, 0.0f), 2.0f)
	, mColSphereBody(this,&mMatrix,  CVector(0.0f,1.0f,0.0f),2.0f)
	,mHp(HP)
	,mJump(0.0f)
	, mEnemyDamage(60)
	,mMove(0)
	,mMoveCount(false)
	,mColliderCount(0.0f)
	,mTime(0.0f)
	,CurveCount(0.0f)
	,mEnemyVoice(0)
	,mDamageCount(0)
	,mEnemy2AttackHit(false)
	,mEnemyLevel(0)
	,mEnemyHpPercent(1.0f)
	, mEnemy2Bgm(true)
{
	mTag = EENEMY2;
	mColSphereRight.mTag= CCollider::EENEMY2COLLIDERATTACK;
	mColSphereLeft.mTag= CCollider::EENEMY2COLLIDERATTACK;
	mColSphereBody.mTag = CCollider::EENEMY2COLLIDERBODY;
}

//CEnemy(�ʒu�A��]�A�g�k�j
CEnemy2::CEnemy2(const CVector& position, const CVector& rotation, const CVector& scale)
:CEnemy2()
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position;//�ʒu�̐ݒ�
	mRotation = rotation;//��]�̐ݒ�
	mScale = scale;//�g�k�̐ݒ�
	CTransform::Update();//�s��̍X�V
	//�ڕW�n�_�̐ݒ�
	mPoint =mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
	mHp = mHp * mEnemyHpPercent;
}

void CEnemy2::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	mColSphereRight.mpMatrix = &mpCombinedMatrix[9];
	mColSphereLeft.mpMatrix = &mpCombinedMatrix[20];
	mColSphereBody.mpMatrix = &mpCombinedMatrix[0];
	mState = EAUTOMOVE;
}
//�ҋ@����
void CEnemy2::Idle() {
	    //60���܂�܂őҋ@�̃A�j���[�V����
		ChangeAnimation(8, true, 60);
		mMove++;
		if (mMove >= 300) {
			//60���܂�����ԂŃA�j���[�V�������I���ƍU�������Ɉڍs
			if (mAnimationFrame >= mAnimationFrameSize)
			{
				mState = EATTACK;
			}
		}
		//60���܂�O�ɃA�j���[�V�������I�������ړ������Ɉڍs
	    else if (mAnimationFrame >= mAnimationFrameSize) {
			if (mMoveCount == true) {
             mState = EAUTOMOVE;
			}
		}
}		
//�ړ�����
void CEnemy2::AutoMove() {
	//CXPlayer���g�����|�C���^�Ƀv���C���[�̏���Ԃ�������������(CXPlayer�̒��̏����Ȃ̂Ń|�C���^�����K�v����j
	CXPlayer* tPlayer = CXPlayer::GetInstance();
	mPlayerMarkingX = tPlayer->mPosition.mX - mPosition.mX;
	mPlayerMarkingZ = tPlayer->mPosition.mZ - mPosition.mZ;
	//����
	mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
	ChangeAnimation(1, true, 60);
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
	int r = rand() % 60; //rand()�͐����̗�����Ԃ�
	//%180�͂P�W�O�Ŋ������]������߂�
	if (r == 0) {
		mPoint = tPlayer->mPosition;
	}
}	
//�U������
void CEnemy2::Attack() {
	    //�U���A�j���[�V����
		ChangeAnimation(4, false, 120);
		if (mAnimationFrame >= mAnimationFrameSize) {
			mMove = 0;
			mState = EAUTOMOVE;//�U���̃A�j���[�V�����̂��Ƃ͈ړ��̃A�j���[�V�����ɐ؂�ւ��
        
		}
}	
//�_���[�W����
void CEnemy2::Damaged() {
	if (CSceneGame::mBgmCount <= 2) {

		if (mEnemy2Bgm == true) {
			CSceneGame::mBgmCountCheck = false;
			CSceneGame::mBgmCount = 2;
			mEnemy2Bgm = false;
		}
	}
	//���G���ԕt�^
	if (mDamageCount < 60) {
		mDamageCount++;
	}
	//�����G�t�F�N�g�t�^
	if (mEffectCount % 15 == 0) {
		//�G�t�F�N�g����
		new CEffect(mPosition, 1.0f, 1.0f, CEffect::EFF_EXP, 4, 4, 2);
	}
	//�q�b�g�o�b�N�iX,Z��)
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition = mPosition + mCollisionEnemy * mColliderCount;
	}

	if (mDamageCount >= 60) {
	//�_���[�W�̂��Ƃ͈ړ�����
    mState = EAUTOMOVE;
	
	}
}		
//���S����
void CEnemy2::Death() {
	//�̗͂��Ȃ��Ȃ�����
	if (mHp <= 0) {
		//mTime��mJump�ɐ������������A������Ԃ悤�ɂȂ�
		mPosition.mY += mJump;
		if (mJump >= -0.5f) {
			mJump -= G;
		}
		mHp--;
		//15�t���[�����ƂɃG�t�F�N�g
		if (mEffectCount % 15 == 0) {
			//�G�t�F�N�g����
			//new CEffect(mPosition, 1.0f, 1.0f, DAMAGEEFFECT, 4, 4, 2);
		}
		CTransform::Update();
	}
	//������ԁiX,Z��)
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition.mX = mPosition.mX + mCollisionEnemy.mX * mColliderCount;
		mPosition.mZ = mPosition.mZ + mCollisionEnemy.mZ * mColliderCount;
	}
	//���΂炭�o���������
	if (mHp <= -120) {
		mEnabled = false;
		CSceneGame::mEnemy2Count --;
		CSceneGame::mEnemy2CountStopper--;
	}
}		

//�X�V����
void CEnemy2::Update() {
	mEnemyLevel = rand() % 10 + 1;
	mEnemyHpPercent += mEnemyLevel / 10;
	
	mEffectCount--;
	//mpPlayer = mpPointPlayer;
	//�A�j���[�V�����̊Ǘ�
	switch (mAnimationIndex) {
		//�U���A�j���[�V����
	case(4):
		if (mAnimationFrame == 30) {
			mEnemy2AttackHit = true;
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mEnemy2AttackHit = false;
			
		}
        break;
		
	}
	//�������s�����Ƃɕ���
	switch (mState) {
	case EIDLE:	//�ҋ@
		Idle();
		break;
	case EAUTOMOVE://�ړ�
		AutoMove();
		break;
	case EATTACK://�U��
		Attack();
		break;
	case EDAMAGED://�_���[�W
		Damaged();
		break;
	case EDEATH://���S
		Death();
		break;
	}
	mPosition.mY -= 0.1f;
	mEnemyVoice++;
	if (mEnemyVoice>=180) {
		if (CSceneGame::mVoiceSwitch == true) {
			Enemy2Voice.Play();
		}
		mEnemyVoice = 0;
	}

	if (mState != EDAMAGED) {
		mDamageCount = 0;
	}
	CXCharacter::Update();
}

//Collision(�R���C�_�P�C�R���C�_�Q�C�j
void CEnemy2::Collision(CCollider* m, CCollider* o) {

	m->mType = CCollider::ESPHERE;
	//EENEMY2COLLIDER(���n�T�~�̃R���C�_�j�̎�
	if (m->mTag == CCollider::EENEMY2COLLIDERATTACK) {

		if (o->mType == CCollider::ESPHERE) {

			if (o->mpParent->mTag == EPLAYER|| o->mpParent->mTag == EITEM) {
				//���肪�v���C���[�̕���̂Ƃ�
				if (o->mTag == CCollider::EPLAYERSWORD) {
					//�Փ˂��Ă���Ƃ�
					if (CCollider::Collision(m, o)) {
						//�v���C���[�̓����蔻�肪�L���ȂƂ�
						//�e��CXPlayer�����Ƀ|�C���^�����A�ϐ����Q��
						if (((CXPlayer*)(o->mpParent))->mAttackHit == true)
						{//�q�b�g�o�b�N���_���[�W���󂯂�
							if (mDamageCount <= 0) {
								//�v���C���[�̃W�����v�U���K�v�|�C���g����
								((CXPlayer*)(o->mpParent))->mSpAttack++;
								mEffectCount = 0;
								//�̗͌��� 
								mHp--;
								//�q�b�g�o�b�N�t�^ 
								mColliderCount = 3.0f;
								mCollisionEnemy = mPosition - o->mpParent->mPosition;
								//HP���O�̂Ƃ��ȊO�͑O�㍶�E�ɐ������
								mCollisionEnemy.mY = 0;
								mCollisionEnemy = mCollisionEnemy.Normalize();
							    //�_���[�W�����Ɉڍs
								if (mHp > 0) {

								 mState = EDAMAGED;

								}
								else if (mHp <= 0) {
									mJump = JUMP;
									mState = EDEATH;

								}
							}
						}
					}
				}
				if (o->mTag == CCollider::EITEMCOLLIDER) {
					//�Փ˂��Ă���Ƃ�
					if (CCollider::Collision(m, o)) {
						//�v���C���[�̓����蔻�肪�L���ȂƂ�
						//�e��CXPlayer�����Ƀ|�C���^�����A�ϐ����Q��
						if (((CItem*)(o->mpParent))->mItemAttackHit == true)
						{//�q�b�g�o�b�N���_���[�W���󂯂�
								//�v���C���[�̃W�����v�U���K�v�|�C���g����
								((CXPlayer*)(o->mpParent))->mSpAttack++;
								mEffectCount = 0;
								//�̗͌��� 
								mHp--;
								//�q�b�g�o�b�N�t�^ 
								mColliderCount = 1.5f;
								mCollisionEnemy = mPosition - o->mpParent->mPosition;
								//HP���O�̂Ƃ��ȊO�͑O�㍶�E�ɐ������
								mCollisionEnemy.mY = 0;
								mCollisionEnemy = mCollisionEnemy.Normalize();
								//�_���[�W�����Ɉڍs
								if (mHp > 0) {
									mState = EDAMAGED;
								}
								else if (mHp <= 0) {
									mJump = JUMP;
									mState = EDEATH;
								}
							
						}
					}
				}
				//���肪EPLAYERBODY(�v���C���[�̑̂̃R���C�_�j�̎�
				if (o->mTag == CCollider::EPLAYERBODY) {
					if (CCollider::Collision(m, o)) {
						//EIDLE�i�ҋ@��ԁj
						if (mState != EATTACK) {
							if (mState != EIDLE) {
								mState = EIDLE;
							}

						}
						//����ȏ�O�ɐi�߂Ȃ��Ȃ�
						mColliderCount = 1.5f;
						mCollisionEnemy = mPosition - o->mpParent->mPosition;
						mCollisionEnemy = mCollisionEnemy.Normalize();
					}
				}
			}
		}
	}
	//ETRIANGLE(�}�b�v�Ȃǂ̃R���C�_�j
	if (m->mTag == CCollider::EENEMY2COLLIDERBODY) {
		if (o->mType == CCollider::ETRIANGLE) {
			CVector adjust;//�����l
			//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
			//adjust�A�A�A�����l
				if (CCollider::CollisionTriangleSphere(o, m, &adjust))
				{
						//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
						mPosition = mPosition + adjust;
				}
			
			
		}
		
		if (o->mpParent->mTag == EENEMY2) {

			if (o->mTag == CCollider::EENEMY2COLLIDERBODY) {
				CVector adjust;
				if (CCollider::CollisionSylinder(o, m, &adjust)) {
					//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
					mPosition = mPosition + adjust;
				}
			}
		}
		return;
	}
	
}

void CEnemy2::TaskCollision() {
    //�R���C�_�̗D��x�ύX
	mColSphereRight.ChangePriority();
	mColSphereLeft.ChangePriority();
	mColSphereBody.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mColSphereRight, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereLeft, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereBody, COLLISIONRANGEFIELD);
	
}
