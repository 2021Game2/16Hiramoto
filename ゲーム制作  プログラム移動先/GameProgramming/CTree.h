#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CColliderMesh.h"
#ifndef CTREE_H
#define CTREE_H
class CTree : public CCharacter {
private:

public:
	CColliderMesh mColliderMesh;//岩の３Dモデルのコライダ
	//モデルデータ
	static CModel mModel;
	//コライダ
	CTree(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	void TaskCollision();
	void Render();
};
#endif
