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
#define OBJ "Resource\\3DModel\\Bee\\BEE1.obj"//���f���̃t�@�C��
#define MTL "Resource\\3DModel\\Bee\\BEE1.mtl"//���f���̃}�e���A���t�@�C��
#define DAMAGEEFFECT "Resource\\png,tga\\exp.tga"
#define HP 1
#define VELOCITY 0.25f //�s���P
#define COLLIDERCOUNT 5.0f
#define COLLIDERCOUNT2 1.5f
#define VELOCITY2 0.1f
#define VELOCITY3 0.2f
#define FIRECOUNT 1	//���ˊԊu

#define JUMP 2.0f
#define G 0.1f
#define PLAYERSPPOINT_MAX 30
extern CSound Enemy3Fry;
CModel CEnemy3::mModel;//���f���f�[�^�쐬
//�f�t�H���g�R���X�g���N�^
//�G�i�n�`�j
CEnemy3::CEnemy3()
//�R���C�_�̐ݒ�
	:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 5.0f)
	,mColSearch2(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f),70.0f)
	,mpPlayer(0)
	,mMoveCount (0)
	,mJump2(0)
	,mMove2(0)
	,mColliderCount(0)
	,mCount(0)
	,mFireCount(0)
	,mEnemy3Fry(0)
	,mStopCount(0)
	,mEnemyDamage(60)
	,mJump(0.0f)
	,mHp(HP)
	, mDeathSwitch(false)
	
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
	mEnabled = true;
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
void CEnemy3::Idle() {

	mCount++;
	//0����20�t���[��
	if (mCount < 20) {
			mPosition.mY -= 0.5f;
	}
	//20����40�t���[��
	else if (mCount < 40) {
		mPosition.mY += 0.5f;
	}
	else if (mCount >= 40) {
		mCount = 0;
		//�v���C���[���߂��ɂ���΍s���p�^�[���ύX
		if (mColSearch2.mRenderEnabled == false) {
			mState = EMOVE1;
		}
	}
}
//����1
//�v���C���[�ɋ߂Â�
void CEnemy3::Move1() {
	mCount++;
	if (mCount < 180) {
		mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
		//CXPlayer���g�����|�C���^�Ƀv���C���[�̏���Ԃ�������������(CXPlayer�̒��̏����Ȃ̂Ń|�C���^�����K�v����j
		CXPlayer* tPlayer = CXPlayer::GetInstance();
		if (mPosition.mY >= tPlayer->mPosition.mY + 5.0f) {
			mPosition = mPosition + CVector(0.0f, -0.1f, VELOCITY) * mMatrixRotate;
		}
		else {
			if (mPosition.mY <= 30.0f) {
				mPosition.mY++;
			}
		}
	}
	else if (mCount >= 180) {
		mState = EMOVE2;
		
		mCount = 0;

	}
}
//����2
//�E���Ɉړ�
void CEnemy3::Move2() {
	mCount++;
	if (mCount < 10) {
		mPosition = mPosition + CVector(3.0f, 2.0f, -1.5f) * mMatrixRotate;
	}
	else{
		mState = EMOVE3;
		mCount = 0;
	}
}
//����3
//���Ɉړ�
void CEnemy3::Move3() {
	mCount++;
	if (mCount < 10) {
		mPosition = mPosition + CVector(-6.0f, -1.0f, 0.0f) * mMatrixRotate;
	}
	else{
		mState = EMOVE4;
		mCount = 0;
	}
}
//����4
//���̈ʒu�ɖ߂�
void CEnemy3::Move4() {
	mCount++;
	if (mCount < 30) {
		mPosition = mPosition + CVector(1.5f, VELOCITY, 0.5f) * mMatrixRotate;
	}
	if (mCount >= 30) {
		mState = EMOVE1;
		mCount = 0;

	}
}
//���S
void CEnemy3::Death() {
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	//�̗͂��Ȃ��Ȃ�����
	if (mHp <= 0) {
		mPosition.mY += mJump;
		if (mJump >= -1.0f) {
			mJump -= G;
		}
			//15�t���[�����ƂɃG�t�F�N�g
		if (mEffectCount % 15 == 0) {
			//�G�t�F�N�g����
			new CEffect2(mPosition, 1.0f, 1.0f, CEffect2::EFF_EXP, 4, 4, 2, false, &mRotation);
		}
		CTransform::Update();
	}
	//������ԁiX,Z��)
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition.mX = mPosition.mX + mCollisionEnemy.mX * mColliderCount;
		mPosition.mZ = mPosition.mZ + mCollisionEnemy.mZ * mColliderCount;
	}
	if (mHp <= -300) {
		mEnabled = false;
		tSceneGame->mEnemy3Count--;
		tSceneGame->mEnemy3CountStopper--;
	}
	
}
//�X�V����
void CEnemy3::Update() {
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	//�������s�����Ƃɕ���
	switch (mState) {
	case EIDLE://���V
		Idle();
		break;
	case EMOVE1://�ړ��i�܂������ړ��j
		Move1();
		break;
	case EMOVE2://�ړ��Q�i�E���Ɉړ��j
		Move2();
		break;
	case EMOVE3://�ړ��R�i���Ɉړ��j
		Move3();
		break;
	case EMOVE4://�ړ��S�i�E�O�Ɉړ��i���̈ʒu�ɖ߂�j
		Move4();
		break;
	case EDEATH://���S
		Death();
		break;
	}
	//CXPlayer���g�����|�C���^�Ƀv���C���[�̏���Ԃ�������������(CXPlayer�̒��̏����Ȃ̂Ń|�C���^�����K�v����j
	CXPlayer* tPlayer = CXPlayer::GetInstance();
	
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
	if (dy > margin) {
		if (mRotation.mX > -30.0f) {
		    mRotation.mX -= 3.0f;//��։�]
		}
	}
	else if (dy < -margin) {//���։�]
		if (mRotation.mX < 30.0f) {
			mRotation.mX += 3.0f;
		}
	}
	int r = rand() % 10; //rand()�͐����̗�����Ԃ�
	//%180�͂P�W�O�Ŋ������]������߂�
	if (r == 0) {
		mPoint = CVector(tPlayer->mPosition.mX, tPlayer->mPosition.mY+3.0f, tPlayer->mPosition.mZ);
	}
	if (mFireCount > 0) {
		mFireCount--;
	}
	mEnemy3Fry++;
	if (mEnemy3Fry >= 300) {
		if (tSceneGame->mVoiceSwitch == true) {
			Enemy3Fry.Play();
		}
		mEnemy3Fry = 0;
	}
	if (tSceneGame->mBossGaugeSwitch == true) {
		mEnabled = false;
	}
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition = mPosition + mCollisionEnemy * mColliderCount;
	}
	mEffectCount--;
	CTransform::Update();
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
			//���肪����̂Ƃ�
			switch (o->mpParent->mTag) {
			case(EPLAYER):
               if (o->mTag == CCollider::EPLAYERSWORD) {
					//�Փ˂��Ă���Ƃ�
					if (CCollider::Collision(m, o)) {
						//�e��CXPlayer�����Ƀ|�C���^�����A�ϐ����Q��
						if (((CXPlayer*)(o->mpParent))->GetAttackHit() == true)
						{
							if (((CXPlayer*)(o->mpParent))->GetSpAttack() < PLAYERSPPOINT_MAX) {
								((CXPlayer*)(o->mpParent))->CXPlayer::SpAttackPoint();
							}
							CXPlayer* tPlayer = CXPlayer::GetInstance();
							if (tPlayer->GetAttackSp() == true) {
								if (mState != EDEATH) {
									mHp = 0;
									mState = EDEATH;
								}
							}
							mEffectCount = 0;
							mColliderCount = COLLIDERCOUNT;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy.mY = 0;
							mCollisionEnemy = mCollisionEnemy.Normalize();
							
							mHp--; 
							if (mHp <= 0) {
								mJump = JUMP;
								mState = EDEATH;
							}
						}

					}
			   }
			   //���肪EPLAYERBODY(�v���C���[�̑̂̃R���C�_�j�̎�
		       if (o->mTag == CCollider::EPLAYERBODY) {
					if (CCollider::Collision(m, o)) {
						mStopCount++;
						if (mStopCount>=120) {
							mStopCount = 0;
							//����ȏ�O�ɐi�߂Ȃ��Ȃ�
							mColliderCount = 10.0f;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy = mCollisionEnemy.Normalize();

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
							if (((CXPlayer*)(o->mpParent))->GetSpAttack() < PLAYERSPPOINT_MAX) {
								((CXPlayer*)(o->mpParent))->SpAttackPoint();
							}
							mEffectCount = 0;
							//�̗͌��� 
							if (mState != EDEATH) {
								mHp = 0;
								mState = EDEATH;
							}
							//�q�b�g�o�b�N�t�^ 
							mColliderCount = 1.5f;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							//HP��0�̂Ƃ��ȊO�͑O�㍶�E�ɐ������
							mCollisionEnemy.mY = 0;
							mCollisionEnemy = mCollisionEnemy.Normalize();
							if (mHp <= 0) {
								mJump = JUMP;
							}

						}
					}
				}
				break;
			case(EENEMY2):
				if (o->mTag == CCollider::EENEMY2COLLIDERBODY) {
					CVector adjust;
					if (CCollider::CollisionSylinder(o, m, &adjust)) {
						//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
						mPosition = mPosition + adjust;
					}
				}
				break;
			case(EENEMY3):
				if (o->mTag == CCollider::EENEMY3COLLIDERBODY) {
					CVector adjust;
					if (CCollider::CollisionSylinder(o, m, &adjust)) {
						//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
					mPosition = mPosition + adjust;
					}
			    }
				break;
			}
		}
		if (o->mType == CCollider::ETRIANGLE) {
			 //�v���C���[���߂��ɂ���ꍇ
			if (mMoveCount == 1) {
				CVector adjust;//�����l
				//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
				//adjust�A�A�A�����l
				//�̗͂�0�̂Ƃ�
				if (mState == EDEATH) {
					if (CCollider::CollisionTriangleSphere(o, m, &adjust))
					{
						mHp--;
						//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
						mPosition = mPosition + adjust;
					}
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
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
