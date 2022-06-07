
#include"CXPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CItem.h"
#define OBJ "Resource\\3DModel\\Weapon\\Weapon.obj"
#define MTL "Resource\\3DModel\\Weapon\\Weapon.mtl"
int CItem::mItemCount = 0;
bool CItem::mItemAttackHit = false;
CModel CItem::mModel;//モデルデータ作成
CItem::CItem(const CVector& position, const CVector& rotation, const CVector& scale)
	: CItem()
{
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mCollider.mTag = CCollider::EITEMCOLLIDER;
	mCollider2.mTag = CCollider::EITEMCOLLIDER2;
	mTag = EITEM;
	CTransform::Update();//行列の更新
	//優先度を１に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
}
CItem::CItem() 
	: mCollider(this, &mMatrix, CVector(0.0f, 3.0f, 0.0f), 6.0f)
	, mCollider2(this, &mMatrix, CVector(0.0f, 3.0f, 0.0f), 6.0f)
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
	//取得すると武器になってプレイヤーの周りを回る
	if (mItemCount > 0) {
	     mItemCount--;
		 mItemAttackHit = true;
		if (mRotation.mX < 90.0f) {
			mRotation = CVector(90.0f, 0.0f, 0.0f);
		}
	  mRotation.mY += 10.0f;
	  CCollider mCollider2;
	  mPosition.mX = tPlayer->mPosition.mX;
	  mPosition.mY = tPlayer->mPosition.mY+1.0f;
	  mPosition.mZ = tPlayer->mPosition.mZ;
	}
	//アイテムを取得していないとき
	if (mItemAttackHit == true&&mItemCount <= 0) {
		mItemAttackHit = false;
		mEnabled = false;
	}
	//アイテムを取得していないとき
	if (mItemAttackHit == false) {
       mRotation.mY++;
	}
    //CCharacterの更新
    CTransform::Update();
    //自分の行列に親の行列を掛けて、最終的な合成行列にする
}
void CItem::Collision(CCollider* m, CCollider* o) {
	switch (m->mType) {
	case CCollider::ESPHERE:
		if (m->mpParent->mTag == EITEM) {
			if (m->mTag == CCollider::EITEMCOLLIDER2) {
				if (o->mType == CCollider::ESPHERE) {
					if (o->mpParent->mTag == EPLAYER) {
						//衝突しているとき
						if (CCollider::Collision(m, o)) {

							if (mCollider2.mRenderEnabled == true) {
								mCollider2.mRenderEnabled = false;
								mItemCount = 1800;
							}
						}
					}
				}
			}
		}
	}
}
void CItem::TaskCollision() {
	//コライダの優先度変更
	mCollider.ChangePriority();
	mCollider2.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider2, COLLISIONRANGE);
}
