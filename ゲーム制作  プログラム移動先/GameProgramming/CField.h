#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CColliderMesh.h"
#ifndef CFIELD_H
#define CFIELD_H
class CField : public CCharacter {
private:

public:
	CColliderMesh mColliderMesh;//岩の３Dモデルのコライダ
	//モデルデータ
	static CModel mModel;
	//コライダ
	CField(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	void TaskCollision();
	void Render();
};
#endif
