#include "CXPlayer.h"
#include "CKey.h"
#include "CCamera.h"
#include "CUtil.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define JUMP 3.0f
#define JUMP2 10.0f
#define STEP 10.0f
#include"CItem.h"
int CXPlayer::mSpAttack = 0;
int CXPlayer::mStamina = 0;
int CXPlayer::mAttackCount = 0;
CXPlayer::CXPlayer()

	: mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 1.7f)

	, mColSphereFoot(this, nullptr, CVector(0.0f, -10.0f, -3.0f), 1.0f)
	, mCollider2(this, &mMatrix, CVector(0.0f, -5.0f, 0.0f), 5.0f)
	, mJump(0.0f)
	, mHp(10)
	, mGravity(0.0f)
	,mSpaceCount1(0)
	,mSpaceCount2(0)
	,mSpaceCount3(0)
	, mDamageCount(0)
	,mAnimationCount(0)
	,mStep(0.0f)
	,mTime(0.0f)
	,mV0(0)
{
	//�^�O�Ƀv���C���[��ݒ肵�܂�
	mTag = EPLAYER;
	mColSphereSword.mTag = CCollider::ESWORD;
	mCollider2.mTag = CCollider::ESWORD;
	mColSphereFoot.mTag = CCollider::EBODY;
	CXPlayer::mStamina = 400;
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
	mRotation.mY = 0.01f;
	mGravity = 0.20f;
	
	
}

void CXPlayer::Update()
{
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
			ChangeAnimation(0, true, 60);
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
			ChangeAnimation(0, true, 60);
		}
		break;
	case(7):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(8, false, 30);
		}
		break;
	case(8):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(0, true, 60);
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
			ChangeAnimation(0, true, 60);
		}
		break;
	}
	
	
	
	
		
		//��]�ړ� �ʏ̃o�C�I�ړ�
		//if (CKey::Push('A'))
		//{
		//	mRotation.mY += 2.0f;
		//}
		//if (CKey::Push('D'))
		//{
		//	mRotation.mY -= 2.0f;
		//}
		//if (CKey::Push(' '))
		//{
		//	ChangeAnimation(3, true, 30);
		//}
		//else if (CKey::Push('W'))
		//{
		//	ChangeAnimation(1, true, 60);
		//	mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
		//}
		//else {
		//	ChangeAnimation(0, true, 60);
		//}

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


		if (CKey::Push('A'))
		{
			//			mRotation.mY += 2.0f;
			Move -= SideVec;
			mAnimationCount = 10;
			
			if (mStamina > 0) {
				if (CKey::Push('C')) {
					speed = 0.30f;
					mStamina-=2;
					ChangeAnimation(1, true, 30);
				}
				
			}
			else {
				speed = 0.05f;
			}
		}
		else if (CKey::Push('D'))
		{
			//mRotation.mY -= 2.0f;
			Move += SideVec;
			mAnimationCount = 10;
			
			if (mStamina > 0) {
				if (CKey::Push('C')) {
					speed = 0.30f;
					mStamina-=2;
					ChangeAnimation(1, true, 30);
				}
				
			}
			else {
				speed = 0.05f;
			}
		}
		 if (CKey::Push('W'))
		{
			Move += FrontVec;
			//			mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
			mAnimationCount = 10;
			if(mStamina > 0) {
				if (CKey::Push('C')) {
					speed = 0.30f;
					mStamina-=2;
					ChangeAnimation(1, true, 30);
				}
				
			}
			else {
				speed = 0.05f;
			}
		}
		else if (CKey::Push('S'))
		{
			Move -= FrontVec;
			
			//			mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
			mAnimationCount = 10;
			if (mStamina > 0) {
				if (CKey::Push('C')) {
					speed = 0.30f;
					mStamina-=2;
					
					ChangeAnimation(1, true, 30);
				}
			}
			else {
				speed = 0.05f;
			}
		}
		 if (mSpaceCount1 <=0&& mSpaceCount3 <=0) {
		     if (CKey::Once(' '))
		     {
				ChangeAnimation(3,false, 10);
				mSpaceCount1 = 20;//�P��߂̍U���̑��t���[��
				mAttackCount = 20;//�����蔻�肪�K�p����鎞��
				mAnimationCount = 10;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�

                mStep = STEP;
				//mPosition.mX -= mStep;
				
		     }
		 }
		
		else if (mSpaceCount2 <= 0&& mSpaceCount1>1) {
			 if (mSpaceCount1 < 15) {
				if (CKey::Once(' ')) {
				ChangeAnimation(5, true, 10);
				mSpaceCount2 = 20;//�Q��߂̍U���̑��t���[��
				mSpaceCount1 = 0;
				mAttackCount = 20;//�����蔻�肪�K�p����鎞��
				mAnimationCount = 10;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
				mStep = STEP;
				//mPosition.mX -= mStep;
		        }
			 }
			
		}
		else if (mSpaceCount3<=0&&mSpaceCount2 > 1) {
			 if (mSpaceCount2 < 15) {
				 if (CKey::Push(' ')) {
					 ChangeAnimation(7, true, 10);
					 mAnimationCount = 10;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
					 mSpaceCount3 = 20;//�R��߂̍U���̑��t���[��
					 mSpaceCount1 = 0;
					 mAttackCount = 20;//�����蔻�肪�K�p����鎞��
					 mStep = STEP;
					// mPosition.mX -= mStep;
				 }
			 }
		}
		 if (mSpAttack >= 0) {
			 if (CKey::Once('F')) {
				 mJump = JUMP;
				
				// mSpAttack -= 30;
				 mAnimationCount = 80;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
				 ChangeAnimation(7, true, 50);
				 mAttackCount = 80;//�����蔻�肪�K�p����鎞��
				 mTime = 1;
				 mPosition.mY = 1.0f;// mJump* mTime - 0.5 * mGravity * mTime * mTime;
			 }
		 }
			else if (mHp <= 0) {
				ChangeAnimation(11, false, 60);

			}
		  if (Move.Length() != 0.0f) {
			 if (mAttackCount <= 0) {
				 mAnimationCount = 1;
                 ChangeAnimation(1, true, 60);
				 if (CKey::Push('C')) {
					 ChangeAnimation(1, true, 30);
				 }
			 }
		  }
			else if(mAnimationCount=0) {
				ChangeAnimation(0, true, 60);
			}
		 
		
		//�ړ��ʐ��K���@��������Ȃ��Ǝ΂߈ړ��������Ȃ��Ă��܂��̂Œ���
		//�W�����v���Ȃǂ�Y���𐳋K�����Ȃ��悤����
		Move.Normalize();
		//���s�ړ���
		Move = Move*speed;
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
		mPosition += Move;



         if (mStep > 0) {
			 mStep--;
			 Move += FrontVec;
			 speed = 1.0f;
		 }

		 if (mAnimationCount > 0) {
			 mAnimationCount--;
	     }
		 if (mSpaceCount1 > 0) {
			 mSpaceCount1--;
		 }
		 if (mSpaceCount2 > 0) {
			 mSpaceCount2--;
		 }
		 if (mSpaceCount3 > 0) {
			 mSpaceCount3--;
		 }
		 if (mStamina < 400) {
			mStamina++;
		 }
		 if (mAttackCount > 0) {
			 mAttackCount--;
		 }
		 if (mDamageCount > 0) {
			 mDamageCount--;
		 }
		if (mPosition.mY > 0) {
			//mJump--;
			mCollider2.mRenderEnabled = true;
		}
		else {
			mPosition.mY = 0.0f;
			mCollider2.mRenderEnabled = false;
		}
		//�d��
		if (mPosition.mY>0) {
			mPosition.mY =mJump*mTime- 0.5 * mGravity * mTime * mTime;
		}
		mTime++;
		if (CItem::mItemCount > 0) {
			mColSphereSword.mRadius = 2.5f;
		}
		//�������
		if (mColliderCount > 0) {
			mColliderCount--;
			mPosition = mPosition + mCollisionEnemy * mColliderCount;
			ChangeAnimation(4, false, 10);
		}



	//�����_�ݒ�
	Camera.SetTarget(mPosition);
	CXCharacter::Update();
}

