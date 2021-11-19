#include"CEnemy2.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CUtil.h"
#include"CText.h"
#define OBJ "mini.obj"//モデルのファイル
#define MTL "mini.mtl"//モデルのマテリアルファイル
#define HP 1
#define VELOCITY 0.2f //マクロ

#define JUMP 4.0f
#define G 0.1f
int CEnemy2::mEnemy2AttackCount = 0;
CModel CEnemy2::mModel;//モデルデータ作成
//デフォルトコンストラクタ
CEnemy2::CEnemy2()
//コライダの設定
	:mCollider(this,&mMatrix,CVector(-0.5f,0.0f,-1.0f),1.0f)
	,mColSearch(this,&mMatrix,CVector(0.0f,0.0f,0.0f),500.0f)
	,mpPlayer(0)
	,mHp(HP)
	,mJump(0)
	,mJump2(0)
	, mEnemyDamage(60)
	,mMove(0)
	,mMove2(0)
	, mColliderCount(0)
{
	mTag = EENEMY2;
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		//mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
	mTag = EENEMY2;
	mColSearch.mTag = CCollider::ESEARCH;//タグ設定
	mCollider.mTag = CCollider::EENEMY2COLLIDER;
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
}
//更新処理
void CEnemy2::Update() {
	//if(mPosition.mY<=mpPlayer->mPosition.mY)
	//左向き（X軸）のベクトルを求める
	CVector vx = CVector(1.0f, 0.0f, 0.0f)*mMatrixRotate;
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
	
		if (mMove <= 300) {
		mMove++;
		}
		if (mMove >= 180) {
        //移動する
			//動いている時間だけ当たり判定
			mEnemy2AttackCount = 3;
		mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
		}
		if (mMove >= 300) {
			mMove = 0;
		}
		
	int r = rand() % 60; //rand()は整数の乱数を返す
	          
    //%180は１８０で割った余りを求める
	if (r == 0) {
		if (mpPlayer) {
			mPoint = mpPlayer->mPosition;

		}
		else {
			mPoint = mPoint * CMatrix().RotateY(80);
		}
	}
	mpPlayer = 0;
	if (mHp <= 0) {
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

     //mPosition.mZ -= mJump;
	 mJump--;
	}
	//吹き飛ぶ（X,Z軸)
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition = mPosition + mCollisionEnemy * mColliderCount;
	}
	//吹き飛ぶ（Y軸）
	if (mJump2 > 0) {
		mPosition.mY += mJump;
		mJump2--;
	}
	//重力
	if (mPosition.mY > 1.0f) {
      mPosition.mY -= G;
      
	}
	//吹き飛んでいる間回転
	if (mPosition.mY > 3.0f) {
     mRotation.mX += 20.0f;
	}
	//当たり判定が適用される時間
	if (mEnemy2AttackCount > 0) {
		mEnemy2AttackCount--;
	}

}

void CEnemy2::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	mColSphereBody.mpMatrix = &mpCombinedMatrix[1];
	//頭
	mColSphereHead.mpMatrix = &mpCombinedMatrix[1];
	//剣
	mColSphereSword0.mpMatrix = &mpCombinedMatrix[26];
	mColSphereSword1.mpMatrix = &mpCombinedMatrix[26];
	mColSphereSword2.mpMatrix = &mpCombinedMatrix[26];

	/*
	mColSphereBody.mpMatrix = &mpCombinedMatrix[8];
	//頭
	mColSphereHead.mpMatrix = &mpCombinedMatrix[11];
	//剣
	mColSphereSword.mpMatrix = &mpCombinedMatrix[21];
	*/
}
//Collision(コライダ１，コライダ２，）
void CEnemy2::Collision(CCollider* m, CCollider* o) {
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
			if (o->mpParent->mTag == EPLAYER) {
				if(o->mTag== CCollider::ESWORD)
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
						mPosition = mPosition - adjust ;
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
void CEnemy2::TaskCollision() {
	mColSearch.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
