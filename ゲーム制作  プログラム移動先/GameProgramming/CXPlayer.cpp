#include "CXPlayer.h"
#include "CKey.h"
#include "CCamera.h"
#include "CUtil.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define JUMP 30.0f
#include"CItem.h"
CXPlayer::CXPlayer()

	: mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.7f)
	, mColSphereFoot(this, nullptr, CVector(0.0f, 0.0f, -3.0f), 0.5f)
	, mJump(0.0f)
	, mSpAttack(0)
	, mHp(0)
	, mStamina(400)
	, mGravity(0.0f)
	,mSpaceCount1(0)
	,mSpaceCount2(0)
	,mSpaceCount3(0)
{
	//タグにプレイヤーを設定します
	mTag = EPLAYER;
	mColSphereSword.mTag = CCollider::ESWORD;
}

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	mColSphereBody.mpMatrix = &mpCombinedMatrix[9];
	//頭
	mColSphereHead.mpMatrix = &mpCombinedMatrix[12];
	//剣
	mColSphereSword.mpMatrix = &mpCombinedMatrix[22];
	//合成行列の設定
	mColSphereFoot.mpMatrix = &mpCombinedMatrix[3];
	mRotation.mY = 0.01f;
}

void CXPlayer::Update()
{
	switch (mAnimationIndex) {
	case(3):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(4, false, 30);
			mSpaceCount1 = 60;
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
    default:
		break;
	}
	
	
	
	
		
		//回転移動 通称バイオ移動
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

		//カメラ視点移動　通称無双移動

		//カメラの左右と前後のベクトルを取得
		CVector SideVec = Camera.GetMat().GetXVec();
		CVector FrontVec = Camera.GetMat().GetZVec();
		//高さ移動はカットする
		SideVec.mY = 0.0f;
		FrontVec.mY = 0.0f;
		//正規化する
		SideVec.Normalize();
		FrontVec.Normalize();

		float speed = 0.15f;
		CVector Move(0, 0, 0);


		if (CKey::Push('A'))
		{
			//			mRotation.mY += 2.0f;
			Move -= SideVec;
			if (mStamina > 0) {
				if (CKey::Push('C')) {
					speed = 0.30f;
					mStamina--;
					ChangeAnimation(1, true, 30);
				}
				
			}
		}
		else if (CKey::Push('D'))
		{
			//			mRotation.mY -= 2.0f;
			Move += SideVec;
			if (mStamina > 0) {
				if (CKey::Push('C')) {
					speed = 0.30f;
					mStamina--;
					ChangeAnimation(1, true, 30);
				}
				
			}
		}
		if (CKey::Push('W'))
		{
			Move += FrontVec;
			//			mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
			if (mStamina > 0) {
				if (CKey::Push('C')) {
					speed = 0.30f;
					mStamina--;
					ChangeAnimation(1, true, 30);
				}
				
			}
		}
		else if (CKey::Push('S'))
		{
			Move -= FrontVec;
			//			mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
			if (mStamina > 0) {
				if (CKey::Push('C')) {
					speed = 0.30f;
					mStamina--;
					ChangeAnimation(1, true, 30);
				}
				
			}
		}

		if (CKey::Push(' '))
		{
			ChangeAnimation(3, true, 30);
			
			
		}
		 if (mSpaceCount1 > 1) {
			if (CKey::Push(' ')) {
			ChangeAnimation(7, true, 30);
		    }
		}
		else if (Move.Length() != 0.0f){
			ChangeAnimation(1, true, 60);
		}
		else {
			ChangeAnimation(0, true, 60);
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
		//移動量正規化　これをしないと斜め移動が早くなってしまうので注意
		//ジャンプ時などはY軸を正規化しないよう注意
		Move.Normalize();
		//平行移動量
		Move = Move*speed;

		
		//普通に3次元ベクトル計算で算出したほうが正確だが計算量を懸念する場合は擬似計算で軽量化
		//擬似ベクトル計算
		Check tCheck = CUtil::GetCheck2D(Move.mX,Move.mZ,0,0, mRotation.mY*(M_PI/180.0f));
		
		//回転速度　degreeに直す
		float turnspeed = (180.0f / M_PI)*0.5f;

		//急な振り返りを抑制
		if (tCheck.turn > 1.5f) tCheck.turn =1.5f;

		//移動方向へキャラを向かせる
		if (tCheck.cross > 0.0f) {
			mRotation.mY += tCheck.turn * turnspeed;
		}
		if (tCheck.cross < 0.0f) {
			mRotation.mY -= tCheck.turn * turnspeed;
		}

		//座標移動
		mPosition += Move;


	
	if (mSpAttack >= 30) {
		if (CKey::Once('N')) {
			mJump = JUMP;
			mPosition.mY += mJump;
			mSpAttack -= 30;
		}
	}
	mPosition.mY -= mGravity;
	//重力
	if (mPosition.mY > 0.0f) {
		
		mGravity = 0.98;

	}
	else
	{
		mPosition.mY = 0.0f;
		mGravity = 0;
	}
	if (CItem::mItemCount > 0) {
		mColSphereSword.mRadius = 1.5f;
	}
	//注視点設定
	Camera.SetTarget(mPosition);

	CXCharacter::Update();
}

void CXPlayer::Collision(CCollider* m, CCollider* o) {

	//自身のコライダの設定
	switch (m->mType) {
	case CCollider::ELINE://線分コライダ
			//相手のコライダが三角コライダの場合
		if (o->mType == CCollider::ETRIANGLE) {
			CVector adjust;//調整用ベクトル
			//三角形と線分の衝突判定
			CCollider::CollisionTriangleLine(o, m, &adjust);
			//位置の更新（mPosition+adjust)
			mPosition = mPosition - adjust * -1;
			//mPosition.mY = 0.0f;
			//行列の更新
			CTransform::Update();
		}
	case CCollider::ESPHERE:

		if (o->mType == CCollider::ESPHERE) {

			 if (o->mpParent->mTag == EENEMY2) {
				if (o->mTag == CCollider::EENEMY2COLLIDER) {
					//衝突しているとき
					CVector adjust;//調整用ベクトル
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
