#include "CXPlayer.h"
#include "CKey.h"
#include "CCamera.h"
#include "CUtil.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define JUMP 30.0f
CXPlayer::CXPlayer()
	: mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 1.0f)

	, mJump(0.0f)
	,mSpAttack(0)
	,mHp(0)
{
	//�^�O�Ƀv���C���[��ݒ肵�܂�
	mTag = EPLAYER;
	mColSphereSword.mTag = CCollider::ESWORD;
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

	mRotation.mY = 0.01f;
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
		if (mSpAttack >= 30) {
			if (CKey::Once('N')) {
				mJump = JUMP;
				mPosition.mY += mJump;
				mSpAttack -= 30;
			}
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
		}
		else if (CKey::Push('D'))
		{
			//			mRotation.mY -= 2.0f;
			Move += SideVec;
		}
		if (CKey::Push('W'))
		{
			Move += FrontVec;
			//			mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
		}
		else if (CKey::Push('S'))
		{
			Move -= FrontVec;
			//			mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
		}

		if (CKey::Push(' '))
		{
			ChangeAnimation(3, true, 30);
		}else if (Move.Length() != 0.0f){
			ChangeAnimation(1, true, 60);
		}else {
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

			 if (o->mpParent->mTag == EENEMY2) {
				if (o->mTag == CCollider::EENEMY2COLLIDER) {
					//�Փ˂��Ă���Ƃ�
					CVector adjust;//�����p�x�N�g��
					if (CCollider::Collision(m, o)) {
						mColliderCount = 5;
						mCollisionEnemy = mPosition - o->mpParent->mPosition;
						mCollisionEnemy.mY = 0;
						mCollisionEnemy = mCollisionEnemy.Normalize();
						mHp--;
					}
				}


			}



		}


	}




}
