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
#include"CSound.h"
#include"CBoss.h"
/*
ゲームのシーン
*/

class CSceneGame : public CScene {
private:

	CSound mJump;
	//CBillBoard2 mBillBoard;

	//マップのインスタンス
	CMap mMap;
	CModel mModelC5;
	CRock*mpRock;
	CTree* mpTree;
	int mSpawn;
	CEnemySummon* mpEnemySummon;
	CEnemy2* mpEnemy2;
	CBoss* mpBoss;

	//キャラクタのインスタンス
	CXPlayer mPlayer;
	//敵のインスタンス
	CXEnemy mEnemy;
	CEnemy3* mpEnemy3;
public:

	CSound FirstAttack;
	CSound SecondAttack;
	CSound ThirdAttack;
	CSound JumpAttack;
	CSound Damage;
	CSound Enemy3Fry;
	CSound BossVoice;
	CSound BossMove;
	CSound Enemy2Voice;
	 CSound Bgm;
	 
	static int mEnemy2Count;
	static int mEnemy2CountStopper;

	~CSceneGame();
	//初期化処理のオーバーライド
	void Init();
	//更新処理のオーバーライド
	void Update();
	//描画処理
	void Render();

};

#endif
