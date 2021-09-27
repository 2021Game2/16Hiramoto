#include"CModel.h"
//標準入出力のインクルード
#include<stdio.h> //システムのヘッダは＜＞で囲む
#include<string.h>
#include"CVector.h"
#include"CTriangle.h"

//C標準関数を使うためにインクルード
//モデルファイルの入力
//Load(モデルファイル名　マテリアルファイル名）

CModel::CModel() 
	: mpVertex(0),
	mpNormal(0), 
	mpTextureCoord(0)
{
}
//デストラクタ
CModel::~CModel() {
	//delete...削除
	//mpMaterials[i]を削除
	for (int i = 0; i < mpMaterials.size(); i++) {
		delete mpMaterials[i];
	}
	if (mpVertex) {
		//頂点座標配列削除
		delete[]mpVertex;
	}
	if (mpNormal) {
	  //法線配列削除
		delete[]mpNormal;
	}
	if (mpTextureCoord) {
      //テクスチャマッピング配列削除
		delete[]mpTextureCoord;
	}
}
void CModel::Load(char* obj, char* mtl) {//変数のアドレス
	//頂点データの保存（CVector型）
	std::vector<CVector>vertex;
	//法線データの保存
	std::vector<CVector>normal;
	//テクスチャマッピングの保存
	std::vector<CVector>uv;
	//ファイルポインタ変数の作成
	FILE* fp;
	//ファイルのオープン
	//fopen（ファイル名,モード）
	//オープンできないときはNULLを返す
	//マテリアルファイルをオープン
	fp = fopen(mtl, "r"); //r=入力、w=出力、a=追加


	//ファイルオープンエラー判定
	//	fpがNULLのとき（ファイルが開けないとき）はエラー
	if (fp == NULL) {
		//コンソールにエラー出力して戻る
		printf("%s file open error\n", obj);//s＝出力
		return;
	}


	//ファイル名からデータを入力
	//入力エリアを作成
	char buf[256];//バイト数
	//マテリアルインデックス
	int idx = 0;
	//ファイル名から１行入力
	//fgets（入力エリア、エリアサイズ、ファイルポインタ）
	//ファイルの最後になるとNULLを返す
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		//データを分割する
		char str[4][64] = { "","","","" };
		sscanf(buf, "%s %s %s %s", str[0], str[1], str[2], str[3]);
		if (strcmp(str[0], "newmtl") == 0)
		{//文字列の比較（０＝等しい）
			CMaterial* pm = new CMaterial();
			//マテリアル名のコピー
			strncpy(pm->mName, str[1], sizeof(pm->mName) - 1);
			//マテリアルの可変長列に追加
			mpMaterials.push_back(pm);
			//配列の長さを取得
			idx = mpMaterials.size() - 1;
		}
		//先頭がKdのとき、Diffuseを設定する
		else if (strcmp(str[0], "Kd") == 0) {
			mpMaterials[idx]->mDiffuse[0] = atof(str[1]);
			mpMaterials[idx]->mDiffuse[1] = atof(str[2]);
			mpMaterials[idx]->mDiffuse[2] = atof(str[3]);
			//mpMaterialのｍDiffuseにstrに入った文字列を代入
		}
		//先頭がｄのときa値を設定する
		else if (strcmp(str[0], "d") == 0) {
			mpMaterials[idx]->mDiffuse[3] = atof(str[1]);
		}
		else if (strcmp(str[0], "map_Kd") == 0) {
			mpMaterials[idx]->mTexture.Load(str[1]);
		}

		//sizeof=指定した変数のバイト数を取得
		//入力した値をコンソールに出力する
	}
	//ファイルのクローズ
	fclose(fp);//使用後のファイルはクローズ

 	//オブジェクトファイルをオープン
	fp = fopen(obj, "r");
	//	fpがNULLのとき（ファイルが開けないとき）はエラー
	if (fp == NULL) {
		//コンソールにエラー出力して戻る
		printf("%s file open error\n", obj);
		return;
	}


	//ファイル名からデータを入力
	//入力エリアを作成

		 //ファイル名から１行入力
		 //fgets（入力エリア、エリアサイズ、ファイルポインタ）
		 //ファイルの最後になるとNULLを返す
	//bufを使い回す
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		//データを分割する
		//""=文字列　''=文字
		char str[4][64] = { "","","","" };

		//文字列からデータを４つ変数に代入する
		//sscanf（文字列１、文字列２）オブジェクトファイルの文字列を分割
		sscanf(buf, "%s %s %s %s", str[0], str[1], str[2], str[3]);
		//%S＝文字列で入力
		//文字列１と文字列２が同じ時０，異なるときは０以外を返す
		//先頭がｖのとき、頂点をvertexに追加する
		//strcmp＝文字列の比較
		if (strcmp(str[0], "v") == 0) {//文字列の比較（０＝等しい）
			//可変長列vertexに追加
			//atof(文字列）　文字列float型の値を返す
			vertex.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));


		}
		//先頭がvnのとき
		//normalに法線を追加
		if (strcmp(str[0], "vn") == 0) {//文字列の比較（０＝等しい）
			//可変長列normalに追加
			//atof(文字列）　文字列float型の値を返す
			normal.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
		}
		//先頭がusemtlのとき
		else if (strcmp(str[0], "usemtl") == 0) {

			//可変長配列をあとから比較
			for (idx = mpMaterials.size() - 1; idx > 0; idx--) {
				//同じ名前のマテリアルがあればループ終了
				if (strcmp(mpMaterials[idx]->mName, str[1]) == 0) {
					break;//ループから抜ける
				}
			}
		}
		//先頭がｆのとき三角形を作成して追加する
		else if (strcmp(str[0], "f") == 0) { //f=Face
		//頂点と法線の番号作成
			int v[3], n[3];
			//テクスチャマッピングの有無を判定
			if (strstr(str[1], "//")) {


				//頂点と法線の番号取得
				sscanf(str[1], "%d//%d", &v[0], &n[0]); //「頂点//法線」となる
				sscanf(str[2], "%d//%d", &v[1], &n[1]);//sscanf=文字列の値を変数に代入
				sscanf(str[3], "%d//%d", &v[2], &n[2]);//%d＝int型に変換する演算子、１０進数で入力
				//三角形作成
				CTriangle t;
				t.SetVertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
				//法線ベクトルの追加
				t.SetNormal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
				//mMaterialIdxにidxを代入
				t.mMaterialIdx = idx;
				//可変長列ｍTrianglesに三角形を追加
				//push_back＝配列に要素を追加
				mTriangles.push_back(t);
			}

			else {
				//テクスチャマッピングあり
				int u[3];//テクスチャマッピングの番号

				//頂点と法線の番号取得とマッピングの番号取得
				sscanf(str[1], "%d/%d/%d", &v[0], &u[0], &n[0]); //「頂点//マッピング//法線」となる
				sscanf(str[2], "%d/%d/%d", &v[1], &u[1], &n[1]);//sscanf=文字列の値を変数に代入
				sscanf(str[3], "%d/%d/%d", &v[2], &u[2], &n[2]);//%d＝int型に変換する演算子、１０進数で入力
				//三角形作成
				CTriangle t;
				//配列の１番目は０からなので１減算して０からカウントする
				t.SetVertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
				//法線ベクトルの追加
				t.SetNormal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
				//テクスチャマッピングの設定
				t.mUv[0] = uv[u[0] - 1];
				t.mUv[1] = uv[u[1] - 1];
				t.mUv[2] = uv[u[2] - 1];
				//マテリアル番号の設定
				t.mMaterialIdx = idx;
				//可変長配列ｍTrianglesbに三角形を追加
				mTriangles.push_back(t);
			}
		}
		else if (strcmp(str[0], "vt") == 0) {
			//可変長配列うｖに追加
			//atof(文字列)  文字列からfloat型の値を返す
			uv.push_back(CVector(atof(str[1]), atof(str[2]), 0.0));
		}

	}
	//ファイルのクローズ
	fclose(fp);//使用後のファイルはクローズ
	   //fgets（入力エリア、エリアサイズ、ファイルポインタ）
	   //ファイルの最後になるとNULLを返す

 //頂点座標配列の作成	
	//すべての座標の値をマテリアルの順番に保存する
	mpVertex = new float[mTriangles.size() * 9];
	//全ての法線の値をマテリアルの順番に保存する
	mpNormal = new float[mTriangles.size() * 9];
	//全てのテクスチャマッピングの値をマテリアルの順番に保存する
	mpTextureCoord = new float[mTriangles.size() * 6];

	int v = 0, t = 0;
	//マテリアルごとに頂点配列に設定する
	for (int i = 0; i < mpMaterials.size(); i++) {
		//全ての三角形を比較	
		for (int j = 0; j < mTriangles.size(); j++) {
			//マテリアル番号が一致するとき
			if (i == mTriangles[j].mMaterialIdx) {
				//頂点配列に設定する
			//頂点座標
				//float型の値を代入
				//3種類*X、Y、Z
				mpVertex[v++] = mTriangles[j].mV[0].mX;
				mpVertex[v++] = mTriangles[j].mV[0].mY;
				mpVertex[v++] = mTriangles[j].mV[0].mZ;
				mpVertex[v++] = mTriangles[j].mV[1].mX;
				mpVertex[v++] = mTriangles[j].mV[1].mY;
				mpVertex[v++] = mTriangles[j].mV[1].mZ;
				mpVertex[v++] = mTriangles[j].mV[2].mX;
				mpVertex[v++] = mTriangles[j].mV[2].mY;
				mpVertex[v++] = mTriangles[j].mV[2].mZ;
				v -= 9;
				//法線
				//3種類*X、Y、Z
				mpNormal[v++] = mTriangles[j].mN[0].mX;
				mpNormal[v++] = mTriangles[j].mN[0].mY;
			    mpNormal[v++] = mTriangles[j].mN[0].mZ;
				mpNormal[v++] = mTriangles[j].mN[1].mX;
				mpNormal[v++] = mTriangles[j].mN[1].mY;
				mpNormal[v++] = mTriangles[j].mN[1].mZ;
				mpNormal[v++] = mTriangles[j].mN[2].mX;
				mpNormal[v++] = mTriangles[j].mN[2].mY;
				mpNormal[v++] = mTriangles[j].mN[2].mZ;
				//テクスチャマッピング
				//3種類*X、Y
				mpTextureCoord[t++] = mTriangles[j].mUv[0].mX;
				mpTextureCoord[t++] = mTriangles[j].mUv[0].mY;
				mpTextureCoord[t++] = mTriangles[j].mUv[1].mX;
				mpTextureCoord[t++] = mTriangles[j].mUv[1].mY;
				mpTextureCoord[t++] = mTriangles[j].mUv[2].mX;
				mpTextureCoord[t++] = mTriangles[j].mUv[2].mY;
		
			}
	    }
		//頂点数を設定
		//X軸、Y軸、Z軸（３個）で１個の頂点
		mpMaterials[i]->mVertexNum = v/3;
     }
}	

