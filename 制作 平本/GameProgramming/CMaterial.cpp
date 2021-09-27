#include "CMaterial.h"
//memsetのインクルード
#include <string.h>
#include"glut.h"


//デフォルトコンストラクタ
CMaterial::CMaterial()
:mVertexNum(0)
{
	//名前を０で埋め
	//memset=指定したエリアに値をバイト数分設定
	//memset(エリア、値、バイト数）
	memset(mName, 0, sizeof(mName));
	memset(mDiffuse, 0, sizeof(mDiffuse));
	

}
void CMaterial::Enabled() {
	//拡張子の設定
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuse);
	//テクスチャあり
	if (mTexture.mId) {
		//テクスチャを使用可能にする
		glEnable(GL_TEXTURE_2D);
		//テクスチャをバインドする
		glBindTexture(GL_TEXTURE_2D, mTexture.mId);
		//アルファブレンドを有効にする
		glEnable(GL_BLEND);
		//ブレンド方法を指定
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}
	//glMaterialfv(面、種類、設定値）
	//面：GL_FRONT：表面のみ
	//種類：GL_DIFFUSE：拡張子
	//設定値：float型の配列[0]赤[1]緑[2]青[3]アルファ
}
void CMaterial::Disabled() {
	//テクスチャあり
	if (mTexture.mId)
	{
		//アルファフレンドを無効
		glDisable(GL_BLEND);
		//テクスチャのバインドを解く
		glBindTexture(GL_TEXTURE_2D, 0);
		//テクスチャを無効にする
		glDisable(GL_TEXTURE_2D);

	}
}

