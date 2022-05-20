#include"CEnemy2.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CSceneGame.h"
#include"CXCharacter.h"
#include"CUtil.h"
#include"CText.h"
#include"CItem.h"
#define DAMAGEEFFECT "Resource\\png,tga\\exp.tga"
#define HP 30
#define VELOCITY 0.2f //マクロ
#define ROTATION 180.0f
#define JUMP 2.0f
#define G 0.1f
 extern CSound Enemy2Voice;
CModel CEnemy2::mModel;//モデルデータ作成
//デフォルトコンストラクタ
//敵（サソリ）
CEnemy2::CEnemy2()
//コライダの設定
	: mColSphereRight(this,&mMatrix, CVector(1.5f, 3.0f, 0.5f), 2.0f)
	, mColSphereLeft(this,&mMatrix,  CVector(-1.0f, 0.5f, 0.0f), 2.0f)
	, mColSphereBody(this,&mMatrix,  CVector(0.0f,1.0f,0.0f),2.0f)
	,mHp(HP)
	,mJump(0.0f)
	, mEnemyDamage(60)
	,mMove(0)
	,mMoveCount(false)
	,mColliderCount(0.0f)
	,mTime(0.0f)
	,CurveCount(0.0f)
	,mEnemyVoice(0)
	,mDamageCount(0)
	,mEnemy2AttackHit(false)
	,mEnemyLevel(0)
	,mEnemyHpPercent(1.0f)
	, mEnemy2Bgm(true)
{
	mTag = EENEMY2;
	mColSphereRight.mTag= CCollider::EENEMY2COLLIDERATTACK;
	mColSphereLeft.mTag= CCollider::EENEMY2COLLIDERATTACK;
	mColSphereBody.mTag = CCollider::EENEMY2COLLIDERBODY;
}

//CEnemy(位置、回転、拡縮）
CEnemy2::CEnemy2(const CVector& position, const CVector& rotation, const CVector& scale)
:CEnemy2()
{
	//位置、回転、拡縮を設定する
	mPosition = position;//位置の設定
	mRotation = rotation;//回転の設定
	mScale = scale;//拡縮の設定
	CTransform::Update();//行列の更新
	//目標地点の設定
	mPoint =mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
	//優先度を１に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
	mHp = mHp * mEnemyHpPercent;
}

