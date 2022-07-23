#ifndef CENEMYSUMMON_H
#define CENEMYSUMMON_H
#include"CCharacter.h"
#include"CCollider.h"
class CEnemySummon:public CCharacter {
private:
	//コライダ
	CCollider mCollider;
	int mDamageCount;
	int mEffectCount;
	int mHp;
public:
	int GetHp() {
		return mHp;
	}
	//モデルデータ
	static CModel mModel;
	CEnemySummon();
	//CEnemy2(位置、回転、拡縮）
	CEnemySummon(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	void Damage();
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
};
#endif

