
#ifndef CSHADOWMAP_H
#define CSHADOWMAP_H
#include "glew.h"
#include "glut.h"
class CShadowMap {
private:
	GLfloat mLightPos[3]; //光源の位置
	GLfloat mShadowCol[4]; //影の色
	int mTextureWidth;// デプステクスチャの幅
	float mTextureHeight;// デプステクスチャの高さ
	GLuint mDepthTextureID; //デプステクスチャID
	GLuint mFb; //フレームバッファ識別子
	//void (*mpRender)(); //Render関数のポインタ
public:
	CShadowMap();
	~CShadowMap();
	//初期化処理
	void Init();

	//wight// デプステクスチャの幅
	//height// デプステクスチャの高さ
    //funcRender;// 描画関数のポインタ
	//shadowCol;// 影の色
	//lightPos;// 光源の位置
	/*
	void Init(int width, int height, void (*funcRender)(),
		float shadouCol[], float lightPos[]);
	*/
	void Init(int width, int height, void (*funcRender)(), void (*funcEffectRender)(),
		float shadouCol[], float lightPos[]);
	void Render();
	void (*mpRender)(); //Render関数のポインタ
	void (*mpEffectRender)();//EffectRender関数ポインタ

};
#endif
