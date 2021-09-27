
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CItem.h"
#define OBJ "Coca_Cola.obj"
#define MTL "Coca_Cola.mtl"
int CItem::mItemCount = 0;
CModel CItem::mModel;//モデルデータ作成
CItem::CItem(const CVector& position, const CVector& rotation, const CVector& scale)
	: CItem()
{

	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mCollider.mTag = CCollider::EITEMCCOLLIDER;
	mTag = EITEM;
	CTransform::Update();//行列の更新
	//優先度を１に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
}
CItem::CItem() 
	: mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
	
{
	//モデルのポインタ設定
	mpModel = &mModel;
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}

}
void CItem::Update() {
mRotation.mY++;
}
void CItem::Collision(CCollider* m, CCollider* o) {
	switch (m->mType) {
	case CCollider::ESPHERE:
		if (o->mType == CCollider::ESPHERE) {
			if (o->mpParent->mTag == EPLAYER) {

				//衝突しているとき
				if (CCollider::Collision(m, o)) {
					mItemCount = 1800;
					mEnabled = false;
				}

			}
		}
	}
	
}
void CItem::TaskCollision() {
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
