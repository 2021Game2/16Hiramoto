#ifndef CMAP_H
#define CMAP_H

#include "CCharacter.h"
#include"CCollider.h"
class CMap : public CCharacter
{
	CModel mModel;
public:
	CMap();
	//void CollisionTriangleLine(CCollider* t, CCollider* l, CVector* a) ;
};

#endif