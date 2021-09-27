#ifndef CVECTOR_H
#define CVECTOR_H
#include"CMatrix.h"

/*
ベクトルクラス
ベクトルデータの扱う*/

class CVector {
public:


	//+演算子のオ-バーロード
	//CVector+CVector の演算結果をかえす
	CVector operator+(const CVector& v);

//デフォルトコンストラクタ
    CVector();
	CVector(float x, float y, float z);//引数コンストラクタ

	//内積
	//Dot(ベクトル）
	float Dot(const CVector& v);

	//外積
	//Cross(ベクトル）
	CVector Cross(const CVector& v);
	//*演算子のオーバーロード
	//CVector*floatの演算結果をかえす	
	CVector operator*(const float& f);

	//正規化
	//大きさ１のベクトルをかえす
	CVector Normalize();

//コンストラクタ
//CVector(X座標、Y座標、Z座標）

	//３D各軸での値を設定
	float mX, mY, mZ;

	void Set(float x, float y, float z);
	/*
	x;x座標の値を代入
	y；y座標の値を代入
	z；z座標の値を代入*/
	//CVector*CMatrixの結果をCVectorで返す
	CVector operator*(const CMatrix &m);
	
	//演算子のオーバーロード
	//CVector-Cvectorの演算結果をかえす
	CVector operator-(const CVector& v);
	//ベクトルの長さをかえす
	float Length();

};

#endif