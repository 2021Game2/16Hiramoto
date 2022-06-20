#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CColliderMesh.h"
#ifndef CROCK_H
#define CROCK_H
class CRock : public CCharacter {
private:

public:
	CColliderMesh mColliderMesh;//岩の３Dモデルのコライダ
	//モデルデータ
	static CModel mModel;
	//コライダ
	CRock(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理

	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	void Render();
};
#endif