#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"
#include"CVector.h"
class CXPlayer : public CXCharacter
{
public:
	//コライダの宣言
	CCollider mColSphereBody;	//体
	CCollider mColSphereHead;	//頭
	CCollider mColSphereSword;	//剣
	CCollider  mColSphereFoot;
	int mSpaceCount1;
	int mSpaceCount2;
	int mSpaceCount3;
	int mStamina;
	float mJump;
	float mGravity;
	CXPlayer();
	int mSpAttack;
	int mColliderCount;
	CVector mCollisionEnemy;
	int mHp;
	/*
	初期化(Xモデルクラスのポインタ)
	*/
	void Init(CModelX* model);

	void Update();
	void Collision(CCollider* m, CCollider* o);
};

#endif
