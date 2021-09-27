#ifndef CMODEL_H
#define CMODEL_H
#include"CMaterial.h"
/*
モデルクラス
モデルデータの入力や表示
*/
#include<vector>
#include"CTriangle.h"
class CModel {
public:
	//モデルファイルの入力
	//Load （モデルファイル名　マテリアルファイル名）
	void Load(char* obj, char* mtl);
	//三角形の可変長配列
	std::vector<CTriangle>mTriangles;
	std::vector<CMaterial*>mpMaterials;
	//デストラクタ、、、インスタンスが削除されると実行
	~CModel();
	//頂点配列  （ポインタ変数）
	float *mpVertex;//頂点座標	
	float *mpNormal;//法線
	float *mpTextureCoord;//テクスチャマッピング
	//デフォルトコンストラクタ
	CModel();
	void Render(const CMatrix& m);
};
#endif
