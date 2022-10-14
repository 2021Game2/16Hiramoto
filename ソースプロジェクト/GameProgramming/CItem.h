
#ifndef CITEM_H
#define CITEM_H
#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CXPlayer.h"
class CItem : public CCharacter {
private:
    int mItemCount;
	//コライダ
	CCollider mCollider;
	CCollider mCollider2;
	CCharacter* mpParent;//親へのポインタ
	CCharacter* mpPlayer;//プレイヤーのポインタ
public:
	void SetItemCount(int ItemCount) {
		mItemCount = ItemCount;
	}
	 bool mItemAttackHit;
	static CItem* mpItemInstance;
	//staticで処理を作る
	static CItem* GetInstance();
	//モデルデータ
	static CModel mModel;
	
	CItem();
	//CItem(位置、回転、拡縮）
	CItem(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	
};
#endif
