#include "CXPlayer.h"
#include "CKey.h"
#include "CCamera.h"
#include "CUtil.h"
#include"CBullet.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define JUMP 9.0f
#define JUMP2 10.0f
#define STEP  10.0f
#define STEP2 20.0f
#define G 0.1f
#define G2 1.0f
#include"CItem.h"
int CXPlayer::mSpAttack = 0;
int CXPlayer::mStamina = 0;
int CXPlayer::mAttackCount = 0;
CXPlayer::CXPlayer()

	: mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 5.7f)

	, mColSphereFoot(this, nullptr, CVector(0.0f, -10.0f, -3.0f), 0.5f)
	,mCollider(this, &mMatrix, CVector(0.0f, 0.0f, -0.0f), 5.0f)
	, mCollider2(this, &mMatrix, CVector(0.0f, -2.0f, 0.0f), 4.0f)
	,mLine(this, &mMatrix, CVector(0.0f, 5.0f, 0.0f), CVector(0.0f, -3.0f, 0.0f))  //�㉺�̐����R���C�_
	, mJump(0.0f)
	, mHp(10)
	, mGravity(1.1f)
	,mSpaceCount1(0)
	,mSpaceCount2(0)
	,mSpaceCount3(0)
	, mDamageCount(0)
	,mAnimationCount(0)
	,mColliderCount(1.0f)
	,mTime(0.0f)
	,mSpeed(0.0f)
{
	//�^�O�Ƀv���C���[��ݒ肵�܂�
	mTag = EPLAYER;
	mColSphereSword.mTag = CCollider::ESWORD;
	mCollider2.mTag = CCollider::ESWORD;
	mColSphereFoot.mTag = CCollider::EBODY;
	mColSphereBody.mTag = CCollider::EBODY;

	mLine.mType = CCollider::ELINE;
	CXPlayer::mStamina = 1000;
	mCollider.mTag = CCollider::ESTOPPER;

}

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	mColSphereBody.mpMatrix = &mpCombinedMatrix[9];
	//��
	mColSphereHead.mpMatrix = &mpCombinedMatrix[12];
	//��
	mColSphereSword.mpMatrix = &mpCombinedMatrix[22];

	//�����s��̐ݒ�
	mColSphereFoot.mpMatrix = &mpCombinedMatrix[1];
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
		break;
	case EMOVE://�ړ�
			 if (CKey::Push('C')) {
					// mAnimationCount = 1;
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
			ChangeAnimation(1, true, 30);
			if (mAnimationCount <= 0) {
				mState = EIDLE;

			}
		break;
	case EESCAPE:
		ChangeAnimation(1, true, 10);
		//�U�����A�i�s�����ɃX�e�b�v�𓥂�
		mStep = STEP2;
		if (mRotation.mX!=360.0f) {
			mRotation.mX += 36.0f;
		}
		if(mAnimationFrame >= mAnimationFrameSize){
         mState = EMOVE;
		 mRotation.mX = 0.0f;
		}
		break;
	case EATTACK1://�U��
		if (mAttackCount>0) {
		ChangeAnimation(3, false, 20);//+4�Ԗڂ̃A�j���[�V�����̃t���[���R�O
		}
		break;
	case EATTACK2://�U��
		if (mAttackCount>0) {
		ChangeAnimation(5, false, 20);//+�U�Ԗڂ̃A�j���[�V�����̃t���[���R�O
		}
		break;
	case EATTACK3://�U��
		if (mAttackCount>0) {
		ChangeAnimation(7, false, 30);//+�W�Ԗڂ̃A�j���[�V�����̃t���[���R�O
		}
		break;
	case EATTACKSP://�U��
		if (mAttackCount>0) {
		ChangeAnimation(7, false, 50);//�V�Ԗڂ̃A�j���[�V�����T�O�t���[����
		}
		
		if (mAnimationCount <= 0) {

			mState = EIDLE;
		}
		break;
	case EDAMAGED://�_���[�W
		ChangeAnimation(4, false, 10);
		break;
	case EDEATH://���S
		ChangeAnimation(11, false, 60);
		break;
	}
	switch (mAnimationIndex) {
	case(3): 
		if (mAnimationFrame >= mAnimationFrameSize)
		{
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
		if (mAnimationFrame >= mAnimationFrameSize)
		{
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
		
		if (mAnimationFrame >= mAnimationFrameSize)
		{
		
			ChangeAnimation(8, false, 30);
		}
		break;
	case(8):

		mCollider2.mRenderEnabled = true;
		
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			
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
				mAnimationCount = 10;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�
				if (mStamina > 0) {
					if (CKey::Push('C')) {
						speed = 0.30f;//�X�s�[�h�{
						mStamina-=2;//�X�^�~�i����
						
					}
				}
				else {
					speed = 0.05f;//�X�s�[�h1/2
				}
			if (CKey::Push(' ')) {
				speed= mStep;//�U�����A�i�s�����ɃX�e�b�v�𓥂�
			}
		}
		//�E
		else if (CKey::Push('D'))
		{
			Move += SideVec;
			mAnimationCount = 10;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�
			if (mStamina > 0) {
				if (CKey::Push('C')) {
					speed = 0.30f;//�X�s�[�h�{
					mStamina-=2;//�X�^�~�i����	
				}
			}
			else {
				speed = 0.05f;//�X�s�[�h1/2
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
			mAnimationCount = 10;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�
			if(mStamina > 0) {
				if (CKey::Push('C')) {
					speed = 0.30f;//�X�s�[�h�{
					mStamina-=2;//�X�^�~�i����
				
				}
			}
			else {
				speed = 0.05f;//�X�s�[�h1/2
			}
            if (CKey::Push(' ')) {
					speed= mStep;//�U�����A�i�s�����ɃX�e�b�v�𓥂�
		    } 
			/*
			CBullet* bullet = new CBullet();
			bullet->Set(0.1f, 1.5f);
			bullet->mPosition = CVector(0.0f, 0.0f, 10.0f) * mMatrix;
			bullet->mRotation = mRotation;
			bullet->Update();*/
		}
		 //���
		else if (CKey::Push('S'))
		{
			Move -= FrontVec;
			//mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
			mAnimationCount = 10;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�
			if (mStamina > 0) {
				if (CKey::Push('C')) {
					speed = 0.30f;//�X�s�[�h�{
					mStamina-=2;//�X�^�~�i����
				}
			}
			else {
				speed = 0.05f;//�X�s�[�h1/2
			}
			if (CKey::Push(' ')) {
				speed = mStep;//�U�����i�s�����ɃX�e�b�v�𓥂�
			}
		}
		 //���ڂ̍U��
			 //���ڂ̍U���̃t���[�������O���R��ڂ̍U���̑��t���[�������O
			 //�P���Q���R���P   �U���̏��Ԃ����[�v
		 if (mDamageCount <= 0) {

			 if (mSpaceCount1 == 0) {
				 if (mAttackCount <= 0) {
					 if (CKey::Once(' '))
					 {
						 mState = EATTACK1;
						 mSpaceCount1 = 1;//�P��ڂ̍U���̃t���O
						 mSpaceCount2 = 0;
						 mAttackCount = 20;//�����蔻�肪�K�p����鎞��
						 mAnimationCount = 50;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
						 mStep = STEP;
					 }
				 }
			 }

			 //2��ڂ̍U��
			 else if (mSpaceCount2 == 0) {
				 if (mAttackCount <= 0) {
					 if (CKey::Once(' ')) {
						 mState = EATTACK2;
						 mSpaceCount2 = 1;//�Q��ڂ̍U���̃t���O
						 mSpaceCount3 = 0;
						 mAttackCount = 20;//�����蔻�肪�K�p����鎞��
						 mAnimationCount = 50;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
						 mStep = STEP;
					 }
				 }
			 }
			 //�R��ڂ̍U��
			 else if (mSpaceCount3 == 0) {
				 if (mAttackCount <= 0) {
					 if (CKey::Once(' ')) {
						 mState = EATTACK3;
						 mAnimationCount = 50;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
						 mSpaceCount3 = 1;//�R��ڂ̍U���̃t���O
						 mSpaceCount1 = 0;
						 mAttackCount = 30;//�����蔻�肪�K�p����鎞��
						 mStep = STEP;//�W�����v�͂���
					 }
				 }
			 }
		 }
		 //�W�����v�U��
		 if (mSpAttack >= 0) {
			 if (CKey::Once('F')) {
				 if (mAttackCount <= 0) {
					 mState = EATTACKSP;
					 mJump = JUMP;//�W�����v�͂���
					 // mSpAttack -= 30;//����U���̃Q�[�W����
					 mAnimationCount = 100;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
					 mAttackCount = 100;
					 //mPosition.mY = 1.0f;// mJump* mTime - 0.5 * mGravity * mTime * mTime;
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
					  mAnimationCount = 20;
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
		//���ʂ�3�����x�N�g���v�Z�ŎZ�o�����ق������m�����v�Z�ʂ����O����ꍇ�͋[���v�Z�Ōy�ʉ�
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
		//���W�ړ�
		if (mState == EMOVE||mState==EDUSH||mStep > 0) {
		
             mPosition += Move;
		}
		 if (mStep > 0) {
				mStep--;
		 }
		 if (mAnimationCount > 0) {
			 mAnimationCount--;
	     }
		 if (mAnimationCount <= 0) {
			 mSpaceCount1 = 0;
			 mSpaceCount2 = 0;
			 mSpaceCount3 = 0;
		 }
		 if (mStamina < 1000) {
			mStamina++;
		 }
		 if (mAttackCount > 0) {
			 mAttackCount--;
		 }
		 if (mDamageCount > 0) {
			 mDamageCount--;
		 }
		
		 if (mState == EATTACKSP) {

			 //�}�b�v�ɐڐG���Ă��Ȃ��Ԃ����Əd�͂�������
			 mPosition.mY = mJump * mTime - 0.5 * mGravity * mTime * mTime;
			 if (mTime >= 3.0) {
					 mTime += 1.5f;
			 }
			 else {
						 mTime += 0.2f;
			 }
			 
		 }
		 else if (mState != EATTACKSP) {
			 mTime = 0.0f;
			

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
				if (m->mTag == CCollider::EBODY) {//����̃R���C�_���O�p�R���C�_�̏ꍇ
					//�e���O�p�R���C�_
					if (o->mType == CCollider::ETRIANGLE) {
						
								CVector adjust;//�����p�x�N�g��
							if (CCollider::CollisionTriangleSphere(o,m,&adjust)) {

								//�O�p�`�Ɛ����̏Փ˔���
								//CCollider::CollisionTriangleLine(o, m, &adjust);
								//�ʒu�̍X�V�imPosition+adjust)
								mPosition = mPosition - adjust * -1;
								
								//�s��̍X�V
								CTransform::Update();
								
							}
							else {

								//�}�b�v�ɐڐG���Ă��Ȃ��Ԃ����Əd�͂�������
								//mPosition.mY = mJump * mTime - 0.5 * mGravity * mTime * mTime;
								
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
										if (CEnemy2::mEnemy2AttackCount > 0) {
											if (mHp > 0) {
												//���ɉ�����
												mColliderCount = 5.0f;
												mCollisionEnemy = mPosition - o->mpParent->mPosition;
												mCollisionEnemy.mY = 0;
												mCollisionEnemy = mCollisionEnemy.Normalize();
												//�̗͌���
												mHp--;
												//���G���ԕt�^
												mDamageCount = 60;
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
							if (o->mTag == CCollider::EBOSSCOLLIDER) {
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
										if (CBoss::mBossAttackCount > 0) {
											if (mHp > 0) {
												//���ɉ�����
												mColliderCount = 5.0f;
												mCollisionEnemy = mPosition - o->mpParent->mPosition;
												mCollisionEnemy.mY = 0;
												mCollisionEnemy = mCollisionEnemy.Normalize();
												//�̗͌���
												mHp--;
												//���G���ԕt�^
												mDamageCount = 60;
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
				
				 //�v���C���[�̌�
				 else if (m->mTag == CCollider::ESWORD) {
					//���R���C�_
					if (o->mType == CCollider::ESPHERE) {
						//�G�i�Q�j
						if (o->mpParent->mTag == EENEMY2) {
							//�G�̃R���C�_
							if (o->mTag == CCollider::EENEMY2COLLIDER) {
								if (CCollider::Collision(m, o)) {
									if (mAttackCount > 0) {
										//����U���̃Q�[�W����
										mSpAttack += 2;
										break;
									}
								}
							}
						}
					}
				 }

			}
			break;
	case CCollider::ELINE://�����R���C�_
			//����̃R���C�_���O�p�R���C�_�̏ꍇ
			if (o->mType == CCollider::ETRIANGLE) {
				CVector adjust;//�����p�x�N�g��
				//�O�p�`�Ɛ����̏Փ˔���
				CCollider::CollisionTriangleLine(o, m, &adjust);
				//�ʒu�̍X�V�imPosition+adjust)
				mPosition = mPosition - adjust * -1;
				//�s��̍X�V
				CTransform::Update();

			}
			break;
	
	}
}

void CXPlayer::TaskCollision() {
	mLine.ChangePriority();
	//	�Փˏ��������s
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
}

	


