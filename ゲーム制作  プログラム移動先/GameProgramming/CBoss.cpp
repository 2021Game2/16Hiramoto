#include"CBoss.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CXCharacter.h"
#include"CUtil.h"
#include"CText.h"
#define HP 100

#define VELOCITY 0.5f //マクロ
#define HPCOUNT1 90 //ダメージを受けたときにのけぞりを行う体力の数値
#define HPCOUNT2 60 //ダメージを受けたときにのけぞりを行う体力の数値
#define HPCOUNT3 30 //ダメージを受けたときにのけぞりを行う体力の数値
#define JUMP 5.0f
#define G 0.1f
#define PLAYERSPPOINT_MAX 30
#define GAUGE_WID_MAXHP 700.0f	//HPゲージの幅の最大値
#define GAUGE_LEFT 20			//ゲージ描画時の左端

#define IMAGE_GAUGE "Resource\\png,tga\\Gauge.png"		//ゲージ画像
int CBoss::mBossAttackCount = 0;
int CBoss::mHp = HP;

 extern CSound BossVoice;
 extern CSound BossMove;
 CBoss* CBoss::mpBossInstance;
 //ボスのポインタを返すことで、座標などが参照できるようになる
 CBoss* CBoss::GetInstance()
 {
	 return mpBossInstance;
 }
CModel CBoss::mModel;//モデルデータ作成
//デフォルトコンストラクタ
CBoss::CBoss()
//コライダの設定
	: mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 20.0f)
	, mColSphereHead(this, &mMatrix, CVector(0.0f, 1.0f, 5.0f), 7.0f)
	, mColSphereRightFront(this, &mMatrix, CVector(0.0f, -2.0f, 0.0f), 2.0f)
	, mColSphereLeftFront(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
	, mpPlayer(0)
	, mJump2(0)
	, mEnemyDamage(60)
	, mMove(0)
	, mMove2(0)
	, mColliderCount(0.0f)
	, mGravity(0.0f)
	, mTime(0.0f)
	, mBossDamageCount(0)
	
	, mBossAttackHit(false)
	,mColSearchCount(false)
	, mBossBgm(true)
	, mBossBgmDeath(true)
{

	mImageGauge.Load(IMAGE_GAUGE);
	mGravity = 0.20f;
	mTag = EBOSS;
	mColSearch.mTag = CCollider::ESEARCH;//タグ設定
	mColSphereHead.mTag = CCollider::EBOSSCOLLIDERHEAD;
	mColSphereRightFront.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mColSphereLeftFront.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mpBossInstance = this;
	mRotation.mY += 180.0f;
	mGravity = 0.20f;
	mState = EIDLE;
}

//CEnemy(位置、回転、拡縮）
CBoss::CBoss(const CVector& position, const CVector& rotation, const CVector& scale)
	:CBoss()
{
	//位置、回転、拡縮を設定する
	mPosition = position;//位置の設定
	mRotation = rotation;//回転の設定
	mScale = scale;//拡縮の設定
	CTransform::Update();//行列の更新
	//目標地点の設定
	mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
	//優先度を１に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
}

void CBoss::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.mpMatrix = &mpCombinedMatrix[6];
    mColSphereRightFront.mpMatrix = &mpCombinedMatrix[12];//右前足
	mColSphereLeftFront.mpMatrix = &mpCombinedMatrix[19];//左前足
	mState = EATTACK2;


}
//待機処理
void CBoss::Idle() {
	//30溜まるまで待機のアニメーション
	ChangeAnimation(8, false, 60);
	mMove++;
	if (mMove >= 120) {
		//30溜まった状態でアニメーションが終わると攻撃処理に移行
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			if (mAttackPercent <= 5) {
			mState = EATTACK;
		    }
			else if(mAttackPercent > 5) {
				mState = EATTACK2;
			}
		}
	}
	//30溜まる前にアニメーションが終わったら移動処理に移行
	else if (mAnimationFrame >= mAnimationFrameSize) {
		if (mColSearch.mRenderEnabled == false) {
	    	mState = EAUTOMOVE;
		}
	}


}
//移動処理
void CBoss::AutoMove() {
	//歩く

	if (CSceneGame::mVoiceSwitch == true) {
		BossMove.Play();
	}
	//CXPlayerを使ったポインタにプレイヤーの情報を返す処理をさせる(CXPlayerの中の処理なのでポインタを作る必要あり）
	CXPlayer* tPlayer = CXPlayer::GetInstance();
	mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
	ChangeAnimation(4, true,180);
	//プレイヤーに向かって回転する処理
	//左向き（X軸）のベクトルを求める
	CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//上向き（Y軸）のベクトルを求める
	CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//前方向（Z軸）のベクトルを求める
	CVector vz = CVector(0.0f, 0.0f, 5.0f) * mMatrixRotate;
	//目標地点までのベクトルを求める
	CVector vp = mPoint - mPosition;
	//左ベクトルとの内積を求める
	float dx = vp.Dot(vx);
	//上ベクトルとの内積を求める
	float dy = vp.Dot(vy);
	//前ベクトルとの内積を求める
	float dz = vp.Dot(vz);
	float margin = 0.1f;
	//左右方向へ回転
	if (dx > margin) {
		mRotation.mY += 3.0f;//左へ回転
	}
	else if (dx < -margin) {
		mRotation.mY -= 3.0f;//右へ回転
	}
	CTransform::Update();//行列更新
	//定期的にプレイヤーの座標を記録
	int r = rand() % 60; //rand()は整数の乱数を返す
	//%180は１８０で割った余りを求める
	if (mColSearch.mRenderEnabled == false) {
		if (r == 0) {
				//ESEARCHに衝突してポインタに設定した
				//プレイヤーの座標を記録
				mPoint = tPlayer->mPosition;
		
		}
	}
	
}
//攻撃処理
void CBoss::Attack() {
	//攻撃アニメーション
	ChangeAnimation(5, false, 80);
	//攻撃のあとは移動処理に移行
	if (mAnimationFrame >= mAnimationFrameSize) {
			mMove = 0;//攻撃のアニメーションのあとは移動のアニメーションに切り替わる
	}

}
//攻撃処理
void CBoss::Attack2() {
	//攻撃アニメーション
	ChangeAnimation(6, false, 80);
	//攻撃のあとは移動処理に移行
	if (mAnimationFrame>=mAnimationFrameSize) {
		mMove = 0;//攻撃のアニメーションのあとは移動のアニメーションに切り替わる
	}
}
//ダメージ処理
void CBoss::Damaged() {
	//体力減少
	if (mHp <= 0) {
		mState = EDEATH;
	}
	else {
    //ダメージのあとは移動処理
	mState = EIDLE;
	}
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition = mPosition + mCollisionEnemy * mColliderCount;
	}
}
//死亡処理
void CBoss::Death() {
	if (mBossBgmDeath == true) {
		CSceneGame::mBgmCountCheck = false;
		CSceneGame::mBgmCount = 4;
		mBossBgmDeath = false;
	}
	ChangeAnimation(9, false, 250);
	//体力がなくなったら
	if (mHp <= 0) {
		mHp--;
		//15フレームごとにエフェクト
		if (mHp % 15 == 0) {
			//エフェクト生成
			new CEffect2(mPosition, 1.0f, 1.0f, CEffect2::EFF_EXP, 4, 4, 2);
		}
		CTransform::Update();
	}
	//しばらく経ったら消去
	if (mHp <= -250) {
		mEnabled = false;
	}
	CXCharacter::Update();
}

