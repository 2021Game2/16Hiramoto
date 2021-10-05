#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXPlayer : public CXCharacter
{
public:
    int mColliderCount;
    int mRotationCountFirst;
    int mRotationCount;
    int mRotationCount2;
    int mRotationCount3;
    int mRotationCount4;
    int mGravity;
    int mGravity2;
    static  int mStamina;
    static int mSpaceCount1;
    static int mSpaceCount2;
    static int mSpaceCount3;
    static int mWeaponCount;
    int mCount;
    int mJump;
    static int mAction1;
    static int mAction2;
    static int mAction3;
    static int mSpAttack;
    int mHp;
	//�R���C�_�̐錾
	CCollider mColSphereBody;	//��
	CCollider mColSphereHead;	//��
	CCollider mColSphereSword;	//��

	CXPlayer();

	/*
	������(X���f���N���X�̃|�C���^)
	*/
	void Init(CModelX* model);

	void Update();
};

#endif
