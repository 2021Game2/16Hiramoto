#include"CMatrix.h"
//標準入出力用のインクルード
#include <stdio.h>
//円周率M_PIを有効にする
#define _USE_MATH_DEFINES
//数学関数のインクルード
#include<math.h>

//デフォルトコンストラクタ
CMatrix::CMatrix() {
	//型名Identityの呼び出し
	Identity();
}
//拡大縮小行列の作成
	//Scale(倍率X、倍率Y、倍率Z,）
CMatrix CMatrix::Scale(float sx, float sy, float sz) {
	
	/*sx,sy,szを追加することでX軸、Y軸、Z軸の拡大、縮小が
	簡単にできる
	*/
	mM[0][0] = sx; mM[1][1] = sy; mM[2][2] = sz;
	//代入するとこうなる
  /*mM[0][0] = sx; mM[0][1] = 0; mM[0][2] = 0; mM[0][3] = 0;
	mM[1][0] = 0; mM[1][1] = sy; mM[1][2] = 0; mM[1][3] = 0;
	mM[2][0] = 0; mM[2][1] = 0; mM[2][2] = sz; mM[2][3] = 0;
	mM[3][0] = 0; mM[3][1] = 0; mM[3][2] = 0; mM[3][3] = 1;*/
	//この行列を返す
	return* this;
}
//単位行列の作成
CMatrix CMatrix::Identity() {
	

mM[0][0] = 1; mM[0][1] = 0; mM[0][2] = 0; mM[0][3] = 0;
mM[1][0] = 0; mM[1][1] = 1; mM[1][2] = 0; mM[1][3] = 0;
mM[2][0] = 0; mM[2][1] = 0; mM[2][2] = 1; mM[2][3] = 0;
mM[3][0] = 0; mM[3][1] = 0; mM[3][2] = 0; mM[3][3] = 1;
//この行列を返す
	return*this;
}
void CMatrix::Print() {
	
	//4つの文字列を入力	
	printf("%10f%10f%10f%10f\n",
		mM[0][0], mM[0][1], mM[0][2], mM[0][3]);
	printf("%10f%10f%10f%10f\n",
		mM[1][0], mM[1][1], mM[1][2], mM[1][3]);
	printf("%10f%10f%10f%10f\n",
		mM[2][0], mM[2][1], mM[2][2], mM[2][3]);
	printf("%10f%10f%10f%10f\n",
		mM[3][0], mM[3][1], mM[3][2], mM[3][3]);
}
//回転行列（Y軸）の作成
//RptateY(角度）
CMatrix CMatrix::RotateY(float degree) {
	//角度からラジアンを取り出す
	//M_PI=円周率の定数
	/*rad＝ラジアン（円周上で半径と同じ長さの弧
	を取るための２本の線で作られる角度）*/
	float rad = degree / 180.0f * M_PI;
	//単位行列にする
	Identity();
	//Y軸で回転する行列の設定
	//行列のsyのところが１になり、sx,szをcosにして回転
	/*cos 0 -sin 0
	  0   1   0  0
	  sin 0  cos 0
	  0   0   0  1*/
	mM[0][0] = mM[2][2] = cosf(rad);//cosθ
	mM[0][2] = -sinf(rad);//-sinθ
	mM[2][0] = -mM[0][2];//sinθ
	//行列を返す
	return *this;
}
CMatrix CMatrix::RotateZ(float degree) {
	//角度からラジアンを取り出す
	//M_PI=円周率の定数
	/*rad＝ラジアン（円周上で半径と同じ長さの弧
	を取るための２本の線で作られる角度）*/
	//degree=度数
	//180.0f=180°
	float rad = degree / 180.0f * M_PI;
	//単位行列にする
	Identity();
	//Z軸で回転する行列の設定
	//行列のszのところが１になり、sx,syをcosにして回転
	/*cos 0 -sin 0
	  sin cos 0  0
	   0  0   1  0
	   0  0   0  1*/
	mM[0][0] = mM[1][1] = cosf(rad);//cosθ
	mM[0][1] = sinf(rad);//sinθ
	mM[1][0] = -mM[0][1];//-sinθ
	//行列を返す
	return *this;
}
CMatrix CMatrix::RotateX(float degree) {
	//角度からラジアンを取り出す
	//M_PI=円周率の定数
	/*rad＝ラジアン（円周上で半径と同じ長さの弧
	を取るための２本の線で作られる角度）*/
	float rad = degree / 180.0f * M_PI;
	//単位行列にする
	Identity();
	//X軸で回転する行列の設定
	//行列のsxのところが１になり、sy,szをcosにして回転
	/*1   0   0  0
	  0  cos sin 0
	  0 -sin cos 0
	  0   0   0  1*/
	mM[1][1] = mM[2][2] = cosf(rad);//cosθ
	mM[1][2] = sinf(rad);//sinθ
	mM[2][1] = -mM[1][2];//-sinθ
	//行列を返す
	return *this;
}
CMatrix CMatrix::Translate(float mx, float my, float mz) {
	//3Dの座標を移動させる
	Identity();
	mM[3][0] = mx,
	mM[3][1] = my,
	mM[3][2] = mz;
	//この行列を返す
	return*this;
}
//*演算子のオーバーロード
//CMatrix*CMatrixの演算結果をかえす
CMatrix CMatrix::operator*(const CMatrix& m){
	
	CMatrix t;
	//回転、拡大縮小、移動の行列を合成することで、数値を変える作業が簡単になる
	//使っている行列がすべてｍM[4][4]なので合成が可能
	t.mM[0][0] = mM[0][0] * m.mM[0][0] + mM[0][1] * m.mM[1][0] + mM[0][2] * m.mM[2][0] + mM[0][3] * m.mM[3][0];
	t.mM[0][1] = mM[0][0] * m.mM[0][1] + mM[0][1] * m.mM[1][1] + mM[0][2] * m.mM[2][1] + mM[0][3] * m.mM[3][1];
	t.mM[0][2] = mM[0][0] * m.mM[0][2] + mM[0][1] * m.mM[1][2] + mM[0][2] * m.mM[2][2] + mM[0][3] * m.mM[3][2];
	t.mM[0][3] = mM[0][0] * m.mM[0][3] + mM[0][1] * m.mM[1][3] + mM[0][2] * m.mM[2][3] + mM[0][3] * m.mM[3][3];
	t.mM[1][0] = mM[1][0] * m.mM[0][0] + mM[1][1] * m.mM[1][0] + mM[1][2] * m.mM[2][0] + mM[1][3] * m.mM[3][0];
	t.mM[1][1] = mM[1][0] * m.mM[0][1] + mM[1][1] * m.mM[1][1] + mM[1][2] * m.mM[2][1] + mM[1][3] * m.mM[3][1];
	t.mM[1][2] = mM[1][0] * m.mM[0][2] + mM[1][1] * m.mM[1][2] + mM[1][2] * m.mM[2][2] + mM[1][3] * m.mM[3][2];
	t.mM[1][3] = mM[1][0] * m.mM[0][3] + mM[1][1] * m.mM[1][3] + mM[1][2] * m.mM[2][3] + mM[1][3] * m.mM[3][3];
	t.mM[2][0] = mM[2][0] * m.mM[0][0] + mM[2][1] * m.mM[1][0] + mM[2][2] * m.mM[2][0] + mM[2][3] * m.mM[3][0];
	t.mM[2][1] = mM[2][0] * m.mM[0][1] + mM[2][1] * m.mM[1][1] + mM[2][2] * m.mM[2][1] + mM[2][3] * m.mM[3][1];
	t.mM[2][2] = mM[2][0] * m.mM[0][2] + mM[2][1] * m.mM[1][2] + mM[2][2] * m.mM[2][2] + mM[2][3] * m.mM[3][2];
	t.mM[2][3] = mM[2][0] * m.mM[0][3] + mM[2][1] * m.mM[1][3] + mM[2][2] * m.mM[2][3] + mM[2][3] * m.mM[3][3];
	t.mM[3][0] = mM[3][0] * m.mM[0][0] + mM[3][1] * m.mM[1][0] + mM[3][2] * m.mM[2][0] + mM[3][3] * m.mM[3][0];
	t.mM[3][1] = mM[3][0] * m.mM[0][1] + mM[3][1] * m.mM[1][1] + mM[3][2] * m.mM[2][1] + mM[3][3] * m.mM[3][1];
	t.mM[3][2] = mM[3][0] * m.mM[0][2] + mM[3][1] * m.mM[1][2] + mM[3][2] * m.mM[2][2] + mM[3][3] * m.mM[3][2];
	t.mM[3][3] = mM[3][0] * m.mM[0][3] + mM[3][1] * m.mM[1][3] + mM[3][2] * m.mM[2][3] + mM[3][3] * m.mM[3][3];

	return t;
}