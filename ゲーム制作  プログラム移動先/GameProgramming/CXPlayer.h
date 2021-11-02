#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"
#include"CVector.h"
#include"CEnemy2.h"
class CXPlayer : public CXCharacter
{
public:
	//コライダの宣言
	CCollider mColSphereBody;	//体
	CCollider mColSphereHead;	//頭
	CCollider mColSphereSword;	//剣


	
	CVector mEnemyLoad;
	CCollider  mColSphereFoot;
	CCollider mCollider2;
    CVector mCollisionEnemy;
	int mSpaceCount1;
	int mSpaceCount2;
	int mSpaceCount3;
	static int mStamina;
	int mDamageCount;
	float mJump;
	float mStep;
	float mGravity;
    static int mSpAttack;
	static int mAttackCount;
	int mColliderCount;
	int mAnimationCount;
	int mHp;
	float mTime;
	CXPlayer();
	
	
	
	/*
	初期化(Xモデルクラスのポインタ)
	*/
	void Init(CModelX* model);

	void Update();
	void Collision(CCollider* m, CCollider* o);
};

#endif
