#include "CXPlayer.h"
#include "CKey.h"
#include "CCamera.h"
#include "CUtil.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CEnemy2.h"
#include"CBoss.h"
#include"CItem.h"
#include"CSound.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define ATTACKCOUNT1 20
#define ATTACKCOUNT2 20
#define ATTACKCOUNT3 40
#define JUMP 5.0f
#define JUMP2 10.0f
#define STEP  20.0f //�U���������O�i
#define STEP2 200.0f //
#define STAMINA 400 //�X�^�~�i

#define HP_MAX 10				//�̗͍ő�l
#define STAMINA_MAX 1000		//�X�^�~�i�ő�l
#define GAUGE_WID_MAX 400.0f	//�Q�[�W�̕��̍ő�l
#define GAUGE_LEFT 20			//�Q�[�W�`�掞�̍��[
#define IMAGE_GAUGE "Resource\\png,tga\\Gauge.png"		//�Q�[�W�摜
#define G 0.1f
#define G2 2.0f


int CXPlayer::mSpAttack = 0;
int CXPlayer::mStamina = STAMINA_MAX;
int CXPlayer::mAttackCount = 0;
int CXPlayer::mHp = HP_MAX;

extern CSound PlayerFirstAttack;
extern CSound PlayerSecondAttack;
extern CSound PlayerThirdAttack;
extern CSound PlayerJumpAttack;
extern CSound PlayerDamage;

CXPlayer* CXPlayer::mpPlayerInstance;
//�v���C���[�̃|�C���^��Ԃ����ƂŁA���W�Ȃǂ��Q�Ƃł���悤�ɂȂ�
CXPlayer* CXPlayer::GetInstance()
{
	return mpPlayerInstance;
}
CXPlayer::CXPlayer()
	: mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 2.5f)//���̃R���C�_�P
	, mColSphereFoot(this, nullptr, CVector(0.0f, 0.0f, -3.0f), 3.0f)//���t�߂̃R���C�_
	, mCollider2(this, &mMatrix, CVector(0.0f, -2.0f, 0.0f), 10.0f)//���̃R���C�_�Q
	, mJump(0.0f)
	, mGravity(0.0f)
	,mSpaceCount1(true)
	,mSpaceCount2(false)
	,mSpaceCount3(false)
	, mDamageCount(0)
	,mAnimationCount(0)
	,mColliderCount(1.0f)
	,mTime(0.0f)
	,mSpeed(0.0f)
{
	//�^�O�Ƀv���C���[��ݒ肵�܂�
	mTag = EPLAYER;
	
	mColSphereSword.mTag = CCollider::EPLAYERSWORD;
	mCollider2.mTag = CCollider::EPLAYERSWORD;
	mColSphereFoot.mTag = CCollider::EPLAYERBODY;
	//this���v���C���[���̂���
	mpPlayerInstance = this;
	mImageGauge.Load(IMAGE_GAUGE);

	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSphereSword.mpMatrix = &mpCombinedMatrix[22];
	//�����s��̐ݒ�
	mColSphereFoot.mpMatrix = &mpCombinedMatrix[2];
	mGravity = 0.20f;
	mState = EIDLE;
	mRotation.mY += 180.0f;
	mCollider2.mRenderEnabled = false;
}

