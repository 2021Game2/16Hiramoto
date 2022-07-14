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
#define JUMP 5.0f//�X�y�V�����U�����̃W�����v��
#define STEP  20.0f //�U���������O�i
#define STEP2 0.2f //����s���������O�i
#define STAMINA 400 //�X�^�~�i
#define HP_MAX 10	//�̗͍ő�l
#define STOPPER 0.1f
#define STAMINA_MAX 1000 //�X�^�~�i�ő�l
#define SPPOINT_MAX 30 //SP�|�C���g�ő�l
#define GAUGE_WID_MAXHP 400.0f	//HP�Q�[�W�̕��̍ő�l
#define GAUGE_WID_MAXST 400.0f //�X�^�~�i�Q�[�W�̕��̍ő�l
#define GAUGE_WID_MAXSP 300.0f//SP�|�C���g�Q�[�W�̕��̍ő�l
#define GAUGE_LEFT 20			//�Q�[�W�`�掞�̍��[
#define STEPG 0.9f
#define IMAGE_GAUGE "Resource\\png,tga\\Gauge.png"		//�Q�[�W�摜
#define G 0.1f//�d��
#define G2 1.5f//�X�y�V�����U�����̏d��
#define G3 0.01f


extern CSound PlayerFirstAttack;//���ڂ̍U����SE
extern CSound PlayerSecondAttack;//���ڂ̍U����SE
extern CSound PlayerThirdAttack;//�O��ڂ̍U����SE
extern CSound PlayerJumpAttack;//�X�y�V�����U����SE
extern CSound PlayerDamage;//�_���[�W��SE

CXPlayer* CXPlayer::mpPlayerInstance;
//�v���C���[�̃|�C���^��Ԃ����ƂŁA���W�Ȃǂ��Q�Ƃł���悤�ɂȂ�
CXPlayer* CXPlayer::GetInstance()
{
	return mpPlayerInstance;
}

CXPlayer::CXPlayer()
	:mColSphereSword(this, &mMatrix, CVector(-10.0f, 10.0f, 50.0f), 2.5f)//���̃R���C�_�P
	,mColSphereFoot(this, &mMatrix, CVector(0.0f, 0.0f, -3.0f), 2.0f)//���t�߂̃R���C�_
	,mColliderSwordSp(this, &mMatrix, CVector(0.0f, -2.0f, 0.0f), 10.0f)//���̃R���C�_�Q
	,mColEscapeStopperLine(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f),CVector(0.0f,0.0f,0.0f))//������ɂ��蔲���Ȃ��悤�ɂ�������R���C�_
	,mDamageCount(0)
	,mAnimationCount(0)
	,mAttackCount(0)
	,mJump(0.0f)
	,mStep(0.0f)
	,mColliderCount(1.0f)
	,mSpeed(0.0f)
	,mPlayerBgm(true)
	,mSpaceCount1(true)
	,mJumpStopper(true)
	, mEscapeFlg(true)
	,mGaugeEnabled(true)
	,mSpaceCount2(false)
	,mSpaceCount3(false)
	,mMoveCheck(false)
	,mAnimationFrameLock(false)
	,mEffectStopper(false)
	,mAttackSp(false)
	,mHp(0)
	,mSpAttack(SPPOINT_MAX)
	,mStamina(STAMINA_MAX)
{
	//�^�O�Ƀv���C���[��ݒ肵�܂�
	mTag = EPLAYER;
	mColSphereSword.mTag = CCollider::EPLAYERSWORD;
	mColliderSwordSp.mTag = CCollider::EPLAYERSWORD;
	mColSphereFoot.mTag = CCollider::EPLAYERBODY;
	mColEscapeStopperLine.mTag = CColliderLine::EPLAYERESCAPESTOPPER;
	//this���v���C���[���̂���
	mpPlayerInstance = this;
	mImageGauge.Load(IMAGE_GAUGE);
	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}
