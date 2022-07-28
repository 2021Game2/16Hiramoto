#ifndef CSCENEGAME_H
#define CSCENEGAME_H

#include "CScene.h"
#include "CXPlayer.h"
#include "CBillBoard2.h"
#include "CSound.h"
#include "CMap.h"
#include"CBossStage.h"
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
	CBossStage* mpBossStage;
	CMap* mpMap;//フィールド
	CFlag* mpFlag;//旗
	//敵のインスタンス
	CEnemy2* mpEnemy2;//敵２
	CEnemy3* mpEnemy3;//敵３
	CEnemySummon* mpEnemySummon;//敵２の生成場所
	CEnemySummon* mpEnemySummon2;//敵３の生成場所
	CTarget* mpTarget;//目的地の方向を示す矢印
	CItem* mpItem;//ハンマー
	CBoss* mpBoss;//ボス
	//マップのインスタンス
	
	CShadowMap mShadowMap;//シャドウマップ（影）
	EScene mNextScene;//次のシーンの設定
	clock_t mStartTime, mEndTime; //計測開始時刻、計測終了時刻
	CTexture mImageMouse;//マウスのテクスチャ
	CTexture mImageMoveKey;//移動キー（ASWD）のテクスチャ
	CTexture mImageCkey;//Cキーのテクスチャ
	CTexture mImageWork;//歩いている人のテクスチャ
	CTexture mImageDush;//走っている人のテクスチャ
    CSound mBgmStart;//最初のBGM
	CSound mBgmBattle;//敵と戦っているときのBGM
	CSound mBgmBossBattle;//ボスと戦っているときのBGM
	CSound mBgmGameClear;//ゲームクリア時のBGM
	CSound mBgmGameOver;//ゲームオーバー時のBGM
	std::vector<CEnemy2*> mEnemy2List;//Enemy2専用の部屋を作る
	std::vector<CEnemy3*> mEnemy3List;//Enemy3専用の部屋を作る
	 int mSpawn;//敵2が生成されるまでの時間
	 int mSpawn2;//敵3が生成されるまでの時間
	
	 bool mSceneChange;//シーンの切り替え

	 bool mBgmStartStopper;//BGMを止める
	 bool mBgmBattleStopper;//BGMを止める
	 bool mBgmBossStopper;//BGMを止める
	 bool mBgmOverStopper;//BGMを止める
	 bool mBgmClearStopper;//BGMを止める
	 bool mBgmCountCheck2;//BGMが連続で再生しないようにするフラグ
	 bool mGameClear;
	 bool mGameOver;
	 bool mCountStart; //クリア時間計測開始用
	
	 int mTimeMinute; 
	 //staticでポインタを作る
	 static CSceneGame* mpSceneGameInstance;
	
public: 
	CVector mBossStageCenter;
	CVector mBossStageEnd;
	float mBossStageLengthX;//ボスの行動範囲のX軸の長さ
	float mBossStageLengthZ;//ボスの行動範囲のZ軸の長さ
	float mBossStageLengthSum;//ボスの行動範囲の半径
	float mBossStageCircle;//ボスの行動可能範囲
	 float mClearTime;
	 bool mEnemy2Bgm;
	 bool mBgmCountCheck;
	 bool mVoiceSwitch;//BGM SEのオンオフ切り替え 
	 bool mBossSwitch;
     //ボス関係のゲージの表示・非表示
	 //ボスの生成、削除の切り替え時に利用
     bool mBossGaugeSwitch;
	 bool mBossBattleStage;
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
	void SetEnemy3Count(int Enemy3Count) {
		if (Enemy3Count < 0) return;//< ここにブレークポイントを置けば誰が犯人なのかわかる
		this->mEnemy3Count = Enemy3Count;
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