void CXPlayer::Collision(CCollider* m, CCollider* o) {

	//���g�̃R���C�_�̐ݒ�
	switch (m->mType) {
	case CCollider::ELINE://�����R���C�_
			//����̃R���C�_���O�p�R���C�_�̏ꍇ
		if (o->mType == CCollider::ETRIANGLE) {
			CVector adjust;//�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			CCollider::CollisionTriangleLine(o, m, &adjust);
			//�ʒu�̍X�V�imPosition+adjust)
			mPosition = mPosition - adjust * -1;
			//mPosition.mY = 0.0f;
			//�s��̍X�V
			CTransform::Update();
		}
	case CCollider::ESPHERE:

		if (o->mType == CCollider::ESPHERE) {
			if (m->mpParent->mTag == EPLAYER) {
				if (m->mTag == CCollider::EBODY) {
					if (o->mpParent->mTag == EENEMY2) {
						if (o->mTag == CCollider::EENEMY2COLLIDER) {
							//�Փ˂��Ă���Ƃ�
							CVector adjust;//�����p�x�N�g��
							if (CCollider::Collision(m, o)) {
								if (mDamageCount == 0) {
									if (mHp > 0) {
                                    mColliderCount = 5;
									mCollisionEnemy = mPosition - o->mpParent->mPosition;
									mCollisionEnemy.mY = 0;
									mCollisionEnemy = mCollisionEnemy.Normalize();
									mHp--;
									mDamageCount = 180;
									}	
								}
							}
						}
					}
				}
				 if (m->mTag == CCollider::ESWORD) {
					 if (o->mpParent->mTag == EENEMY2) {
						 if (o->mTag == CCollider::EENEMY2COLLIDER) {
							 if (CCollider::Collision(m, o)) {
								 if (mAttackCount > 0) {
									 mSpAttack+=5;
									 break;
								 }
							 }
						 }
					 }
				 }
			}
		}
	}
}

	


