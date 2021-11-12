#ifndef CSCENEGAME_H
#define CSCENEGAME_H
#include "CScene.h"
#include "CXPlayer.h"
#include "CXEnemy.h"
#include "CBillBoard2.h"
#include "CSound.h"
#include "CMap.h"
#include"CEnemySummon.h"
#include"CEnemy2.h"
#include"CBillBoard2.h"
#include"CRock.h"
#include"CTree.h"
#include"CEnemy3.h"
/*
ゲームのシーン
*/
class CSceneGame : public CScene {
	CSound mJump;
	//CBillBoard2 mBillBoard;

public:
	//マップのインスタンス
	CMap mMap;
	CModel mModelC5;
	CRock*mpRock;
	CTree* mpTree;
	int mSpawn;
	CEnemySummon* mpEnemySummon;
	static int mEnemyCount;

	//キャラクタのインスタンス
	CXPlayer mPlayer;
	//敵のインスタンス
	CXEnemy mEnemy;
	CEnemy3* mpEnemy3;
	~CSceneGame();
	//初期化処理のオーバーライド
	void Init();
	//更新処理のオーバーライド
	void Update();
	//描画処理
	void Render();

};

#endif
