#include "CXPlayer.h"
#include "CKey.h"


#define G 1.0f
#define JUMP 10.0f
int CXPlayer::mAction1 = 60;//�P��ڂ̍U���̓�������
int CXPlayer::mAction2 = 60;//�Q��ڂ̍U���̓�������
int CXPlayer::mAction3 = 60;//�R��ڂ̍U���̓�������
int CXPlayer::mSpaceCount1 = 0;//1��ڍU����̃N�[���^�C��
int CXPlayer::mSpaceCount2 = 0;//�Q��ڍU����̃N�[���^�C��
int CXPlayer::mSpaceCount3 = 0;//�R��ڍU����̃N�[���^�C��
int CXPlayer::mStamina = 400;
int CXPlayer::mWeaponCount = 0;
int CXPlayer::mSpAttack = 0;
CXPlayer::CXPlayer()
	: mColSphereBody(this, nullptr, CVector(), 0.5f)//�̂̃R���C�_
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)//���̃R���C�_
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f)//����̃R���C�_
	, mRotationCountFirst(1)
	, mRotationCount(0)
	, mRotationCount2(0)
	, mRotationCount3(0)
	, mRotationCount4(0)
	, mColliderCount(0)
	, mHp(10)
	, mJump(0)
	, mGravity(0)
	, mGravity2(0)
{
	//�^�O�Ƀv���C���[��ݒ肵�܂�
	mTag = EPLAYER;
	mColSphereSword.mTag = CCollider::ESWORD;
}

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//9�Ԗڂ̃{�[���ɃR���C�_��A��
	mColSphereBody.mpMatrix = &mpCombinedMatrix[9];
	//��
	//�P�Q�Ԗڂ̃{�[���ɃR���C�_��A��
	mColSphereHead.mpMatrix = &mpCombinedMatrix[12];
	//��
	//�Q�Q�Ԗڂ̃{�[���ɃR���C�_��A��
	mColSphereSword.mpMatrix = &mpCombinedMatrix[22];

}

void CXPlayer::Update()

