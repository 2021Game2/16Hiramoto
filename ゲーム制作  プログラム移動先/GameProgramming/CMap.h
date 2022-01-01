#ifndef CMAP_H
#define CMAP_H

#include "CCharacter.h"
#include"CCollider.h"

#include"CColliderTriangle.h"
#include"CCollisionManager.h"
class CMap : public CCharacter
{

	CModel mModel;
public:
	CMap();

	CColliderTriangle mColliderTriangle;

	CColliderTriangle mColliderTriangle2;
	void TaskCollision();
};

#endif