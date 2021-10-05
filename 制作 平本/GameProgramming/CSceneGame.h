#ifndef CSCENEGAME_H
#define CSCENEGAME_H
#include "CScene.h"
#include"CVector.h"
#include"CModel.h"
#include"CCharacter.h"
#include "CPlayer.h"
#include"CEnemy.h"
#include"CCollisionManager.h"
#include"CItem.h"
#include"CBillBoard.h"
#include"CCamera.h"
#include"CColliderTriangle.h"
#include"CColliderMesh.h"
#include"CSound.h"
#include"CEnemySummon.h"

/*
ゲームのシーン
*/

class CSceneGame : public CScene {
public:
	
	CVector mEye; //カメラ視点の追加
	CModel mModel;//CModelクラスのインスタンス変数
	CModel mBackGround;//背景モデル
	CMatrix mMatrixRotate;//回転行列
	CMatrix mMatrix;//合成行列
	CMatrix mBackGroundMatrix;
	CPlayer mPlayer;
	CSound Bgm;
	CEnemySummon mEnemySummon;
	//モデルからコライダ生成
      CColliderMesh mColliderMesh;
	  int mMapCount;
	  static int mEnemyCount;
	//敵モデルのインスタンス（クラスと同じものは使えない）
	CModel mModelC5;
	CModel mModelc5;
	CModel mModelI;
	
	//三角コライダの作成
	CColliderTriangle mColliderTriangle;
	CColliderTriangle mColliderTriangle2;

	int mSpawn;

	//初期化処理のオーバーライド

	void Init();
	
	//更新処理のオーバーライド
	void Update();
};

#endif