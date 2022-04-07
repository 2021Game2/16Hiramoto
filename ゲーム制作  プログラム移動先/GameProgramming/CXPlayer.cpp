#include "CXPlayer.h"
#include "CKey.h"
#include "CCamera.h"
#include "CUtil.h"
#include"CBullet.h"

#include"CEnemy2.h"
#include"CBoss.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define JUMP 5.0f
#define JUMP2 10.0f
#define STEP  20.0f
#define STEP2 20.0f
#define STAMINA 1000 //�X�^�~�i

#define HP_MAX 100	//HP			//�̗͍ő�l
#define STAMINA_MAX 1000		//�X�^�~�i�ő�l
#define GAUGE_WID_MAX 400.0f	//�Q�[�W�̕��̍ő�l
#define GAUGE_LEFT 20			//�Q�[�W�`�掞�̍��[
#define IMAGE_GAUGE "Resource\\Gauge.png"		//�Q�[�W�摜
#define G 0.1f
#define G2 2.0f
#include"CItem.h"
#include"CSound.h"


int CXPlayer::mSpAttack = 0;
int CXPlayer::mStamina = STAMINA_MAX;
int CXPlayer::mAttackCount = 0;
int CXPlayer::mHp = HP_MAX;

extern CSound FirstAttack;
extern CSound SecondAttack;
extern CSound ThirdAttack;
extern CSound JumpAttack;
extern CSound Damage;