{	
	
	 if (mAnimationIndex == 3)
	{
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(4, false, 30);
		}
	}
	else if (mAnimationIndex == 4)
	{
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(0, true, 60);
		}
	}
	else
	{
		if (CKey::Push('A') || CKey::Push('S') || CKey::Push('W') || CKey::Push('D')) {
			ChangeAnimation(1, true, 60);
		}
			//�E��
		if (CKey::Push('A')) {
			
			mWeaponCount = 3;
			if (mRotationCount2 <= 0) {
				mRotationCount2 += 1;
				if (mRotationCountFirst > 0) {
					mRotation.mY -= 90;
					mRotationCountFirst--;
				}
			}
			else if (mRotationCount > 0) {//���ʂ������Ă����Ƃ�
				mRotation.mY -= 90;
				mRotationCount -= 1;
			}
			else if (mRotationCount3 > 0) {//���������Ă����Ƃ�
				mRotation.mY += 90;
				mRotationCount3 -= 1;
			}
			else if (mRotationCount4 > 0) {//���������Ă����Ƃ�
				mRotation.mY += 180;
				mRotationCount4 -= 1;
			}
			//�_�b�V��
			if (CKey::Push('C')) {
				if (mStamina > 0) {
					//Z�������ɂS�i�܂���
					mPosition = CVector(0.0f, 0.0f, 0.3f) * mMatrix;
					mStamina -= 2;
				}
				//�X�^�~�i�؂�
				else if (mStamina <= 2 || mStamina >= 0) {
					//Z�������ɂP�i�܂���
					mPosition = CVector(0.0f, 0.0f, 1.0f) * mMatrix;
					mStamina -= 2;
				}
			}
			else {
				//Z�������ɂQ�i�܂���
				mPosition = CVector(0.0f, 0.0f, 0.1f) * mMatrix;
			}
		}
    

		
		
			//����
		else if (CKey::Push('D')) {
			
			mWeaponCount = 2;
			if (mRotationCount4 <= 0) {
				mRotationCount4 += 1;
				if (mRotationCountFirst > 0) {
					mRotation.mY += 90;
					mRotationCountFirst--;
				}
			}
			else if (mRotationCount2 > 0) {//�E�������Ă����Ƃ�
				mRotation.mY += 180;
				mRotationCount2 -= 1;
			}
			else if (mRotationCount3 > 0) {//���������Ă����Ƃ�
				mRotation.mY -= 90;
				mRotationCount3 -= 1;
			}
			else if (mRotationCount > 0) {//���ʂ������Ă����Ƃ�
				mRotation.mY += 90;
				mRotationCount -= 1;
			}
			//�_�b�V��
			if (CKey::Push('C')) {
				if (mStamina > 0) {
					//Z�������ɂS�i�܂���
					mPosition = CVector(0.0f, 0.0f, 0.3f) * mMatrix;
					mStamina -= 2;
				}
				//�X�^�~�i�؂�
				else if (mStamina <= 2 || mStamina >= 0) {
					//Z�������ɂP�i�܂���
					mPosition = CVector(0.0f, 0.0f, 1.0f) * mMatrix;
					mStamina -= 2;
				}
			}
			else {
				//Z�������ɂQ�i�܂���
				mPosition = CVector(0.0f, 0.0f,0.1f) * mMatrix;
			}


			
		}
		
		else if (CKey::Push('W')) {
          
			//���
			mWeaponCount = 1;
			if (mRotationCount3 <= 0) {
				mRotationCount3 += 1;
				if (mRotationCountFirst > 0) {
					mRotation.mY += 180;
					mRotationCountFirst--;
				}
			}
			else if (mRotationCount2 > 0) {//�E�������Ă����Ƃ�
				mRotation.mY -= 90;
				mRotationCount2 -= 1;
			}
			else if (mRotationCount > 0) {//���ʂ������Ă����Ƃ�
				mRotation.mY += 180;
				mRotationCount -= 1;
			}
			else if (mRotationCount4 > 0) {//���������Ă����Ƃ�
				mRotation.mY += 90;
				mRotationCount4 -= 1;
			}
			//�_�b�V��
			if (CKey::Push('C')) {
				if (mStamina > 0) {
					//Z�������ɂS�i�܂���
					mPosition = CVector(0.0f, 0.0f,0.3f) * mMatrix;
					mStamina -= 2;
				}
				//�X�^�~�i�؂�
				else if (mStamina <= 2 || mStamina >= 0) {
					//Z�������ɂP�i�܂���
					mPosition = CVector(0.0f, 0.0f, 1.0f) * mMatrix;
					mStamina -= 2;
				}
			}
			else {
				//Z�������ɂQ�i�܂���
				mPosition = CVector(0.0f, 0.0f, 0.1f) * mMatrix;
			}

		}
		
		else if (CKey::Push('S')) {
			
			mWeaponCount = 0;
			if (mRotationCount <= 0) {
				mRotationCount += 1;
				if (mRotationCountFirst > 0) {
					mRotationCountFirst--;

				}
			}
			else if (mRotationCount2 > 0) {//�E�������Ă����Ƃ�
				mRotation.mY += 90;
				mRotationCount2 -= 1;
			}
			else if (mRotationCount3 > 0) {//���������Ă����Ƃ�
				mRotation.mY += 180;
				mRotationCount3 -= 1;
			}
			else if (mRotationCount4 > 0) {//���������Ă����Ƃ�
				mRotation.mY -= 90;
				mRotationCount4 -= 1;
			}
			//�_�b�V��
			if (CKey::Push('C')) {
				if (mStamina > 0) {

					//Z�������ɂS�i�܂���
					mPosition = CVector(0.0f, 0.0f, 0.3f) * mMatrix;
					mStamina -= 2;
				}
				//�X�^�~�i�؂�
				else if (mStamina <= 2 || mStamina >= 0) {
					//Z�������ɂP�i�܂���
					mPosition = CVector(0.0f, 0.0f, 1.0f) * mMatrix;
					mStamina -= 2;
				}
			}
			else {
				//Z�������ɂQ�i�܂���
				mPosition = CVector(0.0f, 0.0f, 0.1f) * mMatrix;
			}

			if (CKey::Push(' '))
			{
				ChangeAnimation(3, true, 30);
			}

		}
		else {
			ChangeAnimation(0, true, 60);
		}
	}
	CXCharacter::Update();
}
