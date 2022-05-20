#include "CBillBoard2.h"
#include "CCamera.h"
#define _USE_MATH_DEFINES
#include <math.h>

//Task�̃v���C�I���e�B����Ԍ��ɂ���悤��CCharacter�̈������P��
CBillBoard2::CBillBoard2()
	:CCharacter(1),mBillBoardScale(1.0f)
{

}

CBillBoard2::CBillBoard2(CVector pos, float scale, float rotate)
	: CCharacter(1), mBillBoardScale(scale)
{
	Set(pos, scale, rotate);
}

void CBillBoard2::Set(CVector pos, float scale, float rotate) {
	//�ʒu
	mPosition = pos;
	//�傫���̐ݒ�
	mBillBoardScale = scale;
	mRotation.mZ = rotate;
	for (int i = 0; i < 4; i++) {
		mN[i] = CVector(0.0f, 0.0f, 1.0f);
	}

	//�F�𔒐F��ݒ�
	mMaterial.mDiffuse[0] = mMaterial.mDiffuse[1] =
	mMaterial.mDiffuse[2] = mMaterial.mDiffuse[3] = 1.0f;
}


void CBillBoard2::Update() {
	////�J�����ւ̕����x�N�g���̎擾
	//CVector dir = Camera.mEye - mPosition;
	////Y���̉�]�l�����߂�
	//mRotation.mY = atan2f(dir.mX, dir.mZ) * 180.0f / M_PI;
	////X���̉�]�l�����߂�
	//mRotation.mX = asinf(-dir.mY / dir.Length()) * 180.0f / M_PI;
	////�s��̍X�V
	//CTransform::Update();
}

void CBillBoard2::Render()
{
	Render(&mMaterial);
}
//Render(�}�e���A���̃|�C���^)
void CBillBoard2::Render(CMaterial *mpMaterial) {

	CMatrix mat;
	mat = Camera.GetMat();

	float	x1, x2, y1, y2, s, c;
	CVector		v[4];

	//mRotation.mZ += 0.1f;//�e�X�g�ŉ�]
	//mBillBoardScale += 0.5f;//�e�X�g�ŏk��

	s = sinf(mRotation.mZ) * mBillBoardScale;
	c = cosf(mRotation.mZ) * mBillBoardScale;
	//	���_�ݒ�
	x1 = s - c;
	x2 = c + s;
	y1 = c + s;
	y2 = c - s;

	//���㍶���E��E���̏�

	v[0].mX = (mat.m00 * x1 + mat.m10 * y1) + mPosition.mX;
	v[0].mY = (mat.m01 * x1 + mat.m11 * y1) + mPosition.mY;
	v[0].mZ = (mat.m02 * x1 + mat.m12 * y1) + mPosition.mZ;
	v[1].mX = -(mat.m00 * x2 + mat.m10 * y2) + mPosition.mX;
	v[1].mY = -(mat.m01 * x2 + mat.m11 * y2) + mPosition.mY;
	v[1].mZ = -(mat.m02 * x2 + mat.m12 * y2) + mPosition.mZ;
	v[2].mX = (mat.m00 * x2 + mat.m10 * y2) + mPosition.mX;
	v[2].mY = (mat.m01 * x2 + mat.m11 * y2) + mPosition.mY;
	v[2].mZ = (mat.m02 * x2 + mat.m12 * y2) + mPosition.mZ;
	v[3].mX = -(mat.m00 * x1 + mat.m10 * y1) + mPosition.mX;
	v[3].mY = -(mat.m01 * x1 + mat.m11 * y1) + mPosition.mY;
	v[3].mZ = -(mat.m02 * x1 + mat.m12 * y1) + mPosition.mZ;


	//�s��̕ۑ�
	glPushMatrix();
	//�g�k�A��]�A�ړ�������
//	glMultMatrixf(mMatrix.mM[0]);
	//���C�g�I�t
	glDisable(GL_LIGHTING);
	//�`��F�̐ݒ�
	glColor4fv(mpMaterial->mDiffuse);
	//�}�e���A���K�p
	mpMaterial->Enabled();
//	glDisable(GL_CULL_FACE);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 4; i++) {
		glNormal3f(mN[i].mX, mN[i].mY, mN[i].mZ);
		glTexCoord2f(mUv[i].mX, mUv[i].mY);
		glVertex3f(v[i].mX, v[i].mY, v[i].mZ);
	}

	glEnd();

	//�}�e���A������
	mpMaterial->Disabled();
	//���C�g�I��
	glEnable(GL_LIGHTING);
	//�s���߂�
	glPopMatrix();
}
