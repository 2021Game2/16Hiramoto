#include"CEnemy3.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CUtil.h"
#include"CText.h"
#include"CXCharacter.h"
#include"CSound.h"
#include"CSceneGame.h"
#include"CItem.h"
#define OBJ "Resource\\3DModel\\Bee\\BEE1.obj"//モデルのファイル
#define MTL "Resource\\3DModel\\Bee\\BEE1.mtl"//モデルのマテリアルファイル
#define DAMAGEEFFECT "Resource\\png,tga\\exp.tga"
#define HP 1
#define VELOCITY 0.25f //行動１
#define COLLIDERCOUNT 5.0f
#define COLLIDERCOUNT2 1.5f
#define VELOCITY2 0.1f
#define VELOCITY3 0.2f
#define JUMP 4.0f
#define G 0.1f
#define PLAYERSPPOINT_MAX 30

 extern CSound Enemy3Fry;

CModel CEnemy3::mModel;//モデルデータ作成
//デフォルトコンストラクタ
//敵（ハチ）
CEnemy3::CEnemy3()
//コライダの設定
	:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 5.0f)
	,mColSearch2(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f),70.0f)
	,mpPlayer(0)
	,mMoveCount (0)
	,mJump(0)
	,mJump2(0)
	,mMove2(0)
	,mColliderCount(0)
	,mCount(0)
	,mFireCount(0)
	,mEnemy3Fry(0)
	,mEnemyDamage(60)
	,mHp(HP)
	, mState(EIDLE)
{
	mRotation.mY += 90.0f;
	mTag = EENEMY3;
	
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
    mCollider.mTag = CCollider::EENEMY3COLLIDERBODY;
	mColSearch2.mTag = CCollider::EENEMY3ESEARCH2;//タグ設定
	mEnabled = true;
}

