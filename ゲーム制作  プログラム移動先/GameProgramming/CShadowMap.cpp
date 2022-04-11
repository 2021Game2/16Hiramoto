/*
#include"CShadowMap.h"
#include"CTaskManager.h"
#include "glew.h"
#include "CShadowMap.h"
#include "CMatrix.h"


#define TEXWIDTH  8192  //テクスチャ幅
#define TEXHEIGHT  6144  //テクスチャ高さ


float shadowColor[] = { 0.4f, 0.4f, 0.4f, 0.2f };  //影の色
float lightPos[] = { 50.0f, 160.0f, 50.0f };  //光源の位置


void CShadowMap::Init()
{
	// テクスチャユニット１をDepthテクスチャで使用 
	glActiveTexture(GL_TEXTURE1);
	//テクスチャの生成
	glGenTextures(1, &mDepthTextureID);
	//使用するテクスチャのバインド
	glBindTexture(GL_TEXTURE_2D, mDepthTextureID);

	// Depthテクスチャの割り当て 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mTextureWidth, mTextureHeight, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	// テクスチャを拡大・縮小する方法の指定 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// テクスチャの繰り返し方法の指定 //
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	// 書き込むポリゴンのテクスチャ座標値のＲとテクスチャとの比較を行うようにする //
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	//もしＲの値がテクスチャの値以下なら真（つまり日向） //
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	//比較の結果をアルファ値として得る//
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);
	// アルファテストの比較関数（しきい値） //
	glAlphaFunc(GL_GEQUAL, 0.5f);

	// テクスチャ座標に視点座標系における物体の座標値を用いる //
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

	// 生成したテクスチャ座標をそのまま (S, T, R, Q) に使う //
	static const GLdouble genfunc[][4] = {
	  { 1.0, 0.0, 0.0, 0.0 },
	  { 0.0, 1.0, 0.0, 0.0 },
	  { 0.0, 0.0, 1.0, 0.0 },
	  { 0.0, 0.0, 0.0, 1.0 },
	};

	glTexGendv(GL_S, GL_EYE_PLANE, genfunc[0]);
	glTexGendv(GL_T, GL_EYE_PLANE, genfunc[1]);
	glTexGendv(GL_R, GL_EYE_PLANE, genfunc[2]);
	glTexGendv(GL_Q, GL_EYE_PLANE, genfunc[3]);

	//テクスチャの解除
	glBindTexture(GL_TEXTURE_2D, 0);

	//フレームバッファ追加
	// フレームバッファオブジェクトを生成して結合する 
	glGenFramebuffersEXT(1, &mFb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFb);

	// フレームバッファオブジェクトにデプスバッファ用のテクスチャを結合する 
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
		GL_DEPTH_ATTACHMENT_EXT,
		GL_TEXTURE_2D, mDepthTextureID, 0);
	// カラーバッファが無いので読み書きしない 
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	//フレームバッファオブジェクトの結合を解除する 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//テクスチャユニットを0に戻す
	glActiveTexture(GL_TEXTURE0);
}






void CShadowMap::Init(int width, int height, void (*funcRender)(),
	float shadouCol[], float lightPos[]) {
	mDepthTextureID = 0;
	mFb = 0;
	mTextureHeight = height;
	mTextureWidth = width;
	mpRender = funcRender;
	mShadowCol[0] = shadouCol[0];
	mShadowCol[1] = shadouCol[1];
	mShadowCol[2] = shadouCol[2];
	mShadowCol[3] = shadouCol[3];
	mLightPos[0] = lightPos[0];
	mLightPos[1] = lightPos[1];
	mLightPos[2] = lightPos[2];
	Init();

}
void CShadowMap::Render()
{

	//日向のライト
	const GLfloat lightcol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLint	viewport[4]; //ビューポートの保存用
	CMatrix	modelview; //モデルビュー変換行列の保存用
	CMatrix	projection; //透視変換行列の保存用
	//
	** 第１ステップ：デプステクスチャの作成
	//

	/// フレームバッファオブジェクトへのレンダリング開始
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFb);

	//デプスバッファをクリアする //
	glClear(GL_DEPTH_BUFFER_BIT);

	// 現在のビューポートを保存しておく //
	glGetIntegerv(GL_VIEWPORT, viewport);

	// ビューポートをテクスチャのサイズに設定する //
	glViewport(0, 0, mTextureWidth, mTextureHeight);

	// 透視変換行列を設定する //
	glMatrixMode(GL_PROJECTION); //透視変換行列に切り替え
	glPushMatrix(); //現在の設定はスタックに保存
	glLoadIdentity(); //行列の初期化

	/// Depthテクスチャの透視変換行列を保存しておく //
	gluPerspective(75.0, (GLdouble)mTextureWidth / (GLdouble)mTextureHeight, 1.0, 100000.0);
	glGetFloatv(GL_PROJECTION_MATRIX, projection.mM[0]); //透視変換行列の保存
	// 光源位置を視点としシーンが視野に収まるようモデルビュー変換行列を設定する //
	glMatrixMode(GL_MODELVIEW); //モデルビュー行列に切り替え
	glPushMatrix(); //現在の設定はスタックに保存
	glLoadIdentity(); //行列の初期化
	//光源位置から見るように行列を設定する
	gluLookAt(mLightPos[0], mLightPos[1], mLightPos[2], mLightPos[0] - 1, 0, mLightPos[2] - 1, 0.0, 1.0, 0.0);
	// 設定したモデルビュー変換行列を保存しておく //
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview.mM[0]);

	// デプスバッファの内容だけを取得するのでフレームバッファには書き込まない //
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// したがって陰影付けも不要なのでライティングをオフにする //
	glDisable(GL_LIGHTING);

	// デプスバッファには背面のポリゴンの奥行きを記録するようにする //
	glCullFace(GL_FRONT);

	//デプステクスチャへの描画
	if (mpRender)
	{
		(*mpRender)();
	}
	// フレームバッファオブジェクトへのレンダリング終了 //
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	// テクスチャユニット１に切り替える //
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mDepthTextureID);

	// 通常の描画の設定に戻す //
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glMatrixMode(GL_PROJECTION); //透視変換行列に切り替え
	glPopMatrix(); //設定をスタックから戻す

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);
	//
	** 第２ステップ：全体の描画
	//

	//フレームバッファとデプスバッファをクリアする //
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// モデルビュー変換行列の設定 //
	glMatrixMode(GL_MODELVIEW); //モデルビュー行列に切り替え
	glPopMatrix(); //スタックから元に戻す
	// モデルビュー変換行列を保存しておく //
	CMatrix modelviewCamera;
	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewCamera.mM[0]);

	// テクスチャ変換行列を設定する //
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	// テクスチャ座標の [-1,1] の範囲を [0,1] の範囲に収める //
	glTranslated(0.5, 0.5, 0.5);
	glScaled(0.5, 0.5, 0.5);
	// テクスチャのモデルビュー変換行列と透視変換行列の積をかける //
	glMultMatrixf(projection.mM[0]);
	glMultMatrixf(modelview.mM[0]);
	// 現在のモデルビュー変換の逆変換をかけておく //
	CMatrix inverse;
	inverse.mM[0][0] = modelviewCamera.mM[0][0];
	inverse.mM[0][1] = modelviewCamera.mM[1][0];
	inverse.mM[0][2] = modelviewCamera.mM[2][0];
	inverse.mM[1][0] = modelviewCamera.mM[0][1];
	inverse.mM[1][1] = modelviewCamera.mM[1][1];
	inverse.mM[1][2] = modelviewCamera.mM[2][1];
	inverse.mM[2][0] = modelviewCamera.mM[0][2];
	inverse.mM[2][1] = modelviewCamera.mM[1][2];
	inverse.mM[2][2] = modelviewCamera.mM[2][2];
	inverse = CMatrix().Translate(-modelviewCamera.mM[3][0], -modelviewCamera.mM[3][1],
		-modelviewCamera.mM[3][2]) * inverse;
	glMultMatrixf(inverse.mM[0]);

	// モデルビュー変換行列に戻す//
	glMatrixMode(GL_MODELVIEW);

	// テクスチャマッピングとテクスチャ座標の自動生成を有効にする //
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);
	// アルファテストを有効にして影の部分だけを描画する //
	glEnable(GL_ALPHA_TEST);
	///日向の部分がもとの図形に重ねて描かれるように奥行きの比較関数を変更する //
	glDepthFunc(GL_LEQUAL);

	// 光源の明るさを日向の部分での明るさに設定 //
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);

	//テクスチャユニット0に切り替える
	glActiveTexture(GL_TEXTURE0);

	//日向の描画
	if (mpRender)
	{
		(*mpRender)();
	}

	//テクスチャユニット1に切り替える
	glActiveTexture(GL_TEXTURE1);

	// 奥行きの比較関数を元に戻す //
	glDepthFunc(GL_LESS);
	// アルファテストを無効にする //
	glDisable(GL_ALPHA_TEST);
	// テクスチャマッピングとテクスチャ座標の自動生成を無効にする //
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_2D);
	//テクスチャを解除する
	glBindTexture(GL_TEXTURE_2D, 0);
	// テクスチャ変換行列を設定する //
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glActiveTexture(GL_TEXTURE0);

	// 光源の明るさを影の部分での明るさに設定 //
	glLightfv(GL_LIGHT0, GL_DIFFUSE, mShadowCol);

	//影の描画
	if (mpRender)
	{
		(*mpRender)();
	}

	// 光源の明るさを元の明るさに設定 //
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);

}*/









