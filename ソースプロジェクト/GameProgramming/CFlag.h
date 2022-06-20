#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CColliderMesh.h"
#ifndef CFLAG_H
#define CFLAG_H
class CFlag : public CCharacter {
private:
	CCollider mColliderFlag;
public:
	
	//モデルデータ
	static CModel mModel;
	
	//コライダ
	CFlag(const CVector& position, const CVector& rotation, const CVector& scale);
	CFlag();
	static CFlag* GetInstance();
	static CFlag* mpFlagInstance;
	//更新処理
	void Update();
	//衝突処理
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
};
#endif