//CEnemy(位置、回転、拡縮）
CEnemy3::CEnemy3(const CVector& position, const CVector& rotation, const CVector& scale)
	:CEnemy3()
{
	//位置、回転、拡縮を設定する
	mPosition = position;//位置の設定
	mRotation = rotation;//回転の設定
	mScale = scale;//拡縮の設定
	CTransform::Update();//行列の更新
	//目標地点の設定
	//mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
	//優先度を１に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
}
void CEnemy3::Idle() {

	mCount++;
	//０から６０フレーム
	if (mCount < 60) {
		if (mCount >= 0) {
			mPosition.mY -= 0.5f;
		}
	}
	//６０から１２０フレーム
	else if (mCount < 120) {
		mPosition.mY += 0.5f;
	}
	else if (mCount >= 120) {
		mCount = 0;
		if (mColSearch2.mRenderEnabled == false) {
			mState = EMOVE1;
		}
	}
}
void CEnemy3::Move1() {
	mCount++;
	if (mCount < 180) {
		mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
		//CXPlayerを使ったポインタにプレイヤーの情報を返す処理をさせる(CXPlayerの中の処理なのでポインタを作る必要あり）
		CXPlayer* tPlayer = CXPlayer::GetInstance();
		if (mPosition.mY >= tPlayer->mPosition.mY + 5.0f) {
			mPosition = mPosition + CVector(0.0f, -0.1f, VELOCITY) * mMatrixRotate;
		}
		else {
			if (mPosition.mY <= 30.0f) {
				mPosition.mY++;
			}
		}
	}
	else if (mCount >= 180) {
		mState = EMOVE2;
		
		mCount = 0;

	}
}
void CEnemy3::Move2() {
	mCount++;
	if (mCount < 10) {

		mPosition = mPosition + CVector(3.0f, 2.0f, -1.5f) * mMatrixRotate;
	}
	else{
		mState = EMOVE3;
		mCount = 0;

	}
}
void CEnemy3::Move3() {
	mCount++;
	if (mCount < 10) {
		mPosition = mPosition + CVector(-6.0f, -1.0f, 0.0f) * mMatrixRotate;
	}
	else{
		mState = EMOVE4;
		mCount = 0;

	}
}
void CEnemy3::Move4() {
	mCount++;
	if (mCount < 30) {
		mPosition = mPosition + CVector(1.5f, VELOCITY, 0.5f) * mMatrixRotate;
	}
	if (mCount >= 30) {
		mState = EMOVE1;
		mCount = 0;

	}
}
void CEnemy3::Attack() {

}
void CEnemy3::Death() {
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	if (mHp <= -70) {
		mEnabled = false;
		tSceneGame->mEnemy3Count--;
		tSceneGame->mEnemy3CountStopper--;
	}
	if (mJump > 0) {
		mJump--;
	}
	//吹き飛ぶ(X軸方向）
	if (mColliderCount > 0) {
		mColliderCount--;

	}
	//吹き飛ぶ（Y軸方向）
	if (mJump2 > 0) {
		mPosition.mY += mJump;
		mJump2--;
	}
	mHp--;
	//15フレームごとにエフェクト
	if (mHp % 15 == 0) {
		//エフェクト生成
		new CEffect2(mPosition, 1.0f, 1.0f, CEffect2::EFF_EXP, 4, 4, 2, false, &mRotation);
	}
	CTransform::Update();
}
//更新処理
void CEnemy3::Update() {
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	//処理を行動ごとに分割
	switch (mState) {
	case EIDLE://浮遊
		Idle();
		break;
	case EMOVE1://移動（まっすぐ移動）
		Move1();
		break;
	case EMOVE2://移動２（右後ろに移動）
		Move2();
		break;
	case EMOVE3://移動３（左に移動）
		Move3();
		break;
	case EMOVE4://移動４（右前に移動（元の位置に戻る）
		Move4();
		break;
	
	case EATTACK://攻撃
		Attack();
		break;
	
	case EDEATH://死亡
		Death();
		break;
	}
	//CXPlayerを使ったポインタにプレイヤーの情報を返す処理をさせる(CXPlayerの中の処理なのでポインタを作る必要あり）
	CXPlayer* tPlayer = CXPlayer::GetInstance();
	
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
	if (dy > margin) {
		mRotation.mX -= 3.0f;
	}
	else if (dy < -margin) {
		mRotation.mX += 3.0f;
	}
	CTransform::Update();//行列更新
	int r = rand() % 30; //rand()は整数の乱数を返す
	//%180は１８０で割った余りを求める
	if (r == 0) {
		mPoint = CVector(tPlayer->mPosition.mX, tPlayer->mPosition.mY+3.0f, tPlayer->mPosition.mZ);
	}
	if (mFireCount > 0) {
		mFireCount--;
	}
	mEnemy3Fry++;
	if (mEnemy3Fry >= 300) {
		if (tSceneGame->mVoiceSwitch == true) {
			Enemy3Fry.Play();
		}
		mEnemy3Fry = 0;
	}
	if (tSceneGame->mBossGaugeSwitch == true) {
		mEnabled = false;
	}
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition = mPosition + mCollisionEnemy * mColliderCount;
	}
}
//Collision(コライダ１，コライダ２，）
void CEnemy3::Collision(CCollider* m, CCollider* o) {
	m->mType = CCollider::ESPHERE;
	//自分がサーチ用のとき

	if (m->mpParent->mTag == EENEMY3) {
		if (m->mTag == CCollider::EENEMY3ESEARCH2) {
			//相手が弾コライダのとき
			if (o->mType == CCollider::ESPHERE) {
				//相手がプレイヤーのとき、
				if (o->mpParent->mTag == EPLAYER) {
					if (o->mTag == CCollider::EPLAYERBODY) {
						//衝突しているとき
						if (CCollider::Collision(m, o)) {
							if (mColSearch2.mRenderEnabled == true) {
								mCount = 0;
								if (mMoveCount == 0|| mMoveCount == 1) {
									mMoveCount = 1;
								}
								mColSearch2.mRenderEnabled = false;
							}
						}
					}
				}
			}
			return;
		}
	}
	
	if (m->mTag == CCollider::EENEMY3COLLIDERBODY) {
		if (o->mType == CCollider::ESPHERE) {
			//相手が武器のとき
			switch (o->mpParent->mTag) {
			case(EPLAYER):
               if (o->mTag == CCollider::EPLAYERSWORD) {
					//衝突しているとき
					if (CCollider::Collision(m, o)) {
						//親をCXPlayerを元にポインタ化し、変数を参照
						if (((CXPlayer*)(o->mpParent))->mAttackHit == true)
						{
							if (((CXPlayer*)(o->mpParent))->mSpAttack < PLAYERSPPOINT_MAX) {

								((CXPlayer*)(o->mpParent))->CXPlayer::SpAttackPoint();
							}
							mColliderCount = COLLIDERCOUNT;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy.mY = 0;
							mCollisionEnemy = mCollisionEnemy.Normalize();
							mJump = JUMP;
							mHp--;
							if (mHp <= 0) {
								mState = EDEATH;
							}
						}

					}
			   }
			   //相手がEPLAYERBODY(プレイヤーの体のコライダ）の時
		       if (o->mTag == CCollider::EPLAYERBODY) {
					if (CCollider::Collision(m, o)) {
			
						//これ以上前に進めなくなる
						mColliderCount = 10.0f;
						mCollisionEnemy = mPosition - o->mpParent->mPosition;
						mCollisionEnemy = mCollisionEnemy.Normalize();
					}
		       }
				break;
			case(EITEM):
				 if (o->mTag == CCollider::EITEMCOLLIDER) {
					//衝突しているとき
					if (CCollider::Collision(m, o)) {
						if (((CItem*)(o->mpParent))->mItemAttackHit == true)
						{

							mColliderCount = COLLIDERCOUNT;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy.mY = 0;
							mCollisionEnemy = mCollisionEnemy.Normalize();
							mJump = JUMP;
							mHp--;
							if (mHp <= 0) {

								mState = EDEATH;
							}
						}
					}
				 }
				break;
			case(EENEMY2):
				if (o->mTag == CCollider::EENEMY2COLLIDERBODY) {
					CVector adjust;
					if (CCollider::CollisionSylinder(o, m, &adjust)) {
						//衝突しない位置まで戻す
						mPosition = mPosition + adjust;
					}
				}
				break;
			case(EENEMY3):
				if (o->mTag == CCollider::EENEMY3COLLIDERBODY) {
					CVector adjust;
					if (CCollider::CollisionSylinder(o, m, &adjust)) {
						//衝突しない位置まで戻す
					mPosition = mPosition + adjust;
					}
			    }
				break;
			}
		}
		if (o->mType == CCollider::ETRIANGLE) {
			if (mMoveCount == 1) {
				CVector adjust;//調整値
				//三角コライダと球コライダの衝突判定
				//adjust、、、調整値
				if (CCollider::CollisionTriangleSphere(o, m, &adjust))
				{
						//衝突しない位置まで戻す
						//mPosition = mPosition + adjust;
					
				}
			}
			
		}
		
		
	}
	return;
}
void CEnemy3::TaskCollision() {
	mColSearch2.ChangePriority();
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mColSearch2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
