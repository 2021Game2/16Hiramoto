#include "CEffect2.h"

#include <cassert>
CMaterial CEffect2::sMaterial[CEffect2::EFF_MAX]; //マテリアル.テクスチャ

void CEffect2::TexPreLoad()
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
CEffect2::CEffect2(const CVector &pos, float Scale, float Rotate, CEffect2::EffType efftype, int row, int col, int fps)
: CBillBoard2(pos, Scale, Rotate), mRows(row), mCols(col), mFps(fps), mFrame(0)
{

	//テクスチャを読んでない場合は読む
	assert(sMaterial[efftype].mTexture.mId != 0);
}
CEffect2::~CEffect2()
{
}
void CEffect2::MoveUpdate() {
	
}
void CEffect2::Update() {
	//コマ数の計算
	int frame = mFrame++ / mFps;
	if (frame >= mRows*mCols)
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

	mUv[0] = CVector(left, top, 0.0f);
	mUv[1] = CVector(left, bot, 0.0f);
	mUv[2] = CVector(righ, top, 0.0f);
	mUv[3] = CVector(righ, bot, 0.0f);


	//ビルボード更新
	CBillBoard2::Update();
}

void CEffect2::Render()
{
	glDisable(GL_DEPTH_TEST); //深度テスト無効
	CBillBoard2::Render(&sMaterial[mEffType]);
	glEnable(GL_DEPTH_TEST); //深度テスト有効
}
