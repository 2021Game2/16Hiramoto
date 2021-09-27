#include "CXPlayer.h"
#include "CKey.h"

CXPlayer::CXPlayer()
	: mColSphereBody(this, nullptr, CVector(), 0.5f)//体のコライダ
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)//頭のコライダ
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f)//武器のコライダ
{
	//タグにプレイヤーを設定します
	mTag = EPLAYER;
	mColSphereSword.mTag = CCollider::ESWORD;
}

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//9番目のボーンにコライダを連動
	mColSphereBody.mpMatrix = &mpCombinedMatrix[9];
	//頭
	//１２番目のボーンにコライダを連動
	mColSphereHead.mpMatrix = &mpCombinedMatrix[12];
	//剣
	//２２番目のボーンにコライダを連動
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
