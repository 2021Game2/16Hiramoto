#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CColliderMesh.h"
#ifndef CROCK_H
#define CROCK_H
class CRock : public CCharacter {
private:

	//CModel mModel;//モデルデータ作成
public:
	CColliderMesh mColliderMesh;
	//モデルデータ
	static CModel mModel;
	//コライダ
	//CCollider mCollider;
	CRock();
	//CEnemy2(位置、回転、拡縮）
	
	CRock(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	void Render();
};
#endif