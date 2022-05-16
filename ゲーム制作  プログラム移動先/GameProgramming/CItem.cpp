
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CItem.h"
#define OBJ "3DModel\\Weapon\\Weapon.obj"
#define MTL "3DModel\\Weapon\\Weapon.mtl"


int CItem::mItemCount = 0;
CModel CItem::mModel;//モデルデータ作成
CItem::CItem(const CVector& position, const CVector& rotation, const CVector& scale)
	: CItem()
{

	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mCollider.mTag = CCollider::EITEMCOLLIDER;
	mTag = EITEM;
	CTransform::Update();//行列の更新

	//優先度を１に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
}
CItem::CItem() 
	: mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 4.0f)
	
{
	//モデルのポインタ設定
	mpModel = &mModel;
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}

}
void CItem::Update() {
	CXPlayer* tPlayer = CXPlayer::GetInstance();
	if (mItemCount > 0) {
	     mItemCount--;
		if (mRotation.mX < 90.0f) {
			mRotation = CVector(90.0f, 0.0f, 0.0f);
		}
	  mRotation.mY += 10.0f;
	  CCollider mCollider2;

	  mPosition.mX = tPlayer->mPosition.mX;
	  mPosition.mY = tPlayer->mPosition.mY;
	  mPosition.mZ = tPlayer->mPosition.mZ;

	}
    mRotation.mY++;
//CCharacterの更新
CTransform::Update();

//自分の行列に親の行列を掛けて、最終的な合成行列にする

}
void CItem::Collision(CCollider* m, CCollider* o) {
	switch (m->mType) {
	case CCollider::ESPHERE:
		
		if (o->mType == CCollider::ESPHERE) {
			if (o->mpParent->mTag == EPLAYER) {

				//衝突しているとき
				if (CCollider::Collision(m, o)) {
					mItemCount = 1800;
					mCollider.mTag = CCollider::EPLAYERSWORD;
					//mEnabled = false;
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
