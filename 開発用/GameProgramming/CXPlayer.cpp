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
#define JUMP 5.0f//スペシャル攻撃時のジャンプ力
#define STEP  20.0f //攻撃時少し前進
#define STEP2 0.2f //回避行動時少し前進
#define STAMINA 400 //スタミナ
#define HP_MAX 10	//体力最大値
#define STOPPER 0.1f
#define STAMINA_MAX 1000 //スタミナ最大値
#define SPPOINT_MAX 30 //SPポイント最大値
#define GAUGE_WID_MAXHP 400.0f	//HPゲージの幅の最大値
#define GAUGE_WID_MAXST 400.0f //スタミナゲージの幅の最大値
#define GAUGE_WID_MAXSP 300.0f//SPポイントゲージの幅の最大値
#define GAUGE_LEFT 20			//ゲージ描画時の左端
#define STEPG 0.9f
#define IMAGE_GAUGE "Resource\\png,tga\\Gauge.png"		//ゲージ画像
#define G 0.1f//重力
#define G2 1.5f//スペシャル攻撃時の重力
#define G3 0.01f


extern CSound PlayerFirstAttack;//一回目の攻撃のSE
extern CSound PlayerSecondAttack;//二回目の攻撃のSE
extern CSound PlayerThirdAttack;//三回目の攻撃のSE
extern CSound PlayerJumpAttack;//スペシャル攻撃のSE
extern CSound PlayerDamage;//ダメージのSE

CXPlayer* CXPlayer::mpPlayerInstance;
//プレイヤーのポインタを返すことで、座標などが参照できるようになる
CXPlayer* CXPlayer::GetInstance()
{
	return mpPlayerInstance;
}

CXPlayer::CXPlayer()
	:mColSphereSword(this, &mMatrix, CVector(-10.0f, 10.0f, 50.0f), 2.5f)//剣のコライダ１
	,mColSphereFoot(this, &mMatrix, CVector(0.0f, 0.0f, -3.0f), 2.0f)//足付近のコライダ
	,mColliderSwordSp(this, &mMatrix, CVector(0.0f, -2.0f, 0.0f), 10.0f)//剣のコライダ２
	,mColEscapeStopperLine(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f),CVector(0.0f,0.0f,0.0f))//回避時にすり抜けないようにする線分コライダ
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
	//タグにプレイヤーを設定します
	mTag = EPLAYER;
	mColSphereSword.mTag = CCollider::EPLAYERSWORD;
	mColliderSwordSp.mTag = CCollider::EPLAYERSWORD;
	mColSphereFoot.mTag = CCollider::EPLAYERBODY;
	mColEscapeStopperLine.mTag = CColliderLine::EPLAYERESCAPESTOPPER;
	//this＝プレイヤーそのもの
	mpPlayerInstance = this;
	mImageGauge.Load(IMAGE_GAUGE);
	//優先度を１に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
}
//剣のコライダの座標を取得する
CVector CXPlayer::GetSwordColPos()
{
	return mColSphereSword.mpMatrix->GetPos();	//剣のコライダの座標を返す
}
void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//３Dモデルの剣のボーンに場所を合わせる
	mColSphereSword.mpMatrix = &mpCombinedMatrix[22];
	//3Dモデルの足のボーンに場所に合わせる
	mColSphereFoot.mpMatrix = &mpCombinedMatrix[2];
	mColEscapeStopperLine.mpMatrix = &mpCombinedMatrix[2];
	//待機状態から始まる
	mState = EIDLE;
	//最初は表示させない
	mColliderSwordSp.mRenderEnabled = false;
}

