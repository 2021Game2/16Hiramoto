#ifndef CMATRIX_H
#define CMATRIX_H
/*
マトリクスクラス
４行４列のデータを扱う*/
class CMatrix {
public:
		//４*４のデータを設定
		float mM[4][4];
	//表示確認用
	void Print();
	//デフォルトコンストラクタ
	CMatrix();
	//単位行列の作成
	CMatrix Identity();
	//拡大縮小行列の作成
	//Scale(倍率X、倍率Y、倍率Z,）
	CMatrix CMatrix::Scale(float sx, float sy, float sz);
	//回転行列（Y軸）の作成
	//RotateY（角度)
	
	CMatrix RotateY(float degree);
	//回転行列（Z軸）の作成
	CMatrix RotateZ(float degree);
	//回転行列（X軸）の作成
	CMatrix RotateX(float degree);
	//移動行列の作成
	//Translate(移動量X、移動量Y,移動量Z）
	CMatrix Translate(float mx, float my, float mZ);
	//*演算子のオーバーロード
	//CMatrix*CMatrixの演算結果をかえす
	CMatrix operator*(const CMatrix& m);
	
};
#endif