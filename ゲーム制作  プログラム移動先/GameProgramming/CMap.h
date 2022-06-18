#ifndef CMAP_H
#define CMAP_H

#include "CCharacter.h"
#include"CCollider.h"
#include"CColliderMesh.h"
#include"CColliderTriangle.h"
#include"CCollisionManager.h"
class CMap : public CCharacter
{
private:
	CMatrix mBackGroundMatrix;
	
	//モデルデータ
	static CModel mModel;

	CModel* mpModel; //モデルのポインタ
public:
	CMap(const CVector& position, const CVector& rotation, const CVector& scale);
	CMap();
	void Update();
	
	void TaskCollision();
};

#endif