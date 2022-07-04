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
#include <time.h>
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
	//マップのインスタンス
	CModel mModelC5;
	CShadowMap mShadowMap;
	EScene mNextScene;
	clock_t mStartTime, mEndTime; //計測開始時刻、計測終了時刻
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
	std::vector<CEnemy2*> mEnemy2List;//Enemy2専用の部屋を作る
	std::vector<CEnemy3*> mEnemy3List;
	 int mSpawn;//敵が生成されるまでの時間
	 int mSpawn2;
	 int mTimeCount;
	 bool mSceneChange;
	 bool mBgmStartStopper;
	 bool mBgmBattleStopper;
	 bool mBgmBossStopper;
	 bool mBgmOverStopper;
	 bool mBgmClearStopper;
	 bool mBgmCountCheck2;//BGMが連続で再生しないようにするフラグ
	 bool mGameClear;
	 bool mGameOver;
	 bool mCountStart; //クリア時間計測開始用

	
	 int mTimeMinute; 
	 //staticでポインタを作る
	 static CSceneGame* mpSceneGameInstance;
	
public: 
	 float mClearTime;
	 bool mEnemy2Bgm;
	 bool mBgmCountCheck;
	 bool mVoiceSwitch;//BGM SEのオンオフ切り替え 
	 bool mBossSwitch;
     bool mBossGaugeSwitch;//BGMを流すか止めるか分けるフラグ
	 int mBgmCount;
	void SetBgmCount(int v) {
		if (v < 0) return;//< ここにブレークポイントを置けば誰が犯人なのかわかる
		this->mBgmCount = v;
	}
	int GetBgmCount() {
		return mBgmCount;
	}
	int mTimeSecond;
	void SetTimeSecond(int v) {
		if (v < 0) return;//< ここにブレークポイントを置けば誰が犯人なのかわかる
		this->mTimeSecond = v;
	}
	int GetTimeSecond() {
		return mTimeSecond;
	}
	int mEnemy2Count;//今生成されている敵2の数
	void SetEnemy2Count(int v) {
		if (v < 0) return;//< ここにブレークポイントを置けば誰が犯人なのかわかる
		this->mEnemy2Count = v;
	}
	int GetEnemy2Count() {
		return mEnemy2Count;
	}
	int mEnemy3Count;//今生成されている敵3の数
	void SetEnemy3Count(int v) {
		if (v < 0) return;//< ここにブレークポイントを置けば誰が犯人なのかわかる
		this->mEnemy3Count = v;
	}
	int GetEnemy3Count() {
		return mEnemy3Count;
	}
	int mEnemy2CountStopper;//生成できる敵2の限度
	void SetEnemy2CountStopper(int v) {
		if (v < 0) return;//< ここにブレークポイントを置けば誰が犯人なのかわかる
		this->mEnemy2CountStopper = v;
	}
	int  GetEnemy2CountStopper() {
		return mEnemy2CountStopper;
	}
	int mEnemy3CountStopper;//生成できる敵3の限度
	void SetEnemy3CountStopper(int v) {
		if (v < 0) return;//< ここにブレークポイントを置けば誰が犯人なのかわかる
		this->mEnemy3CountStopper = v;
	}
	int  GetEnemy3CountStopper() {
		return mEnemy3CountStopper;
	}
	//staticで処理を作る
	static CSceneGame* GetInstance();
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
