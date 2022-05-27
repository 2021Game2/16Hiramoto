#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CXPlayer.h"
#ifndef CITEM_H
#define CITEM_H

class CItem : public CCharacter {
private:
	//コライダ
	CCollider mCollider;
	CCollider mCollider2;
public:
	static bool mItemAttackHit;
	//モデルデータ
	static CModel mModel;
	static int mItemCount;
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