CXPlayer* CXPlayer::mpPlayerInstance;
//�v���C���[�̃|�C���^��Ԃ����ƂŁA���W�Ȃǂ��Q�Ƃł���悤�ɂȂ�
CXPlayer* CXPlayer::GetInstance()
{
	return mpPlayerInstance;
}
CXPlayer::CXPlayer()

	//: mColSphereBody(this, nullptr, CVector(0.0f,1.1f,0.0f), 0.5f)//�̂̃R���C�_
	//, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	: mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 4.5f)//���̃R���C�_�P
	, mColSphereFoot(this, nullptr, CVector(0.0f, 0.0f, -3.0f), 2.0f)//���t�߂̃R���C�_
	//,mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 5.0f)//�X�g�b�p�[
	, mCollider2(this, &mMatrix, CVector(0.0f, -2.0f, 0.0f), 4.0f)//���̃R���C�_�Q
	, mJump(0.0f)
	, mGravity(0.0f)
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
	
	mColSphereSword.mTag = CCollider::EPLAYERSWORD;
	mCollider2.mTag = CCollider::EPLAYERSWORD;
	mColSphereFoot.mTag = CCollider::EPLAYERBODY;
	//mColSphereBody.mTag = CCollider::EPLAYERBODY;
	//mColSphereHead.mTag = CCollider::EPLAYERBODY;
	//mCollider.mTag = CCollider::ESTOPPER;

	//this���v���C���[���̂���
	mpPlayerInstance = this;
	mImageGauge.Load(IMAGE_GAUGE);
	
}

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//mColSphereBody.mpMatrix = &mpCombinedMatrix[9];
	//��
	//mColSphereHead.mpMatrix = &mpCombinedMatrix[12];
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
			 if (CKey::Push('C')) {
					
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
		ChangeAnimation(7, false, 100);//�V�Ԗڂ̃A�j���[�V�����T�O�t���[����
		
		}
		break;
	case EDAMAGED://�_���[�W
		Damage.Play();
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
			mAttackHit = true;
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mAttackHit = false;
			ChangeAnimation(8, false, 100);

		}
		break;
	case(8):
		if (mState == EATTACKSP) {

			if (mJump >= -5.0f) {
				mJump -= G2;
			  mCollider2.mRenderEnabled = true;
		
		    }
	    }

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
				mAnimationCount = 10;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�
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
			mAnimationCount = 10;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�

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
			mAnimationCount = 10;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�

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
			 if (mSpaceCount1 == 0) {

				 if (mAttackCount <= 0) {
					 if (CKey::Once(' '))
					 {
						 FirstAttack.Play();
						 mState = EATTACK1;
						 mSpaceCount1 = 1;//�P��ڂ̍U���̃t���O
						 mSpaceCount2 = 0;
						mAttackCount = 10;//�����蔻�肪�K�p����鎞��
						 mAnimationCount = 50;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
						 mStep = STEP;
					 }
				 }
			 }

			 //2��ڂ̍U��
			 else if (mSpaceCount2 == 0) {
				 if (mAttackCount <= 0) {
					 if (CKey::Once(' ')) {
						 SecondAttack.Play();
						 mState = EATTACK2;
						 mSpaceCount2 = 1;//�Q��ڂ̍U���̃t���O
						 mSpaceCount3 = 0;
						 mAttackCount = 10;//�����蔻�肪�K�p����鎞��
						 mAnimationCount = 50;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
						 mStep = STEP;
					 }
				 }
			 }
			 //�R��ڂ̍U��
			 else if (mSpaceCount3 == 0) {
				 if (mAttackCount <= 0) {
					 if (CKey::Once(' ')) {
						 ThirdAttack.Play();
						 mState = EATTACK3;
						 mAnimationCount = 50;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
						 mSpaceCount3 = 1;//�R��ڂ̍U���̃t���O
						 mSpaceCount1 = 0;
						 mAttackCount = 30;//�����蔻�肪�K�p����鎞��
						 mStep = STEP;//�W�����v�͂���
					 }
				 }
			 }
		// }
		 //�W�����v�U��
		 if (mSpAttack >= 30) {
			 if (CKey::Once('F')) {
			     if (mAttackCount <= 0) {
					 JumpAttack.Play();
					 mState = EATTACKSP;
					 mJump = JUMP;//�W�����v�͂���
					  mSpAttack -= 30;//����U���̃Q�[�W����
					 mAnimationCount = 200;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
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
			 mSpaceCount1 = 0;
			 mSpaceCount2 = 0;
			 mSpaceCount3 = 0;
		 }
		 //�X�^�~�i��
		 if (mStamina < 1000) {
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
		// }
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
					if (o->mpParent->mTag == EMAP) {

						CVector adjust;//�����p�x�N�g��
						if (CCollider::CollisionTriangleSphere(o, m, &adjust)) {

							if (mState != EESCAPE) {
								mGravity = 0;
								mJump = 0;
								//�O�p�`�Ɛ����̏Փ˔���
								//CCollider::CollisionTriangleLine(o, m, &adjust);
								//�ʒu�̍X�V�imPosition+adjust)
								mPosition = mPosition + adjust;

								//�s��̍X�V
								CTransform::Update();
							}

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
					//�v���C���[�̌�
					else if (m->mTag == CCollider::EPLAYERSWORD) {
						//���R���C�_
						if (o->mType == CCollider::ESPHERE) {
							//�G�i�Q�j
							if (o->mpParent->mTag == EENEMY2) {
								//�G�̃R���C�_
								if (o->mTag == CCollider::EENEMY2COLLIDERATTACK) {
									if (mAttackHit == true) {
										if (CCollider::Collision(m, o)) {
											//o->mpParent->Collision(o, m);

												//����U���̃Q�[�W����
											mSpAttack += 2;
											break;
										}
									}
								}
							}
						}
					}

			break;
		}
	}
		
	
}


void CXPlayer::TaskCollision() {
	//�R���C�_�̗D��x�ύX
	//mColSphereBody.ChangePriority();
	mColSphereSword.ChangePriority();
	mColSphereFoot.ChangePriority();
	//mCollider.ChangePriority();
	mCollider2.ChangePriority();

	//�Փˏ��������s
;
//CCollisionManager::Get()->Collision(&mColSphereBody, COLLISIONRANGE);
//CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
CCollisionManager::Get()->Collision(&mCollider2, COLLISIONRANGE);
CCollisionManager::Get()->Collision(&mColSphereFoot, COLLISIONRANGE);
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

