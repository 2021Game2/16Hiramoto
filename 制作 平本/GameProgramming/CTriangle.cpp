#include "CTriangle.h"
#include "glut.h"
//頂点座標設定
//SetVertex(頂点１，頂点２、頂点３）

void CTriangle::SetVertex(const CVector& v0, const CVector& v1, const CVector& v2)
{
	mV[0] = v0;//頂点
	mV[1] = v1;
	mV[2] = v2;
}

//法線設定
	//SetNormal(法線ベクトル）
	void CTriangle::SetNormal(const CVector & n) {
		mN[0]=mN[1]=mN[2] = n; //法線は全てｎと同じ
	}
	void CTriangle::SetNormal(const CVector& v0, CVector const& v1, CVector const& v2) {
	mN[0] = v0;//法線ベクトル
	mN[1] = v1;
	mN[2] = v2;
	}
	//描画
	
	void CTriangle::Render(const CMatrix& m) {
		CVector mV[3], mN[3];
		//CVector*CMatrix
		mV[0] = CTriangle::mV[0] * m;
		mV[1] = CTriangle::mV[1] * m;
		mV[2] = CTriangle::mV[2] * m;
		//法線は移動値を削除
		CMatrix mat = m;
		mat.mM[3][0] = mat.mM[3][1] = mat.mM[3][2] = mat.mM[3][2] = 0.0f;
		mN[0] = CTriangle::mN[0] * mat;
		mN[1] = CTriangle::mN[1] * mat;
		mN[2] = CTriangle::mN[2] * mat;
		
	}
	void CTriangle::Render() {
//三角形の描画
		//mN=法線、mV=ベクトル、mUv=テクスチャマッピング
		//CMatrixとかけたものを設定
		glBegin(GL_TRIANGLES);
		glNormal3f(mN[0].mX, mN[0].mY, mN[0].mZ);
		glTexCoord2f(mUv[0].mX, mUv[0].mY);
		glVertex3f(mV[0].mX, mV[0].mY, mV[0].mZ);

		glNormal3f(mN[1].mX, mN[1].mY, mN[1].mZ);
		glTexCoord2f(mUv[1].mX, mUv[1].mY);
		glVertex3f(mV[1].mX, mV[1].mY, mV[1].mZ);

		glNormal3f(mN[2].mX, mN[2].mY, mN[2].mZ);
		glTexCoord2f(mUv[2].mX, mUv[2].mY);
		glVertex3f(mV[2].mX, mV[2].mY, mV[2].mZ);
		glEnd();

	}