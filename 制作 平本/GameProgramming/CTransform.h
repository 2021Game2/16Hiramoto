#ifndef CTRANSFORM_H
#define CTRANSFORM_H
#include"CVector.h"

class CTransform {
public:
	//CMatrixクラスにあるものを利用
	CVector mPosition;//位置
	CVector mRotation;//回転
	CVector mRotation2;//回転
	CVector mScale;//拡大縮小
	CMatrix mMatrixTranslate;//平行移動行列
	CMatrix mMatrixRotate;//回転行列
	CMatrix mMatrixRotate2;
	CMatrix mMatrixScale;//拡大縮小行列；

	CMatrix mMatrix;//合成行列
	//行列更新処理
	void Update();

};
#endif
