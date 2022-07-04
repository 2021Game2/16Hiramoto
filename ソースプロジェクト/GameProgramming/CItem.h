
#ifndef CITEM_H
#define CITEM_H
#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CXPlayer.h"
class CItem : public CCharacter {
private:
	//コライダ
	CCollider mCollider;
	CCollider mCollider2;
public:
	static bool mItemAttackHit;

	static CItem* mpItemInstance;
	//staticで処理を作る
	static CItem* GetInstance();
	
	//モデルデータ
	static CModel mModel;
    int mItemCount;
	void SetItemCount(int v) {
		if (v < 0)return;
		mItemCount = v;
	}
	int GetItemCount() {
		return mItemCount;
	}
	CItem();
	//親へのポインタ
	CCharacter* mpParent;
	CCharacter* mpPlayer;//プレイヤーのポインタ
	//CEnemy2(位置、回転、拡縮）
	CItem(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	
};
#endif
