#include"CBoss.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CXCharacter.h"
#include"CUtil.h"
#include"CText.h"
#define HP 20

#define VELOCITY 0.5f //マクロ
#define HPCOUNT1 15 //ダメージを受けたときにのけぞりを行う体力の数値
#define HPCOUNT2 10 //ダメージを受けたときにのけぞりを行う体力の数値
#define HPCOUNT3 5 //ダメージを受けたときにのけぞりを行う体力の数値
#define JUMP 5.0f //ジャンプ攻撃時のジャンプ力
#define JUMP2 2.5f //ひっかき攻撃時のジャンプ力
#define G 0.1f //重力
#define G2  0.2f //ひっかき攻撃時の重力
#define GAUGE_WID_MAXHP 700.0f	//HPゲージの幅の最大値
#define GAUGE_LEFT 20			//ゲージ描画時の左端
#define ATTACKSELECT 4  //攻撃の種類
#define ROTATIONBASE 14400.0f//回転攻撃の総フレーム
#define ROTATIONCOUNT 36.0f//回転攻撃の際、アニメーションの終了と回転の終了をあわせる
#define ROTATIONCOUNTM -36.0f
#define ROTATION 3.6f//攻撃時に回転する量
#define ROTATIONMIN -0.36f
#define ROTATIONMIN2 -0.1f
#define ROTATIONBEFOREATTACK -3.6f
#define IMAGE_GAUGE "Resource\\png,tga\\Gauge.png"		//ゲージ画像


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
	, mColSphereHead(this, &mMatrix, CVector(0.0f, 3.0f, 5.0f), 5.0f)
	, mColSphereRightFront(this, &mMatrix, CVector(0.0f, -2.0f, 0.0f), 2.0f)
	, mColSphereLeftFront(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
	, mColSphereAttack(this, &mMatrix, CVector(0.0f, 0.0f, 10.0f), 10.0f)
	, mpPlayer(0)
	, mMove(0)
	, mMove2(0)
	, mBossDamageCount(0)
	, mBossJumpCount(0)
	, mBossAttackMove(0)
	, mEnemyDamage(60)
	, mJump(0.0f)
	, mColliderCount(0.0f)
	, mTime(0.0f)
	, mHp(HP)
	, mJumpStopper(true)
	, mBossBgm(true)
	, mBossBgmDeath(true)
	, mBossAttackHit(false)
	, mColSearchCount(false)
	
{

	mImageGauge.Load(IMAGE_GAUGE);//体力ゲージのテクスチャ
	
	mTag = EBOSS;
	mColSearch.mTag = CCollider::ESEARCH;//タグ設定
	mColSphereHead.mTag = CCollider::EBOSSCOLLIDERHEAD;
	//mColSphereRightFront.mTag = CCollider::EBOSSCOLLIDERATTACK;
	//mColSphereLeftFront.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mColSphereAttack.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mpBossInstance = this;
	mRotation.mY += 180.0f;//後ろを向いておく
	mState = EIDLE;//最初は待機状態
	mColSphereAttack.mRenderEnabled = false;//最初は表示させない
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
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
	//頭のコライダ
	mColSphereHead.mpMatrix = &mpCombinedMatrix[6];
	//ジャンプ攻撃をするときのコライダ
	mColSphereAttack.mpMatrix = &mpCombinedMatrix[6];
    mColSphereRightFront.mpMatrix = &mpCombinedMatrix[12];//右前足
	mColSphereLeftFront.mpMatrix = &mpCombinedMatrix[19];//左前足
	

}
//待機状態
void CBoss::Idle() {
	mPosition.mY -= G;//重力をかける
	//mMoveが120溜まるまで待機のアニメーション
	ChangeAnimation(8, true, 60);
	mMove++;

	if (mMove >= 120) {
		//120溜まった状態でアニメーションが終わると攻撃処理に移行
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			int num = rand() % ATTACKSELECT + 1;
			 
				mAttackPercent = num;
				switch (mAttackPercent) {
				case(0):
					if (mColSearch.mRenderEnabled == false) {
						mState = EIDLE;
					}
					else {
						mState = EAUTOMOVE;
					}
					break;
				case(1):
					if (mColSearch.mRenderEnabled == false) {
						mState = EATTACK;
						mBossAttackMove = 0;
					}
					else {
						mState = EIDLE;
					}
					break;
				case(2):
					if (mColSearch.mRenderEnabled == false) {
						mState = EATTACK2;
						mBossAttackMove = 0;
					}
					else {
						mState = EIDLE;
					}
					break;
				case(3):
					if (mColSearch.mRenderEnabled == false) {
					mJumpStopper = false;
					mJump = JUMP;
					mState = EATTACK3;
					}
					else {
					 mState = EIDLE;

					}
					break;
				case(4):
					if (mColSearch.mRenderEnabled == false) {
						mRotationCount = ROTATIONBASE;
						mAttack4Count = 1;
						mAttack4RotationCount = 0.0f;
						mState = EATTACK4;
					}
					else {
						mState = EIDLE;
					}
					break;
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
	mPosition.mY -= G;
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	//歩く
	if (tSceneGame->mVoiceSwitch == true) {
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
//攻撃処理(ひっかき攻撃）
void CBoss::Attack() {
	//攻撃アニメーション
	ChangeAnimation(5, false, 80);
	switch (mBossAttackMove) {
	case(0):
		if (mAnimationFrame < 30) {

			mPosition.mZ -= 0.1f;
		}
		else {
			mJump = JUMP2;
			mJumpZ = JUMP2;
			mBossAttackMove = 1;	
		}
		break;
	case(1):
		if (mAnimationFrame < 60) {
			mJumpCount = CVector(0.0f, mJump, mJumpZ) * mMatrixRotate;
			mBossAttackHit = true;
			mPosition += mJumpCount;
			mJump -= G2;
			if (mJumpZ > 0) {
				mJumpZ -= G2;
			}
		}
		else {
			mBossAttackHit = false;
			mJump = 0.0f;
		    mBossAttackMove = 2;

		}
		break;
	case(2):
			mPosition.mZ -= 0.1f;
		
		break;
	}
	//攻撃のあとは待機状態に移行
	if (mAnimationFrame >= mAnimationFrameSize) {
			mMove = 0;//攻撃のアニメーションのあとは待機のアニメーションに切り替わる
			mState = EAUTOMOVE;
	}

}
//攻撃処理2（なぎばらい）
void CBoss::Attack2() {
	//攻撃アニメーション
	ChangeAnimation(6, false, 80);

	switch (mBossAttackMove) {
	case(0):
		if (mAnimationFrame < 30) {
			mRotation.mY += 4.5f;
		}
		else {
			mBossAttackMove = 1;
		}
		break;
	case(1):
		mBossAttackHit = true;
		if (mAnimationFrame < 50) {
			mRotation.mY -= 9.0f;
	     }
		else {
			mBossAttackMove = 2;
		}
		break;
	case(2):
		if (mAnimationFrame < 80) {
			mRotation.mY += 2.0f;
		}
		
			
		break;
	}
	if (mAnimationFrame>=mAnimationFrameSize) {
             mBossAttackHit = false;		
		     mMove = 0;//攻撃のアニメーションのあとは待機のアニメーションに切り替わる
			 mState = EAUTOMOVE;
	}
}
//攻撃処理３（ジャンプ攻撃）
void CBoss::Attack3() {
	ChangeAnimation(8, false, 120);
	mPosition.mY += mJump;//ジャンプ
	mRotation.mX+=6.0f;//回転
	mColSphereHead.mRadius = 6.0f;//地面から落ちないようにコライダーを大きく
	if (mJump > -0.5f) {
				mJump -= G;//重力
	}
    else if (mJump >= -2.5f) {
		mColSphereAttack.mRenderEnabled = true;//コライダーを表示
		mBossAttackHit = true;//攻撃判定を有効
			mJump -= G2;//飛び上がるときよりも重力を大きく
	}
	//着地したら攻撃判定を解除し、待機処理に
	if (mJumpStopper == true) {
		if (mAnimationFrame >= mAnimationFrameSize) {	
			mColSphereAttack.mRenderEnabled = false;
			mBossAttackHit = false;
			mRotation.mX = 0.0f;
			mColSphereHead.mRadius = 3.0f;//コライダーの半径をもとに戻す
				mState = EAUTOMOVE;
		}
	}
	
}
//攻撃処理４（回転攻撃）
void CBoss::Attack4() {
	
		mPosition.mY -= G;//重力をかける
	
	ChangeAnimation(6, false, 2000);
	switch (mAttack4Count) {
	case(1):
		if (mAttack4RotationCount > -90.0f) {
			mAttackRotation = ROTATIONBEFOREATTACK;
			mRotation.mY += mAttackRotation;
			mAttack4RotationCount += mAttackRotation;
		}
		else if (mAttack4RotationCount <= -90.0f) {
            mAttackRotation = 0.0f;
			mAttack4directionCount = 1;
			mAttack4MoveX = 0.5f;
			mAttack4MoveZ = 0.5f;
			mAttack4Count = 2;
		}
		break;
	case(2):
		mBossAttackHit = true;//攻撃判定を有効
		mAttack4MoveCount++;
		mPosition += CVector(mAttack4MoveX, 0.0f, mAttack4MoveZ) ;
			switch (mAttack4directionCount) {
			case(1)://X+Z+
				
				if (mAttack4MoveCount >= 60) {
					mAttack4MoveCount = 0;
					mAttack4MoveZ *= -1;
					mAttack4directionCount = 2;
				}
				break;
			case(2)://X-Z+
				
				if (mAttack4MoveCount >= 60) {
					mAttack4MoveCount = 0;
					mAttack4MoveX *= -1;
					mAttack4directionCount = 3;
				}

				break;
			case(3)://X-Z-
				
				if (mAttack4MoveCount >= 60) {
					mAttack4MoveCount = 0;
					mAttack4MoveZ *= -1;
					mAttack4directionCount = 4;
				}
				break;
			case(4)://X+Z-
				
				if (mAttack4MoveCount >= 60) {
					mAttack4MoveCount = 0;
					
					mAttack4MoveZ *= -1;
					mAttack4directionCount = 1;
				}
				break;
			}
		//回転
		mRotation.mY += mAttackRotation;
		//回転値Yが増えるごとに増加
		mAttack4RotationCount += mAttackRotation;
		//だんだん早くなる
		if (mAttackRotation < ROTATIONCOUNT) {
			mAttackRotation += ROTATION;
		}
		//減速開始
		 if (mAttack4RotationCount >= ROTATIONBASE) {
			 //移動しない
			//mAttack4directionCount = 0;
			//減速処理
			mAttack4Count = 3;
		}
		break;
	case(3):
		//減速
		if (mAttackRotation > 0.0f) {
			mAttackRotation += ROTATIONMIN;
			mRotation.mY += mAttackRotation;
		}
		 if (mAttackRotation <= 0.0f) {
			mAnimationFrame = mAnimationFrameSize;
			mAttack4RotationCount = 0.0f;
			mAttackRotation = 0.0f;
			mAttack4Count = 0;
			mBossAttackHit = false;
			mState = EAUTOMOVE;
		}

		break;
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
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	mColSphereHead.mRadius = 2.0f;                                    
	if (mBossBgmDeath == true) {
		tSceneGame->mBgmCountCheck = false;
		tSceneGame->mBgmCount = 4;
		mBossBgmDeath = false;
	}
	ChangeAnimation(9, false, 250);
	//体力がなくなったら
	if (mHp <= 0) {
		mHp--;
		//15フレームごとにエフェクト
		if (mHp % 15 == 0) {
			//エフェクト生成
			new CEffect2(mPosition, 10.0f, 10.0f, CEffect2::EFF_EXP, 4, 4, 2);
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
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
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
	case EATTACK3:
		Attack3();
		break;
	case EATTACK4:
		Attack4();
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
			if (tSceneGame->mVoiceSwitch == true) {
				BossVoice.Play();
			}
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mBossAttackHit = false;
			if (mState != EATTACK3) {
			
			}
		}
		break;
	case(6):
		if (mAnimationFrame == 30) {
			if (tSceneGame->mVoiceSwitch == true) {
			  BossVoice.Play();
			}
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mBossAttackHit = false;
			
		}
		break;
	}
	if (mAttackPercent >= 10) {
		mAttackPercent = 0;
	}
	if (mBossDamageCount > 0) {
		mBossDamageCount--;
	}
	if (mBossDamageCount > 0) {
		if (mEffectCount % 10 == 0) {
			//エフェクト生成
				CXPlayer* tPlayer = CXPlayer::GetInstance();
				mBossEffect = new CEffect2(tPlayer->GetSwordColPos(), 3.0f, 3.0f, CEffect2::EFF_EXP, 4, 4, 2,false, &mRotation);
		}
	}
	if (mHp <= 0 && mState != EDEATH) {
		mState = EDEATH;
	}
	if (mColSearch.mRenderEnabled == false) {
		if (mBossBgm == true) {
			tSceneGame->mBgmCountCheck = false;
			tSceneGame->mBossBattleStage = true;
			tSceneGame->mBgmCount = 3;
			mBossBgm = false;
		}
	}
	mEffectCount--;
	CXCharacter::Update();
}

//Collision(コライダ１，コライダ２，）
void CBoss::Collision(CCollider* m, CCollider* o) {
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
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
							if (mColSearch.mRenderEnabled == true) mColSearch.mRenderEnabled = false;
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
					switch(o->mTag) {
					case CCollider::EPLAYERSWORD:
                            //衝突しているとき
							if (CCollider::Collision(m, o)) {
								if (mBossDamageCount <= 0) {
									//親をCXPlayerを元にポインタ化し、変数を参照
									if (((CXPlayer*)(o->mpParent))->GetAttackHit()==true)
									{
										if (mColSearch.mRenderEnabled == true) mColSearch.mRenderEnabled = false;
										//爆発エフェクト秒数付与
										mEffectCount = 5;
										if (mHp > 0) {
											((CXPlayer*)(o->mpParent))->CXPlayer::SpAttackPoint2();
											//30％減るごとにのけぞる
											if (mHp == HPCOUNT1 || mHp == HPCOUNT2 || mHp == HPCOUNT3) {
												mColliderCount = 10;
												mCollisionEnemy = mPosition - o->mpParent->mPosition;
												mCollisionEnemy.mY = 0;
												mCollisionEnemy = mCollisionEnemy.Normalize();
												mState = EDAMAGED;
											}
											mHp--;
											if (((CXPlayer*)(o->mpParent))->GetAttackSp() == true) {
												mBossDamageCount = 3;
											}
											else {
											mBossDamageCount = 30;
											}
										}
									}
								}
							}
						break;
					case CCollider::EITEMCOLLIDER:
						//衝突しているとき
						if (((CXPlayer*)(o->mpParent))->GetAttackHit() == false) {
							if (CCollider::Collision(m, o)) {
								if (mBossDamageCount <= 0) {
									if (mColSearch.mRenderEnabled == true) mColSearch.mRenderEnabled = false;
									//爆発エフェクト秒数付与
									mEffectCount = 5;
									if (mHp > 0) {
										((CXPlayer*)(o->mpParent))->CXPlayer::SpAttackPoint2();
										//30％減るごとにのけぞる
										if (mHp == HPCOUNT1 || mHp == HPCOUNT2 || mHp == HPCOUNT3) {
											mColliderCount = 10;
											mCollisionEnemy = mPosition - o->mpParent->mPosition;
											mCollisionEnemy.mY = 0;
											mCollisionEnemy = mCollisionEnemy.Normalize();
											mState = EDAMAGED;

										}
										mHp--;
										mBossDamageCount = 30;

									}
								}
							}
						}
						break;
					case CCollider::EPLAYERBODY:
                        if (CCollider::Collision(m, o)) {
							
									if (mState ==EAUTOMOVE) {
										if (mHp > 0) {
											mState = EIDLE;
										}
									}
							mColliderCount = 1.5f;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy = mCollisionEnemy.Normalize();
						}
						break;
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
					if (mState == EATTACK3) {

						if (mAnimationFrame >= mAnimationFrameSize) {
							mJumpStopper = true;
						}
				    }	
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
	mColSphereAttack.ChangePriority();
	//衝突処理を実行

	CCollisionManager::Get()->Collision(&mColSphereRightFront, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereLeftFront, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereHead, COLLISIONRANGEFIELD);
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereAttack, COLLISIONRANGE);
}

void CBoss::Render2D()
{
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	//2D描画開始
	CUtil::Start2D(0, 800, 0, 600);

	//体力の割合
	float hpRate = (float)mHp / (float)HP;
	//体力ゲージの幅
	float hpGaugeWid = GAUGE_WID_MAXHP * hpRate;
	if (tSceneGame->mBossGaugeSwitch == true&&mHp>0) {
		mImageGauge.Draw(50, GAUGE_WID_MAXHP, 510, 550, 201, 300, 63, 0);//ゲージ背景
		mImageGauge.Draw(50, hpGaugeWid, 510, 550, 487, 572, 63, 0);//体力ゲージ
	}

	CUtil::End2D();
}
