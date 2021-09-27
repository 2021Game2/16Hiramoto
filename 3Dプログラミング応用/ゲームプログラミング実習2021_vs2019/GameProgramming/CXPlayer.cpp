#include "CXPlayer.h"
#include "CKey.h"

CXPlayer::CXPlayer()
	: mColSphereBody(this, nullptr, CVector(), 0.5f)//�̂̃R���C�_
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)//���̃R���C�_
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f)//����̃R���C�_
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
{	if (mAnimationIndex == 3)
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
		if (CKey::Push('A'))
		{
			mRotation.mY += 2.0f;
		}
		if (CKey::Push('D'))
		{
			mRotation.mY -= 2.0f;
		}
		if (CKey::Push(' '))
		{
			ChangeAnimation(3, true, 30);
		}
		else if (CKey::Push('W'))
		{
			ChangeAnimation(1, true, 60);
			mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
		}
		else {
			ChangeAnimation(0, true, 60);
		}
	}
	CXCharacter::Update();
}
