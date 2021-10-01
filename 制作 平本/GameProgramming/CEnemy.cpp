#include"CEnemy.h"
#include"CVector.h"

#include"CTaskManager.h"
#include"CEffect.h"
#include"CBullet.h"
#include"CCollisionManager.h"
#include"CPlayer.h"
#define COLLISIONRANGE 30
#define HP 500
#define VELOCITY 0.1f //マクロ



//コンストラクタ
//CEnemy（モデル、位置、回転、拡縮)
//&mMatrix=敵
CEnemy::CEnemy(CModel* model, CVector position, CVector rotation, CVector scale)
	:mCollider1(this, &mMatrix, CVector(-0.5f, 1.0f, 0.0f), 4.0f)
	, mCollider2(this, &mMatrix, CVector(-0.5f, 1.0f, -1.0f), 4.0f)
	, mCollider3(this, &mMatrix, CVector(-0.5f, 1.0f, -2.0f), 4.0f)
	, mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 500.0f)
	, mpPlayer(nullptr)
	, mFireCount(0)
	,mHp(HP)
	,mDamageCount(0)
{
	


	//モデル、位置、回転、拡縮を設定（クラス内のみ適用）
	mpModel = model;//モデル
	mPosition = position;//位置
	mRotation = rotation;//回転
	mScale = scale; //拡縮
	//優先度を1に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
	mPoint = mPosition + CVector(0.0f, 0.0f, 30.0f) * mMatrixRotate;
	CTransform::Update();//行列の更新
	mTag = EENEMY;
	mColSearch.mTag = CCollider::ESEARCH;
	mCollider1.mTag = CCollider::EENEMYCOLLIDER;
	mCollider2.mTag = CCollider::EENEMYCOLLIDER;
	mCollider3.mTag = CCollider::EENEMYCOLLIDER;
}
void CEnemy::Update() {
	//左向き（X軸）のベクトルを求める
	CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//上向き（Y軸）のベクトルを求める
	CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//前方向（Z軸）のベクトルを求める
	CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;
	//プレイヤーのポインタが０以外のとき
	if (mpPlayer) {
		//プレイヤーまでのベクトルを求める
		//プレイヤーと敵の座標の差
		CVector vp = mpPlayer->mPosition - mPosition;
		float dx = vp.Dot(vx);
		float dy = vp.Dot(vy);
		float dz = vp.Dot(vz);
		
	}
 
	//目標地点までのベクトルを求める
	CVector vp = mPoint - mPosition;
	//左ベクトルとの内積を求める
	float dx = vp.Dot(vx);

	//上ベクトルとの内積を求める
	float dy = vp.Dot(vy);
	float margin = 0.1f;
	//左右方向へ回転

	if (dx > margin) {
		mRotation.mY += 1.0f;//左へ回転
	}
	else if (dx < -margin) {
		mRotation.mY -= 1.0f;//右へ回転
	}
	
	//移動する
   mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
    //行列を更新
	CTransform:: Update();
	int r = rand() % 180; //rand()は整数の乱数を返す
						 //%180は１８０で割った余りを求める
	if (r == 0) {
		if (mpPlayer) {
			mPoint = mpPlayer->mPosition;
		}
		else {
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}
	mpPlayer = 0;
	if (mHp <= 0) {
		mHp--;
		if (mHp <= -100) {
		mEnabled = false;
	    }
		//15フレームごとにエフェクト
		if (mHp % 15 == 0) {
			//エフェクト生成
			new CEffect(mPosition, 30.0f, 30.0f, "exp.tga", 4, 4, 2);
			//new CEffect(mPosition, 30.0f, 30.0f, "Attack.tga", 2, 6, 2);
		}
		CTransform::Update();
		return;
	}
	if (mDamageCount > 0) {
		mDamageCount--;
	}
	
}
void CEnemy::Collision(CCollider* m, CCollider* o) {
	//自分がサーチ用のとき
	if (m->mTag == CCollider::ESEARCH) {
		//相手が弾コライダのとき
		if (o->mType == CCollider::ESPHERE) {
			//相手がプレイヤーのとき、
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
	if (m->mType == CCollider::ESPHERE) {
		
		if (o->mType == CCollider::ESPHERE) {
			//相手が武器のとき、
			if (o->mpParent->mTag == EWEAPON) {
				//衝突しているとき
				if (CCollider::Collision(m, o)) {
					new CEffect(mPosition, 15.0f, 15.0f, "exp.tga", 4, 4, 2);
					//if (mDamageCount = 0) {
						mHp--;
						mDamageCount = 1;
						CPlayer::mSpAttack +=1;
					//}
					
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
					}
					else {
						//衝突しない位置まで戻す
						mPosition = mPosition + adjust;
					}
				}
				
	    }
		return;
	}
	


	
}
void CEnemy::TaskCollision() {

	//コライダの優先度変更
	mCollider1.ChangePriority();
	mCollider2.ChangePriority();
	mCollider3.ChangePriority();
	mColSearch.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mCollider1, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
}