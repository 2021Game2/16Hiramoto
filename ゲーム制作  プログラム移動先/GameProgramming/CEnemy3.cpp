#include"CEnemy3.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CUtil.h"
#include"CText.h"
#include"CXCharacter.h"
#include"CSound.h"
#include"CSceneGame.h"
#include"CItem.h"
#define OBJ "3DModel\\Bee\\BEE1.obj"//���f���̃t�@�C��
#define MTL "3DModel\\Bee\\BEE1.mtl"//���f���̃}�e���A���t�@�C��
#define DAMAGEEFFECT "Resource\\png,tga\\exp.tga"
#define HP 1
#define VELOCITY 0.25f //�s���P
#define VELOCITY1  -0.1f//�s���P
#define COLLIDERCOUNT 5.0
#define VELOCITY2 0.1f
#define VELOCITY3 0.2f
#define JUMP 4.0f
#define G 0.1f
#define PLAYERSPPOINT_MAX 30
int CEnemy3::mMoveCount = 0;
 extern CSound Enemy3Fry;

CModel CEnemy3::mModel;//���f���f�[�^�쐬
//�f�t�H���g�R���X�g���N�^
//�G�i�n�`�j
CEnemy3::CEnemy3()
//�R���C�_�̐ݒ�
	:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 5.0f)
	, mColSearch2(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f),70.0f)
	, mpPlayer(0)
	, mHp(HP)
	, mJump(0)
	, mJump2(0)
	, mEnemyDamage(60)
	, mMove2(0)
	, mColliderCount(0)
	,mCount(0)
	,mFireCount(0)
	,mEnemy3Fry(0)
{
	mRotation.mY += 90.0f;
	mTag = EENEMY3;
	mState = EIDLE;
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
    mCollider.mTag = CCollider::EENEMY3COLLIDERBODY;
	mColSearch2.mTag = CCollider::EENEMY3ESEARCH2;//�^�O�ݒ�
	
}

//CEnemy(�ʒu�A��]�A�g�k�j
CEnemy3::CEnemy3(const CVector& position, const CVector& rotation, const CVector& scale)
	:CEnemy3()
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position;//�ʒu�̐ݒ�
	mRotation = rotation;//��]�̐ݒ�
	mScale = scale;//�g�k�̐ݒ�
	CTransform::Update();//�s��̍X�V
	//�ڕW�n�_�̐ݒ�
	//mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}

