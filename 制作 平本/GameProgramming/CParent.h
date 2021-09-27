#ifndef CPARENT_H
#define CPARENT_H

#include "CCharacter.h"
#include "CCollider.h"

//子クラス
class CChild : public CCharacter
{
private:
	static CModel sModel;
	static int sCount;
	//親へのポインタ
	CCharacter* mpParent;
	//コライダ
	CCollider mCollider;
	CVector mInitPose;
   
public:
	CChild(CCharacter* parent);
	void CChild::Collision(CCollider* m, CCollider* o);
	void Update();
	void CChild::TaskCollision();
};


#endif
