#include"CEnemy3.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CUtil.h"
#include"CText.h"
#include"CXCharacter.h"
#define OBJ "3DModel\\Bee\\BEE1.obj"//モデルのファイル
#define MTL "3DModel\\Bee\\BEE1.mtl"//モデルのマテリアルファイル
#define HP 1
#define VELOCITY 0.05f //行動１
#define VELOCITY1  -0.1f//行動１

#define VELOCITY2 0.1f

#define VELOCITY3 0.2f
#define JUMP 4.0f
#define G 0.1f
int CEnemy3::mMoveCount = 0;
CModel CEnemy3::mModel;//モデルデータ作成
//デフォルトコンストラクタ
CEnemy3::CEnemy3()
//コライダの設定
	:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 20.0f)
	, mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 70.0f)
	, mColSearch2(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f),35.0f)
	, mpPlayer(0)
	, mHp(HP)
	, mJump(0)
	, mJump2(0)
	, mEnemyDamage(60)
	
	, mMove2(0)
	, mColliderCount(0)
	,mCount(0)
	,mFireCount(0)
{
	mRotation.mY += 90.0f;
	mTag = EENEMY3;
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
	mTag = EENEMY2;
    mCollider.mTag = CCollider::EENEMY3COLLIDER;
	mColSearch.mTag = CCollider::ESEARCH;//タグ設定
	mColSearch2.mTag = CCollider::ESEARCH2;//タグ設定
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
		mRotation.mY += 1.0f;//左へ回転
	}
	else if (dx < -margin) {
		mRotation.mY -= 1.0f;//右へ回転
	}

	CTransform::Update();//行列更新
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
	    }
		break;
		//移動（まっすぐ移動）
	case(1):
		if (mCount < 180) {
			mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
			if (mPosition.mY > 3.0f) {
				mPosition = mPosition + CVector(0.0f,-0.1f , VELOCITY) * mMatrixRotate;
				//mPosition.mY -= 0.1f;
			}
		}
		if (mCount >= 180) {
			mMoveCount = 2;
			mCount = 0;
			
		}
		break;
		//移動２（右後ろに移動）
	case(2):
		if (mCount <= 10) {

			//mPosition.mX += 3.0f;
			//mPosition.mZ -= 1.5f;
			//mPosition.mY += 2.0f;
			mPosition = mPosition + CVector(3.0f, 2.0f,-1.5f) * mMatrixRotate;
		}
		if (mCount >= 10) {
            mMoveCount = 3;
			mCount = 0;
			
		}
		break;
		//移動３（左に移動）
	case(3):
		if (mCount <= 10) {
			//mPosition.mX -= 6.0f;
			//mPosition.mY -= 1.0f;
			mPosition = mPosition + CVector(-6.0f, -1.0f, 0.0f) * mMatrixRotate;
		}
		if (mCount >= 10) {
            mMoveCount = 4;
			mCount = 0;
			
		}
		break;
		//移動４（右前に移動（元の位置に戻る）
	case(4):
		if (mCount <= 30) {
			//mPosition.mX += 1.5f;
			//mPosition.mZ += 0.5f;
           // mPosition.mY += 0.1f;
			
			
			mPosition = mPosition + CVector(1.5f,VELOCITY ,0.5f ) * mMatrixRotate;
		}
		if (mCount >= 30) {
           mMoveCount = 0;
			mCount = 0;
			
		}
		break;
	}
	mCount++;
		

	int r = rand() % 60; //rand()は整数の乱数を返す

	//%180は１８０で割った余りを求める
	if (r == 0) {
		if (mpPlayer) {
			mPoint = mpPlayer->mPosition;

		}
		else {
			mPoint = mPoint * CMatrix().RotateY(80);
			//mPoint = mPoint * CMatrix().RotateX(80);
		}
	}
	mpPlayer = 0;
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
		
		//回転
		if (mPosition.mY > 3.0f) {
			mRotation.mX += 20.0f;
		}
		mHp--;
		//15フレームごとにエフェクト
		if (mHp % 15 == 0) {
			//エフェクト生成
			new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		CTransform::Update();
	}
	if (mHp <= -70) {
		mEnabled = false;
		CSceneGame::mEnemyCount -= 1;

	}
	if (mJump > 0) {
		mJump--;
	}
	if (mFireCount > 0) {
		mFireCount--;
	}
	//CTransform::Update();

	//CMatrix mMatrixCol;
	//CModel cube;
	//cube.Load("cube.obj", "cube.mtl");
	//CColliderMesh.Set(this, &mMatrix, mpModel);
	//mMatrixCol = CMatrix().Scale(10.0f,10.0f,10.0f)* mMatrix;
	//ColliderMesh.Set(this, &mMatrixCol, &cube);

}
//Collision(コライダ１，コライダ２，）
void CEnemy3::Collision(CCollider* m, CCollider* o) {
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
					if (mMoveCount<= 0) {
                          mMoveCount = 1;
						
                     
					}
					
				}
			}
		}
		return;
	}
	if (m->mTag == CCollider::ESEARCH2) {
		//相手が弾コライダのとき
		if (o->mType == CCollider::ESPHERE) {
			//相手がプレイヤーのとき、
			if (o->mpParent->mTag == EPLAYER) {
				//衝突しているとき
				if (CCollider::Collision(m, o)) {
					//プレイヤーのポインタ設定
					/*mpPlayer = o->mpParent;
					mpBullet = new CBullet;
					mpBullet->mPosition = mPosition;
					mpBullet->mScale = CVector(2.5f, 2.5f, 2.5f);*/
					mFireCount = 60;
					CBullet* bullet = new CBullet();
					bullet->Set(0.1f, 1.5f);
				bullet->mPosition = CVector(0.0f, 0.0f, 10.0f) * mMatrix;
					bullet->mRotation = mRotation;
					bullet->Update();
				}
				
			}
		}
		return;
	}
	if (m->mType == CCollider::ESPHERE) {
		if (o->mType == CCollider::ESPHERE) {
			//相手が武器のとき、
			if (o->mpParent->mTag == EPLAYER) {
				if (o->mTag == CCollider::ESWORD) {
                     //衝突しているとき
					if (CCollider::Collision(m, o)) {
						if (CXPlayer::mAttackCount > 0) {
							mColliderCount = 5;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy.mY = 0;
							mCollisionEnemy = mCollisionEnemy.Normalize();

							mJump = JUMP;
							mJump2 = JUMP;
							mHp--;
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
void CEnemy3::TaskCollision() {
	mColSearch.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	mColSearch2.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mColSearch2, COLLISIONRANGE);
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
