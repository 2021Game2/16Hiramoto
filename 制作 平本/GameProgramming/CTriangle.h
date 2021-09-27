#ifndef CTRIANGLE _H
#define CTRIANGLE _H
#include"CVector.h"

/*
三角形クラス
*/
class CTriangle {
public:
	int mMaterialIdx;//マテリアル番号
	CVector mV[3];//頂点座標[1～3]
	CVector mN[3];//法線ベクトル
	CVector mUv[3];//テクスチャマッピング
	//頂点座標設定
	//SetVertex（頂点１，頂点２，頂点３）
	void SetVertex(const CVector& v0, const CVector& v1, const CVector& v2);
	//法線設定
	//SetNormal（法線ベクトル1、 法線ベクトル２，法線ベクトル3）
	void SetNormal(const CVector& n); //ｎ＝法線
    void SetNormal(const CVector& v0, CVector const& v1, CVector const& v2);
	//描画
	void Render();
	void Render(const CMatrix& m);
};
#endif