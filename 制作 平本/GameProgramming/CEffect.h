#ifndef CEFFECT_H
#define CEFFECT_H
#include"CBillBoard.h"
/*エフェクトクラス
テクスチャのアニメーション*/
class CEffect :public CBillBoard {
public:
	//行数
	int mRows;
	//列数
	int mCols;
	//１コマのフレーム数
	int mFps;
	//フレームカウンタ
	int mFrame;
	//マテリアル	
	static CMaterial sMaterial;
	//コンストラクタ
   //CEffect(位置、幅、高さ、テクスチャ名、行数、列数、１コマあたりのフレーム数）行数、列数、フレーム数はデフォルト引数で呼び出し時省略可能
	CEffect(const CVector& pos, float w, float h, char* texture, int row = 1, int col = 1, int fps = 1);
	//更新
	void Update();
	//描画
	void Render();

};
#endif
