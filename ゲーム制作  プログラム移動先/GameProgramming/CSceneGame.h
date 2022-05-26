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
#include"CShadowMap.h"
#include"CTarget.h"
/*
ゲームのシーン
*/

class CSceneGame : public CScene {
private:


	//マップのインスタンス
	CModel mModelC5;
	int mSpawn;//敵が生成されるまでの時間
	int mSpawn2;
	int mTimeMinute;
	int mTimeSecond;
	int mTimeCount;
	CShadowMap mShadowMap;
	CMap mMap;//フィールド	
	CRock*mpRock;//周りの岩
	CTree* mpTree;//木
	//キャラクタのインスタンス
	CXPlayer mPlayer;//プレイヤー

	//敵のインスタンス
	//CXEnemy mEnemy;//敵１
	CEnemy2* mpEnemy2;//敵２
	CEnemy3* mpEnemy3;//敵３
	CEnemySummon* mpEnemySummon;//敵２の生成場所
	CEnemySummon* mpEnemySummon2;//敵３の生成場所
	CBoss* mpBoss;//ボス
public:
	CTexture mImageMouse;
	CTexture mImageMoveKey;
	CTexture mImageCkey;
	CTexture mImageWork;
	CTexture mImageDush;

    CSound mBgmStart;
	CSound mBgmBattle;
	CSound mBgmBossBattle;
	CSound mBgmGameClear;
	CSound mBgmGameOver;
	 static int mBgmCount;
	 bool mBgmStartStopper;
	 bool mBgmBattleStopper;
	 bool mBgmBossStopper;
	 bool mBgmOverStopper;
	 bool mBgmClearStopper;
	static bool mBgmCountCheck;
//BGMを流すか止めるか分けるフラグ
	bool mBgmCountCheck2;//BGMが連続で再生しないようにするフラグ
	static bool mVoiceSwitch;//BGM SEのオンオフ切り替え
	static int mEnemy2Count;//今生成されている敵2の数
	static int mEnemy3Count;//今生成されている敵3の数
	static int mEnemy2CountStopper;//生成できる敵2の限度
	static int mEnemy3CountStopper;//生成できる敵3の限度
	CSceneGame();
	~CSceneGame();
	//初期化処理のオーバーライド
	void Init();
	//更新処理のオーバーライド
	void Update();
	//描画処理
	void Render();
	void BgmStart();
	void BgmBattle();
	void BgmBoss();
	void BgmGameOver();
	void BgmGameClear();
	EScene GetNextScene();
};

#endif
