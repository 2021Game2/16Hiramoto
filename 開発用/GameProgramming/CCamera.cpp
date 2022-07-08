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
//�J�����̊O���ϐ�
CCamera* Camera;

#define WIN_CENTRAL_X WINDOW_WIDTH/2 //��ʂ̒����iX���j
#define WIN_CENTRAL_Y WINDOW_HEIGHT/2 //��ʂ̒��� �iY���j

CCamera* CCamera::mpCameraInstance;

//�v���C���[�̃|�C���^��Ԃ����ƂŁA���W�Ȃǂ��Q�Ƃł���悤�ɂȂ�
CCamera* CCamera::GetInstance()
{
	return mpCameraInstance;
}

void CCamera::Init()
{
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;//���_�A�����_�A������ivoid CCamera::Set�Ŏg�p�j
	//���_�����߂�
	e = CVector(1.0f, 2.0f, 10.0f);
	//�����_�����߂�
	c = CVector();
	//����������߂�
	u = CVector(0.0f, 1.0f, 0.0f);

	//�J�����N���X�̐ݒ�
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
	//�D��x��ݒ�
	mPriority = 100;
	CTaskManager::Get()->Remove(this);//
	CTaskManager::Get()->Add(this);//�ǉ�����
	mpCameraInstance = this;
}
void CCamera::Set(const CVector &eye, const CVector &center,
	const CVector &up) {
	mEye = eye;
	mCenter = center;
	mUp = up;
	mPos = eye;
	mTarget = center;
	mAngleX = 3.50f;//�J�������A���O��
	mAngleY = 1.0f;//�J�����c�A���O��

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
	//�}�E�X�J�[�\���������������ɃJ�����̌��_�����킹��
	if (mSkip == false) {

		//CXPlayer���g�����|�C���^�Ƀv���C���[�̏���Ԃ�������������(CXPlayer�̒��̏����Ȃ̂Ń|�C���^�����K�v����j
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
	mDist = 8.0f;//�J�������s���i���̐��ŗ���ĕ��̐��ŋ߂Â��j
	//Y������ 0�`3.14��180�x�͈�
	if (mAngleY < 0.05f) mAngleY = 0.05f;
	if (mAngleY > 3.12f) mAngleY = 3.12f;
	
	mPos.mX = mTarget.mX + (sinf(mAngleX)) * (mDist * sinf(mAngleY));
	mPos.mY = mTarget.mY + cosf(mAngleY) * mDist;
	mPos.mZ = mTarget.mZ + (cosf(mAngleX)) * (mDist * sinf(mAngleY));
	mCenter = mTarget;
	mCenter.mY += DEF_CAMERA_HEAD_ADJUST;//����␳
	mEye = mPos;
	mColliderLine.Set(this, nullptr, mEye, mCenter);

	
	/*
	oldMouseX = mouseX;
	oldMouseY = mouseY;
	*/
	CInput::InputReset();

}
//Render���Ə���ɌĂ΂��̂ŏꏊ��ς���
void CCamera::CameraRender() {
	//�J�����̈ʒu�������ɐݒ�ł���
	//float shadowColor[] = { 0.4f, 0.4f, 0.4f, 0.2f };  //�e�̐F
	//float lightPos[] = { 50.0f, 160.0f, 50.0f };  //�����̈ʒu
	//gluLookAt(lightPos[0], lightPos[1], lightPos[2], lightPos[0] - 1, 0, lightPos[2] - 1, 0.0, 1.0, 0.0);
	//��x�����Ăяo��
	
	//�J�����̈ʒu���v���C���[�ɐݒ�
	gluLookAt(mEye.mX, mEye.mY, mEye.mZ,
		mCenter.mX, mCenter.mY, mCenter.mZ,
		mUp.mX, mUp.mY, mUp.mZ);
		
	//�J�����s��i�[
	glGetFloatv(GL_MODELVIEW_MATRIX, mMatrix.mF);
}
CMatrix CCamera::GetMat() {
	return mMatrix;
}

bool CCamera::WorldToScreen(CVector* pOut, const CVector& pos)
{
	//���W�ϊ�
	CVector	screen_pos = mMatrix * pos;

	//��ʊO�Ȃ̂Ń��^�[��
	if (screen_pos.mZ >= 0.0f) {
		return false;
	}

	float Z = -screen_pos.mZ;
	//���W����
	screen_pos = screen_pos / -screen_pos.mZ;

	//	printf("%f,%f,%f\n", screen_pos.mX, screen_pos.mY, screen_pos.mZ);

	//�X�N���[���ϊ�
	pOut->mX = (1.0f + screen_pos.mX) * WINDOW_WIDTH * 0.5f;
	pOut->mY = (1.0f + screen_pos.mY) * WINDOW_HEIGHT * 0.5f;
	pOut->mZ = Z; //screen_pos.mZ

	return true;
}

void CCamera::Collision(CCollider* m, CCollider* o) {
	m->mType = CCollider::ELINE;
	//����̃R���C�_�̐ݒ�


	if (o->mType == CCollider::ETRIANGLE) {
		CVector adjust;//�����p�x�N�g��
		if (CCollider::CollisionTriangleLine(o,m, &adjust)) {
			//�}�b�v���ɏՓ˂���ƁA���_���v���C���[�ɋ߂Â�
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