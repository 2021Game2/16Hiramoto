
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CFlag.h"
#include"CCollider.h"
#include"CSceneGame.h"
#include"CXPlayer.h"
#define OBJ "Resource\\3DModel\\Flag\\Flag.obj"
#define MTL "Resource\\3DModel\\Flag\\Flag.mtl"

CModel CFlag::mModel;//モデルデータ作成
CFlag* CFlag::mpFlagInstance;
//フラグのポインタを返すことで、座標などが参照できるようになる
CFlag* CFlag::GetInstance()
{
	return mpFlagInstance;
}
CFlag::CFlag(const CVector& position, const CVector& rotation, const CVector& scale)
	:CFlag()
{

	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mpFlagInstance = this;
	//優先度を2に変更する
	mPriority = 2;
	mColliderFlag.mTag = CCollider::EFLAGCOLLIDER;
	mTag = EFLAG;
	CTransform::Update();//行列の更新
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
	
	
}
CFlag::CFlag()
	:mColliderFlag(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 5.0f)
{
//モデルのポインタ設定
	mpModel = &mModel;
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
}
void CFlag::Update() {
	//CCharacterの更新
	CTransform::Update();
}
void CFlag::Collision(CCollider* m, CCollider* o) {
	
	if (m->mType == CCollider::ESPHERE) {
	   if (m->mpParent->mTag == EFLAG) {
		  if (m->mTag == CCollider::EFLAGCOLLIDER) {
			 if (o->mType == CCollider::ESPHERE) {
				if (o->mpParent->mTag == EPLAYER) {
					if (o->mTag == CCollider::EPLAYERBODY) {
						if (CCollider::Collision(m, o)) {
							CSceneGame* tSceneGame = CSceneGame::GetInstance();
							tSceneGame->mBossSwitch = true;
							tSceneGame->mBossGaugeSwitch = true;
							mEnabled = false;

						}
					}
				}
			 } 
		  }
	   }

	}

}
void CFlag::TaskCollision() {
	//コライダの優先度変更
	mColliderFlag.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mColliderFlag, COLLISIONRANGE);
}
