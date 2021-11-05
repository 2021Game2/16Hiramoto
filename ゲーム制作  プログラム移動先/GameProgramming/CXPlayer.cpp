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
	//タグにプレイヤーを設定します
	mTag = EPLAYER;
	mColSphereSword.mTag = CCollider::ESWORD;
	mCollider2.mTag = CCollider::ESWORD;
	mColSphereFoot.mTag = CCollider::EBODY;
	CXPlayer::mStamina = 400;
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
				mSpaceCount1 = 20;//１回めの攻撃の総フレーム
				mAttackCount = 20;//当たり判定が適用される時間
				mAnimationCount = 10;//0になるまでアニメーションが変わらない

                mStep = STEP;
				//mPosition.mX -= mStep;
				
		     }
		 }
		
		else if (mSpaceCount2 <= 0&& mSpaceCount1>1) {
			 if (mSpaceCount1 < 15) {
				if (CKey::Once(' ')) {
				ChangeAnimation(5, true, 10);
				mSpaceCount2 = 20;//２回めの攻撃の総フレーム
				mSpaceCount1 = 0;
				mAttackCount = 20;//当たり判定が適用される時間
				mAnimationCount = 10;//0になるまでアニメーションが変わらない
				mStep = STEP;
				//mPosition.mX -= mStep;
		        }
			 }
			
		}
		else if (mSpaceCount3<=0&&mSpaceCount2 > 1) {
			 if (mSpaceCount2 < 15) {
				 if (CKey::Push(' ')) {
					 ChangeAnimation(7, true, 10);
					 mAnimationCount = 10;//0になるまでアニメーションが変わらない
					 mSpaceCount3 = 20;//３回めの攻撃の総フレーム
					 mSpaceCount1 = 0;
					 mAttackCount = 20;//当たり判定が適用される時間
					 mStep = STEP;
					// mPosition.mX -= mStep;
				 }
			 }
		}
		 if (mSpAttack >= 0) {
			 if (CKey::Once('F')) {
				 mJump = JUMP;
				
				// mSpAttack -= 30;
				 mAnimationCount = 80;//0になるまでアニメーションが変わらない
				 ChangeAnimation(7, true, 50);
				 mAttackCount = 80;//当たり判定が適用される時間
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
		//重力
		if (mPosition.mY>0) {
			mPosition.mY =mJump*mTime- 0.5 * mGravity * mTime * mTime;
		}
		mTime++;
		if (CItem::mItemCount > 0) {
			mColSphereSword.mRadius = 2.5f;
		}
		//吹き飛ぶ
		if (mColliderCount > 0) {
			mColliderCount--;
			mPosition = mPosition + mCollisionEnemy * mColliderCount;
			ChangeAnimation(4, false, 10);
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
			if (m->mpParent->mTag == EPLAYER) {
				if (m->mTag == CCollider::EBODY) {
					if (o->mpParent->mTag == EENEMY2) {
						if (o->mTag == CCollider::EENEMY2COLLIDER) {
							//衝突しているとき
							CVector adjust;//調整用ベクトル
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

	