void CEnemy2::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	mColSphereRight.mpMatrix = &mpCombinedMatrix[9];
	mColSphereLeft.mpMatrix = &mpCombinedMatrix[20];
	mColSphereBody.mpMatrix = &mpCombinedMatrix[0];
	mState = EAUTOMOVE;
}
//待機処理
void CEnemy2::Idle() {
	    //60溜まるまで待機のアニメーション
		ChangeAnimation(8, true, 60);
		mMove++;
		if (mMove >= 300) {
			//60溜まった状態でアニメーションが終わると攻撃処理に移行
			if (mAnimationFrame >= mAnimationFrameSize)
			{
				mState = EATTACK;
			}
		}
		//60溜まる前にアニメーションが終わったら移動処理に移行
	    else if (mAnimationFrame >= mAnimationFrameSize) {
			if (mMoveCount == true) {
             mState = EAUTOMOVE;
			}
		}
}		
//移動処理
void CEnemy2::AutoMove() {
	//CXPlayerを使ったポインタにプレイヤーの情報を返す処理をさせる(CXPlayerの中の処理なのでポインタを作る必要あり）
	CXPlayer* tPlayer = CXPlayer::GetInstance();
	mPlayerMarkingX = tPlayer->mPosition.mX - mPosition.mX;
	mPlayerMarkingZ = tPlayer->mPosition.mZ - mPosition.mZ;
	//歩く
	mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
	ChangeAnimation(1, true, 60);
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
	int r = rand() % 60; //rand()は整数の乱数を返す
	//%180は１８０で割った余りを求める
	if (r == 0) {
		mPoint = tPlayer->mPosition;
	}
}	
//攻撃処理
void CEnemy2::Attack() {
	    //攻撃アニメーション
		ChangeAnimation(4, false, 120);
		if (mAnimationFrame >= mAnimationFrameSize) {
			mMove = 0;
			mState = EAUTOMOVE;//攻撃のアニメーションのあとは移動のアニメーションに切り替わる
        
		}
}	
//ダメージ処理
void CEnemy2::Damaged() {
	if (CSceneGame::mBgmCount <= 2) {

		if (mEnemy2Bgm == true) {
			CSceneGame::mBgmCountCheck = false;
			CSceneGame::mBgmCount = 2;
			mEnemy2Bgm = false;
		}
	}
	//無敵時間付与
	if (mDamageCount < 60) {
		mDamageCount++;
	}
	//爆発エフェクト付与
	if (mEffectCount % 15 == 0) {
		//エフェクト生成
		new CEffect(mPosition, 1.0f, 1.0f, CEffect::EFF_EXP, 4, 4, 2);
	}
	//ヒットバック（X,Z軸)
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition = mPosition + mCollisionEnemy * mColliderCount;
	}

	if (mDamageCount >= 60) {
	//ダメージのあとは移動処理
    mState = EAUTOMOVE;
	
	}
}		
//死亡処理
void CEnemy2::Death() {
	//体力がなくなったら
	if (mHp <= 0) {
		//mTimeとmJumpに整数が代入され、吹っ飛ぶようになる
		mPosition.mY += mJump;
		if (mJump >= -0.5f) {
			mJump -= G;
		}
		mHp--;
		//15フレームごとにエフェクト
		if (mEffectCount % 15 == 0) {
			//エフェクト生成
			//new CEffect(mPosition, 1.0f, 1.0f, DAMAGEEFFECT, 4, 4, 2);
		}
		CTransform::Update();
	}
	//吹き飛ぶ（X,Z軸)
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition.mX = mPosition.mX + mCollisionEnemy.mX * mColliderCount;
		mPosition.mZ = mPosition.mZ + mCollisionEnemy.mZ * mColliderCount;
	}
	//しばらく経ったら消去
	if (mHp <= -120) {
		mEnabled = false;
		CSceneGame::mEnemy2Count --;
		CSceneGame::mEnemy2CountStopper--;
	}
}		

//更新処理
void CEnemy2::Update() {
	mEnemyLevel = rand() % 10 + 1;
	mEnemyHpPercent += mEnemyLevel / 10;
	
	mEffectCount--;
	//mpPlayer = mpPointPlayer;
	//アニメーションの管理
	switch (mAnimationIndex) {
		//攻撃アニメーション
	case(4):
		if (mAnimationFrame == 30) {
			mEnemy2AttackHit = true;
		}
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			mEnemy2AttackHit = false;
			
		}
        break;
		
	}
	//処理を行動ごとに分割
	switch (mState) {
	case EIDLE:	//待機
		Idle();
		break;
	case EAUTOMOVE://移動
		AutoMove();
		break;
	case EATTACK://攻撃
		Attack();
		break;
	case EDAMAGED://ダメージ
		Damaged();
		break;
	case EDEATH://死亡
		Death();
		break;
	}
	mPosition.mY -= 0.1f;
	mEnemyVoice++;
	if (mEnemyVoice>=180) {
		if (CSceneGame::mVoiceSwitch == true) {
			Enemy2Voice.Play();
		}
		mEnemyVoice = 0;
	}

	if (mState != EDAMAGED) {
		mDamageCount = 0;
	}
	CXCharacter::Update();
}