void CModel::Render(const CMatrix& m) {
	//行列の退避
	glPushMatrix();
	//合成行列を掛ける
	glMultMatrixf(&m.mM[0][0]);


	//頂点配列の座標を有効
	glEnableClientState(GL_VERTEX_ARRAY);
	//法線の配列を有効にする
	glEnableClientState(GL_NORMAL_ARRAY);
	//テクスチャマッピングの配列を有効
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	//頂点座標の配列を指定
	glVertexPointer(3, GL_FLOAT, 0, mpVertex);
	//法線の配列を指定
	glNormalPointer(GL_FLOAT, 0, mpNormal);
	//テクスチャコードの配列を指定
	glTexCoordPointer(2, GL_FLOAT, 0, mpTextureCoord);

	int first = 0;//描画位置
	//マテリアルごとに描画
	for (int i = 0; i < mpMaterials.size(); i++) {
		//マテリアルを適用する
		mpMaterials[i]->Enabled();
		//描画位置からのデータで三角形を描画
		glDrawArrays(GL_TRIANGLES, first, mpMaterials[i]->mVertexNum - first);
		//マテリアルを無効にする
		mpMaterials[i]->Disabled();
		//描画位置を移動
		first = mpMaterials[i]->mVertexNum;

	}

	glPopMatrix();
	//Disable...無効　Cliant...クライアント
	//頂点の配列を無効にする
	glDisableClientState(GL_VERTEX_ARRAY);
	//法線の配列を無効にする
	glDisableClientState(GL_NORMAL_ARRAY);
	//テクスチャマッピングの配列を無効にする
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}