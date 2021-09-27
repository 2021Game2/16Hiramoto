#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#ifndef CITEM_H
#define CITEM_H
class CItem : public CCharacter {
public:
	//モデルデータ
	static CModel mModel;
	static int mItemCount;
	//コライダ
	CCollider mCollider;
	CItem();
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