//Collision(コライダ１，コライダ２，）
void CEnemy2::Collision(CCollider* m, CCollider* o) {

	m->mType = CCollider::ESPHERE;
	//EENEMY2COLLIDER(両ハサミのコライダ）の時
	if (m->mTag == CCollider::EENEMY2COLLIDERATTACK) {

		if (o->mType == CCollider::ESPHERE) {

			if (o->mpParent->mTag == EPLAYER|| o->mpParent->mTag == EITEM) {
				//相手がプレイヤーの武器のとき
				if (o->mTag == CCollider::EPLAYERSWORD) {
					//衝突しているとき
					if (CCollider::Collision(m, o)) {
						//プレイヤーの当たり判定が有効なとき
						//親をCXPlayerを元にポインタ化し、変数を参照
						if (((CXPlayer*)(o->mpParent))->mAttackHit == true)
						{//ヒットバック＆ダメージを受ける
							if (mDamageCount <= 0) {
								//プレイヤーのジャンプ攻撃必要ポイント増加
								((CXPlayer*)(o->mpParent))->mSpAttack++;
								mEffectCount = 0;
								//体力減少 
								mHp--;
								//ヒットバック付与 
								mColliderCount = 3.0f;
								mCollisionEnemy = mPosition - o->mpParent->mPosition;
								//HPが０のとき以外は前後左右に吹っ飛ぶ
								mCollisionEnemy.mY = 0;
								mCollisionEnemy = mCollisionEnemy.Normalize();
							    //ダメージ処理に移行
								if (mHp > 0) {

								 mState = EDAMAGED;

								}
								else if (mHp <= 0) {
									mJump = JUMP;
									mState = EDEATH;

								}
							}
						}
					}
				}
				if (o->mTag == CCollider::EITEMCOLLIDER) {
					//衝突しているとき
					if (CCollider::Collision(m, o)) {
						//プレイヤーの当たり判定が有効なとき
						//親をCXPlayerを元にポインタ化し、変数を参照
						if (((CItem*)(o->mpParent))->mItemAttackHit == true)
						{//ヒットバック＆ダメージを受ける
								//プレイヤーのジャンプ攻撃必要ポイント増加
								((CXPlayer*)(o->mpParent))->mSpAttack++;
								mEffectCount = 0;
								//体力減少 
								mHp--;
								//ヒットバック付与 
								mColliderCount = 1.5f;
								mCollisionEnemy = mPosition - o->mpParent->mPosition;
								//HPが０のとき以外は前後左右に吹っ飛ぶ
								mCollisionEnemy.mY = 0;
								mCollisionEnemy = mCollisionEnemy.Normalize();
								//ダメージ処理に移行
								if (mHp > 0) {
									mState = EDAMAGED;
								}
								else if (mHp <= 0) {
									mJump = JUMP;
									mState = EDEATH;
								}
							
						}
					}
				}
				//相手がEPLAYERBODY(プレイヤーの体のコライダ）の時
				if (o->mTag == CCollider::EPLAYERBODY) {
					if (CCollider::Collision(m, o)) {
						//EIDLE（待機状態）
						if (mState != EATTACK) {
							if (mState != EIDLE) {
								mState = EIDLE;
							}

						}
						//これ以上前に進めなくなる
						mColliderCount = 1.5f;
						mCollisionEnemy = mPosition - o->mpParent->mPosition;
						mCollisionEnemy = mCollisionEnemy.Normalize();
					}
				}
			}
		}
	}
	//ETRIANGLE(マップなどのコライダ）
	if (m->mTag == CCollider::EENEMY2COLLIDERBODY) {
		if (o->mType == CCollider::ETRIANGLE) {
			CVector adjust;//調整値
			//三角コライダと球コライダの衝突判定
			//adjust、、、調整値
				if (CCollider::CollisionTriangleSphere(o, m, &adjust))
				{
						//衝突しない位置まで戻す
						mPosition = mPosition + adjust;
				}
			
			
		}
		
		if (o->mpParent->mTag == EENEMY2) {

			if (o->mTag == CCollider::EENEMY2COLLIDERBODY) {
				CVector adjust;
				if (CCollider::CollisionSylinder(o, m, &adjust)) {
					//衝突しない位置まで戻す
					mPosition = mPosition + adjust;
				}
			}
		}
		return;
	}
	
}

void CEnemy2::TaskCollision() {
    //コライダの優先度変更
	mColSphereRight.ChangePriority();
	mColSphereLeft.ChangePriority();
	mColSphereBody.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mColSphereRight, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereLeft, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereBody, COLLISIONRANGEFIELD);
	
}
