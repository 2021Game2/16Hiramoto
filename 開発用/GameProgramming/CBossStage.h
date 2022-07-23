#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CColliderMesh.h"
#ifndef CBOSSSTAGE_H
#define CBOSSSTAGE_H
class CBossStage : public CCharacter {
private:

	CColliderMesh mColliderMesh;//岩の３Dモデルのコライダ
public:
	//モデルデータ
	static CModel mModel;
	//コライダ
	CBossStage(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理

	void Render();
};
#endif