void CXPlayer::Update()
{
	//�������s�����Ƃɕ���
	switch (mState) {
	case EIDLE:	//�ҋ@
		ChangeAnimation(0, true, 60);
		mSpeed = 0;
		mGravity = 0;
		break;
	case EMOVE://�ړ�
		mAnimationFrameSize = 60;
		mAttackHit = false;
			 if (CKey::Push('C')) {
				 mAnimationFrameSize = 30;
					 mState = EDUSH;
			 }
			 else {
             ChangeAnimation(1, true, 60);
			 }
			if (mAnimationCount <= 0) {
				mState = EIDLE;
		    }
		break;
	case EDUSH:

		mAttackHit = false;
			ChangeAnimation(1, true, 30);
			if (mAnimationCount <= 0) {
				mState = EIDLE;

			}
		break;
	case EESCAPE:

		mAttackHit = false;
		ChangeAnimation(1, true, 10);
		if (mRotation.mX!=360.0f) {
			mRotation.mX += 36.0f;
		}
		if(mAnimationFrame >= mAnimationFrameSize){
         mState = EMOVE;
		 mRotation.mX = 0.0f;
		}
		break;
	case EATTACK1://�U��
		if (mAttackCount>ATTACKCOUNT1/2) {
		ChangeAnimation(3, false, 20);//+4�Ԗڂ̃A�j���[�V�����̃t���[���R�O
		}
		break;
	case EATTACK2://�U��
		if (mAttackCount>ATTACKCOUNT2/2) {
		
		ChangeAnimation(5, false, 20);//+�U�Ԗڂ̃A�j���[�V�����̃t���[���R�O
		}
		break;
	case EATTACK3://�U��
		if (mAttackCount>ATTACKCOUNT3/2) {
		
		ChangeAnimation(7, false, 30);//+�W�Ԗڂ̃A�j���[�V�����̃t���[���R�O
		
		}
		break;
	case EATTACKSP://�U��
		if (mAttackCount>0) {
		ChangeAnimation(7, false, 100);//�V�Ԗڂ̃A�j���[�V�����T�O�t���[����
		
		}
		break;
	case EDAMAGED://�_���[�W

		if (CSceneGame::mVoiceSwitch == true) {
			PlayerDamage.Play();
		}
		ChangeAnimation(4, false, 10);
		break;
	case EDEATH://���S
		ChangeAnimation(11, false, 60);
		break;
	}
	//�A�j���[�V�����̎��
	switch (mAnimationIndex) {
	case(3): 
		if (mAnimationFrame == 15) {

			//�G�t�F�N�g����
			
			mEffect1=new CEffect(CVector(mPosition.mX,mPosition.mY+1.0f,mPosition.mZ-5.0f), 3.0f, 3.0f, CEffect::EFF_ATTACK, 2, 5, 3);
			mAttackHit = true;
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mAttackHit = false;
			ChangeAnimation(4, false, 30);
		}
		break;
	case(4):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mState = EIDLE;
		}
		break;
	case(5):
		if (mAnimationFrame == 15) {

			mEffect2 = new CEffect(CVector(mPosition.mX, mPosition.mY + 1.0f, mPosition.mZ - 5.0f), 3.0f, 3.0f, CEffect::EFF_ATTACK2, 3, 5, 3);
			mAttackHit = true;
			
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mAttackHit = false;
			ChangeAnimation(6, false, 30);
		}
		break;
	case(6):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mState = EIDLE;
		}
		break;
	case(7):
		if (mJump >= -0.1f) {
			mJump -= G;
		}
		if (mAnimationFrame == 15) {
			if (mState == EATTACK3) {

		    	mEffect3 = new CEffect(CVector(mPosition.mX, mPosition.mY + 1.0f, mPosition.mZ + 1.0f), 3.0f, 3.0f, CEffect::EFF_ATTACK3, 3, 5, 3);
			}
			mAttackHit = true;
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(8, false, 100);
		}
		break;
	case(8):
		if (mState == EATTACKSP) {
			if (mJump >= -3.0f) {

				mEffectSp = new CEffect(CVector(mPosition.mX, mPosition.mY + 1.0f, mPosition.mZ - 5.0f), 3.0f, 3.0f, CEffect::EFF_ATTACKSP, 4, 5, 3);
				mJump -= G2;

			  mCollider2.mRenderEnabled = true;
		
		    }
	    }
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mAttackHit = false;
			mCollider2.mRenderEnabled = false;
			mState = EIDLE;
		}
		break;
	case(9):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(10, false, 30);
		}
		break;
	case(10):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mState = EIDLE;
		}
		break;
	}

	if (mHp > HP_MAX)mHp = HP_MAX;

		//�J�������_�ړ��@�ʏ̖��o�ړ�
		//�J�����̍��E�ƑO��̃x�N�g�����擾
		CVector SideVec = Camera.GetMat().GetXVec();
		CVector FrontVec = Camera.GetMat().GetZVec();
		//�����ړ��̓J�b�g����
		SideVec.mY = 0.0f;
		FrontVec.mY = 0.0f;
		//���K������
		SideVec.Normalize();
		FrontVec.Normalize();
		float speed = 0.15f;
		CVector Move(0, 0, 0);
		//��
		if (CKey::Push('A'))
		{
				Move -= SideVec;
				mAnimationCount = 5;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�
				if (CKey::Push('C')) {
					if (mStamina > -1) {
						speed = 0.30f;//�X�s�[�h�{
						mStamina -= 2;//�X�^�~�i����
					}
					else {
						speed = 0.05f;//�X�s�[�h1/2
					}
				}
				
			if (CKey::Push(' ')) {
				speed= mStep;//�U�����A�i�s�����ɃX�e�b�v�𓥂�
			}
		}
		//�E
		else if (CKey::Push('D'))
		{
			Move += SideVec;
			mAnimationCount = 5;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�
            if (CKey::Push('C')) {
		    	if (mStamina > -1) {
					speed = 0.30f;//�X�s�[�h�{
					  mStamina-=2;//�X�^�~�i����	
				}

				else {
					speed = 0.05f;//�X�s�[�h1/2
				}
			}
			if (CKey::Push(' ')) {
				speed=mStep;//�U�����A�i�s�����ɃX�e�b�v�𓥂�
			}
		}
		//�O
		 if (CKey::Push('W'))
		{
			Move += FrontVec;
			//mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
			mAnimationCount = 5;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�

				if (CKey::Push('C')) {
					if(mStamina > -1) {
					speed = 0.30f;//�X�s�[�h�{
					mStamina-=2;//�X�^�~�i����
				
					}

					else {
						speed = 0.05f;//�X�s�[�h1/2
					}
				}
            if (CKey::Push(' ')) {
					speed= mStep;//�U�����A�i�s�����ɃX�e�b�v�𓥂�
		    } 
			
		}
		 //���
		else if (CKey::Push('S'))
		{
			Move -= FrontVec;
			//mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
			mAnimationCount = 5;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�

				if (CKey::Push('C')) {
			      if (mStamina > -1) {

					speed = 0.30f;//�X�s�[�h�{
					mStamina-=2;//�X�^�~�i����
				  }
				  else {
					speed = 0.05f;//�X�s�[�h1/2
			      }
			    }
			
			if (CKey::Push(' ')) {
				speed = mStep;//�U�����i�s�����ɃX�e�b�v�𓥂�
			}
		}
		 //���ڂ̍U��
			 //���ڂ̍U���̃t���[�������O���R��ڂ̍U���̑��t���[�������O
			 //�P���Q���R���P   �U���̏��Ԃ����[�v
		 //���G���Ԓ�����Ȃ��Ƃ�
		 //if (mDamageCount <= 0) {
			 //�U���P���g����Ƃ�
			 if (mSpaceCount1 == true) {

				 if (mAttackCount <= 0) {
					 if (CKey::Once(' '))
					 {

						 if (CSceneGame::mVoiceSwitch == true) {
							 PlayerFirstAttack.Play();
						 }
						 mState = EATTACK1;
						 mSpaceCount1 = false;//�P��ڂ̍U���̃t���O
						 mSpaceCount2 =true;
						mAttackCount = ATTACKCOUNT1;//�����蔻�肪�K�p����鎞��
						 mAnimationCount = 50;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
						 mStep = STEP;
					 }
				 }
			 }

			 //2��ڂ̍U��
			 else if (mSpaceCount2 == true) {
				 if (mAttackCount <= 0) {
					 if (CKey::Once(' ')) {

						 if (CSceneGame::mVoiceSwitch == true) {
							 PlayerSecondAttack.Play();
						 }
						 mState = EATTACK2;
						 mSpaceCount2 = false;//�Q��ڂ̍U���̃t���O
						 mSpaceCount3 = true;
						 mAttackCount = ATTACKCOUNT2;//�����蔻�肪�K�p����鎞��
						 mAnimationCount = 50;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
						 mStep = STEP;
					 }
				 }
			 }
			 //�R��ڂ̍U��
			 else if (mSpaceCount3 == true) {
				 if (mAttackCount <= 0) {
					 if (CKey::Once(' ')) {

						 if (CSceneGame::mVoiceSwitch == true) {
							 PlayerThirdAttack.Play();
						 }
						 mState = EATTACK3;
						 mAnimationCount = 50;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
						 mSpaceCount3 = false;//�R��ڂ̍U���̃t���O
						 mSpaceCount1 = true;
						 mAttackCount = ATTACKCOUNT3;//�����蔻�肪�K�p����鎞��
						 mStep = STEP;//�W�����v�͂���
					 }
				 }
			 }
		
		 //�W�����v�U��
		 if (mSpAttack >= 30) {
			 if (CKey::Once('F')) {
			     if (mAttackCount <= 0) {

					 if (CSceneGame::mVoiceSwitch == true) {
						 PlayerJumpAttack.Play();
					 }
					 mState = EATTACKSP;
					 mJump = JUMP;//�W�����v�͂���
					  mSpAttack -= 30;//����U���̃Q�[�W����
					 mAnimationCount = 200;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
					 mAttackCount = 100;
				 }
			 }
		 }
		 //���S
		 if (mHp <= 0) {
			 mState = EDEATH;
		  }
		 //�ړ���
		  if (Move.Length() != 0.0f) {
			  //C�L�[�������Ɖ���s��
			  if (CKey::Once('C')){
				  if (mState !=EATTACKSP) {
					  mState = EESCAPE;
					  mAnimationCount = 30;
					  mDamageCount = 40;
					  mStep = STEP;//�W�����v�͂���

					  mStamina -= 20;
				  }
				  //�ҋ@���̂Ƃ���C�������Ă���ƃ_�b�V��
				  if (mState == EIDLE) {

					  mAnimationCount = 1;
					  mState = EDUSH;
				  }
			  }
			  //�����Ă��Ȃ�����C�L�[�������Ă��Ȃ���Αҋ@
                  else {
					  if (mState == EIDLE) {
						  mAnimationCount = 1;
						  mState = EMOVE;
					  }
				  } 
		  }
		//�ړ��ʐ��K���@��������Ȃ��Ǝ΂߈ړ��������Ȃ��Ă��܂��̂Œ���
		//�W�����v���Ȃǂ�Y���𐳋K�����Ȃ��悤����
		Move.Normalize();
		//���s�ړ���
		if (mSpeed < speed) {
			mSpeed += 0.01f;
		}
		else {
			mSpeed -= 0.01f;
		}
         Move = Move*mSpeed;
		//3�����x�N�g���v�Z�ŎZ�o�����ق������m�����v�Z�ʂ����O����ꍇ�͋[���v�Z�Ōy�ʉ�
		//�[���x�N�g���v�Z
		Check tCheck = CUtil::GetCheck2D(Move.mX,Move.mZ,0,0, mRotation.mY*(M_PI/180.0f));
		//��]���x�@degree�ɒ���
		float turnspeed = (180.0f / M_PI)*0.5f;
		//�}�ȐU��Ԃ��}��
		if (tCheck.turn > 1.5f) tCheck.turn =1.5f;
		//�ړ������փL��������������
		if (tCheck.cross > 0.0f) {
			mRotation.mY += tCheck.turn * turnspeed;
		}
		if (tCheck.cross < 0.0f) {
			mRotation.mY -= tCheck.turn * turnspeed;
		}
		//�ړ�
		if (mState == EMOVE||mState==EDUSH||mStep > 0) {
		
             mPosition += Move;
		}
		//����s�����̈ړ���
		 if (mStep > 0) {
				mStep--;
		 }

		 if (mAnimationCount > 0) {
			 mAnimationCount--;
	     }
		 //�A�j���[�V�������I��邽�тɍU�����[�V�����ŏ�����
		 if (mAnimationCount <= 0) {
			 mSpaceCount1 = true;
			 mSpaceCount2 = false;
			 mSpaceCount3 = false;
		 }
		 //�X�^�~�i��
		 if (mStamina < STAMINA_MAX) {
			mStamina++;
		 }
		 //�U���A�j���[�V������
		 if (mAttackCount > 0) {
			 mAttackCount--;
		 }
		 //���G����
		 if (mDamageCount > 0) {
			 mDamageCount--;
		 }
		
		 

		 //�}�b�v�ɐڐG���Ă��Ȃ��Ԃ����Əd�͂�������
		 if (mState != EATTACKSP) {
			 if (mJump >= -0.1) {

			   mJump -= G;
			 }
		 }
		 if (mState != EESCAPE) {
			  mPosition.mY += mJump;
		 }
		
		//�A�C�e���擾���ɕ���̓����蔻��g��
		 if (CItem::mItemCount > 0) {
			mColSphereSword.mRadius = 7.5f;
		 }
		 //�������
		 if (mColliderCount > 0) {
			mColliderCount--;
			mPosition = mPosition + mCollisionEnemy * mColliderCount;
			
		 }
	    //�����_�ݒ�
	    Camera.SetTarget(mPosition);
	    CXCharacter::Update();
}