void CXPlayer::Update()
{
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	CCamera* tCamera = CCamera::GetInstance();
	//処理を行動ごとに分割
	switch (mState) {
	case EIDLE:	//待機
		ChangeAnimation(0, true, 60);//待機アニメーション
		mSpeed = 0;//進まない
		break;
	case EMOVE://歩行
		mAnimationFrameSize = 60;
		//攻撃判定なし
		mAttackHit = false;
			 if (CKey::Push('C')) {
				 //アニメーションの速度を半分にしてEDUSHに移行
				 mAnimationFrameSize = 30;
					 mState = EDUSH;
			 }
			 else {
             ChangeAnimation(1, true, 60);
			 }
			 //アニメーションの終了を待たずに待機状態に移行
			if (mAnimationCount <= 0) {
				mMoveCheck = false;
				mState = EIDLE;
		    }
		break;
	case EDUSH://ダッシュ
		//攻撃判定なし
        mAttackHit = false;
		
		if (CKey::Push('C')) {
        mStamina -= 2;//スタミナ減少
		}
		//Cを押していないなら歩行
		else {
			mState = EMOVE;
		}
			ChangeAnimation(1, true, 30);

			//アニメーションの終了を待たずに待機状態に移行
			if (mAnimationCount <= 0) {	
				mState = EIDLE;
			}
		break;
		//回避
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
		//回転（回避してるように見える）
		if (mRotation.mX!=360.0f) {
		   mRotation.mX += 36.0f;
		}
		else if(mAnimationFrame >= mAnimationFrameSize){
         mState = EMOVE;//待機状態に移行
		 mEscapeFlg = true;
		 //回転値を元に戻す
		 mRotation.mX = 0.0f;
		}
		break;
	case EATTACK1://攻撃1
		if (mAttackCount>ATTACKCOUNT1/2) {
		ChangeAnimation(3, false, 20);
		}
		break;
	case EATTACK2://攻撃2
		if (mAttackCount>ATTACKCOUNT2/2) {
		
		ChangeAnimation(5, false, 20);//+６番目のアニメーションのフレーム３０
		}
		break;
	case EATTACK3://攻撃3
		if (mAttackCount>ATTACKCOUNT3/2) {
		
		ChangeAnimation(7, false, 30);
		
		}
		break;
	case EATTACKSP://攻撃
		tCamera->mAngleY -= 0.01f;
		if (mAttackCount>0) {
		ChangeAnimation(7, false, 100);
		
		}
		break;
	case EDAMAGED://ダメージ

		if (tSceneGame->mVoiceSwitch == true) {
			if (mDamageVoise == true) {

			  PlayerDamage.Play();
			  mDamageVoise = false;
			}
		}

		ChangeAnimation(4, false, 60);
		break;
	case EDEATH://死亡
		if (mPlayerBgm == true) {
			tSceneGame->mBgmCountCheck = false;
			tSceneGame->mBgmCount = 5;

			mPlayerBgm = false;
		}
		ChangeAnimation(11, false, 60);
		break;
	}
	//アニメーションの種類
	switch (mAnimationIndex) {
	case(3): 
		//アニメーションのロックが外れているとき
		if (mAnimationFrameLock == false) {
			//エフェクトのストッパーが外れているとき
			if (mEffectStopper==false) {
				//エフェクトが表示されるまでアニメーションを進めない
				if (mAnimationFrame >= 15) {
					mAnimationFrameLock = true;
					mEffectStopper = true;
				}
			}
		}
		//mEffectStopperをつかって一度だけエフェクトが出るように
		if (mAnimationFrameLock == true) {
			//エフェクト生成
			//剣コライダの座標を参照
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
		//アニメーションのロックが外れているとき
		if (mAnimationFrameLock == false) {
			//エフェクトのストッパーが外れているとき
			if (mEffectStopper == false) {
				//エフェクトが表示されるまでアニメーションを進めない
				if (mAnimationFrame >= 15) {
					mAnimationFrameLock = true;
					mEffectStopper = true;
				}
			}
		}
		//mEffectStopperをつかって一度だけエフェクトが出るように
		if (mAnimationFrameLock == true) {
			//剣コライダの座標を参照
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

		//アニメーションのロックが外れているとき
		if (mAnimationFrameLock == false) {
			//エフェクトのストッパーが外れているとき
			if (mEffectStopper == false) {
				//エフェクトが表示されるまでアニメーションを進めない
				if (mAnimationFrame >= 15) {
					mAnimationFrameLock = true;
					mEffectStopper = true;
				}
			}
		}
		//アニメーションを止めてからエフェクト再生
		if (mAnimationFrameLock == true) {
					//三回目の攻撃のときだけエフェクト再生
					if (mState == EATTACK3) {
						//剣コライダの座標を参照
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
				//剣コライダの座標を参照
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
	//攻撃判定が有効のとき
	
	//ロックがかかっている間はアニメーションが進まないようにする
	if (mAnimationFrameLock == true) {
		mAnimationFrame--;
	}
		//カメラ視点移動　通称無双移動
		//カメラの左右と前後のベクトルを取得
		CVector SideVec = Camera->GetMat().GetXVec();
		CVector FrontVec = Camera->GetMat().GetZVec();
		//高さ移動はカットする
		SideVec.mY = 0.0f;
		FrontVec.mY = 0.0f;
		//正規化する
		SideVec.Normalize();
		FrontVec.Normalize();
		float speed = 0.15f;
		CVector Move(0, 0, 0);
		if (mHp > 0) {


			//左
			if (CKey::Push('A'))
			{
				//左方向に移動
				Move -= SideVec;
				mAnimationCount = 5;//0になるまでアニメーションを変更できない
				mMoveCheck = true;
				if (CKey::Push('C')) {
					if (mState == EDUSH) {


						if (mStamina > -1) {

							speed = 0.5f;//スピード倍
						}
						else {
							speed = 0.2f;//スピード1/2
						}
					}
				}

			}
			//右
			else if (CKey::Push('D'))
			{
				Move += SideVec;
				mAnimationCount = 5;//0になるまでアニメーションを変更できない
				mMoveCheck = true;
				if (CKey::Push('C')) {
					if (mState == EDUSH) {
						if (mStamina > -1) {
							speed = 0.5f;//スピード倍
						}
						else {
							speed = 0.2f;//スピード1/2
						}
					}
				}
			}
			//前
			if (CKey::Push('W'))
			{
				Move += FrontVec;
				mAnimationCount = 5;//0になるまでアニメーションを変更できない
				mMoveCheck = true;
				if (CKey::Push('C')) {
					if (mState == EDUSH) {
						if (mStamina > -1) {
							speed = 0.5f;//スピード倍
						}

						else {
							speed = 0.2f;//スピード1/2
						}
					}
				}

			}
			//後ろ
			else if (CKey::Push('S'))
			{
				Move -= FrontVec;
				mAnimationCount = 5;//0になるまでアニメーションを変更できない
				mMoveCheck = true;
				if (CKey::Push('C')) {
					if (mState == EDUSH) {
						if (mStamina > -1) {
							speed = 0.5f;//スピード倍
						}
						else {
							speed = 0.2f;//スピード1/2
						}
					}
				}
			}

			//１→２→３→１   攻撃の順番がループ

		   //回避行動中じゃないとき
			if (mState != EESCAPE) {
				//攻撃１が使えるとき
				if (mSpaceCount1 == true) {

					if (mAttackCount <= 0) {
						if (CKey::Once(VK_LBUTTON))
						{

							if (tSceneGame->mVoiceSwitch == true) {
								PlayerFirstAttack.Play();
							}
							mState = EATTACK1;
							mSpaceCount1 = false;//１回目の攻撃のフラグ
							mSpaceCount2 = true;
							mAttackCount = ATTACKCOUNT1;//攻撃のアニメーションがループしないように
							mAnimationCount = 50;//0になるまでアニメーションが変わらない
							//mMoveAttack.Normalize();
							mStep = STEP;
						}

					}
				}
				//2回目の攻撃
				else if (mSpaceCount2 == true) {
					if (mAttackCount <= 0) {
						if (CKey::Once(VK_LBUTTON)) {

							if (tSceneGame->mVoiceSwitch == true) {
								PlayerSecondAttack.Play();
							}
							mState = EATTACK2;
							mSpaceCount2 = false;//２回目の攻撃のフラグ
							mSpaceCount3 = true;//３回目の攻撃のフラグ
							mAttackCount = ATTACKCOUNT2;//攻撃のアニメーションがループしないように
							mAnimationCount = 50;//0になるまでアニメーションが変わらない
							mStep = STEP;
						}
					}
				}
				//３回目の攻撃
				else if (mSpaceCount3 == true) {
					if (mAttackCount <= 0) {
						if (CKey::Once(VK_LBUTTON)) {

							if (tSceneGame->mVoiceSwitch == true) {
								PlayerThirdAttack.Play();
							}
							mState = EATTACK3;
							mAnimationCount = 50;//0になるまでアニメーションが変わらない
							mSpaceCount3 = false;//３回目の攻撃のフラグ
							mSpaceCount1 = true;//１回目の攻撃のフラグ
							mAttackCount = ATTACKCOUNT3;//攻撃のアニメーションがループしないように
							mStep = STEP;//ジャンプ力を代入
						}
					}
				}

				//ジャンプ攻撃
				if (mSpAttack >= 30) {
					if (CKey::Once(VK_RBUTTON)) {
						if (mAttackCount <= 0) {

							if (tSceneGame->mVoiceSwitch == true) {
								PlayerJumpAttack.Play();
							}
							mState = EATTACKSP;
							mAttackSp=true;
							mJump = JUMP;//ジャンプ力を代入
							mSpAttack -= 30;//特殊攻撃のゲージ減少
							mAnimationCount = 200;//0になるまでアニメーションが変わらない
							mAttackCount = 100;
						}
					}
				}

			}
			//移動中
			if (Move.Length() != 0.0f) {
				//Cキーを押すと回避行動
				if (CKey::Once('C')) {
					if (mState != EATTACKSP) {
						if (mAttackCount <= 0) {
							if (mDamageCount <= 0) {
								mState = EESCAPE;
								mAnimationCount = 20;
								mDamageCount = 40;//無敵時間
								mStamina -= 20;//スタミナ使用
							}
						}
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
			if (CKey::Once('C')) {
				if (Move.Length() != 0.0f) {
					mState = EDUSH;
				}
				
			}
		}
		//移動量正規化　斜め移動が早くなってしまう
		//ジャンプ時などはY軸を正規化しない
		Move.Normalize();
		//平行移動量
		//設定した移動量になるまで加速
		if (mSpeed < speed) {
			mSpeed += 0.01f;
		}
		//減速
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
		if (mState == EMOVE||mState==EDUSH||mState==EATTACKSP||mStep > 0) {
		
             mPosition += Move;
		}
		//Move = Move * mMoveAttack;
		//mMoveAttack.mZ * mStep;
		//mStep* STEPG;
		//回避行動時の移動量(だんだん遅くなる）
		 if (mStep > 0) {
			 Move.mZ += mStep;
				mStep--;
		 }

		 if (mAnimationCount > 0) {
			 mAnimationCount--;
	     }
		 //アニメーションが終わるたびに攻撃モーション最初から
		 if (mState != EATTACK1 && mState != EATTACK2 && mState != EATTACK3) {
			 if (mAttackCount <= 0) {
				 mSpaceCount1 = true;
				 mSpaceCount2 = false;
				 mSpaceCount3 = false;
			 }

		 }
		 //スタミナ回復
		 if (mStamina < STAMINA_MAX) {
			mStamina++;
		 }
		 if (mSpAttack > SPPOINT_MAX) {
		    mSpAttack = SPPOINT_MAX;
		 }
		 //攻撃アニメーション中
		 if (mAttackCount > 0) {
			 mAttackCount--;
		 }
		 //無敵時間
		 if (mDamageCount > 0) {
			 mDamageCount--;
		 }

		 //死亡
		 if (mHp <= 0) {
			 mState = EDEATH;
		 }
		 //マップに接触していない間ずっと重力がかかる
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
		 
		 //吹き飛ぶ
		 if (mColliderCount > 0) {
			mColliderCount-=0.2f;
			mPosition = mPosition + mCollisionEnemy * mColliderCount;
		 
		 }
		 mColEscapeStopperLine.Set(this, &mMatrix, CVector(0.0f, 0.0f, -2.0f), CVector(0.0f, 0.0f,0.0f));
		 if (mState == EESCAPE) {
			 mPosition.mY -= G3;
		 }
	    //注視点設定
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

	//自身のコライダの設定
	switch (m->mType) {


	case CCollider::ELINE:
		//親がプレイヤー
		if (m->mpParent->mTag == EPLAYER) {
			//プレイヤーの体部分
			if (m->mTag == CColliderLine::EPLAYERESCAPESTOPPER) {//相手のコライダが三角コライダの場合
				//親が三角コライダ
				if (o->mType == CCollider::ETRIANGLE) {
					
					CVector adjust;//調整用ベクトル
					if (CCollider::CollisionTriangleLine(o, m, &adjust)) {
					    if (mState == EESCAPE) {
							//位置の更新（mPosition+adjust)
							mPosition = mPosition + adjust;
							//行列の更新
							CTransform::Update();
						}
					}
				}
			}
		}
		break;
	case CCollider::ESPHERE:
		//親がプレイヤー
		if (m->mpParent->mTag == EPLAYER) {
			//プレイヤーの体部分
			if (m->mTag == CCollider::EPLAYERBODY) {//相手のコライダが三角コライダの場合
				//親が三角コライダ
				if (o->mType == CCollider::ETRIANGLE) {
					//親が三角コライダ
		            //左向き（X軸）のベクトルを求める
					CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
					//上向き（Y軸）のベクトルを求める
					CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
					//前方向（Z軸）のベクトルを求める
					CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;
					vz.Normalize();
						CVector adjust;//調整用ベクトル
						if (CCollider::CollisionTriangleSphere(o, m, &adjust)) {
								if (mState != EESCAPE) {

									mJumpStopper = true;
									mJump =0;
									if (mAnimationIndex == 8) {
										mAnimationFrame += 2;
									}
									//位置の更新（mPosition+adjust)
									mPosition = mPosition + adjust;
									//行列の更新
									CTransform::Update();
								}
								
						}
						else {

							mJumpStopper = false;
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
								if (mDamageCount == 0&&mState!=EESCAPE) {
									//敵の攻撃判定が適用されている間
									if (((CEnemy2*)(o->mpParent))->mEnemy2AttackHit == true)
									{
										if (mHp > 0) {
											//後ろに下がる
											mColliderCount = 2.0f;
											mCollisionEnemy = mPosition - o->mpParent->mPosition;
											mCollisionEnemy.mY = 0;
											mCollisionEnemy = mCollisionEnemy.Normalize();
											//体力減少
											mHp--;
											//無敵時間付与
											mDamageCount = 60;
											//ダメージ時の処理を開始
											mDamageVoise = true;
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
								mColliderCount = 0.5f;
								mCollisionEnemy = mPosition - o->mpParent->mPosition;
								mCollisionEnemy.mY = 0;
								mCollisionEnemy = mCollisionEnemy.Normalize();
							}
						}
						//ボスの攻撃部位との衝突判定
						else if (o->mTag == CCollider::EBOSSCOLLIDERATTACK) {
							if (CCollider::Collision(m, o)) {
								//ダメージが入ったあとの無敵時間
								if (mDamageCount == 0&&mState != EESCAPE) {
									//敵の攻撃判定が適用されている間
									if (((CBoss*)(o->mpParent))->mBossAttackHit == true)
									{
										if (mHp > 0) {
											//後ろに下がる
											mColliderCount = 3.0f;
											mCollisionEnemy = mPosition - o->mpParent->mPosition;
											mCollisionEnemy.mY = 0;
											mCollisionEnemy = mCollisionEnemy.Normalize();
											//体力減少
											mHp--;
											//無敵時間付与
											mDamageCount = 30;
											//ダメージ時の処理開始
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
	//コライダの優先度変更
	mColSphereSword.ChangePriority();
	mColSphereFoot.ChangePriority();
	mColliderSwordSp.ChangePriority();
	mColEscapeStopperLine.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mColliderSwordSp, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereFoot, COLLISIONRANGEFIELD);
	CCollisionManager::Get()->Collision(&mColSphereSword, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColEscapeStopperLine, COLLISIONRANGE);
}
void CXPlayer::Render2D()
{
	//2D描画開始
	CUtil::Start2D(0, 800, 0, 600);
	//体力の割合
	float hpRate = (float)mHp / (float)HP_MAX;
	//体力ゲージの幅
	float hpGaugeWid = GAUGE_WID_MAXHP * hpRate;
	//スタミナの割合
	float staminaRate = (float)mStamina / (float)STAMINA_MAX;
	//スタミナゲージの幅
	float staminaGaugeWid = GAUGE_WID_MAXST * staminaRate;

	float spRate = (float)mSpAttack / (float)SPPOINT_MAX;
	float spGaugeWid = GAUGE_WID_MAXSP * spRate;

	if (mGaugeEnabled==true) {
		/*
		mImageGauge.Draw(20, GAUGE_WID_MAXHP, 500, 510, 201, 300, 63, 0);//ゲージ背景
		mImageGauge.Draw(20, hpGaugeWid, 500, 510, 0, 99, 63, 0);//体力ゲージ

		mImageGauge.Draw(20, GAUGE_WID_MAXST, 490, 500, 201, 300, 63, 0);//ゲージ背景
		mImageGauge.Draw(20, staminaGaugeWid, 490, 500, 100, 200, 63, 0);//スタミナゲージ

		mImageGauge.Draw(20, GAUGE_WID_MAXSP, 480, 490, 201, 300, 63, 0);//ゲージ背景
		mImageGauge.Draw(20, spGaugeWid, 480, 490, 401, 486, 63, 0);//SPゲージ
		*/
	}

	CUtil::End2D();



}

