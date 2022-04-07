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
#define STAMINA 1000 //スタミナ

#define HP_MAX 100	//HP			//体力最大値
#define STAMINA_MAX 1000		//スタミナ最大値
#define GAUGE_WID_MAX 400.0f	//ゲージの幅の最大値
#define GAUGE_LEFT 20			//ゲージ描画時の左端
#define IMAGE_GAUGE "Resource\\Gauge.png"		//ゲージ画像
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
//プレイヤーのポインタを返すことで、座標などが参照できるようになる
CXPlayer* CXPlayer::GetInstance()
{
	return mpPlayerInstance;
}
CXPlayer::CXPlayer()

	//: mColSphereBody(this, nullptr, CVector(0.0f,1.1f,0.0f), 0.5f)//体のコライダ
	//, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	: mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 4.5f)//剣のコライダ１
	, mColSphereFoot(this, nullptr, CVector(0.0f, 0.0f, -3.0f), 2.0f)//足付近のコライダ
	//,mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 5.0f)//ストッパー
	, mCollider2(this, &mMatrix, CVector(0.0f, -2.0f, 0.0f), 4.0f)//剣のコライダ２
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
	//タグにプレイヤーを設定します
	mTag = EPLAYER;
	
	mColSphereSword.mTag = CCollider::EPLAYERSWORD;
	mCollider2.mTag = CCollider::EPLAYERSWORD;
	mColSphereFoot.mTag = CCollider::EPLAYERBODY;
	//mColSphereBody.mTag = CCollider::EPLAYERBODY;
	//mColSphereHead.mTag = CCollider::EPLAYERBODY;
	//mCollider.mTag = CCollider::ESTOPPER;

	//this＝プレイヤーそのもの
	mpPlayerInstance = this;
	mImageGauge.Load(IMAGE_GAUGE);
	
}

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//mColSphereBody.mpMatrix = &mpCombinedMatrix[9];
	//頭
	//mColSphereHead.mpMatrix = &mpCombinedMatrix[12];
	//剣
	mColSphereSword.mpMatrix = &mpCombinedMatrix[22];

	//合成行列の設定
	mColSphereFoot.mpMatrix = &mpCombinedMatrix[2];
	mGravity = 0.20f;
	mState = EIDLE;
	mRotation.mY += 180.0f;

	mCollider2.mRenderEnabled = false;

}

