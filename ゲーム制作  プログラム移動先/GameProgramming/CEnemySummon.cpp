#include"CEnemySummon.h"
#include"CCharacter.h"
#include"CCollider.h"
#include"CEnemy2.h"
#include"CModel.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#define OBJ "egg.obj"
#define MTL "egg.mtl"
CModel CEnemySummon::mModel;
CEnemySummon::CEnemySummon()
    :mHp(0)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)

{
   

	//モデルのポインタ設定
	mpModel = &mModel;
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
}
CEnemySummon::CEnemySummon(const CVector& position, const CVector& rotation, const CVector& scale)
	:CEnemySummon()
	
{
	mHp = 10;
 mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mCollider.mTag = CCollider:: EENEMYSUMMON;
	mTag = EENEMYSUMMON;
	CTransform::Update();//行列の更新
	//優先度を１に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
}
void CEnemySummon::Update() {
		if(mHp<=0){
		mEnabled=false;
		}
}
//Collision(コライダ１、コライダ２）
void CEnemySummon::Collision(CCollider* m, CCollider* o) {
	if (m->mType == CCollider::ESPHERE) {
		if (o->mType == CCollider::ESPHERE) {
			if (o->mpParent->mTag ==EPLAYER) {
				if(o->mTag==CCollider::ESWORD)
				//衝突しているとき
				if (CCollider::Collision(m, o)) {
				//	mHp--;
				}
			}
		}
	}
}
void CEnemySummon::Render() {
	//親の描画処理
	CCharacter::Render();
}
void CEnemySummon::TaskCollision() {
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}