//更新処理
void CBoss::Update() {
	//処理を行動ごとに分割
	switch (mState) {
	case EIDLE:	//待機
		Idle();
		break;
	case EAUTOMOVE://移動
		AutoMove();
		break;
	case EATTACK://攻撃1
		Attack();
		break;
	case EATTACK2://攻撃２
		Attack2();
		break;
	case EDAMAGED://ダメージ
		Damaged();
		break;
	case EDEATH://死亡
		Death();
		break;
	}
	//アニメーションの種類
	switch (mAnimationIndex) {
	case(5):
		if (mAnimationFrame == 30) {
			mBossAttackHit = true;
			if (CSceneGame::mVoiceSwitch == true) {
				BossVoice.Play();
			}
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mBossAttackHit = false;
			mState = EIDLE;
		}
		break;
	case(6):
		if (mAnimationFrame == 30) {
			mBossAttackHit = true;
			if (CSceneGame::mVoiceSwitch == true) {
			  BossVoice.Play();
			}
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mBossAttackHit = false;
			mState = EIDLE;
		}
		break;
	}
		
	if (mAttackPercent < 10) {
		mAttackPercent++;
	}
	if (mAttackPercent >= 10) {
		mAttackPercent = 0;
	}
	if (mBossDamageCount > 0) {
		mBossDamageCount--;
	}
	if (mBossDamageCount > 0) {
		if (mEffectCount % 15 == 0) {
			//エフェクト生成
			//足に攻撃されたとき
			if (mBossColliderCheck == 1) {
				CXPlayer* tPlayer = CXPlayer::GetInstance();

				mBossEffect = new CEffect2(tPlayer->mPosition, 3.0f, 3.0f, CEffect2::EFF_EXP, 4, 4, 2);
			}
			//頭に攻撃されたとき
			else if (mBossColliderCheck == 2) {
				CXPlayer* tPlayer = CXPlayer::GetInstance();
			    mBossEffect=new CEffect2(CVector(tPlayer->mPosition.mX, tPlayer->mPosition.mY+1.0f, tPlayer->mPosition.mZ), 3.0f, 3.0f, CEffect2::EFF_EXP, 4, 4, 2);
			}
		}
	}
	if (mHp <= 0 && mState != EDEATH) {
		mState = EDEATH;
	}

	mEffectCount--;
	mGravity  -= G;
	mPosition.mY += mGravity;
	CXCharacter::Update();
}

