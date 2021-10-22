#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"
#include"CVector.h"
class CXPlayer : public CXCharacter
{
public:
	//�R���C�_�̐錾
	CCollider mColSphereBody;	//��
	CCollider mColSphereHead;	//��
	CCollider mColSphereSword;	//��
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
	������(X���f���N���X�̃|�C���^)
	*/
	void Init(CModelX* model);

	void Update();
	void Collision(CCollider* m, CCollider* o);
};

#endif
