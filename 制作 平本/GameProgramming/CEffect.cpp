#include"CEffect.h"

//マテリアル、テクスチャ
CMaterial CEffect::sMaterial;
//(位置、幅、高さ、テクスチャ名、行数、列数、１コマ当たりのフレーム数
CEffect::CEffect(const CVector& pos, float w, float h, char* texture,
	int row, int col, int fps)
	//変数の数値は敵クラスなどで設定
	:CBillBoard(pos, w, h), mRows(row), mCols(col), mFps(fps), mFrame(0) {
	//テクスチャを読んでいない場合は読む
	if (sMaterial.mTexture.mId == 0);
	{
		sMaterial.mTexture.Load(texture);
		sMaterial.mDiffuse[0] = sMaterial.mDiffuse[1] =
			sMaterial.mDiffuse[2] = sMaterial.mDiffuse[3] = 1.0f;
	}
}
void CEffect::Update() {

	//コマ数の計算
	int frame = mFrame++ / mFps;
	frame %= (mRows * mCols);
	//コマ１つの大きさの設定
	//アニメーションのtgaは 行と列からなる
	//UV左(1.0（右端の値）/列数*（コマ数/列数）
	float left = 1.0f / mCols * (frame % mCols);
	//UV右（左座標+１.0/列数)
	float righ = left + 1.0f / mCols;
	//UV上(1.0-1.0/行数*コマ数/列数)
	float top = 1.0f - 1.0f / mRows * (frame / mCols);
	//UV下(上座標ー1.0/行数）
	float bot = top - 1.0f / mRows;
	//テクスチャマッピング(三角形２つでテクスチャ１つの範囲を設定）
	mT[0].mUv[0] = CVector(righ, top, 0.0f);//右上
	mT[0].mUv[1] = CVector(left, bot, 0.0f);//左下
	mT[0].mUv[2] = CVector(righ,bot, 0.0f);//右下

	mT[1].mUv[0] = CVector(left, top, 0.0f);//左上
	mT[1].mUv[1] = CVector(left, bot, 0.0f);//左下
	mT[1].mUv[2] = CVector(righ, top, 0.0f);//右上
	//ビルボード更新
	CBillBoard::Update();

	if ((mRows * mCols)==(mFrame/mFps)) {  //ｍFrame,,コマ数（frame*mFps)敵クラスから参照
      mEnabled = false;
    }
	
}
void CEffect::Render() {
	glDisable(GL_DEPTH_TEST);  //震度テスト無効
	CBillBoard::Render(&sMaterial);
	glEnable(GL_DEPTH_TEST);  //震度テスト有効

}