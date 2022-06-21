#ifndef CEFFECT_H
#define CEFFECT_H

//ビルボードクラスのインクルード
#include "CBillBoard.h"
/*
エフェクトクラス
テクスチャのアニメーション
*/
/*
class CEffect : public CBillBoard {

public:
	/*
enum EffType {
		EFF_ATTACK,
		EFF_ATTACK2,
		EFF_ATTACK3,
		EFF_ATTACKSP,
		EFF_DAMAGE,
		EFF_EXP,
		EFF_MAX,
	};
	*/
/*
private:
	/*
	EffType mEffType;*/
/*
	//行数
	int mRows;
	//列数
	int mCols;
	//1コマのフレーム数
	int mFps;
	//フレームカウンタ
	int mFrame;
public:
	//マテリアル
	static CMaterial sMaterial[EFF_MAX];
	//コンストラクタ
    //マテリアルの事前読み込み
	static void TexPreLoad();

	//CEffect(位置, 幅, 高さ, テクスチャ名, 行数, 列数, 1コマあたりのフレーム数)　行数、列数、フレーム数はデフォルト引数で呼出し時省略可能
	CEffect(const CVector &pos, float w, float h, EffType efftype, int row = 1, int col = 1, int fps = 1);
	//移動更新
	void MoveUpdate();
	//更新
	void Update();
	//描画
	void Render();

	CVector	mMove;
	CVector	mPower;
	float	mRotate;
	float	mStretch;
	float	mAngle;
};*/

#endif

