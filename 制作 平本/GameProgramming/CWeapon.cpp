#include"CWeapon.h"
#include"CVector.h"

#include"CTaskManager.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#define OBJ "Weapon.obj"//モデルのファイル
#define MTL "Weapon.mtl"//モデルのマテリアルファイル
#include"CCharacter.h"
#include"CMatrix.h"
#include"CsceneGame.h"
#include"CKey.h"
/*
CWeapon::CWeapon(CModel* model, CVector position, CVector rotation, CVector scale) 
	
	: mCollider(this, &mMatrix, CVector(0.0f, 8.0f, 0.0f), 1.5f)
	, mRotationCountFirst(1)
	, mRotationCount(0)
	, mRotationCount2(0)
	, mRotationCount3(0)
	, mRotationCount4(0)

	,mStamina2(400)

{
	//モデル、位置、回転、拡縮を設定（クラス内のみ適用）
	mpModel = model;//モデル
	mPosition = position;//位置
	mRotation = rotation;//回転
	mScale = scale; //拡縮
	mModelW.Load("Weapon.obj", "Weapon.mtl");
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する

	CTransform::Update();//行列の更新
}


void CWeapon::Update(){
	//後ろ
	 if (CPlayer::mWeaponCount == 1) {
		 if (mRotation.mY <=  180) {
           mRotation.mY += 180;
		 }
		
	}
	 //左
	else if (CPlayer::mWeaponCount == 2) {
		 if (mRotation.mY >= -90) {
			 mRotation.mY -= 90;
		 }
	}
	 //右
	else if (CPlayer::mWeaponCount == 3) {
		 if (mRotation.mY <= 90) {
			 mRotation.mY += 90;
		 }
	}
	 

	//左向き
	if (CKey::Push('A')) {
		mRotation.mY += 3;
	}
	//右向き
	if (CKey::Push('D')) {
		mRotation.mY -= 3;
	}

		if (CPlayer::mAction1 <60) {
			//if (mPosition.mY <= 3.0f) {
				mPosition.mY += 3;
			//}
            mRotation.mX += 20.0f;
           CPlayer::mAction1++;
		}
		else if (CPlayer::mAction2 <60) {
		if (mRotation.mX < 90) {
        	mRotation.mX += 90.0f;
		}
			mRotation.mY += 50.0f;
			CPlayer::mAction2++;
		}
 		else if (CPlayer::mAction3 <60) {
			mRotation.mZ += 20.0f;
			CPlayer::mAction3++;
		}
    	
		else {

			mEnabled = false;
		}
	    
		CTransform::Update();
}
void CWeapon::Collision(CCollider* m, CCollider* o) {

	switch (o->mType) {
	case CCollider::ESPHERE://球コライダのとき
		
		break;

	case CCollider::ETRIANGLE://三角コライダのとき
		CVector adjust;//調整値
		//三角コライダと球コライダの衝突判定
		//adjust、、、調整値
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//衝突しない位置まで戻す
			mPosition = mPosition + adjust;
		}
		break;
	}
}

void CWeapon::TaskCollision()
{

	//コライダの優先度変更
	mCollider.ChangePriority();
	
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	

}*/