void CXPlayer::Update()
{
	//処理を行動ごとに分割
	switch (mState) {
	case EIDLE:	//待機
		ChangeAnimation(0, true, 60);
		mSpeed = 0;
		mGravity = 0;
		break;
	case EMOVE://移動
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
		//攻撃時、進行方向にステップを踏む
		mStep = STEP2;
		if (mRotation.mX!=360.0f) {
			mRotation.mX += 36.0f;
		}
		if(mAnimationFrame >= mAnimationFrameSize){
         mState = EMOVE;
		 mRotation.mX = 0.0f;
		}
		break;
	case EATTACK1://攻撃
		if (mAttackCount>0) {
		ChangeAnimation(3, false, 20);//+4番目のアニメーションのフレーム３０
		}
		break;
	case EATTACK2://攻撃
		if (mAttackCount>0) {
		
		ChangeAnimation(5, false, 20);//+６番目のアニメーションのフレーム３０
		}
		break;
	case EATTACK3://攻撃
		if (mAttackCount>0) {
		
		ChangeAnimation(7, false, 30);//+８番目のアニメーションのフレーム３０
		
		}
		break;
	case EATTACKSP://攻撃
		if (mAttackCount>0) {
		ChangeAnimation(7, false, 100);//７番目のアニメーション５０フレームで
		
		}
		break;
	case EDAMAGED://ダメージ
		Damage.Play();
		ChangeAnimation(4, false, 10);
		break;
	case EDEATH://死亡
		ChangeAnimation(11, false, 60);
		break;
	}
	//アニメーションの種類
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
		//左
		if (CKey::Push('A'))
		{
				Move -= SideVec;
				mAnimationCount = 10;//0になるまでアニメーションを変更できない
				if (CKey::Push('C')) {
					if (mStamina > -1) {
						speed = 0.30f;//スピード倍
						mStamina -= 2;//スタミナ減少
					}
					else {
						speed = 0.05f;//スピード1/2
					}
				}
				
			if (CKey::Push(' ')) {
				speed= mStep;//攻撃時、進行方向にステップを踏む
			}
		}
		//右
		else if (CKey::Push('D'))
		{
			Move += SideVec;
			mAnimationCount = 5;//0になるまでアニメーションを変更できない
            if (CKey::Push('C')) {
		    	if (mStamina > -1) {
					speed = 0.30f;//スピード倍
					  mStamina-=2;//スタミナ減少	
				}

				else {
					speed = 0.05f;//スピード1/2
				}
			}
			if (CKey::Push(' ')) {
				speed=mStep;//攻撃時、進行方向にステップを踏む
			}
		}
		//前
		 if (CKey::Push('W'))
		{
			Move += FrontVec;
			//mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
			mAnimationCount = 10;//0になるまでアニメーションを変更できない

				if (CKey::Push('C')) {
					if(mStamina > -1) {
					speed = 0.30f;//スピード倍
					mStamina-=2;//スタミナ減少
				
					}

					else {
						speed = 0.05f;//スピード1/2
					}
				}
            if (CKey::Push(' ')) {
					speed= mStep;//攻撃時、進行方向にステップを踏む
		    } 
			
		}
		 //後ろ
		else if (CKey::Push('S'))
		{
			Move -= FrontVec;
			//mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
			mAnimationCount = 10;//0になるまでアニメーションを変更できない

				if (CKey::Push('C')) {
			      if (mStamina > -1) {

					speed = 0.30f;//スピード倍
					mStamina-=2;//スタミナ減少
				  }
				  else {
					speed = 0.05f;//スピード1/2
			      }
			    }
			
			if (CKey::Push(' ')) {
				speed = mStep;//攻撃時進行方向にステップを踏む
			}
		}
		 //一回目の攻撃
			 //一回目の攻撃のフレーム＜＝０かつ３回目の攻撃の総フレーム＞＝０
			 //１→２→３→１   攻撃の順番がループ
		 //無敵時間中じゃないとき
		 //if (mDamageCount <= 0) {
			 //攻撃１が使えるとき
			 if (mSpaceCount1 == 0) {

				 if (mAttackCount <= 0) {
					 if (CKey::Once(' '))
					 {
						 FirstAttack.Play();
						 mState = EATTACK1;
						 mSpaceCount1 = 1;//１回目の攻撃のフラグ
						 mSpaceCount2 = 0;
						mAttackCount = 10;//当たり判定が適用される時間
						 mAnimationCount = 50;//0になるまでアニメーションが変わらない
						 mStep = STEP;
					 }
				 }
			 }

			 //2回目の攻撃
			 else if (mSpaceCount2 == 0) {
				 if (mAttackCount <= 0) {
					 if (CKey::Once(' ')) {
						 SecondAttack.Play();
						 mState = EATTACK2;
						 mSpaceCount2 = 1;//２回目の攻撃のフラグ
						 mSpaceCount3 = 0;
						 mAttackCount = 10;//当たり判定が適用される時間
						 mAnimationCount = 50;//0になるまでアニメーションが変わらない
						 mStep = STEP;
					 }
				 }
			 }
			 //３回目の攻撃
			 else if (mSpaceCount3 == 0) {
				 if (mAttackCount <= 0) {
					 if (CKey::Once(' ')) {
						 ThirdAttack.Play();
						 mState = EATTACK3;
						 mAnimationCount = 50;//0になるまでアニメーションが変わらない
						 mSpaceCount3 = 1;//３回目の攻撃のフラグ
						 mSpaceCount1 = 0;
						 mAttackCount = 30;//当たり判定が適用される時間
						 mStep = STEP;//ジャンプ力を代入
					 }
				 }
			 }
		// }
		 //ジャンプ攻撃
		 if (mSpAttack >= 30) {
			 if (CKey::Once('F')) {
			     if (mAttackCount <= 0) {
					 JumpAttack.Play();
					 mState = EATTACKSP;
					 mJump = JUMP;//ジャンプ力を代入
					  mSpAttack -= 30;//特殊攻撃のゲージ減少
					 mAnimationCount = 200;//0になるまでアニメーションが変わらない
					 mAttackCount = 100;
					 //mPosition.mY = 1.0f;// mJump* mTime - 0.5 * mGravity * mTime * mTime;
				 }
			 }
		 }
		 //死亡
		 if (mHp <= 0) {
			 mState = EDEATH;
		  }
		 //移動中
		  if (Move.Length() != 0.0f) {
			  //Cキーを押すと回避行動
			  if (CKey::Once('C')){
				  if (mState !=EATTACKSP) {
					  mState = EESCAPE;
					  mAnimationCount = 20;
					  mDamageCount = 40;
					  mStep = STEP;//ジャンプ力を代入

					  mStamina -= 20;
				  }
				  //待機中のときにCを押しているとダッシュ
				  if (mState == EIDLE) {

					  mAnimationCount = 1;
					  mState = EDUSH;
				  }
			  }
			  //動いていないかつCキーを押していなければ待機
                  else {
					  if (mState == EIDLE) {
						  mAnimationCount = 1;
						  mState = EMOVE;
					  }
				  } 
		  }
		//移動量正規化　これをしないと斜め移動が早くなってしまうので注意
		//ジャンプ時などはY軸を正規化しないよう注意
		Move.Normalize();
		//平行移動量
		if (mSpeed < speed) {
			mSpeed += 0.01f;
		}
		else {
			mSpeed -= 0.01f;
		}
         Move = Move*mSpeed;
		//3次元ベクトル計算で算出したほうが正確だが計算量を懸念する場合は擬似計算で軽量化
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
		//移動
		if (mState == EMOVE||mState==EDUSH||mStep > 0) {
		
             mPosition += Move;
		}
		//回避行動時の移動量
		 if (mStep > 0) {
				mStep--;
		 }

		 if (mAnimationCount > 0) {
			 mAnimationCount--;
	     }
		 //アニメーションが終わるたびに攻撃モーション最初から
		 if (mAnimationCount <= 0) {
			 mSpaceCount1 = 0;
			 mSpaceCount2 = 0;
			 mSpaceCount3 = 0;
		 }
		 //スタミナ回復
		 if (mStamina < 1000) {
			mStamina++;
		 }
		 //攻撃アニメーション中
		 if (mAttackCount > 0) {
			 mAttackCount--;
		 }
		 //無敵時間
		 if (mDamageCount > 0) {
			 mDamageCount--;
		 }
		
		 

			 //マップに接触していない間ずっと重力がかかる
		
		 if (mState != EATTACKSP) {

			 if (mJump >= -0.1) {

			 mJump -= G;
			 }
		 }
		 if (mState != EESCAPE) {

			  mPosition.mY += mJump;
		 }
		// }
		//アイテム取得時に武器の当たり判定拡大
		 if (CItem::mItemCount > 0) {
			mColSphereSword.mRadius = 7.5f;
		 }
		 //吹き飛ぶ
		 if (mColliderCount > 0) {
			mColliderCount--;
			mPosition = mPosition + mCollisionEnemy * mColliderCount;
			
		 }

	    //注視点設定
	    Camera.SetTarget(mPosition);
	    CXCharacter::Update();
}

