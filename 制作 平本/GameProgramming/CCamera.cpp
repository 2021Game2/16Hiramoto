#include"CCamera.h"
#include"glut.h"
#include"CPlayer.h"
#include"CKey.h"
//ƒJƒƒ‰‚ÌŠO•”•Ï”
CCamera Camera;
//CPlayer Player;
void CCamera::Set(const CVector& eye, const CVector& center,
	const CVector& up) {
	mEye = eye;
	mCenter = center;
	mUp = up;
}
void CCamera::Update() {
	
}
void CCamera::Render() {
	gluLookAt(mEye.mX, mEye.mY, mEye.mZ, mCenter.mX, mCenter.mY, mCenter.mZ,
		mUp.mX, mUp.mY, mUp.mZ);
}