#include "CXPlayer.h"
#include "CKey.h"


#define G 1.0f
#define JUMP 10.0f
int CXPlayer::mAction1 = 60;//１回目の攻撃の動く時間
int CXPlayer::mAction2 = 60;//２回目の攻撃の動く時間
int CXPlayer::mAction3 = 60;//３回目の攻撃の動く時間
int CXPlayer::mSpaceCount1 = 0;//1回目攻撃後のクールタイム
int CXPlayer::mSpaceCount2 = 0;//２回目攻撃後のクールタイム
int CXPlayer::mSpaceCount3 = 0;//３回目攻撃後のクールタイム
int CXPlayer::mStamina = 400;
int CXPlayer::mWeaponCount = 0;
int CXPlayer::mSpAttack = 0;
CXPlayer::CXPlayer()
	: mColSphereBody(this, nullptr, CVector(), 0.5f)//体のコライダ
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)//頭のコライダ
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f)//武器のコライダ
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
			//右折
		if (CKey::Push('A')) {
			
			mWeaponCount = 3;
			if (mRotationCount2 <= 0) {
				mRotationCount2 += 1;
				if (mRotationCountFirst > 0) {
					mRotation.mY -= 90;
					mRotationCountFirst--;
				}
			}
			else if (mRotationCount > 0) {//正面を向いていたとき
				mRotation.mY -= 90;
				mRotationCount -= 1;
			}
			else if (mRotationCount3 > 0) {//後ろを向いていたとき
				mRotation.mY += 90;
				mRotationCount3 -= 1;
			}
			else if (mRotationCount4 > 0) {//左を向いていたとき
				mRotation.mY += 180;
				mRotationCount4 -= 1;
			}
			//ダッシュ
			if (CKey::Push('C')) {
				if (mStamina > 0) {
					//Z軸方向に４進ませる
					mPosition = CVector(0.0f, 0.0f, 0.3f) * mMatrix;
					mStamina -= 2;
				}
				//スタミナ切れ
				else if (mStamina <= 2 || mStamina >= 0) {
					//Z軸方向に１進ませる
					mPosition = CVector(0.0f, 0.0f, 1.0f) * mMatrix;
					mStamina -= 2;
				}
			}
			else {
				//Z軸方向に２進ませる
				mPosition = CVector(0.0f, 0.0f, 0.1f) * mMatrix;
			}
		}
    

		
		
			//左折
		else if (CKey::Push('D')) {
			
			mWeaponCount = 2;
			if (mRotationCount4 <= 0) {
				mRotationCount4 += 1;
				if (mRotationCountFirst > 0) {
					mRotation.mY += 90;
					mRotationCountFirst--;
				}
			}
			else if (mRotationCount2 > 0) {//右を向いていたとき
				mRotation.mY += 180;
				mRotationCount2 -= 1;
			}
			else if (mRotationCount3 > 0) {//後ろを向いていたとき
				mRotation.mY -= 90;
				mRotationCount3 -= 1;
			}
			else if (mRotationCount > 0) {//正面を向いていたとき
				mRotation.mY += 90;
				mRotationCount -= 1;
			}
			//ダッシュ
			if (CKey::Push('C')) {
				if (mStamina > 0) {
					//Z軸方向に４進ませる
					mPosition = CVector(0.0f, 0.0f, 0.3f) * mMatrix;
					mStamina -= 2;
				}
				//スタミナ切れ
				else if (mStamina <= 2 || mStamina >= 0) {
					//Z軸方向に１進ませる
					mPosition = CVector(0.0f, 0.0f, 1.0f) * mMatrix;
					mStamina -= 2;
				}
			}
			else {
				//Z軸方向に２進ませる
				mPosition = CVector(0.0f, 0.0f,0.1f) * mMatrix;
			}


			
		}
		
		else if (CKey::Push('W')) {
          
			//後退
			mWeaponCount = 1;
			if (mRotationCount3 <= 0) {
				mRotationCount3 += 1;
				if (mRotationCountFirst > 0) {
					mRotation.mY += 180;
					mRotationCountFirst--;
				}
			}
			else if (mRotationCount2 > 0) {//右を向いていたとき
				mRotation.mY -= 90;
				mRotationCount2 -= 1;
			}
			else if (mRotationCount > 0) {//正面を向いていたとき
				mRotation.mY += 180;
				mRotationCount -= 1;
			}
			else if (mRotationCount4 > 0) {//左を向いていたとき
				mRotation.mY += 90;
				mRotationCount4 -= 1;
			}
			//ダッシュ
			if (CKey::Push('C')) {
				if (mStamina > 0) {
					//Z軸方向に４進ませる
					mPosition = CVector(0.0f, 0.0f,0.3f) * mMatrix;
					mStamina -= 2;
				}
				//スタミナ切れ
				else if (mStamina <= 2 || mStamina >= 0) {
					//Z軸方向に１進ませる
					mPosition = CVector(0.0f, 0.0f, 1.0f) * mMatrix;
					mStamina -= 2;
				}
			}
			else {
				//Z軸方向に２進ませる
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
			else if (mRotationCount2 > 0) {//右を向いていたとき
				mRotation.mY += 90;
				mRotationCount2 -= 1;
			}
			else if (mRotationCount3 > 0) {//後ろを向いていたとき
				mRotation.mY += 180;
				mRotationCount3 -= 1;
			}
			else if (mRotationCount4 > 0) {//左を向いていたとき
				mRotation.mY -= 90;
				mRotationCount4 -= 1;
			}
			//ダッシュ
			if (CKey::Push('C')) {
				if (mStamina > 0) {

					//Z軸方向に４進ませる
					mPosition = CVector(0.0f, 0.0f, 0.3f) * mMatrix;
					mStamina -= 2;
				}
				//スタミナ切れ
				else if (mStamina <= 2 || mStamina >= 0) {
					//Z軸方向に１進ませる
					mPosition = CVector(0.0f, 0.0f, 1.0f) * mMatrix;
					mStamina -= 2;
				}
			}
			else {
				//Z軸方向に２進ませる
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
