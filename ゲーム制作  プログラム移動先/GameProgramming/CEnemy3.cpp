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
#define OBJ "3DModel\\Bee\\BEE1.obj"//モデルのファイル
#define MTL "3DModel\\Bee\\BEE1.mtl"//モデルのマテリアルファイル
#define DAMAGEEFFECT "Resource\\png,tga\\exp.tga"
#define HP 1
#define VELOCITY 0.25f //行動１
#define VELOCITY1  -0.1f//行動１
#define COLLIDERCOUNT 5.0
#define VELOCITY2 0.1f
#define VELOCITY3 0.2f
#define JUMP 4.0f
#define G 0.1f
#define PLAYERSPPOINT_MAX 30
int CEnemy3::mMoveCount = 0;
 extern CSound Enemy3Fry;

CModel CEnemy3::mModel;//モデルデータ作成
//デフォルトコンストラクタ
//敵（ハチ）
CEnemy3::CEnemy3()
//コライダの設定
	:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 5.0f)
	, mColSearch2(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f),70.0f)
	, mpPlayer(0)
	, mHp(HP)
	, mJump(0)
	, mJump2(0)
	, mEnemyDamage(60)
	, mMove2(0)
	, mColliderCount(0)
	,mCount(0)
	,mFireCount(0)
	,mEnemy3Fry(0)
{
	mRotation.mY += 90.0f;
	mTag = EENEMY3;
	mState = EIDLE;
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
    mCollider.mTag = CCollider::EENEMY3COLLIDERBODY;
	mColSearch2.mTag = CCollider::EENEMY3ESEARCH2;//タグ設定
	
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

//更新処理
void CEnemy3::Update() {
	//CXPlayerを使ったポインタにプレイヤーの情報を返す処理をさせる(CXPlayerの中の処理なのでポインタを作る必要あり）
	CXPlayer* tPlayer = CXPlayer::GetInstance();
	
	//if(mPosition.mY<=mpPlayer->mPosition.mY)
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
	switch (mMoveCount) {
		//浮遊
	case(0):
		//０から６０フレーム
		if (mCount < 60) {
			if (mCount >= 0) {
				mPosition.mY -= 0.1f;
			}
		}
		//６０から１２０フレーム
		if (mCount < 120) {
			if (mCount >= 60) {
				mPosition.mY += 0.1f;
			}
		}
		if (mCount >= 120) {
			mCount = 0;

			if (mColSearch2.mRenderEnabled == false) {
				//mMoveCount = 1;
			}
		}
		break;
		//移動（まっすぐ移動）
	case(1):
		if (mCount < 180) {
			mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;

			//CXPlayerを使ったポインタにプレイヤーの情報を返す処理をさせる(CXPlayerの中の処理なのでポインタを作る必要あり）
			CXPlayer* tPlayer = CXPlayer::GetInstance();

			if (mPosition.mY >= tPlayer->mPosition.mY+5.0f) {
				mPosition = mPosition + CVector(0.0f, -0.1f, VELOCITY) * mMatrixRotate;
				
			}
			else {
				mPosition.mY++;
			}
		}
		if (mCount >= 180) {
			mMoveCount = 2;
			mCount = 0;

		}
		break;
		//移動２（右後ろに移動）
	case(2):
		if (mCount < 10) {

			mPosition = mPosition + CVector(3.0f, 2.0f, -1.5f) * mMatrixRotate;
		}
		if (mCount >= 10) {
			mMoveCount = 3;
			mCount = 0;

		}
		break;
		//移動３（左に移動）
	case(3):
		if (mCount <= 10) {
			mPosition = mPosition + CVector(-6.0f, -1.0f, 0.0f) * mMatrixRotate;
		}
		if (mCount >= 10) {
			mMoveCount = 4;
			mCount = 0;

		}
		break;
		//移動４（右前に移動（元の位置に戻る）
	case(4):
		if (mCount < 30) {
			mPosition = mPosition + CVector(1.5f, VELOCITY, 0.5f) * mMatrixRotate;
		}
		if (mCount >= 30) {
			mMoveCount = 1;
			mCount = 0;

		}
		break;
	}


	if (mHp <= 0) {

		//吹き飛ぶ(X軸方向）
		if (mColliderCount > 0) {
			mColliderCount--;
			mPosition = mPosition + mCollisionEnemy * mColliderCount;
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
			new CEffect2(mPosition, 1.0f, 1.0f,CEffect2::EFF_EXP, 4, 4, 2);
		}
		CTransform::Update();
	}
	mCount++;
	if (mHp <= -70) {
		mEnabled = false;
		CSceneGame::mEnemy3Count--;
		CSceneGame::mEnemy3CountStopper--;
	}
	if (mJump > 0) {
		mJump--;
	}
	if (mFireCount > 0) {
		mFireCount--;
	}
	mEnemy3Fry++;
	if (mEnemy3Fry >= 300) {
		if (CSceneGame::mVoiceSwitch == true) {
			Enemy3Fry.Play();
		}
		mEnemy3Fry = 0;
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
			//相手が武器のとき、
			if (o->mpParent->mTag == EPLAYER || o->mpParent->mTag == EITEM) {
				if (o->mTag == CCollider::EPLAYERSWORD) {
					//衝突しているとき
					if (CCollider::Collision(m, o)) {
						//親をCXPlayerを元にポインタ化し、変数を参照
						if (((CXPlayer*)(o->mpParent))->mAttackHit == true)
						{

							if (((CXPlayer*)(o->mpParent))->mSpAttack < PLAYERSPPOINT_MAX) {

								((CXPlayer*)(o->mpParent))->mSpAttack++;
							}
							mColliderCount = COLLIDERCOUNT;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy.mY = 0;
							mCollisionEnemy = mCollisionEnemy.Normalize();
							mJump = JUMP;
							mHp--;
						}

					}
				}

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
						}
					}
				}
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

		if (o->mpParent->mTag == EENEMY3) {

			if (o->mTag == CCollider::EENEMY3COLLIDERBODY) {
				CVector adjust;
				if (CCollider::CollisionSylinder(o, m, &adjust)) {
					//衝突しない位置まで戻す
					mPosition = mPosition + adjust;
				}
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
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGEFIELD);
}
