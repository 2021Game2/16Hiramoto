#ifndef CPLAYER_H
#define CPLAYER_H
#include"CBullet.h"
#include"CCharacter.h"
#include"CColliderLine.h"
#include"CBullet.h"
#include"CText.h"
#include"CText.h"
#include"CParent.h"
class CWeapon;
class CPlayer : public CCharacter {
public:

    CPlayer();
    CColliderLine mLine;//�����R���C�_
    CColliderLine mLine2;//�����R���C�_
    CColliderLine mLine3;//�����R���C�_
    static int mSpAttack;
    //CBullet bullet();
    CModel mModelW;
    void Update();
   //�Փˏ���
    void Collision(CCollider* m, CCollider* o);
    void TaskCollision();
    CText mText;
    void Render();
    CCollider mCollider;//�{�̗p�R���C�_
    CCollider mCollider2;
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
    CVector mCollisionEnemy;
    int mCount;
    int mJump;
    static int mAction1;
    static int mAction2;
    static int mAction3;
    int mHp;
    //�q�̃C���X�^���X
    CChild mChild;
   // CWeapon* Weapon;
};
#endif
  
