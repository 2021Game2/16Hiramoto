
#include"CPlayer.h"
#include"CTarget.h"
#define OBJ "Resource\\3DModel\\Target\\Target.obj"
#define MTL "Resource\\3DModel\\Target\\Target.mtl"
#include "CBoss.h"
#include"CFlag.h"
#include"CSceneGame.h"
#define VELOCITY 0.5f
CModel CTarget::mModel;//モデルデータ作成
CTarget::CTarget(const CVector& position, const CVector& rotation, const CVector& scale)
	: CTarget()
{

	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	CTransform::Update();//行列の更新

	//優先度を１に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
}
CTarget::CTarget()
{
	//モデルのポインタ設定
	mpModel = &mModel;
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}

}
void CTarget::Update() {
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	CXPlayer* tPlayer = CXPlayer::GetInstance();
	CBoss* tBoss = CBoss::GetInstance();
	CFlag* tFlag = CFlag::GetInstance();
	if (tPlayer->GetHp() > 0) {
		mPosition.mX = tPlayer->mPosition.mX;
		mPosition.mY = tPlayer->mPosition.mY + 4.0f;
		mPosition.mZ = tPlayer->mPosition.mZ - 1.0f;
		//mRotation.mZ += 10.0f;

	}
	else {
		mEnabled = false;
	}
	//mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
	//左向き（X軸）のベクトルを求める
	CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//上向き（Y軸）のベクトルを求める
	CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//前方向（Z軸）のベクトルを求める
	CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;
	//目標地点までのベクトルを求める
	CVector vp = mPoint - mPosition;

	//左ベクトルとの内積を求める
	float dx = vp.Dot(vx);
	float dy = vp.Dot(vy);
	float margin = 0.1f;
	float margin3 = 3.0f;
	float margin2 = 1.0f;
	float marginX = 0.7f;
	//左右方向へ回転
	if (dx > margin) {
		if (dx > margin2) {
			if (dx > margin3) {
				mRotation.mY += 3.0f;//左へ回転
			}
			mRotation.mY += 1.0f;//左へ回転
		}
		mRotation.mY += 0.1f;//左へ回転

	}
	else if (dx < -margin) {
	   if (dx < -margin2) {
			if (dx < -margin3) {
				mRotation.mY -= 3.0f;
			}
			mRotation.mY -= 1.0f;
	   }
		mRotation.mY -= 0.1f;//右へ回転
	}
	if (dy > marginX) {
		if (dy > margin2) {
			if (dy > margin3) {
				mRotation.mX -= 3.0f;
			}
			mRotation.mX -= 1.0f;
		}
		mRotation.mX -= 0.1f;
	}
	else if (dy < -marginX) {
		if(dy < -margin2){
			if (dy < -margin3) {
				mRotation.mX += 3.0f;
			}
			mRotation.mX += 1.0f;
         }
		mRotation.mX += 0.1f;
	}
	
	if (tSceneGame->mBossGaugeSwitch == true) {

		mPoint = CVector(tBoss->mPosition.mX, tBoss->mPosition.mY+3.0f, tBoss->mPosition.mZ);
	}
	else {
		mPoint = CVector(tFlag->mPosition.mX, tFlag->mPosition.mY + 4.0f , tFlag->mPosition.mZ);
	}
	//CCharacterの更新
	CTransform::Update();

}