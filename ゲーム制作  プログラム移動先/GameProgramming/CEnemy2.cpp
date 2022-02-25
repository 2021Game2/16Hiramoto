#include"CEnemy2.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CXCharacter.h"
#include"CUtil.h"
#include"CText.h"
#define HP 30
#define VELOCITY 0.2f //�}�N��
#define ROTATION 180.0f
#define JUMP 2.0f
#define G 0.1f
int CEnemy2::mEnemy2AttackCount = 0;
 extern CSound Enemy2Voice;
CModel CEnemy2::mModel;//���f���f�[�^�쐬
//�f�t�H���g�R���X�g���N�^
//�G�i�T�\���j
CEnemy2::CEnemy2()
//�R���C�_�̐ݒ�
	:mCollider(this,&mMatrix,CVector(-0.5f,0.0f,-1.0f),1.0f)
	,mColSearch(this,&mMatrix,CVector(0.0f,0.0f,0.0f),200.0f)
	,mColSphereHead(this,&mMatrix,CVector(0.0f,0.5f,-1.0f),3.0f)
	, mColSphereRight(this, &mMatrix, CVector(1.5f, 3.0f, 0.5f), 2.0f)
	, mColSphereLeft(this, &mMatrix, CVector(-1.0f, 0.5f, 0.0f), 2.0f)
	//,mpPlayer(0)
	,mHp(HP)
	,mJump(0.0f)
	,mJump2(0)
	, mEnemyDamage(60)
	,mMove(0)
	,mMove2(0)
	, mColliderCount(0.0f)
	,mGravity(0.0f)
	,mTime(0.0f)
	,CurveCount(0.0f)
	,mEnemyVoice(0)
	, mDamageCount(0)
{
	
	mGravity = 0.20f;
	mTag = EENEMY2;
	//mColSearch.mTag = CCollider::ESEARCH;//�^�O�ݒ�
	//mCollider.mTag = CCollider::EENEMY2COLLIDER;
	mColSphereHead.mTag= CCollider::EENEMY2COLLIDER;
		mColSphereRight.mTag= CCollider::EENEMY2COLLIDERATTACK;
		mColSphereLeft.mTag= CCollider::EENEMY2COLLIDERATTACK;
		mGravity = 0.20f;
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
}

