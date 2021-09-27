#include "CParent.h"
#include "Ckey.h"
#include"CPlayer.h"
#include"CItem.h"
#define INIT_POSE CVector(0.0f, 0.0f, 1.0f) //初期の姿勢
#define VELOCITY CVector(0.0f, 0.0f, 2.0f) //移動速度
#define ROTATE_V CVector(0.0f, 2.0f, 0.0f) //回転速度
#define COLLIDER_POS CVector(0.0f, 7.0f, 0.0f) //コライダの位置
#define PARENT_MODEL "Weapon.obj","Weapon.mtl"



CModel CChild::sModel;
int CChild::sCount = 0;

CChild::CChild(CCharacter* parent)
	: mCollider(this, &mMatrix, CVector(0.0f, 7.0f, 0.0f), 2.0f)
	
{
	mCollider.mTag = CCollider::EWEAPONCOLLIDER;
	mTag = EWEAPON;
	mInitPose =CVector (0.0f, 0.0f, 1.0f);
	//親のインスタンスの退避
	mpParent = parent;

	if (sCount == 0)
	{
		
		sModel.Load(PARENT_MODEL);
	}
	mpModel = &sModel;
	//sCount++;
	mScale = CVector(5.5f, 7.5f, 5.5f);
	mPosition = CVector(0.0f, 1.5f, 0.0f);
	mRotation = INIT_POSE;
	CTransform::Update();

}


void CChild::Update()
{
	
	if (CItem::mItemCount > 0) {
        CItem::mItemCount--;
		
		mCollider.mRadius = 5.0;
		
	}
	else if(CItem::mItemCount <=0)  {
		mCollider.mRadius = 2.0;
	}
	if (CPlayer::mAction1 < 60) {
		mRotation.mX += 20.0f;
		CPlayer::mAction1++;
	}
	else if (CPlayer::mAction2 < 3) {
		mRotation = mInitPose;
		CPlayer::mAction2++;
	}
	else if (CPlayer::mAction2 < 60) {
        if (mRotation.mX < 90) {
			mRotation.mX = 90.0f;
		}
        CPlayer::mAction2++;
		mRotation.mY += 40.0f;
	}
	else if (CPlayer::mAction3 < 3) {
			
		mPosition=mInitPose;
		CPlayer::mAction3++;
	}
	else if (CPlayer::mAction3 < 60) {
		mRotation.mX += 20.0f;
        mRotation.mZ += 20.0f;
		CPlayer::mAction3++;
		
	}
	
	else if(CPlayer::mAction1>=60&& CPlayer::mAction2 >= 60&& CPlayer::mAction3 >= 60) {
        mRotation = INIT_POSE;
	}
	//子の行列の更新
	CTransform::Update();
	//自分の行列に親の行列を掛けて、最終的な合成行列にする
	mMatrix = mMatrix * mpParent->mMatrix;
}

void CChild::Collision(CCollider* m, CCollider* o) {
	
}

void CChild::TaskCollision()
{

	//コライダの優先度変更
	mCollider.ChangePriority();

	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);


}

