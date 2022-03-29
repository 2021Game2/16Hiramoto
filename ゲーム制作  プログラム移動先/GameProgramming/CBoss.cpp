#include"CBoss.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CXCharacter.h"
#include"CUtil.h"
#include"CText.h"

#define HP 30

#define VELOCITY 0.5f //�}�N��

#define JUMP 5.0f
#define G 0.1f
int CBoss::mBossAttackCount = 0;
int CBoss::mHp = HP;

 extern CSound BossVoice;
 extern CSound BossMove;

CModel CBoss::mModel;//���f���f�[�^�쐬
//�f�t�H���g�R���X�g���N�^
CBoss::CBoss()
//�R���C�_�̐ݒ�
	: mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 200.0f)
	, mColSphereHead(this, &mMatrix, CVector(0.0f, 1.0f, 5.0f), 5.0f)
	, mColSphereRightFront(this, &mMatrix, CVector(0.0f, -2.0f, 0.0f), 2.0f)
	, mColSphereLeftFront(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
	, mColSphereRightBack(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
	, mColSphereLeftBack(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)


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
	
{

	mGravity = 0.20f;
	mTag = EBOSS;
	mColSearch.mTag = CCollider::ESEARCH;//�^�O�ݒ�
	mColSphereHead.mTag = CCollider::EBOSSCOLLIDERHEAD;
	mColSphereRightFront.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mColSphereLeftFront.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mColSphereRightBack.mTag = CCollider::EBOSSCOLLIDER;
	mColSphereLeftBack.mTag = CCollider::EBOSSCOLLIDER;


	mGravity = 0.20f;
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
	mColSphereRightBack.mpMatrix = &mpCombinedMatrix[27];//�E��둫
	mColSphereLeftBack.mpMatrix = &mpCombinedMatrix[32];//����둫
	//mColSphereLeftBack.mpMatrix = &mpCombinedMatrix[17];
	mState = EATTACK2;


}
//�ҋ@����
void CBoss::Idle() {
	//30���܂�܂őҋ@�̃A�j���[�V����
	ChangeAnimation(8, false, 60);
	if (mMove >= 30) {
		//30���܂�����ԂŃA�j���[�V�������I���ƍU�������Ɉڍs
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			if (mAttackPercent <= 5) {
            //�����蔻�肪�K�p����鎞��
			mBossAttackCount = 80;
			mState = EATTACK;
		    }
			else if(mAttackPercent > 5) {
				//�����蔻�肪�K�p����鎞��
				mBossAttackCount = 80;
				mState = EATTACK2;
			}
		}
	}
	//30���܂�O�ɃA�j���[�V�������I�������ړ������Ɉڍs
	else if (mAnimationFrame >= mAnimationFrameSize) {
		mState = EAUTOMOVE;
	}


}
//�ړ�����
void CBoss::AutoMove() {
	//����
	BossMove.Play();
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
	if (r == 0) {
		if (mpPlayer) {
			//ESEARCH�ɏՓ˂��ă|�C���^�ɐݒ肵��
			//�v���C���[�̍��W���L�^

			mPoint = mpPlayer->mPosition;
		}
		else {
			mPoint = mPoint * CMatrix().RotateY(80);
		}
	}
	mpPlayer = tPlayer;
}
//�U������
void CBoss::Attack() {
	BossVoice.Play();
	//�U���A�j���[�V����
	ChangeAnimation(5, false, 40);
	//�����蔻�肪�K�p����鎞��
	/*
	if (mBossAttackCount > 0) {
		mBossAttackCount--;
	}*/
	
	//�U���̂��Ƃ͈ړ������Ɉڍs
	if (mAnimationFrame > mAnimationFrameSize) {
		//if (mState == EATTACK) {
			mMove = 0;//�U���̃A�j���[�V�����̂��Ƃ͈ړ��̃A�j���[�V�����ɐ؂�ւ��
			//mState = EIDLE;
			//mBossAttackCount = 0;
		//}
	}

}
//�U������
void CBoss::Attack2() {
	BossVoice.Play();
	//�U���A�j���[�V����
	ChangeAnimation(6, false, 40);
	//�����蔻�肪�K�p����鎞��
	/*
	if (mBossAttackCount > 0) {
		mBossAttackCount--;
	}
	*/
	
	//�U���̂��Ƃ͈ړ������Ɉڍs
	if (mAnimationFrame>mAnimationFrameSize) {
		//if (mState == EATTACK2) {
			mMove = 0;//�U���̃A�j���[�V�����̂��Ƃ͈ړ��̃A�j���[�V�����ɐ؂�ւ��
			//mBossAttackCount = 0;
			//mState = EIDLE;
		//}
	}

}
//�_���[�W����
void CBoss::Damaged() {
	//�̗͌���
	if (mBossDamageCount <= 0) {

	mHp--;
	mBossDamageCount = 10;
	}
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

	ChangeAnimation(9, false, 250);
	//�̗͂��Ȃ��Ȃ�����
	if (mHp <= 0) {
		mHp--;
		//15�t���[�����ƂɃG�t�F�N�g
		if (mHp % 15 == 0) {
			//�G�t�F�N�g����
			new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
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
	case EATTACK://�U��
		Attack();
		break;
	case EATTACK2:
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
		/*case(4):

			break;*/
	case(5):
		if (mAnimationFrame == 30) {
			mBossAttackHit = true;
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
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mBossAttackHit = false;
			mState = EIDLE;
		}
		break;
	}
		/*
	case(8):

		break;
	case(9):

		break;
		*/
	if (mAttackPercent < 10) {
		mAttackPercent++;
	}
	if (mAttackPercent >= 10) {
		mAttackPercent = 0;
	}
	if (mBossDamageCount > 0) {
		mBossDamageCount--;
	}

	if (mHp <= 0 && mState != EDEATH) {
		mState = EDEATH;
	}

	mGravity  -= G;
	mPosition.mY += mGravity;
	CXCharacter::Update();
}

//Collision(�R���C�_�P�C�R���C�_�Q�C�j
void CBoss::Collision(CCollider* m, CCollider* o) {
	//�R���C�_�̂Ƃ�
	m->mType == CCollider::ESPHERE;
	/*
		//�������T�[�`�p�̂Ƃ�
		if (m->mTag == CCollider::ESEARCH) {
			//���肪�e�R���C�_�̂Ƃ�
			if (o->mType == CCollider::ESPHERE) {
				//���肪�v���C���[�̂Ƃ�
				if (o->mpParent->mTag == EPLAYER) {
					//�Փ˂��Ă���Ƃ�
					if (CCollider::Collision(m, o)) {
						//�|�C���^���v���C���[�ɐݒ�

						mpPlayer = o->mpParent;
					}
				}
			}
			return;
		}
		*/
	
		//EENEMY2COLLIDER�̎�
		if (m->mTag == CCollider::EBOSSCOLLIDER||m->mTag== CCollider::EBOSSCOLLIDERATTACK) {

			if (o->mType == CCollider::ESPHERE) {

				if (o->mpParent->mTag == EPLAYER) {
					//���肪����̂Ƃ�
					if (o->mTag == CCollider::EPLAYERSWORD) {
						//�Փ˂��Ă���Ƃ�
					    
							if (CCollider::Collision(m, o)) {
								if (((CXPlayer*)(o->mpParent))->mAttackHit == true)
								{
								//if (CXPlayer::mAttackCount > 0) {
									if (mHp > 0) {
										if (mHp % 30 == 0) {
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
					if (o->mTag == CCollider::ESTOPPER) {

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
							mMove++;

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
	mColSphereRightBack.ChangePriority();
	mColSphereLeftBack.ChangePriority();

	//�Փˏ��������s

	CCollisionManager::Get()->Collision(&mColSphereRightFront, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereLeftFront, COLLISIONRANGE);

	CCollisionManager::Get()->Collision(&mColSphereRightBack, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereLeftBack, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereHead, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
}
