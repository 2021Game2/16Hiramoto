#include "CCamera.h"
#include "Ckey.h"
#include "glew.h"
#include "glut.h"
#include <corecrt_math.h>
#include <stdio.h>
#include "CInput.h"
#include "main.h"
#include"CXPlayer.h"
#define _USE_MATH_DEFINES
#include "CUtil.h"
#include "CTaskManager.h"
//カメラの外部変数
CCamera* Camera;

#define WIN_CENTRAL_X WINDOW_WIDTH/2 //画面の中央（X軸）
#define WIN_CENTRAL_Y WINDOW_HEIGHT/2 //画面の中央 （Y軸）

CCamera* CCamera::mpCameraInstance;

//プレイヤーのポインタを返すことで、座標などが参照できるようになる
CCamera* CCamera::GetInstance()
{
	return mpCameraInstance;
}

void CCamera::Init()
{
	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向（void CCamera::Setで使用）
	//視点を求める
	e = CVector(1.0f, 2.0f, 10.0f);
	//注視点を求める
	c = CVector();
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f);

	//カメラクラスの設定
	Set(e, c, u);
}

CCamera::CCamera()

	:mSkip(true)
	,mColliderLine(this, nullptr, mEye, mCenter)
	, mAngleX(0.0f)
	, mAngleY(0.0f)
	, mDist(0.0f)
{
	mColliderLine.mType = CCollider::ELINE;
	//優先度を設定
	mPriority = 100;
	CTaskManager::Get()->Remove(this);//
	CTaskManager::Get()->Add(this);//追加する
	mpCameraInstance = this;
}
void CCamera::Set(const CVector &eye, const CVector &center,
	const CVector &up) {
	mEye = eye;
	mCenter = center;
	mUp = up;
	mPos = eye;
	mTarget = center;
	mAngleX = 3.50f;//カメラ横アングル
	mAngleY = 1.0f;//カメラ縦アングル

	mDist = DEF_CAMERA_DIST;
}

void CCamera::SetTarget(const CVector& target)
{
	mTarget = target;
}
void CCamera::Update() {
	static int oldMouseX(0), oldMouseY(0);
	int mouseX(0), mouseY(0);
	CInput::GetMousePosW(&mouseX, &mouseY);
	float moveX = (float)(oldMouseX - mouseX);
	float moveY = (float)(oldMouseY - mouseY);
	//マウスカーソルが動いた方向にカメラの原点をあわせる
	if (mSkip == false) {

		//CXPlayerを使ったポインタにプレイヤーの情報を返す処理をさせる(CXPlayerの中の処理なのでポインタを作る必要あり）
		CXPlayer* tPlayer = CXPlayer::GetInstance();
		if (tPlayer->mAttackSp ==false){
			if (moveX != 0) mAngleX += (moveX * 0.005f);
		    if (moveY != 0) mAngleY += (moveY * 0.005f);
	    }
	}
	mSkip = false;
	int X = WIN_CENTRAL_X;
	int Y = WIN_CENTRAL_Y;
	CInput::SetMousePosW(X, Y);
	oldMouseX = X;
	oldMouseY = Y;
    /*
	int wheel = CInput::GetWheelValue();
	if (wheel != 0) {
		mDist -= (float)(wheel)*0.5f;
	}*/
	mDist = 8.0f;//カメラ奥行き（正の数で離れて負の数で近づく）
	//Y軸制限 0〜3.14が180度範囲
	if (mAngleY < 0.05f) mAngleY = 0.05f;
	if (mAngleY > 3.12f) mAngleY = 3.12f;
	
	mPos.mX = mTarget.mX + (sinf(mAngleX)) * (mDist * sinf(mAngleY));
	mPos.mY = mTarget.mY + cosf(mAngleY) * mDist;
	mPos.mZ = mTarget.mZ + (cosf(mAngleX)) * (mDist * sinf(mAngleY));

	mCenter = mTarget;
	mCenter.mY += DEF_CAMERA_HEAD_ADJUST;//頭上補正
	mEye = mPos;
	mColliderLine.Set(this, nullptr, mEye, mCenter);

	
	/*
	oldMouseX = mouseX;
	oldMouseY = mouseY;
	*/
	CInput::InputReset();

}
//Renderだと勝手に呼ばれるので場所を変える
void CCamera::CameraRender() {
	//カメラの位置を光源に設定できる
	//float shadowColor[] = { 0.4f, 0.4f, 0.4f, 0.2f };  //影の色
	//float lightPos[] = { 50.0f, 160.0f, 50.0f };  //光源の位置
	//gluLookAt(lightPos[0], lightPos[1], lightPos[2], lightPos[0] - 1, 0, lightPos[2] - 1, 0.0, 1.0, 0.0);
	//一度だけ呼び出す
	
	//カメラの位置をプレイヤーに設定
	gluLookAt(mEye.mX, mEye.mY, mEye.mZ,
		mCenter.mX, mCenter.mY, mCenter.mZ,
		mUp.mX, mUp.mY, mUp.mZ);
		
	//カメラ行列格納
	glGetFloatv(GL_MODELVIEW_MATRIX, mMatrix.mF);
}
CMatrix CCamera::GetMat() {
	return mMatrix;
}

bool CCamera::WorldToScreen(CVector* pOut, const CVector& pos)
{
	//座標変換
	CVector	screen_pos = mMatrix * pos;

	//画面外なのでリターン
	if (screen_pos.mZ >= 0.0f) {
		return false;
	}

	float Z = -screen_pos.mZ;
	//座標調整
	screen_pos = screen_pos / -screen_pos.mZ;

	//	printf("%f,%f,%f\n", screen_pos.mX, screen_pos.mY, screen_pos.mZ);

	//スクリーン変換
	pOut->mX = (1.0f + screen_pos.mX) * WINDOW_WIDTH * 0.5f;
	pOut->mY = (1.0f + screen_pos.mY) * WINDOW_HEIGHT * 0.5f;
	pOut->mZ = Z; //screen_pos.mZ

	return true;
}

void CCamera::Collision(CCollider* m, CCollider* o) {
	m->mType = CCollider::ELINE;
	//相手のコライダの設定


	if (o->mType == CCollider::ETRIANGLE) {
		CVector adjust;//調整用ベクトル
		if (CCollider::CollisionTriangleLine(o,m, &adjust)) {
			//マップ等に衝突すると、視点をプレイヤーに近づく
			mEye += (adjust + adjust.Normalize()*0.5f);
			mColliderLine.Set(this, nullptr, mEye, mCenter);
			
		}
	}
}
void CCamera::Render() {

}
void CCamera::TaskCollision() {
	mColliderLine.ChangePriority();
	CCollisionManager::Get()->Collision(&mColliderLine, COLLISIONRANGE);
}