//Collision(コライダ１，コライダ２，）
void CBoss::Collision(CCollider* m, CCollider* o) {
	//コライダのとき
	m->mType = CCollider::ESPHERE;
		//自分がサーチ用のとき
		if (m->mTag == CCollider::ESEARCH) {
			//相手が弾コライダのとき
			if (o->mType == CCollider::ESPHERE) {
				//相手がプレイヤーのとき
				if (o->mpParent->mTag == EPLAYER) {
					if (o->mTag == CCollider::EPLAYERBODY) {
						//衝突しているとき
						if (CCollider::Collision(m, o)) {
							//ポインタをプレイヤーに設定
							mColSearchCount = true;
							if (mColSearch.mRenderEnabled == true) {
								if (mBossBgm == true) {
									CSceneGame::mBgmCountCheck = false;
									CSceneGame::mBgmCount = 3;
									mBossBgm = false;
								}
								mColSearch.mRenderEnabled = false;
							}
						}
					}
				}
			}
			return;
		}
		//ボスのコライダー
		if (m->mTag== CCollider::EBOSSCOLLIDERATTACK|| CCollider::EBOSSCOLLIDERHEAD) {
			if (o->mType == CCollider::ESPHERE) {
				//プレイヤー
				if (o->mpParent->mTag == EPLAYER) {
					//相手が武器のとき
					//剣とハンマー（アイテム）
					if (o->mTag == CCollider::EPLAYERSWORD || o->mpParent->mTag == EITEM) {
						//衝突しているとき
							if (CCollider::Collision(m, o)) {
								
								//親をCXPlayerを元にポインタ化し、変数を参照
								if (((CXPlayer*)(o->mpParent))->mAttackHit == true)
								{
									if (m->mTag == CCollider::EBOSSCOLLIDERATTACK) mBossColliderCheck = 1;
									else if (m->mTag == CCollider::EBOSSCOLLIDERHEAD) mBossColliderCheck = 2;
									//爆発エフェクト秒数付与
									mEffectCount = 60;
									if (mHp > 0) {
										if (((CXPlayer*)(o->mpParent))->mSpAttack < PLAYERSPPOINT_MAX) {

											((CXPlayer*)(o->mpParent))->mSpAttack++;
										}
										//30％減るごとにのけぞる
										if (mHp ==HPCOUNT1|| mHp == HPCOUNT2|| mHp == HPCOUNT3) {
											mColliderCount = 10;
											mCollisionEnemy = mPosition - o->mpParent->mPosition;
											mCollisionEnemy.mY = 0;
											mCollisionEnemy = mCollisionEnemy.Normalize();
											mState = EDAMAGED;
										}
										if (mBossDamageCount <= 0) {
											mHp--;
											mBossDamageCount = 10;
										}
								    }
										
								}
							}
						
					}
					//相手がESTOPPERの時
					if (o->mTag == CCollider::EPLAYERBODY) {

						if (CCollider::Collision(m, o)) {
							
							if (mState != EATTACK) {
								if (mState != EATTACK2) {
									if (mState != EIDLE) {
										if (mHp > 0) {
											mState = EIDLE;
										}
									}
								}

							}
							mColliderCount = 1.5f;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy = mCollisionEnemy.Normalize();
						}
					}
				}
			}
		}
		if (m->mTag == CCollider::EBOSSCOLLIDERHEAD) {
			if (o->mType == CCollider::ETRIANGLE) {
				CVector adjust;//調整値
				//三角コライダと球コライダの衝突判定
				//adjust、、、調整値	
				if (CCollider::CollisionTriangleSphere(o, m, &adjust))
				{
					mPosition = mPosition + adjust;
					mGravity = 0;
				}
			}
		}
		return;
}
void CBoss::TaskCollision() {
	//コライダの優先度変更
	mColSearch.ChangePriority();
	mColSphereHead.ChangePriority();
	mColSphereRightFront.ChangePriority();
	mColSphereLeftFront.ChangePriority();
	//衝突処理を実行

	CCollisionManager::Get()->Collision(&mColSphereRightFront, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereLeftFront, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereHead, COLLISIONRANGEFIELD);
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
}

void CBoss::Render2D()
{
	//2D描画開始
	CUtil::Start2D(0, 800, 0, 600);
	//体力の割合
	float hpRate = (float)mHp / (float)HP;
	//体力ゲージの幅
	float hpGaugeWid = GAUGE_WID_MAXHP * hpRate;
	if (CSceneGame::mBossGaugeSwitch == true) {
		mImageGauge.Draw(50, GAUGE_WID_MAXHP, 510, 550, 201, 300, 63, 0);//ゲージ背景
		mImageGauge.Draw(50, hpGaugeWid, 510, 550, 487, 572, 63, 0);//体力ゲージ
	}

	CUtil::End2D();
}
