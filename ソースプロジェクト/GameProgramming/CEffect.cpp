#include "CEffect.h"
#include <cassert>
/*
CMaterial CEffect::sMaterial[CEffect::EFF_MAX]; //マテリアル.テクスチャ

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

		//テクスチャを読んでない場合は読む
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
	//テクスチャを読んでない場合は読む
	assert(sMaterial[efftype].mTexture.mId != 0);
}
void CEffect::MoveUpdate() {
	
}
void CEffect::Update() {
	//コマ数の計算
	int frame = mFrame++ / mFps;
	if (frame == mRows*mCols)
	{
		mEnabled = false;
		return;
	}
	frame %= (mRows * mCols);
	//UV左
	float left = 1.0f / mCols * (frame % mCols);
	//UV右
	float righ = left + 1.0f / mCols;
	//UV上
	float top = 1.0f - 1.0f / mRows * (frame / mCols);
	//UV下
	float bot = top - 1.0f / mRows;
	//テクスチャマッピング
	mT[0].mUv[0] = CVector(righ, top, 0.0f);
	mT[0].mUv[1] = CVector(left, bot, 0.0f);
	mT[0].mUv[2] = CVector(righ, bot, 0.0f);
	mT[1].mUv[0] = CVector(left, top, 0.0f);
	mT[1].mUv[1] = CVector(left, bot, 0.0f);
	mT[1].mUv[2] = CVector(righ, top, 0.0f);
	//ビルボード更新
	CBillBoard::Update();
}

void CEffect::Render()
{
	glDisable(GL_DEPTH_TEST); //深度テスト無効
	CBillBoard::Render(&sMaterial[mEffType]);
	glEnable(GL_DEPTH_TEST); //深度テスト有効
}*/
