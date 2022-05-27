#include "CEffect.h"
#include <cassert>
/*
CMaterial CEffect::sMaterial[CEffect::EFF_MAX]; //�}�e���A��.�e�N�X�`��

void CEffect::TexPreLoad()
{
	static char* LoadPath[] = {
		"Resource\\png,tga\\Attack1.png",	//EFF_ATTACK1
		"Resource\\png,tga\\Attack2.png",	//EFF_ATTACK2
		"Resource\\png,tga\\Attack3.png",	//EFF_ATTACK3
		"Resource\\png,tga\\AttackSp.png",	//EFF_ATTACKSP
		"Resource\\png,tga\\Damage.png",	//EFF_DAMAGE
		"Resource\\png,tga\\exp.tga",		//EFF_EXP
	};

	for (int i = 0; i < EFF_MAX; i++) {

		//�e�N�X�`����ǂ�łȂ��ꍇ�͓ǂ�
		if (sMaterial[i].mTexture.mId == 0)
		{
			sMaterial[i].mTexture.Load(LoadPath[i]);
			sMaterial[i].mDiffuse[0] = sMaterial[i].mDiffuse[1] =
			sMaterial[i].mDiffuse[2] = sMaterial[i].mDiffuse[3] = 1.0f;
		}
	}
}
CEffect::CEffect(const CVector& pos, float w, float h, CEffect::EffType efftype, int row, int col, int fps)
	: CBillBoard(pos, w, h), mRows(row), mCols(col), mFps(fps), mFrame(0), mEffType(efftype)
{
	//�e�N�X�`����ǂ�łȂ��ꍇ�͓ǂ�
	assert(sMaterial[efftype].mTexture.mId != 0);
}
void CEffect::MoveUpdate() {
	
}
void CEffect::Update() {
	//�R�}���̌v�Z
	int frame = mFrame++ / mFps;
	if (frame == mRows*mCols)
	{
		mEnabled = false;
		return;
	}
	frame %= (mRows * mCols);
	//UV��
	float left = 1.0f / mCols * (frame % mCols);
	//UV�E
	float righ = left + 1.0f / mCols;
	//UV��
	float top = 1.0f - 1.0f / mRows * (frame / mCols);
	//UV��
	float bot = top - 1.0f / mRows;
	//�e�N�X�`���}�b�s���O
	mT[0].mUv[0] = CVector(righ, top, 0.0f);
	mT[0].mUv[1] = CVector(left, bot, 0.0f);
	mT[0].mUv[2] = CVector(righ, bot, 0.0f);
	mT[1].mUv[0] = CVector(left, top, 0.0f);
	mT[1].mUv[1] = CVector(left, bot, 0.0f);
	mT[1].mUv[2] = CVector(righ, top, 0.0f);
	//�r���{�[�h�X�V
	CBillBoard::Update();
}

void CEffect::Render()
{
	glDisable(GL_DEPTH_TEST); //�[�x�e�X�g����
	CBillBoard::Render(&sMaterial[mEffType]);
	glEnable(GL_DEPTH_TEST); //�[�x�e�X�g�L��
}*/
