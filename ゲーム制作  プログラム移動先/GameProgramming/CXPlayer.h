#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"
#include"CVector.h"
#include"CEnemy2.h"
class CXPlayer : public CXCharacter
{
public:
	//�R���C�_�̐錾
	CCollider mColSphereBody;	//��
	CCollider mColSphereHead;	//��
	CCollider mColSphereSword;	//��


	
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
	������(X���f���N���X�̃|�C���^)
	*/
	void Init(CModelX* model);

	void Update();
	void Collision(CCollider* m, CCollider* o);
};

#endif
