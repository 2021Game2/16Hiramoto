#ifndef CMATERIAL_H
#define CMATERIAL_H
#include "CTexture.h"
/*
マテリアルクラス
マテリアルのデータを使う
*/
class CMaterial {
public:
	//マテリアル名
	char mName[64];
	//拡張光の色R,G,B,A
	float mDiffuse[4];
	//テクスチャ
	CTexture mTexture;
	//マテリアルを無効にする
	void Disabled();
	//デフォルトコンストラクタ
	CMaterial();
	//マテリアルを有効にする
	void Enabled();
	//マテリアルごとの頂点数(ポインタ）
	int mVertexNum;
};



#endif