void CXPlayer::Collision(CCollider* m, CCollider* o) {

	//���g�̃R���C�_�̐ݒ�
	switch (m->mType) {



	case CCollider::ESPHERE:

		//�e���v���C���[
		if (m->mpParent->mTag == EPLAYER) {
			//�v���C���[�̑̕���
			if (m->mTag == CCollider::EPLAYERBODY) {//����̃R���C�_���O�p�R���C�_�̏ꍇ
				//�e���O�p�R���C�_
				if (o->mType == CCollider::ETRIANGLE) {
					//�e���O�p�R���C�_

		            //�������iX���j�̃x�N�g�������߂�
					CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
					//������iY���j�̃x�N�g�������߂�
					CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
					//�O�����iZ���j�̃x�N�g�������߂�
					CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;
					vz.Normalize();

						CVector adjust;//�����p�x�N�g��
						if (CCollider::CollisionTriangleSphere(o, m, &adjust)) {
							

								if (mState != EESCAPE) {
									/*
									//X�x�N�g�����v���C���[�̑O�����x�N�g����CColliderMesh�̖@���̊O��
									CVector mVectorX=vz.Cross(�}�`�̖@��);
									//Z�x�N�g����X�x�N�g����CColliderMesh�̖@���̊O��
									CVector mVectorZ = mVectorX.Cross(�}�`�̖@��);
									//Y�x�N�g����CColliderMesh�̖@���Ɠ���
									CVector mVectorY = �}�`�̖@��;
									//X���̉�]�l�̌v�Z
									//Z����Y���W�ŃA�[�N�T�C�����Ƃ��Ċp�x�����߂�
									//�A�[�N�T�C���i
									asin();
									//���߂��p�x�����W�A���Ɏ���
									�x��=�p�x*(180.0f / M_PI);
									*/

									//Y���̉�]�l�̌v�Z
									//Z�x�N�g����X���W��Z���W�Ń^���W�F���g�����
									//�^���W�F���g�i����/��Ӂj
									mGravity = 0;
									mJump = 0;
									//�ʒu�̍X�V�imPosition+adjust)
									mPosition = mPosition + adjust;
									//�s��̍X�V
									CTransform::Update();
								}
							

						}

				}
				//���R���C�_
				if (o->mType == CCollider::ESPHERE) {
					//�e���G�i�Q�j
					if (o->mpParent->mTag == EENEMY2) {
						//�G�̍U�����ʂƂ̏Փ˔���
						if (o->mTag == CCollider::EENEMY2COLLIDERATTACK) {

							CVector adjust;//�����p�x�N�g��
							if (CCollider::Collision(m, o)) {
								//�_���[�W�����������Ƃ̖��G���Ԃ��O�̂Ƃ�
								if (mDamageCount == 0) {
									//�G�̍U�����肪�K�p����Ă����
									if (((CEnemy2*)(o->mpParent))->mEnemy2AttackHit == true)
									{
										if (mHp > 0) {
											//���ɉ�����
											mColliderCount = 5.0f;
											mCollisionEnemy = mPosition - o->mpParent->mPosition;
											mCollisionEnemy.mY = 0;
											mCollisionEnemy = mCollisionEnemy.Normalize();
											//�̗͌���
											mHp--;
											//���G���ԕt�^
											mDamageCount = 30;
											//�_���[�W���̏������J�n
											mState = EDAMAGED;
										}
									}
								}
							}
						}
					}
					//�e���{�X
					else if (o->mpParent->mTag == EBOSS) {

						//�{�X�̑̂Ƃ̏Փ˔���
						if (o->mTag == CCollider::EBOSSCOLLIDERHEAD) {
							if (CCollider::Collision(m, o)) {
								//���ɉ�����
								mColliderCount = 1.5f;
								mCollisionEnemy = mPosition - o->mpParent->mPosition;
								mCollisionEnemy.mY = 0;
								mCollisionEnemy = mCollisionEnemy.Normalize();
							}
						}
						//�{�X�̍U�����ʂƂ̏Փ˔���
						else if (o->mTag == CCollider::EBOSSCOLLIDERATTACK) {
							if (CCollider::Collision(m, o)) {
								//�_���[�W�����������Ƃ̖��G����
								if (mDamageCount == 0) {
									//�G�̍U�����肪�K�p����Ă����
									if (((CBoss*)(o->mpParent))->mBossAttackHit == true)
									{

										if (mHp > 0) {
											//���ɉ�����
											mColliderCount = 5.0f;
											mCollisionEnemy = mPosition - o->mpParent->mPosition;
											mCollisionEnemy.mY = 0;
											mCollisionEnemy = mCollisionEnemy.Normalize();
											//�̗͌���
											mHp--;
											//���G���ԕt�^
											mDamageCount = 30;
											//�_���[�W���̏����J�n
											mState = EDAMAGED;
										}
									}

								}
							}
						}
					}
				}


			}
			
		}
	}
		
	
}


