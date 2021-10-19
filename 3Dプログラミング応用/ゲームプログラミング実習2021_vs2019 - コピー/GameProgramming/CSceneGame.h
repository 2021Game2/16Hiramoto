#ifndef CSCENEGAME_H
#define CSCENEGAME_H
#include "CScene.h"
#include "CXPlayer.h"
#include "CXEnemy.h"
#include"CMap.h"
#include"CEnemySummon.h"
/*
ゲームのシーン
*/
class CSceneGame : public CScene {
public:
	CMap mMap;
	//キャラクタのインスタンス
	CXPlayer mPlayer;
	//敵のインスタンス
	CXEnemy mEnemy;
	int mSpawn;
	CEnemySummon* mpEnemySummon;
	static int mEnemyCount;
	~CSceneGame();
	//初期化処理のオーバーライド
	void Init();
	//更新処理のオーバーライド
	void Update();

};

#endif
