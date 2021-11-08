#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#ifndef CTREE_H
#define CTREE_H
class CTree : public CCharacter {
public:
	//モデルデータ
	static CModel mModel;
	//コライダ
	CCollider mCollider;
	CTree();
	//CEnemy2(位置、回転、拡縮）
	CTree(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	void Render();
};
#endif
