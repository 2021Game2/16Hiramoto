#include"CBoss.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CXCharacter.h"
#include"CUtil.h"
#include"CText.h"

#define HP 30
#define VELOCITY 0.5f //マクロ

#define JUMP 5.0f
#define G 0.5f
int CBoss::mBossAttackCount = 0;
CModel CBoss::mModel;//モデルデータ作成
//デフォルトコンストラクタ
CBoss::CBoss()
//コライダの設定
	:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 1.0f)
	, mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 200.0f)
	, mColSphereHead(this, &mMatrix, CVector(0.0f, 1.0f, 5.0f), 5.0f)
	, mColSphereRightFront(this, &mMatrix, CVector(0.0f, -2.0f, 0.0f), 4.0f)
	, mColSphereLeftFront(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 4.0f)
	, mColSphereRightBack(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 4.0f)
	, mColSphereLeftBack(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 4.0f)


	, mpPlayer(0)
	, mHp(HP)
	, mJump(0.0f)
	, mJump2(0)
	, mEnemyDamage(60)
	, mMove(0)
	, mMove2(0)
	, mColliderCount(0.0f)
	, mGravity(0.0f)
	, mTime(0.0f)
{

	mGravity = 0.20f;
	mTag = EBOSS;
	mColSearch.mTag = CCollider::ESEARCH;//タグ設定
	mColSphereHead.mTag = CCollider::EBOSSCOLLIDER;
	mColSphereRightFront.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mColSphereLeftFront.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mColSphereRightBack.mTag = CCollider::EBOSSCOLLIDERATTACK;
	mColSphereLeftBack.mTag = CCollider::EBOSSCOLLIDERATTACK;


	mGravity = 0.20f;
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
	mCollider.mpMatrix = &mpCombinedMatrix[1];
	//頭
	mColSphereHead.mpMatrix = &mpCombinedMatrix[6];
    mColSphereRightFront.mpMatrix = &mpCombinedMatrix[12];//右前足
	mColSphereLeftFront.mpMatrix = &mpCombinedMatrix[19];//左前足
	mColSphereRightBack.mpMatrix = &mpCombinedMatrix[27];//右後ろ足
	mColSphereLeftBack.mpMatrix = &mpCombinedMatrix[32];//左後ろ足
	//mColSphereLeftBack.mpMatrix = &mpCombinedMatrix[17];
	mState = EATTACK2;


}
//待機処理
void CBoss::Idle() {
	//60溜まるまで待機のアニメーション
	ChangeAnimation(8, true, 180);
	if (mMove >= 300) {
		//60溜まった状態でアニメーションが終わると攻撃処理に移行
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			if (mAttackPercent <= 30) {
            //当たり判定が適用される時間
			mBossAttackCount = 0;
			mState = EATTACK;
		    }
			else {
				//当たり判定が適用される時間
				mBossAttackCount = 0;
				mState = EATTACK2;
			}
			
		}
	}
	//60溜まる前にアニメーションが終わったら移動処理に移行
	else if (mAnimationFrame >= mAnimationFrameSize) {
		mState = EAUTOMOVE;
	}


}
//移動処理
void CBoss::AutoMove() {
	//歩く
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
		mRotation.mY += 1.0f;//左へ回転
	}
	else if (dx < -margin) {
		mRotation.mY -= 1.0f;//右へ回転
	}
	CTransform::Update();//行列更新
	//定期的にプレイヤーの座標を記録
	int r = rand() % 60; //rand()は整数の乱数を返す
	//%180は１８０で割った余りを求める
	if (r == 0) {
		if (mpPlayer) {
			//プレイヤーの座標を記録
			mPoint = mpPlayer->mPosition;
		}
		else {
			mPoint = mPoint * CMatrix().RotateY(80);
		}
	}
	mpPlayer = 0;
}
//攻撃処理
void CBoss::Attack() {
	//攻撃アニメーション
	ChangeAnimation(5, true, 83);
	//当たり判定が適用される時間
	//if (mBossAttackCount > 0) {
	if (mAnimationFrameSize > 43) {

		mBossAttackCount++;
	}
	//}
	//攻撃のあとは移動処理に移行
	if (mBossAttackCount <= 0) {
		if (mState == EATTACK) {
			mMove = 0;//攻撃のアニメーションのあとは移動のアニメーションに切り替わる
			mState = EIDLE;
			mBossAttackCount = 0;
		}
	}

}
//攻撃処理
void CBoss::Attack2() {
	//攻撃アニメーション
	ChangeAnimation(6, true, 80);
	//当たり判定が適用される時間
	//if (mBossAttackCount > 0) {
	if (mAnimationFrameSize > 40) {

		mBossAttackCount++;
	}
	//}
	//攻撃のあとは移動処理に移行
	if (mBossAttackCount <= 0) {
		if (mState == EATTACK2) {
			mMove = 0;//攻撃のアニメーションのあとは移動のアニメーションに切り替わる
			mBossAttackCount = 0;
			mState = EIDLE;
		}
	}

}
//ダメージ処理
void CBoss::Damaged() {
	//体力減少
	mHp--;
	//ダメージのあとは移動処理
	mState = EIDLE;
}
//死亡処理
void CBoss::Death() {

	//体力がなくなったら
	if (mHp <= 0) {
		mHp--;
		//15フレームごとにエフェクト
		if (mHp % 15 == 0) {
			//エフェクト生成
			new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		CTransform::Update();
	}
	ChangeAnimation(9, false, 250);
	//しばらく経ったら消去
	if (mHp <= -250) {
		mEnabled = false;
		
	}
}

//更新処理
void CBoss::Update() {
	/*
	//アニメーションの管理
	switch (mAnimationIndex) {
	case(1):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(2, false, 80);
		}
		break;
		//攻撃アニメーション
	case(5):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(6, false, 80);
		}
		break;
	case(6):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(8, false, 60);
		}
		break;
	}*/

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
	case EATTACK2:
		Attack2();
		break;
	case EDAMAGED://ダメージ
		Damaged();
		break;
	case EDEATH://死亡
		Death();
		break;
	}
	if (mAttackPercent < 60) {
		mAttackPercent++;
	}
	if (mAttackPercent >= 60) {
		mAttackPercent=0;
	}
	CXCharacter::Update();
}

