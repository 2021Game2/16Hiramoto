#ifndef CENEMYSUMMON_H
#define CENEMYSUMMON_H



#include"CCharacter.h"
#include"CCollider.h"

class CEnemySummon:public CCharacter {
public:
	//モデルデータ
	static CModel mModel;
	//コライダ
	CCollider mCollider;
	CEnemySummon();
	//CEnemy2(位置、回転、拡縮）
	CEnemySummon(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	void Render();
	int mHp;
};




#endif

