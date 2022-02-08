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
	CModel mModel;

	CColliderMesh mColliderMesh;

public:
	CMap();

	//CColliderTriangle mColliderTriangle;

	//CColliderTriangle mColliderTriangle2;
	void TaskCollision();
};

#endif