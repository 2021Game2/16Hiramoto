#ifndef CEFFECT2_H
#define CEFFECT2_H
//ビルボードクラスのインクルード
#include "CBillBoard2.h"
/*
エフェクトクラス
テクスチャのアニメーション
*/
class CEffect2 : public CBillBoard2 {

public:
	enum EffType {
		EFF_ATTACK ,
		EFF_ATTACK2,
		EFF_ATTACK3,
		EFF_ATTACKSP,
		EFF_DAMAGE,
		EFF_EXP,
		EFF_MAX,
	};
private:
	EffType mEffType;
	CVector	mMove;
	CVector	mPower;
	float	mRotate;
	float	mStretch;
	float	mAngle;

public:
	//行数
	int mRows;
	//列数
	int mCols;
	//1コマのフレーム数
	int mFps;
	//フレームカウンタ
	int mFrame;
	//マテリアル
	static CMaterial sMaterial[EFF_MAX];
	
	//マテリアルの事前読み込み
	static void TexPreLoad();
	//コンストラクタ
	//CEffect(位置, 幅, 高さ, テクスチャ名, 行数, 列数, 1コマあたりのフレーム数)　行数、列数、フレーム数はデフォルト引数で呼出し時省略可能
	CEffect2(const CVector& pos, float w, float h, EffType efftype, int row = 1, int col = 1, int fps = 1);
	~CEffect2();
	//移動更新
	void MoveUpdate();
	//更新
	void Update();
	//描画
	void Render();
};
#endif

