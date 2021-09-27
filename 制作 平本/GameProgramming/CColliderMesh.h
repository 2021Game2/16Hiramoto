#ifndef CCOLLIDERMESH_H
#define CCOLLIDERMESH_H

#include "CModel.h"
#include"CColliderTriangle.h"

class CColliderMesh {
public:
	//三角コライダの配列作成
	CColliderTriangle* mpColliderTriangles;
	//コンストラクタ
	CColliderMesh();
	//デストラクタ
	~CColliderMesh();
	//Set（親、親行列、モデル）
	//モデルから三角コライダの作成
	//parent,,,親のインスタンス
	//matrix,,,	コライダの拡縮、回転、移動を行う合成行列
	//model,,,コライダを生成する元となるモデル
	void Set(CCharacter* parent, CMatrix* matrix, CModel* model);


};
#endif