//���̃R���C�_�̍��W���擾����
CVector CXPlayer::GetSwordColPos()
{
	return mColSphereSword.mpMatrix->GetPos();	//���̃R���C�_�̍��W��Ԃ�
}
void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//�RD���f���̌��̃{�[���ɏꏊ�����킹��
	mColSphereSword.mpMatrix = &mpCombinedMatrix[22];
	//3D���f���̑��̃{�[���ɏꏊ�ɍ��킹��
	mColSphereFoot.mpMatrix = &mpCombinedMatrix[2];
	mColEscapeStopperLine.mpMatrix = &mpCombinedMatrix[2];
	//�ҋ@��Ԃ���n�܂�
	mState = EIDLE;
	//�ŏ��͕\�������Ȃ�
	mColliderSwordSp.mRenderEnabled = false;
}

void CXPlayer::Update()
{
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	CCamera* tCamera = CCamera::GetInstance();
	//�������s�����Ƃɕ���
	switch (mState) {
	case EIDLE:	//�ҋ@
		ChangeAnimation(0, true, 60);//�ҋ@�A�j���[�V����
		mSpeed = 0;//�i�܂Ȃ�
		break;
	case EMOVE://���s
		mAnimationFrameSize = 60;
		//�U������Ȃ�
		mAttackHit = false;
			 if (CKey::Push('C')) {
				 //�A�j���[�V�����̑��x�𔼕��ɂ���EDUSH�Ɉڍs
				 mAnimationFrameSize = 30;
					 mState = EDUSH;
			 }
			 else {
             ChangeAnimation(1, true, 60);
			 }
			 //�A�j���[�V�����̏I����҂����ɑҋ@��ԂɈڍs
			if (mAnimationCount <= 0) {
				mMoveCheck = false;
				mState = EIDLE;
		    }
		break;
	case EDUSH://�_�b�V��
		//�U������Ȃ�
        mAttackHit = false;
		
		if (CKey::Push('C')) {
        mStamina -= 2;//�X�^�~�i����
		}
		//C�������Ă��Ȃ��Ȃ���s
		else {
			mState = EMOVE;
		}
			ChangeAnimation(1, true, 30);

			//�A�j���[�V�����̏I����҂����ɑҋ@��ԂɈڍs
			if (mAnimationCount <= 0) {	
				mState = EIDLE;
			}
		break;
		//���
	case EESCAPE:
		if (mEscapeFlg = true) {
			mEscapeFlg = false;
			mStep = STEP2;

		}
		mPosition += CVector(0.0f, 0.0f, mStep)*mMatrixRotate;
			if (mStep > 0.0f) {
				mStep -= STOPPER;
			}
		mAttackHit = false;
		ChangeAnimation(1, true, 10);
		//��]�i������Ă�悤�Ɍ�����j
		if (mRotation.mX!=360.0f) {
		   mRotation.mX += 36.0f;
		}
		else if(mAnimationFrame >= mAnimationFrameSize){
         mState = EMOVE;//�ҋ@��ԂɈڍs
		 mEscapeFlg = true;
		 //��]�l�����ɖ߂�
		 mRotation.mX = 0.0f;
		}
		break;
	case EATTACK1://�U��1
		if (mAttackCount>ATTACKCOUNT1/2) {
		ChangeAnimation(3, false, 20);
		}
		break;
	case EATTACK2://�U��2
		if (mAttackCount>ATTACKCOUNT2/2) {
		
		ChangeAnimation(5, false, 20);//+�U�Ԗڂ̃A�j���[�V�����̃t���[���R�O
		}
		break;
	case EATTACK3://�U��3
		if (mAttackCount>ATTACKCOUNT3/2) {
		
		ChangeAnimation(7, false, 30);
		
		}
		break;
	case EATTACKSP://�U��
		tCamera->mAngleY -= 0.01f;
		if (mAttackCount>0) {
		ChangeAnimation(7, false, 100);
		
		}
		break;
	case EDAMAGED://�_���[�W

		if (tSceneGame->mVoiceSwitch == true) {
			if (mDamageVoise == true) {

			  PlayerDamage.Play();
			  mDamageVoise = false;
			}
		}

		ChangeAnimation(4, false, 60);
		break;
	case EDEATH://���S
		if (mPlayerBgm == true) {
			tSceneGame->mBgmCountCheck = false;
			tSceneGame->mBgmCount = 5;

			mPlayerBgm = false;
		}
		ChangeAnimation(11, false, 60);
		break;
	}
	//�A�j���[�V�����̎��
	switch (mAnimationIndex) {
	case(3): 
		//�A�j���[�V�����̃��b�N���O��Ă���Ƃ�
		if (mAnimationFrameLock == false) {
			//�G�t�F�N�g�̃X�g�b�p�[���O��Ă���Ƃ�
			if (mEffectStopper==false) {
				//�G�t�F�N�g���\�������܂ŃA�j���[�V������i�߂Ȃ�
				if (mAnimationFrame >= 15) {
					mAnimationFrameLock = true;
					mEffectStopper = true;
				}
			}
		}
		//mEffectStopper�������Ĉ�x�����G�t�F�N�g���o��悤��
		if (mAnimationFrameLock == true) {
			//�G�t�F�N�g����
			//���R���C�_�̍��W���Q��
			CVector tpos = mColSphereSword.mpMatrix->GetPos();
			mEffect1 = new CEffect2(CVector(tpos.mX, tpos.mY, tpos.mZ), 3.0f, 3.0f,CEffect2::EFF_ATTACK, 2, 5, 3, true, &mRotation);
			mAttackHit = true;
			mAnimationFrameLock = false;
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mAttackHit = false;
			mEffectStopper = false;
			ChangeAnimation(4, false, 30);
		}
		break;
	case(4):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			if (CKey::Push('A') || CKey::Push('S') || CKey::Push('W') || CKey::Push('D')) {
		    	mState = EMOVE;
				if (CKey::Push('C')) {
					mState = EDUSH;
				}
			}
			else {
				mState = EIDLE;
			}
		}
		break;
	case(5):
		//�A�j���[�V�����̃��b�N���O��Ă���Ƃ�
		if (mAnimationFrameLock == false) {
			//�G�t�F�N�g�̃X�g�b�p�[���O��Ă���Ƃ�
			if (mEffectStopper == false) {
				//�G�t�F�N�g���\�������܂ŃA�j���[�V������i�߂Ȃ�
				if (mAnimationFrame >= 15) {
					mAnimationFrameLock = true;
					mEffectStopper = true;
				}
			}
		}
		//mEffectStopper�������Ĉ�x�����G�t�F�N�g���o��悤��
		if (mAnimationFrameLock == true) {
			//���R���C�_�̍��W���Q��
			CVector tpos = mColSphereSword.mpMatrix->GetPos();
			mEffect2 = new CEffect2(CVector(tpos.mX, tpos.mY, tpos.mZ += 1.0f), 3.0f, 3.0f, CEffect2::EFF_ATTACK2, 3, 5, 3, true, NULL);
			
			mAttackHit = true;
			mAnimationFrameLock = false;
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mAttackHit = false;
			mEffectStopper = false;
			ChangeAnimation(6, false, 30);
		}
		break;
	case(6):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			if (CKey::Push('A') || CKey::Push('S') || CKey::Push('W') || CKey::Push('D')) {
				mState = EMOVE;
				if (CKey::Push('C')) {
					mState = EDUSH;
				}
			}
			else {
				mState = EIDLE;
			}
		}
		break;
	case(7):
		if (mJump >= -0.1f) {
			mJump -= G;
		}

		//�A�j���[�V�����̃��b�N���O��Ă���Ƃ�
		if (mAnimationFrameLock == false) {
			//�G�t�F�N�g�̃X�g�b�p�[���O��Ă���Ƃ�
			if (mEffectStopper == false) {
				//�G�t�F�N�g���\�������܂ŃA�j���[�V������i�߂Ȃ�
				if (mAnimationFrame >= 15) {
					mAnimationFrameLock = true;
					mEffectStopper = true;
				}
			}
		}
		//�A�j���[�V�������~�߂Ă���G�t�F�N�g�Đ�
		if (mAnimationFrameLock == true) {
					//�O��ڂ̍U���̂Ƃ������G�t�F�N�g�Đ�
					if (mState == EATTACK3) {
						//���R���C�_�̍��W���Q��
						CVector tpos = mColSphereSword.mpMatrix->GetPos();
						mEffect3 = new CEffect2(CVector(tpos.mX, tpos.mY, tpos.mZ), 3.0f, 3.0f, CEffect2::EFF_ATTACK3, 3, 5, 3,false, &mRotation);
						
					}
						mAnimationFrameLock = false;
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mAttackHit = true;
			mEffectStopper = false;
			ChangeAnimation(8, false, 100);
		}
		break;
	case(8):
		if (mState == EATTACKSP) {
			if (mJump >= -3.0f) {
				//���R���C�_�̍��W���Q��
				CVector tpos = mColSphereSword.mpMatrix->GetPos();
				mEffectSp = new CEffect2(CVector(tpos.mX, tpos.mY+1.0f, tpos.mZ), 3.0f, 3.0f, CEffect2::EFF_ATTACKSP, 4, 5, 3, false, &mRotation);
				if (mJumpStopper == false) {
				  mJump -= G2;
				}
			  mColliderSwordSp.mRenderEnabled = true;
		    }
	    }
		if (mAnimationFrame >= mAnimationFrameSize)
		{
		    mAttackSp =false;
			tCamera->mAngleY = 1.3f;
			mAttackHit = false;
			mColliderSwordSp.mRenderEnabled = false;

			if (CKey::Push('A') || CKey::Push('S') || CKey::Push('W') || CKey::Push('D')) {
				mState = EMOVE;
				if (CKey::Push('C')) {
					mState = EDUSH;
				}
			}
			else {
				mState = EIDLE;
			}
			
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
	//�U�����肪�L���̂Ƃ�
	
	//���b�N���������Ă���Ԃ̓A�j���[�V�������i�܂Ȃ��悤�ɂ���
	if (mAnimationFrameLock == true) {
		mAnimationFrame--;
	}
		//�J�������_�ړ��@�ʏ̖��o�ړ�
		//�J�����̍��E�ƑO��̃x�N�g�����擾
		CVector SideVec = Camera->GetMat().GetXVec();
		CVector FrontVec = Camera->GetMat().GetZVec();
		//�����ړ��̓J�b�g����
		SideVec.mY = 0.0f;
		FrontVec.mY = 0.0f;
		//���K������
		SideVec.Normalize();
		FrontVec.Normalize();
		float speed = 0.15f;
		CVector Move(0, 0, 0);
		if (mHp > 0) {


			//��
			if (CKey::Push('A'))
			{
				//�������Ɉړ�
				Move -= SideVec;
				mAnimationCount = 5;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�
				mMoveCheck = true;
				if (CKey::Push('C')) {
					if (mState == EDUSH) {


						if (mStamina > -1) {

							speed = 0.5f;//�X�s�[�h�{
						}
						else {
							speed = 0.2f;//�X�s�[�h1/2
						}
					}
				}

			}
			//�E
			else if (CKey::Push('D'))
			{
				Move += SideVec;
				mAnimationCount = 5;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�
				mMoveCheck = true;
				if (CKey::Push('C')) {
					if (mState == EDUSH) {
						if (mStamina > -1) {
							speed = 0.5f;//�X�s�[�h�{
						}
						else {
							speed = 0.2f;//�X�s�[�h1/2
						}
					}
				}
			}
			//�O
			if (CKey::Push('W'))
			{
				Move += FrontVec;
				mAnimationCount = 5;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�
				mMoveCheck = true;
				if (CKey::Push('C')) {
					if (mState == EDUSH) {
						if (mStamina > -1) {
							speed = 0.5f;//�X�s�[�h�{
						}

						else {
							speed = 0.2f;//�X�s�[�h1/2
						}
					}
				}

			}
			//���
			else if (CKey::Push('S'))
			{
				Move -= FrontVec;
				mAnimationCount = 5;//0�ɂȂ�܂ŃA�j���[�V������ύX�ł��Ȃ�
				mMoveCheck = true;
				if (CKey::Push('C')) {
					if (mState == EDUSH) {
						if (mStamina > -1) {
							speed = 0.5f;//�X�s�[�h�{
						}
						else {
							speed = 0.2f;//�X�s�[�h1/2
						}
					}
				}
			}

			//�P���Q���R���P   �U���̏��Ԃ����[�v

		   //����s��������Ȃ��Ƃ�
			if (mState != EESCAPE) {
				//�U���P���g����Ƃ�
				if (mSpaceCount1 == true) {

					if (mAttackCount <= 0) {
						if (CKey::Once(VK_LBUTTON))
						{

							if (tSceneGame->mVoiceSwitch == true) {
								PlayerFirstAttack.Play();
							}
							mState = EATTACK1;
							mSpaceCount1 = false;//�P��ڂ̍U���̃t���O
							mSpaceCount2 = true;
							mAttackCount = ATTACKCOUNT1;//�U���̃A�j���[�V���������[�v���Ȃ��悤��
							mAnimationCount = 50;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
							//mMoveAttack.Normalize();
							mStep = STEP;
						}

					}
				}
				//2��ڂ̍U��
				else if (mSpaceCount2 == true) {
					if (mAttackCount <= 0) {
						if (CKey::Once(VK_LBUTTON)) {

							if (tSceneGame->mVoiceSwitch == true) {
								PlayerSecondAttack.Play();
							}
							mState = EATTACK2;
							mSpaceCount2 = false;//�Q��ڂ̍U���̃t���O
							mSpaceCount3 = true;//�R��ڂ̍U���̃t���O
							mAttackCount = ATTACKCOUNT2;//�U���̃A�j���[�V���������[�v���Ȃ��悤��
							mAnimationCount = 50;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
							mStep = STEP;
						}
					}
				}
				//�R��ڂ̍U��
				else if (mSpaceCount3 == true) {
					if (mAttackCount <= 0) {
						if (CKey::Once(VK_LBUTTON)) {

							if (tSceneGame->mVoiceSwitch == true) {
								PlayerThirdAttack.Play();
							}
							mState = EATTACK3;
							mAnimationCount = 50;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
							mSpaceCount3 = false;//�R��ڂ̍U���̃t���O
							mSpaceCount1 = true;//�P��ڂ̍U���̃t���O
							mAttackCount = ATTACKCOUNT3;//�U���̃A�j���[�V���������[�v���Ȃ��悤��
							mStep = STEP;//�W�����v�͂���
						}
					}
				}

				//�W�����v�U��
				if (mSpAttack >= 30) {
					if (CKey::Once(VK_RBUTTON)) {
						if (mAttackCount <= 0) {

							if (tSceneGame->mVoiceSwitch == true) {
								PlayerJumpAttack.Play();
							}
							mState = EATTACKSP;
							mAttackSp=true;
							mJump = JUMP;//�W�����v�͂���
							mSpAttack -= 30;//����U���̃Q�[�W����
							mAnimationCount = 200;//0�ɂȂ�܂ŃA�j���[�V�������ς��Ȃ�
							mAttackCount = 100;
						}
					}
				}

			}
			//�ړ���
			if (Move.Length() != 0.0f) {
				//C�L�[�������Ɖ���s��
				if (CKey::Once('C')) {
					if (mState != EATTACKSP) {
						if (mAttackCount <= 0) {
							if (mDamageCount <= 0) {
								mState = EESCAPE;
								mAnimationCount = 20;
								mDamageCount = 40;//���G����
								mStamina -= 20;//�X�^�~�i�g�p
							}
						}
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
			if (CKey::Once('C')) {
				if (Move.Length() != 0.0f) {
					mState = EDUSH;
				}
				
			}
		}
		//�ړ��ʐ��K���@�΂߈ړ��������Ȃ��Ă��܂�
		//�W�����v���Ȃǂ�Y���𐳋K�����Ȃ�
		Move.Normalize();
		//���s�ړ���
		//�ݒ肵���ړ��ʂɂȂ�܂ŉ���
		if (mSpeed < speed) {
			mSpeed += 0.01f;
		}
		//����
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
		if (mState == EMOVE||mState==EDUSH||mState==EATTACKSP||mStep > 0) {
		
             mPosition += Move;
		}
		//Move = Move * mMoveAttack;
		//mMoveAttack.mZ * mStep;
		//mStep* STEPG;
		//����s�����̈ړ���(���񂾂�x���Ȃ�j
		 if (mStep > 0) {
			 Move.mZ += mStep;
				mStep--;
		 }

		 if (mAnimationCount > 0) {
			 mAnimationCount--;
	     }
		 //�A�j���[�V�������I��邽�тɍU�����[�V�����ŏ�����
		 if (mState != EATTACK1 && mState != EATTACK2 && mState != EATTACK3) {
			 if (mAttackCount <= 0) {
				 mSpaceCount1 = true;
				 mSpaceCount2 = false;
				 mSpaceCount3 = false;
			 }

		 }
		 //�X�^�~�i��
		 if (mStamina < STAMINA_MAX) {
			mStamina++;
		 }
		 if (mSpAttack > SPPOINT_MAX) {
		    mSpAttack = SPPOINT_MAX;
		 }
		 //�U���A�j���[�V������
		 if (mAttackCount > 0) {
			 mAttackCount--;
		 }
		 //���G����
		 if (mDamageCount > 0) {
			 mDamageCount--;
		 }

		 //���S
		 if (mHp <= 0) {
			 mState = EDEATH;
		 }
		 //�}�b�v�ɐڐG���Ă��Ȃ��Ԃ����Əd�͂�������
		 if (mState != EATTACKSP) {
			if (mJumpStopper == false) {
				 if (mJump >= -0.1) {
				   mJump -= G;
				 }
			 }
		 }
		 if (mState != EESCAPE) {
			  mPosition.mY += mJump;
		 }
		 
		 //�������
		 if (mColliderCount > 0) {
			mColliderCount-=0.2f;
			mPosition = mPosition + mCollisionEnemy * mColliderCount;
		 
		 }
		 mColEscapeStopperLine.Set(this, &mMatrix, CVector(0.0f, 0.0f, -2.0f), CVector(0.0f, 0.0f,0.0f));
		 if (mState == EESCAPE) {
			 mPosition.mY -= G3;
		 }
	    //�����_�ݒ�
	    Camera->SetTarget(mPosition);
	    CXCharacter::Update();
}
void CXPlayer::SpAttackPoint() {
	mSpAttack++;
}
void CXPlayer::SpAttackPoint2() {
	mSpAttack+=2;
}
void CXPlayer::Collision(CCollider* m, CCollider* o) {

	//���g�̃R���C�_�̐ݒ�
	switch (m->mType) {


	case CCollider::ELINE:
		//�e���v���C���[
		if (m->mpParent->mTag == EPLAYER) {
			//�v���C���[�̑̕���
			if (m->mTag == CColliderLine::EPLAYERESCAPESTOPPER) {//����̃R���C�_���O�p�R���C�_�̏ꍇ
				//�e���O�p�R���C�_
				if (o->mType == CCollider::ETRIANGLE) {
					
					CVector adjust;//�����p�x�N�g��
					if (CCollider::CollisionTriangleLine(o, m, &adjust)) {
					    if (mState == EESCAPE) {
							//�ʒu�̍X�V�imPosition+adjust)
							mPosition = mPosition + adjust;
							//�s��̍X�V
							CTransform::Update();
						}
					}
				}
			}
		}
		break;
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

									mJumpStopper = true;
									mJump =0;
									if (mAnimationIndex == 8) {
										mAnimationFrame += 2;
									}
									//�ʒu�̍X�V�imPosition+adjust)
									mPosition = mPosition + adjust;
									//�s��̍X�V
									CTransform::Update();
								}
								
						}
						else {

							mJumpStopper = false;
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
								if (mDamageCount == 0&&mState!=EESCAPE) {
									//�G�̍U�����肪�K�p����Ă����
									if (((CEnemy2*)(o->mpParent))->mEnemy2AttackHit == true)
									{
										if (mHp > 0) {
											//���ɉ�����
											mColliderCount = 2.0f;
											mCollisionEnemy = mPosition - o->mpParent->mPosition;
											mCollisionEnemy.mY = 0;
											mCollisionEnemy = mCollisionEnemy.Normalize();
											//�̗͌���
											mHp--;
											//���G���ԕt�^
											mDamageCount = 60;
											//�_���[�W���̏������J�n
											mDamageVoise = true;
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
								mColliderCount = 0.5f;
								mCollisionEnemy = mPosition - o->mpParent->mPosition;
								mCollisionEnemy.mY = 0;
								mCollisionEnemy = mCollisionEnemy.Normalize();
							}
						}
						//�{�X�̍U�����ʂƂ̏Փ˔���
						else if (o->mTag == CCollider::EBOSSCOLLIDERATTACK) {
							if (CCollider::Collision(m, o)) {
								//�_���[�W�����������Ƃ̖��G����
								if (mDamageCount == 0&&mState != EESCAPE) {
									//�G�̍U�����肪�K�p����Ă����
									if (((CBoss*)(o->mpParent))->mBossAttackHit == true)
									{
										if (mHp > 0) {
											//���ɉ�����
											mColliderCount = 3.0f;
											mCollisionEnemy = mPosition - o->mpParent->mPosition;
											mCollisionEnemy.mY = 0;
											mCollisionEnemy = mCollisionEnemy.Normalize();
											//�̗͌���
											mHp--;
											//���G���ԕt�^
											mDamageCount = 30;
											//�_���[�W���̏����J�n
											mDamageVoise = true;
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
	mColliderSwordSp.ChangePriority();
	mColEscapeStopperLine.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mColliderSwordSp, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereFoot, COLLISIONRANGEFIELD);
	CCollisionManager::Get()->Collision(&mColSphereSword, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColEscapeStopperLine, COLLISIONRANGE);
}
void CXPlayer::Render2D()
{
	//2D�`��J�n
	CUtil::Start2D(0, 800, 0, 600);
	//�̗͂̊���
	float hpRate = (float)mHp / (float)HP_MAX;
	//�̗̓Q�[�W�̕�
	float hpGaugeWid = GAUGE_WID_MAXHP * hpRate;
	//�X�^�~�i�̊���
	float staminaRate = (float)mStamina / (float)STAMINA_MAX;
	//�X�^�~�i�Q�[�W�̕�
	float staminaGaugeWid = GAUGE_WID_MAXST * staminaRate;

	float spRate = (float)mSpAttack / (float)SPPOINT_MAX;
	float spGaugeWid = GAUGE_WID_MAXSP * spRate;

	if (mGaugeEnabled==true) {
		/*
		mImageGauge.Draw(20, GAUGE_WID_MAXHP, 500, 510, 201, 300, 63, 0);//�Q�[�W�w�i
		mImageGauge.Draw(20, hpGaugeWid, 500, 510, 0, 99, 63, 0);//�̗̓Q�[�W

		mImageGauge.Draw(20, GAUGE_WID_MAXST, 490, 500, 201, 300, 63, 0);//�Q�[�W�w�i
		mImageGauge.Draw(20, staminaGaugeWid, 490, 500, 100, 200, 63, 0);//�X�^�~�i�Q�[�W

		mImageGauge.Draw(20, GAUGE_WID_MAXSP, 480, 490, 201, 300, 63, 0);//�Q�[�W�w�i
		mImageGauge.Draw(20, spGaugeWid, 480, 490, 401, 486, 63, 0);//SP�Q�[�W
		*/
	}

	CUtil::End2D();



}

