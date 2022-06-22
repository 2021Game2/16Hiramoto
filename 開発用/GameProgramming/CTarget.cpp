
#include"CPlayer.h"
#include"CTarget.h"
#define OBJ "Resource\\3DModel\\Target\\Target.obj"
#define MTL "Resource\\3DModel\\Target\\Target.mtl"
#include "CBoss.h"
#include"CFlag.h"
#include"CSceneGame.h"
#define VELOCITY 0.5f
CModel CTarget::mModel;//���f���f�[�^�쐬
CTarget::CTarget(const CVector& position, const CVector& rotation, const CVector& scale)
	: CTarget()
{

	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	CTransform::Update();//�s��̍X�V

	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}
CTarget::CTarget()
{
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}

}
void CTarget::Update() {

	CXPlayer* tPlayer = CXPlayer::GetInstance();
	CBoss* tBoss = CBoss::GetInstance();
	CFlag* tFlag = CFlag::GetInstance();
	if (tPlayer->mHp > 0) {
		mPosition.mX = tPlayer->mPosition.mX;
		mPosition.mY = tPlayer->mPosition.mY + 4.0f;
		mPosition.mZ = tPlayer->mPosition.mZ-1.0f;
		//mRotation.mZ += 10.0f;
		
	}
	else {
		mEnabled = false;
	}
	mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
	//�������iX���j�̃x�N�g�������߂�
	CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//������iY���j�̃x�N�g�������߂�
	CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//�O�����iZ���j�̃x�N�g�������߂�
	CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;
	//�ڕW�n�_�܂ł̃x�N�g�������߂�
	CVector vp = mPoint - mPosition;

	//���x�N�g���Ƃ̓��ς����߂�
	float dx = vp.Dot(vx);
	float dy = vp.Dot(vy);
	float margin = 0.1f;
	//���E�����։�]
	if (dx > margin) {
		
			//mRotation.mY += 3.0f;//���։�]
		
		  mRotation.mY += 0.1f;//���։�]
		
	}
	else if (dx < -margin) {
		if()
		mRotation.mY -= 0.1f;//�E�։�]
	}
	if (dy > margin) {
		mRotation.mX -= 0.1f;
	}
	else if(dy<-margin) {
		mRotation.mX += 0.1f;
	}
	
	if (CSceneGame::mBossGaugeSwitch == true) {

		mPoint = tBoss->mPosition;
	}
	else {
		mPoint = tFlag->mPosition;
	}
	//CCharacter�̍X�V
	CTransform::Update();

}