void CXPlayer::TaskCollision() {
	//�R���C�_�̗D��x�ύX
	mColSphereSword.ChangePriority();
	mColSphereFoot.ChangePriority();
	mCollider2.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mCollider2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereFoot, COLLISIONRANGEFIELD);
	CCollisionManager::Get()->Collision(&mColSphereSword, COLLISIONRANGE);
}
void CXPlayer::Render2D()
{
	//2D�`��J�n
	CUtil::Start2D(0, 800, 0, 600);

	//�̗͂̊���
	float hpRate = (float)mHp / (float)HP_MAX;
	//�̗̓Q�[�W�̕�
	float hpGaugeWid = GAUGE_WID_MAX * hpRate;

	//�X�^�~�i�̊���
	float staminaRate = (float)mStamina / (float)STAMINA_MAX;
	//�X�^�~�i�Q�[�W�̕�
	float staminaGaugeWid = GAUGE_WID_MAX * staminaRate;

	mImageGauge.Draw(20, GAUGE_WID_MAX, 560, 590, 210, 290, 63, 0);	//�Q�[�W�w�i
	mImageGauge.Draw(20, hpGaugeWid, 560, 590, 0, 0, 0, 0);			//�̗̓Q�[�W

	mImageGauge.Draw(20, GAUGE_WID_MAX, 520, 550, 210, 290, 63, 0);	//�Q�[�W�w�i
	mImageGauge.Draw(20, staminaGaugeWid, 520, 550, 110, 190, 63, 0);	//�X�^�~�i�Q�[�W


	CUtil::End2D();



}