//Collision(コライダ１，コライダ２，）
void CBoss::Collision(CCollider* m, CCollider* o) {
	//自分がサーチ用のとき
	if (m->mTag == CCollider::ESEARCH) {
		//相手が弾コライダのとき
		if (o->mType == CCollider::ESPHERE) {
			//相手がプレイヤーのとき
			if (o->mpParent->mTag == EPLAYER) {
				//衝突しているとき
				if (CCollider::Collision(m, o)) {
					//プレイヤーのポインタ設定
					mpPlayer = o->mpParent;
				}
			}
		}
		return;
	}
	//弾コライダのとき
	if (m->mType == CCollider::ESPHERE) {
		//EENEMY2COLLIDERの時
		if (m->mTag == CCollider::EBOSSCOLLIDER) {

			if (o->mType == CCollider::ESPHERE) {

				if (o->mpParent->mTag == EPLAYER) {
					//相手が武器のとき
					if (o->mTag == CCollider::ESWORD) {
						//衝突しているとき
						if (CCollider::Collision(m, o)) {
							if (CXPlayer::mAttackCount > 0) {
								/*
								mColliderCount = 5;
								mCollisionEnemy = mPosition - o->mpParent->mPosition;
								mCollisionEnemy.mY = 0;
								mCollisionEnemy = mCollisionEnemy.Normalize();
								*/
								mState = EDAMAGED;
								if (mHp <= 0) {
									mState = EDEATH;
								}
							}
						}
					}
					//相手がESTOPPERの時
					if (o->mTag == CCollider::ESTOPPER) {

						if (CCollider::Collision(m, o)) {
							if (mState != EATTACK) {
								if (mState != EIDLE) {
									mState = EIDLE;
								}

							}
							mColliderCount = 1.5f;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy = mCollisionEnemy.Normalize();
							mMove++;

						}




					}
				}

			}
		}
		if (o->mType == CCollider::ETRIANGLE) {
			CVector adjust;//調整値
			//三角コライダと球コライダの衝突判定
			//adjust、、、調整値
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{
				if (mPosition.mX + mPosition.mZ > 0) {
					//衝突しない位置まで戻す
					mPosition = mPosition - adjust;
					if (mJump > 0) {
						mPosition = mPosition - adjust;
					}
				}
				else {
					//衝突しない位置まで戻す
					mPosition = mPosition + adjust;
					if (mJump > 0) {
						mPosition = mPosition + adjust;
					}
				}


			}
		}
		return;
	}
}
void CBoss::TaskCollision() {
	//コライダの優先度変更
	mCollider.ChangePriority();
	mColSearch.ChangePriority();
	mColSphereHead.ChangePriority();
	mColSphereRightFront.ChangePriority();
	mColSphereLeftFront.ChangePriority();
	mColSphereRightBack.ChangePriority();
	mColSphereLeftBack.ChangePriority();

	//衝突処理を実行

	CCollisionManager::Get()->Collision(&mColSphereRightFront, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereLeftFront, COLLISIONRANGE);

	CCollisionManager::Get()->Collision(&mColSphereRightBack, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereLeftBack, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereHead, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
