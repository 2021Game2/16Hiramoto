#ifndef CSCENEGAME_H
#define CSCENEGAME_H

#include "CScene.h"
#include "CXPlayer.h"
#include "CBillBoard2.h"
#include "CSound.h"
#include "CMap.h"
#include"CEnemySummon.h"
#include"CEnemy2.h"
#include"CBillBoard2.h"
#include"CRock.h"
#include"CFlag.h"
#include"CEnemy3.h"
#include"CSound.h"
#include"CBoss.h"
#include"CShadowMap.h"
#include"CTarget.h"
#include"CItem.h"
/*
ゲームのシーン
*/

class CSceneGame : public CScene {
private:
	enum ESceneSelect
	{
		EGAME,
		ETITLE,
	};
	ESceneSelect mSceneSelect;
	//マップのインスタンス
	CModel mModelC5;
	int mSpawn;//敵が生成されるまでの時間
	int mSpawn2;
	
	int mTimeCount;
	CShadowMap mShadowMap;
	
	//キャラクタのインスタンス
	CXPlayer* mpPlayer;//プレイヤー

	
	CRock*mpRock;//周りの岩
	CMap* mpMap;
	CFlag* mpFlag;//旗
	//敵のインスタンス
	CEnemy2* mpEnemy2;//敵２
	CEnemy3* mpEnemy3;//敵３
	CEnemySummon* mpEnemySummon;//敵２の生成場所
	CEnemySummon* mpEnemySummon2;//敵３の生成場所
	CTarget* mpTarget;
	CItem* mpItem;
	CBoss* mpBoss;//ボス
	bool mSceneChange;
	EScene mNextScene;
public:
	std::vector<CEnemy2*> mEnemy2List;//Enemy2専用の部屋を作る
	std::vector<CEnemy3*> mEnemy3List;
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
	
	 bool mBgmStartStopper;
	 bool mBgmBattleStopper;
	 bool mBgmBossStopper;
	 bool mBgmOverStopper;
	 bool mBgmClearStopper;
	 bool mBgmCountCheck2;//BGMが連続で再生しないようにするフラグ
	
	

	static bool mGameClear;
	static bool mGameOver; 
	static bool mSceneCount;
	static bool mBgmCountCheck;
	static bool mBossSwitch;
	static bool mBossGaugeSwitch;//BGMを流すか止めるか分けるフラグ
	static bool mVoiceSwitch;//BGM SEのオンオフ切り替え 
	static bool mEnemy2Bgm;
	static int mTimeMinute;
    static int mTimeSecond;
	static int mBgmCount;
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