//�X�V����
void CEnemy3::Update() {
	//CXPlayer���g�����|�C���^�Ƀv���C���[�̏���Ԃ�������������(CXPlayer�̒��̏����Ȃ̂Ń|�C���^�����K�v����j
	CXPlayer* tPlayer = CXPlayer::GetInstance();
	
	//if(mPosition.mY<=mpPlayer->mPosition.mY)
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
	switch (mMoveCount) {
		//���V
	case(0):
		//�O����U�O�t���[��
		if (mCount < 60) {
			if (mCount >= 0) {
				mPosition.mY -= 0.1f;
			}
		}
		//�U�O����P�Q�O�t���[��
		if (mCount < 120) {
			if (mCount >= 60) {
				mPosition.mY += 0.1f;
			}
		}
		if (mCount >= 120) {
			mCount = 0;

			if (mColSearch2.mRenderEnabled == false) {
				//mMoveCount = 1;
			}
		}
		break;
		//�ړ��i�܂������ړ��j
	case(1):
		if (mCount < 180) {
			mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;

			//CXPlayer���g�����|�C���^�Ƀv���C���[�̏���Ԃ�������������(CXPlayer�̒��̏����Ȃ̂Ń|�C���^�����K�v����j
			CXPlayer* tPlayer = CXPlayer::GetInstance();

			if (mPosition.mY >= tPlayer->mPosition.mY+5.0f) {
				mPosition = mPosition + CVector(0.0f, -0.1f, VELOCITY) * mMatrixRotate;
				
			}
			else {
				mPosition.mY++;
			}
		}
		if (mCount >= 180) {
			mMoveCount = 2;
			mCount = 0;

		}
		break;
		//�ړ��Q�i�E���Ɉړ��j
	case(2):
		if (mCount < 10) {

			mPosition = mPosition + CVector(3.0f, 2.0f, -1.5f) * mMatrixRotate;
		}
		if (mCount >= 10) {
			mMoveCount = 3;
			mCount = 0;

		}
		break;
		//�ړ��R�i���Ɉړ��j
	case(3):
		if (mCount <= 10) {
			mPosition = mPosition + CVector(-6.0f, -1.0f, 0.0f) * mMatrixRotate;
		}
		if (mCount >= 10) {
			mMoveCount = 4;
			mCount = 0;

		}
		break;
		//�ړ��S�i�E�O�Ɉړ��i���̈ʒu�ɖ߂�j
	case(4):
		if (mCount < 30) {
			mPosition = mPosition + CVector(1.5f, VELOCITY, 0.5f) * mMatrixRotate;
		}
		if (mCount >= 30) {
			mMoveCount = 1;
			mCount = 0;

		}
		break;
	}


	if (mHp <= 0) {

		//�������(X�������j
		if (mColliderCount > 0) {
			mColliderCount--;
			mPosition = mPosition + mCollisionEnemy * mColliderCount;
		}
		//������ԁiY�������j
		if (mJump2 > 0) {
			mPosition.mY += mJump;
			mJump2--;
		}



		mHp--;
		//15�t���[�����ƂɃG�t�F�N�g
		if (mHp % 15 == 0) {
			//�G�t�F�N�g����
			new CEffect2(mPosition, 1.0f, 1.0f,CEffect2::EFF_EXP, 4, 4, 2);
		}
		CTransform::Update();
	}
	mCount++;
	if (mHp <= -70) {
		mEnabled = false;
		CSceneGame::mEnemy3Count--;
		CSceneGame::mEnemy3CountStopper--;
	}
	if (mJump > 0) {
		mJump--;
	}
	if (mFireCount > 0) {
		mFireCount--;
	}
	mEnemy3Fry++;
	if (mEnemy3Fry >= 300) {
		if (CSceneGame::mVoiceSwitch == true) {
			Enemy3Fry.Play();
		}
		mEnemy3Fry = 0;
	}

	

}
//Collision(�R���C�_�P�C�R���C�_�Q�C�j
void CEnemy3::Collision(CCollider* m, CCollider* o) {
	m->mType = CCollider::ESPHERE;
	//�������T�[�`�p�̂Ƃ�

	if (m->mpParent->mTag == EENEMY3) {
		if (m->mTag == CCollider::EENEMY3ESEARCH2) {
			//���肪�e�R���C�_�̂Ƃ�
			if (o->mType == CCollider::ESPHERE) {
				//���肪�v���C���[�̂Ƃ��A
				if (o->mpParent->mTag == EPLAYER) {
					if (o->mTag == CCollider::EPLAYERBODY) {
						//�Փ˂��Ă���Ƃ�
						if (CCollider::Collision(m, o)) {

							if (mColSearch2.mRenderEnabled == true) {
								mCount = 0;
								if (mMoveCount == 0|| mMoveCount == 1) {
									mMoveCount = 1;
								}
								mColSearch2.mRenderEnabled = false;
							}
						}
					}
				}
			}
			return;
		}
	}
	
	if (m->mTag == CCollider::EENEMY3COLLIDERBODY) {
		if (o->mType == CCollider::ESPHERE) {
			//���肪����̂Ƃ��A
			if (o->mpParent->mTag == EPLAYER || o->mpParent->mTag == EITEM) {
				if (o->mTag == CCollider::EPLAYERSWORD) {
					//�Փ˂��Ă���Ƃ�
					if (CCollider::Collision(m, o)) {
						//�e��CXPlayer�����Ƀ|�C���^�����A�ϐ����Q��
						if (((CXPlayer*)(o->mpParent))->mAttackHit == true)
						{

							if (((CXPlayer*)(o->mpParent))->mSpAttack < PLAYERSPPOINT_MAX) {

								((CXPlayer*)(o->mpParent))->mSpAttack++;
							}
							mColliderCount = COLLIDERCOUNT;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy.mY = 0;
							mCollisionEnemy = mCollisionEnemy.Normalize();
							mJump = JUMP;
							mHp--;
						}

					}
				}

				if (o->mTag == CCollider::EITEMCOLLIDER) {
					//�Փ˂��Ă���Ƃ�
					if (CCollider::Collision(m, o)) {
						if (((CItem*)(o->mpParent))->mItemAttackHit == true)
						{

							mColliderCount = COLLIDERCOUNT;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy.mY = 0;
							mCollisionEnemy = mCollisionEnemy.Normalize();
							mJump = JUMP;
							mHp--;
						}
					}
				}
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

		if (o->mpParent->mTag == EENEMY3) {

			if (o->mTag == CCollider::EENEMY3COLLIDERBODY) {
				CVector adjust;
				if (CCollider::CollisionSylinder(o, m, &adjust)) {
					//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
					mPosition = mPosition + adjust;
				}
			}
		}
		if (o->mType == CCollider::ETRIANGLE) {
			if (mMoveCount == 1) {
				CVector adjust;//�����l
				//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
				//adjust�A�A�A�����l
				if (CCollider::CollisionTriangleSphere(o, m, &adjust))
				{
						//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
						//mPosition = mPosition + adjust;
					
				}
			}
			
		}
		
		
	}
	return;
}
void CEnemy3::TaskCollision() {
	mColSearch2.ChangePriority();
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mColSearch2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGEFIELD);
}