void CEnemy2::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	mCollider.mpMatrix = &mpCombinedMatrix[1];
	//��
	mColSphereHead.mpMatrix = &mpCombinedMatrix[10];
	mColSphereRight.mpMatrix = &mpCombinedMatrix[9];
	mColSphereLeft.mpMatrix = &mpCombinedMatrix[20];
	mState = EAUTOMOVE;

	

}
//�ҋ@����
void CEnemy2::Idle() {
	//60���܂�܂őҋ@�̃A�j���[�V����
		ChangeAnimation(8, true, 60);
		if (mMove >= 60) {
            //�����蔻�肪�K�p����鎞��
			mEnemy2AttackCount = 120;
			//60���܂�����ԂŃA�j���[�V�������I���ƍU�������Ɉڍs
			if (mAnimationFrame >= mAnimationFrameSize)
			{
				mState = EATTACK;
			}
		}
		//60���܂�O�ɃA�j���[�V�������I�������ړ������Ɉڍs
	    else if (mAnimationFrame >= mAnimationFrameSize) {
            mState = EAUTOMOVE;
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
void CEnemy2::Attack() {
	
	    //�U���A�j���[�V����
		ChangeAnimation(4, false, 120);//+�T�Ԗڂ̃A�j���[�V�����t���[���P�Q�O
		
		//�����蔻�肪�K�p����鎞��
		if (mEnemy2AttackCount > 0) {
			mEnemy2AttackCount--;
		}
		//�U���̂��Ƃ͈ړ������Ɉڍs
		if(mEnemy2AttackCount<=0){
			if (mState == EATTACK) {
				mMove = 0;//�U���̃A�j���[�V�����̂��Ƃ͈ړ��̃A�j���[�V�����ɐ؂�ւ��
				mState = EAUTOMOVE;
			}
		}
		
}	
//�_���[�W����
void CEnemy2::Damaged() {
	//�̗͌���
	mHp--;

	if (mDamageCount < 60) {
		mDamageCount++;
	}
	//������ԁiX,Z��)
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition = mPosition + mCollisionEnemy * mColliderCount;
     
	}

	if (mDamageCount >= 60) {
	//�_���[�W�̂��Ƃ͈ړ�����
    mState = EAUTOMOVE;
	mDamageCount = 0;
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
		if (mHp % 15 == 0) {
			//�G�t�F�N�g����
			new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		CTransform::Update();
	}
	//������ԁiX,Z��)
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition.mX = mPosition.mX + mCollisionEnemy.mX * mColliderCount;
		mPosition.mZ = mPosition.mZ + mCollisionEnemy.mZ * mColliderCount;
	}
	//������΂�(���������グ�̌����j
	if (mPosition.mY > 0.0f) {
		//mPosition.mY = mJump * mTime - 0.5 * mGravity * mTime * mTime;
		//mTime++;
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
	//mpPlayer = mpPointPlayer;
	//�A�j���[�V�����̊Ǘ�
	switch (mAnimationIndex) {
		//�U���A�j���[�V����
	case(4):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(5, false,120);
		}
break;
	case(5):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(8, false, 60);
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
	mEnemyVoice++;
	if (mEnemyVoice>=180) {
		//Enemy2Voice.Play();
		mEnemyVoice = 0;
	}
	
	CXCharacter::Update();
}

//Collision(�R���C�_�P�C�R���C�_�Q�C�j
void CEnemy2::Collision(CCollider* m, CCollider* o) {

	m->mType == CCollider::ESPHERE;
	//�������T�[�`�p�̂Ƃ�
	/*
	if (m->mTag == CCollider::ESEARCH) {
		//���肪�e�R���C�_�̂Ƃ�
		if (o->mType == CCollider::ESPHERE) {
			//���肪�v���C���[�̂Ƃ�
			if (o->mpParent->mTag == EPLAYER) {
				//�Փ˂��Ă���Ƃ�
				//if (CCollider::Collision(m, o)) {
					//�v���C���[�̃|�C���^�ݒ�
					//mpPlayer = o->mpParent;
				//}
			}
		}
		return;
	}
	*/
	//EENEMY2COLLIDER�̎�
	if (m->mTag == CCollider::EENEMY2COLLIDER) {

		if (o->mType == CCollider::ESPHERE) {

			if (o->mpParent->mTag == EPLAYER) {
				//���肪����̂Ƃ�
				if (o->mTag == CCollider::EPLAYERSWORD) {
					//�Փ˂��Ă���Ƃ�
					if (CCollider::Collision(m, o)) {
						if (CXPlayer::mAttackCount > 0) {
							mColliderCount = 5;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy.mY = 0;
							mCollisionEnemy = mCollisionEnemy.Normalize();
							mState = EDAMAGED;
							if (mHp <= 0) {
								mJump = JUMP;
								
								
								mState = EDEATH;

							}
						}
					}
				}
				//���肪ESTOPPER�̎�
				if (o->mTag == CCollider::ESTOPPER) {

					if (CCollider::Collision(m, o)) {
						if (mState != EATTACK) {
							if (mState != EIDLE) {
								mState = EIDLE;
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
	if (m->mTag == CCollider::EENEMY2COLLIDER) {

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
		return;
	}
	
}
void CEnemy2::TaskCollision() {
    //�R���C�_�̗D��x�ύX
    mCollider.ChangePriority();
    mColSearch.ChangePriority();
	mColSphereHead.ChangePriority();
	mColSphereRight.ChangePriority();
	mColSphereLeft.ChangePriority();
	//�Փˏ��������s

	CCollisionManager::Get()->Collision(&mColSphereRight, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereLeft, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereHead, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