void CXPlayer::Collision(CCollider* m, CCollider* o) {

	//自身のコライダの設定
	switch (m->mType) {



	case CCollider::ESPHERE:

		//親がプレイヤー
		if (m->mpParent->mTag == EPLAYER) {
			//プレイヤーの体部分
			if (m->mTag == CCollider::EPLAYERBODY) {//相手のコライダが三角コライダの場合
				//親が三角コライダ
				if (o->mType == CCollider::ETRIANGLE) {
					//親が三角コライダ
					if (o->mpParent->mTag == EMAP) {

						CVector adjust;//調整用ベクトル
						if (CCollider::CollisionTriangleSphere(o, m, &adjust)) {

							if (mState != EESCAPE) {
								mGravity = 0;
								mJump = 0;
								//三角形と線分の衝突判定
								//CCollider::CollisionTriangleLine(o, m, &adjust);
								//位置の更新（mPosition+adjust)
								mPosition = mPosition + adjust;

								//行列の更新
								CTransform::Update();
							}

						}


					}
				}
				//球コライダ
				if (o->mType == CCollider::ESPHERE) {
					//親が敵（２）
					if (o->mpParent->mTag == EENEMY2) {
						//敵の攻撃部位との衝突判定
						if (o->mTag == CCollider::EENEMY2COLLIDERATTACK) {

							CVector adjust;//調整用ベクトル
							if (CCollider::Collision(m, o)) {
								//ダメージが入ったあとの無敵時間が０のとき
								if (mDamageCount == 0) {
									//敵の攻撃判定が適用されている間
									if (((CEnemy2*)(o->mpParent))->mEnemy2AttackHit == true)
									{
										if (mHp > 0) {
											//後ろに下がる
											mColliderCount = 5.0f;
											mCollisionEnemy = mPosition - o->mpParent->mPosition;
											mCollisionEnemy.mY = 0;
											mCollisionEnemy = mCollisionEnemy.Normalize();
											//体力減少
											mHp--;
											//無敵時間付与
											mDamageCount = 30;
											//ダメージ時の処理を開始
											mState = EDAMAGED;
										}
									}
								}
							}
						}
					}
					//親がボス
					else if (o->mpParent->mTag == EBOSS) {

						//ボスの体との衝突判定
						if (o->mTag == CCollider::EBOSSCOLLIDERHEAD) {
							if (CCollider::Collision(m, o)) {
								//後ろに下がる
								mColliderCount = 1.5f;
								mCollisionEnemy = mPosition - o->mpParent->mPosition;
								mCollisionEnemy.mY = 0;
								mCollisionEnemy = mCollisionEnemy.Normalize();
							}
						}
						//ボスの攻撃部位との衝突判定
						else if (o->mTag == CCollider::EBOSSCOLLIDERATTACK) {
							if (CCollider::Collision(m, o)) {
								//ダメージが入ったあとの無敵時間
								if (mDamageCount == 0) {
									//敵の攻撃判定が適用されている間
									if (((CBoss*)(o->mpParent))->mBossAttackHit == true)
									{

										if (mHp > 0) {
											//後ろに下がる
											mColliderCount = 5.0f;
											mCollisionEnemy = mPosition - o->mpParent->mPosition;
											mCollisionEnemy.mY = 0;
											mCollisionEnemy = mCollisionEnemy.Normalize();
											//体力減少
											mHp--;
											//無敵時間付与
											mDamageCount = 30;
											//ダメージ時の処理開始
											mState = EDAMAGED;
										}
									}

								}
							}
						}
					}
				}


			}
					//プレイヤーの剣
					else if (m->mTag == CCollider::EPLAYERSWORD) {
						//球コライダ
						if (o->mType == CCollider::ESPHERE) {
							//敵（２）
							if (o->mpParent->mTag == EENEMY2) {
								//敵のコライダ
								if (o->mTag == CCollider::EENEMY2COLLIDERATTACK) {
									if (mAttackHit == true) {
										if (CCollider::Collision(m, o)) {
											//o->mpParent->Collision(o, m);

												//特殊攻撃のゲージ増加
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
	//コライダの優先度変更
	//mColSphereBody.ChangePriority();
	mColSphereSword.ChangePriority();
	mColSphereFoot.ChangePriority();
	//mCollider.ChangePriority();
	mCollider2.ChangePriority();

	//衝突処理を実行
;
//CCollisionManager::Get()->Collision(&mColSphereBody, COLLISIONRANGE);
//CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
CCollisionManager::Get()->Collision(&mCollider2, COLLISIONRANGE);
CCollisionManager::Get()->Collision(&mColSphereFoot, COLLISIONRANGE);
CCollisionManager::Get()->Collision(&mColSphereSword, COLLISIONRANGE);




}
void CXPlayer::Render2D()
{
	//2D描画開始
	CUtil::Start2D(0, 800, 0, 600);

	//体力の割合
	float hpRate = (float)mHp / (float)HP_MAX;
	//体力ゲージの幅
	float hpGaugeWid = GAUGE_WID_MAX * hpRate;

	//スタミナの割合
	float staminaRate = (float)mStamina / (float)STAMINA_MAX;
	//スタミナゲージの幅
	float staminaGaugeWid = GAUGE_WID_MAX * staminaRate;

	mImageGauge.Draw(20, GAUGE_WID_MAX, 560, 590, 210, 290, 63, 0);	//ゲージ背景
	mImageGauge.Draw(20, hpGaugeWid, 560, 590, 0, 0, 0, 0);			//体力ゲージ

	mImageGauge.Draw(20, GAUGE_WID_MAX, 520, 550, 210, 290, 63, 0);	//ゲージ背景
	mImageGauge.Draw(20, staminaGaugeWid, 520, 550, 110, 190, 63, 0);	//スタミナゲージ


	CUtil::End2D();



}

