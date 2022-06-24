#include "CSceneGame.h"
#include "CCamera.h"
#include "CUtil.h"
#include "CRes.h"
#include "Ckey.h"
#include "CMaterial.h"
#include"CScene.h"
#include "CCollisionManager.h"
#include"CEnemy2.h"
#include"CItem.h"
#include"CRock.h"
#include"CSound.h"
#include"CTarget.h"
#include"CFlag.h"
#include"CFade.h"
#define HP 10

#define ENEMY2COUNT 1 //一度に出せる敵２の数
#define ENEMY2MINCOUNT 4 //敵２を再生成させるときの敵２の数の下限
#define ENEMY3COUNT 1//一度に出せる敵３の数
#define ENEMY3MINCOUNT 4 //敵３を再生成させるときの敵３の数の下限

#define TEX_BUTTON "Resource\\png,tga\\MoveKey.png"
#define TEX_BUTTON2 "Resource\\png,tga\\CKey.png"
#define TEX_BUTTON3 "Resource\\png,tga\\Mouse.png"
#define TEX_DUSH "Resource\\png,tga\\Dush.png"
#define TEX_WORK "Resource\\png,tga\\Work.png"
#define BGMSTART "Resource\\BGM\\BGMSTART.wav" //BGM
#define BGMBATTLE "Resource\\BGM\\BGMBATTLE.wav" //バトル中のBGM
#define BGMBOSSBATTLE "Resource\\BGM\\BGMBOSSBATTLE.wav" // ボスと戦っているときのBGM
#define BGMGAMECLEAR "Resource\\BGM\\BGMGAMECLEAR.wav" //ゲームクリア時のBGM
#define BGMGAMEOVER "Resource\\BGM\\BGMGAMEOVER.wav" //ゲームオーバー時のBGM
#define ATTACK1 "Resource\\SE\\PlayerAttack1.wav" //プレイヤーの攻撃SE１
#define ATTACK2 "Resource\\SE\\PlayerAttack2.wav" //プレイヤーの攻撃SE２
#define ATTACK3 "Resource\\SE\\PlayerAttack3.wav" //プレイヤーの攻撃SE３
#define ATTACKSP "Resource\\SE\\PlayerAttacksp.wav" //プレイヤーのジャンプ攻撃SE
#define DAMAGE "Resource\\SE\\Damage.wav"  //プレイヤーダメージ時のSE
#define FRY "Resource\\SE\\Beefly.wav"  //敵３の移動中SE
#define BOSSVOICE "Resource\\SE\\BossVoice.wav"//ボスの鳴き声
#define BOSSWORK "Resource\\SE\\BossWork.wav" //ボスの足音
#define ENEMYVOICE "Resource\\SE\\ScopionVoice.wav" //敵２の鳴き声
#define FONT "Resource\\png,tga\\FontG.png"  //ゲームに使われている文字のデータ
#define SCOPION "Resource\\3DModel\\scorpid\\scorpid-monster-X-animated.X" //敵２のXファイル
#define BOSS "Resource\\3DModel\\Boss\\monster-animated-character-X.X" // ボスのXファイル

#define TEXWIDTH  8192  //テクスチャ幅
#define TEXHEIGHT  6144  //テクスチャ高さ

//CMatrix Matrix;
//int CSceneGame::mEnemy2Count = 0;
//int CSceneGame::mEnemy2CountStopper = ENEMY2COUNT;
//int CSceneGame::mEnemy3Count = 0;
//int CSceneGame::mEnemy3CountStopper = ENEMY3COUNT;
//int CSceneGame::mBgmCount = 1;//BGMの切り替え番号
//int CSceneGame::mTimeMinute = 0;
//int CSceneGame::mTimeSecond = 0;
//bool CSceneGame::mBgmCountCheck = true;//BGMを流すか止めるか分けるフラグ
//bool CSceneGame::mVoiceSwitch =true;
//bool CSceneGame::mEnemy2Bgm = true;
//bool CSceneGame::mBossSwitch = false;
//bool CSceneGame::mBossGaugeSwitch = false;
//bool CSceneGame::mSceneCount = false;
//bool CSceneGame::mGameClear = false;
//bool CSceneGame::mGameOver = false;
CSound PlayerFirstAttack;
CSound PlayerSecondAttack;
CSound PlayerThirdAttack;
CSound PlayerJumpAttack;
CSound PlayerDamage;
CSound Enemy3Fry;
CSound BossVoice;
CSound BossMove;
CSound Enemy2Voice;

CSceneGame* CSceneGame::mpSceneGameInstance;
//プレイヤーのポインタを返すことで、座標などが参照できるようになる
CSceneGame* CSceneGame::GetInstance()
{
	return mpSceneGameInstance;
}
CSceneGame::CSceneGame() 
	:mTimeCount(0)
	,mSpawn(0)
    ,mSpawn2(0)
	, mTimeMinute (0)
	, mTimeSecond (0)
	, mBgmCount  (1)
	, mEnemy3Count (0)
	, mEnemy2Count (0)
	, mEnemy2Bgm(true)
	,mBgmCountCheck2(true)
	,mBgmStartStopper(true)
	,mBgmBattleStopper(true)
	,mBgmBossStopper(true)
    ,mBgmOverStopper(true)
    ,mBgmClearStopper(true)
	, mVoiceSwitch(false)//false：音声なし true：音声あり
	, mBossGaugeSwitch(false)
	, mGameClear (false)
	, mGameOver (false)
	, mBgmCountCheck  (true)
	,mpBoss(NULL)
	,mpMap(NULL)
	,mpItem(NULL)
	,mpTarget(NULL)
	,mpFlag(NULL)
	,mpRock(NULL)
	,mpEnemySummon(NULL)
	,mpEnemySummon2(NULL)
	,mpEnemy2(NULL)
	,mpEnemy3(NULL)
	, mEnemy3CountStopper (ENEMY3COUNT)
	, mEnemy2CountStopper (ENEMY2COUNT)
	, mBossSwitch(false)
{

}
CSceneGame::~CSceneGame() {
	/*
	if (mpBoss) delete mpBoss;
	if (mpEnemySummon) delete mpEnemySummon;
	if (mpEnemySummon2) delete mpEnemySummon2;
	if (mpMap) delete mpMap;
	if (mpFlag) delete mpFlag;
	if (mpItem) delete mpItem;
	if (mpRock) delete mpRock;
	if (mpTarget) delete mpTarget;
	for (size_t i; i < mEnemy2List.size(); i++) {
		delete mEnemy2List[i];
	}
	for (size_t i; i < mEnemy3List.size(); i++) {
		delete mEnemy3List[i];
	}*/
	CTaskManager::Destroy();
}
void ShadowRender() {
	//影の影響を受ける用になる
	CTaskManager::Get()->Render();
}
void ShadowEffectRender() {
	CTaskManager::Get()->EffectRender();
}
void CSceneGame::Init()

{
	mpSceneGameInstance = this;
	Camera = new CCamera();
	mScene = CScene::EGAME;
	CFade::SetFade(CFade::FADE_IN);

	mSceneChange = false;
	//サウンド(wav)ファイルの読み込み
	//BGM,SEの読み込み
	PlayerFirstAttack.Load(ATTACK1);
	PlayerSecondAttack.Load(ATTACK2);
	PlayerThirdAttack.Load(ATTACK3);
	PlayerJumpAttack.Load(ATTACKSP);
	PlayerDamage.Load(DAMAGE);
	Enemy3Fry.Load(FRY);
	BossVoice.Load(BOSSVOICE);
	BossMove.Load(BOSSWORK);
	Enemy2Voice.Load(ENEMYVOICE);
	mBgmStart.Load(BGMSTART);
	mBgmBattle.Load(BGMBATTLE);
	mBgmBossBattle.Load(BGMBOSSBATTLE);
	mBgmGameClear.Load(BGMGAMECLEAR);
	mBgmGameOver.Load(BGMGAMEOVER);
	mImageMouse.Load(TEX_BUTTON3);
	mImageCkey.Load(TEX_BUTTON2);
	mImageWork.Load(TEX_WORK);
	mImageDush.Load(TEX_DUSH);
	mImageMoveKey.Load(TEX_BUTTON);
	if (mVoiceSwitch == true) {
	 mBgmStart.Repeat();
	}
	//テキストフォントの読み込みと設定
	mFont.LoadTexture(FONT, 1, 4096 / 64);
	CRes::sModelX.Load(MODEL_FILE);
	//キャラクターにモデルを設定
	mpPlayer = new CXPlayer();
	mpPlayer->Init(&CRes::sModelX);
	mpPlayer->mPosition = CVector(-45.0f, 1.0f, -120.0f);
	//カメラ初期化
	Camera->Init();
	CRes::sScorp.Load(SCOPION);
	CRes::sScorp.SeparateAnimationSet(0, 0, 72, "walk");
	CRes::sScorp.SeparateAnimationSet(0, 72, 120, "strafe left");
	CRes::sScorp.SeparateAnimationSet(0, 120, 168, "strafe right");
	CRes::sScorp.SeparateAnimationSet(0, 168, 220, "attack");
	CRes::sScorp.SeparateAnimationSet(0, 292, 350, "attack2");
	CRes::sScorp.SeparateAnimationSet(0, 350, 440, "attack3");
	CRes::sScorp.SeparateAnimationSet(0, 220, 292, "death");
	CRes::sScorp.SeparateAnimationSet(0, 660, 760, "idle");
	CRes::sScorp.SeparateAnimationSet(0, 761, 849, "idle2");
	CRes::sScorp.SeparateAnimationSet(0, 850, 880, "gethit");
	CRes::sScorp.SeparateAnimationSet(0, 880, 950, "gethit2");
	CRes::sScorp.SeparateAnimationSet(0, 951, 1015, "jump");


	//アニメーションを読み込む
	CRes::sBoss.Load(BOSS);
	CRes::sBoss.SeparateAnimationSet(0, 0, 30, "walk");
	CRes::sBoss.SeparateAnimationSet(0, 0, 120, "walk");
	CRes::sBoss.SeparateAnimationSet(0, 150, 190, "run");
	CRes::sBoss.SeparateAnimationSet(0, 150, 210, "run");
	CRes::sBoss.SeparateAnimationSet(0, 250, 333, "attack - 01");
	CRes::sBoss.SeparateAnimationSet(0, 320, 400, "attack - 02");
	CRes::sBoss.SeparateAnimationSet(0, 390, 418, "death - 01");
	CRes::sBoss.SeparateAnimationSet(0, 478, 500, "growl");
	CRes::sBoss.SeparateAnimationSet(0, 500, 550, "death - 02");
	CRes::sBoss.SeparateAnimationSet(0, 565, 650, "death - 03");
	
	mpItem=new CItem(CVector(-20.0f, 2.0f, -10.0f),
		CVector(), CVector(1.5f, 1.5f, 1.5f));
	mpTarget=new CTarget(mpPlayer->mPosition,
		CVector(), CVector(0.5f, 0.5f, 0.5f));
	mpMap = new CMap(CVector(0.0f, -3.325f, 0.0f), CVector(), CVector(1.0f, 1.0f, 1.0f));
	mpEnemySummon = new CEnemySummon(CVector(-20.0f, -2.0f, -70.0f),
		CVector(), CVector(0.5f, 0.5f, 0.5f));
	mpEnemySummon2 = new CEnemySummon(CVector(6.0f, 14.0f, 20.0f),
		CVector(), CVector(0.5f, 0.5f, 0.5f));

	mpRock = new CRock(CVector(0.0f, 0.0f, -100.0f),
		CVector(0.0f, 180.0f, 0.0f), CVector(0.5f, 0.5f, 0.5f));
	
	mpFlag = new CFlag(CVector(56.0f, 11.0f, 26.0f),
		CVector(), CVector(10.5f, 10.5f, 10.5f));
	
	float shadowColor[] = { 0.4f, 0.4f, 0.4f, 0.2f };  //影の色
	float lightPos[] = { 50.0f, 160.0f, 50.0f };  //光源の位置

	mpPlayer->mHp = HP;
	mpPlayer->mGaugeEnabled = true;
	mBgmCount = 1;
	if (mpEnemy2) mpEnemy2->mEnabled = true;

	
	if(mpEnemy3)mpEnemy3->mEnabled = true;
	mShadowMap.Init(TEXWIDTH, TEXHEIGHT, ShadowRender,ShadowEffectRender, shadowColor, lightPos);//影の初期化
	
	CEffect2::TexPreLoad();
}

void CSceneGame::BgmBattle() {

	if (mVoiceSwitch == true) {
		if (mBgmCountCheck == true) {
			if (mBgmBattleStopper == true) {
				mBgmBattle.Repeat();
				mBgmBattleStopper = false;
			}
		
		}

	}
}
void CSceneGame::BgmBoss() {

	if (mVoiceSwitch == true) {
		if (mBgmCountCheck == true) {
			if (mBgmBossStopper==true) {
				mBgmBossBattle.Repeat();
				mBgmBossStopper = false;
			}
				
		}
	}
}
void CSceneGame::BgmGameOver() {
	if (mVoiceSwitch == true) {
		if (mBgmCountCheck == true) {
			if (mBgmOverStopper == true) {
				mBgmGameOver.Repeat();
				mBgmOverStopper = false;
			}
		}
		
	}
}
void CSceneGame::BgmGameClear() {

	if (mVoiceSwitch == true) {
		if (mBgmCountCheck == true) {
			if (mBgmClearStopper == true) {
				mBgmGameClear.Repeat();
				mBgmClearStopper = false;

			}
		}
	}
}
void CSceneGame::Update() {
	
	if (mBossSwitch == true) {
		//新しく作る
		mpBoss = new CBoss(CVector(0.0f, 10.0f, 0.0f),
			CVector(0.0f, 0.0f, 0.0f), CVector(0.5f, 0.5f, 0.5f));
		//読み込ませる
		mpBoss->Init(&CRes::sBoss);
		//ボスの配置
		mpBoss->mPosition = CVector(3.0f, 10.0f, 100.0f);
		mBossSwitch = false;	
	}
	switch (mBgmCount) {
	case 2:
		BgmBattle();
		break;
	case 3:
		BgmBoss();
		break;
	case 4:
		BgmGameClear();
		break;
	case 5:
		BgmGameOver();
		break;
	}
	if (mpBoss) {
		if (mpBoss->mHp > 0) {
			mTimeCount++;
			if (mTimeCount % 60 == 0) {
				mTimeSecond++;
			}
			if (mTimeSecond == 60) {
				mTimeMinute++;
				mTimeSecond = 0;
			}
		}
	}
	//敵の生成間隔
	if (mSpawn >= 0) {
		mSpawn--;
	}
	//敵の生成間隔
	if (mSpawn2 >= 0) {
		mSpawn2--;
	}
	if (mpEnemySummon->mHp > 0) {


		//mEnemy2CountStopperに設定した数だけ敵を生成
		if (mEnemy2Count < mEnemy2CountStopper) {
			//２秒ごとに生成
			if (mSpawn <= 0) {
				mpEnemy2 = new CEnemy2(mpEnemySummon->mPosition,
					CVector(), CVector(0.75f, 0.75f, 0.75f));
				//push_back  元は同じクラスからだが別々のポインタ生成
				mEnemy2List.push_back(mpEnemy2);//管理リストに入れる
				mpEnemy2->Init(&CRes::sScorp);
				mEnemy2Count++;
				mSpawn = 60;
			}
		}
	}

	//敵が一定の数減るまで再生成しない
	 if( mEnemy2CountStopper<= ENEMY2MINCOUNT) {
		mEnemy2CountStopper = ENEMY2COUNT;
	 }

	if (mpEnemySummon2->mHp > 0) {
		//mEnemy3CountStopperに設定した数だけ敵を生成
		if (mEnemy3Count < mEnemy3CountStopper) {
			//２秒ごとに生成
			if (mSpawn2 <= 0) {
				mpEnemy3 = new CEnemy3(mpEnemySummon2->mPosition, CVector(0.0f, 0.0f, 0.0f),
					CVector(1000.5f, 1000.5f, 1000.5f));

				//push_back  元は同じクラスからだが別々のポインタ生成
				mEnemy3List.push_back(mpEnemy3);//管理リストに入れる
				mEnemy3Count++;
				mSpawn2 = 120;
			}
		}
	}

	//敵が一定の数減るまで再生成しない
	 if (mEnemy3CountStopper <= ENEMY3MINCOUNT) {
		mEnemy3CountStopper = ENEMY3COUNT;
	}
	//エスケープキーで終了
	if (CKey::Push(VK_ESCAPE)) {
		exit(0);
	}
		if (mBgmCountCheck ==false) {
			mBgmStart.Stop();
			mBgmBattle.Stop();
			mBgmBossBattle.Stop();
			mBgmGameClear.Stop();
			mBgmGameOver.Stop();
			mBgmCountCheck = true;
		}
		if (mSceneChange) {
			if (CFade::IsFadeEnd()) {
				mScene = mNextScene;
			}
		}
	//更新
	CTaskManager::Get()->Update();
	//衝突処理(総当り）
	//CCollisionManager::Get()->Collision();
	CTaskManager::Get()->TaskCollision();
	return;
}

void CSceneGame::Render() {
	//タスクの描画
	//CTaskManager::Get()->Render();
	Camera->CameraRender();//カメラ設定
	mShadowMap.Render();//影設定
	//コライダの描画
	//ここをコメントにするとすべてのコライダ非表示
	//CCollisionManager::Get()->Render();
	//2D描画開始
	CUtil::Start2D(0, 800, 0, 600);
	char buf[64];
	if (mpBoss) {


		if (mpBoss->mHp > 0 && mpPlayer->mHp > 0) {
			//時間（分）
			if (mTimeMinute < 10) {
				sprintf(buf, "0%d:", mTimeMinute);
				mFont.DrawString(buf, 700, 500, 8, 16);
			}
			else {
				sprintf(buf, "%d:", mTimeMinute);
				mFont.DrawString(buf, 700, 500, 8, 16);
			}

			//時間（秒）
			if (mTimeSecond < 10) {
				sprintf(buf, "0%d", mTimeSecond);
				mFont.DrawString(buf, 740, 500, 8, 16);
			}
			else {
				sprintf(buf, "%d", mTimeSecond);
				mFont.DrawString(buf, 740, 500, 8, 16);
			}

		}

		else if (mpBoss->mHp <= 0) {
			mGameClear = true;
			mpPlayer->mGaugeEnabled = false;
			mBossGaugeSwitch = false;
			sprintf(buf, "GAMECLEAR");
			mFont.DrawString(buf, 300, 300, 16, 32);

			sprintf(buf, "PUSH CLICK");
			mFont.DrawString(buf, 300, 200, 16, 32);
			if (CKey::Once(VK_LBUTTON)) {
				mBgmCountCheck = false;
				mpItem->mItemCount = 0;
				mEnemy2Count = 0;
				mEnemy3Count = 0;
				mNextScene = CScene::ETITLE;
				mSceneChange = true;
				CFade::SetFade(CFade::FADE_OUT);
			}
		}
		if (mBossGaugeSwitch == true && mpBoss->mHp > 0) {
			sprintf(buf, "BOSS");
			mFont.DrawString(buf, 300, 570, 32, 16);
		}
	}
	if (mpPlayer->mHp <= 0) {
		mGameOver = true;
		mpPlayer->mGaugeEnabled = false;
		mBossGaugeSwitch = false;
		sprintf(buf, "GAMEOVER");
		mFont.DrawString(buf, 300, 300, 16, 32);

		sprintf(buf, "PUSH CLICK");
		mFont.DrawString(buf, 300, 200, 16, 32);
		if (CKey::Once(VK_LBUTTON)) {
			mBgmCountCheck = false;
			mpItem->mItemCount = 0;
			mEnemy2Count = 0;
			mEnemy3Count = 0;
			mNextScene = CScene::ETITLE;
			mSceneChange = true;
			CFade::SetFade(CFade::FADE_OUT);

		}
	}
	else {

		mImageMouse.Draw(570, 770, 0, 170, 0, 500, 500, 0);
		mImageMoveKey.Draw(0, 100, 50, 200, 0, 500, 500, 0);
		mImageCkey.Draw(120, 200, 60, 210, 0, 500, 500, 0);
		mImageWork.Draw(0, 100, 0, 100, 0, 250, 250, 0);
		mImageDush.Draw(120, 190, 20, 100, 0, 200, 210, 0);
	}
	//2Dの描画終了
	CUtil::End2D();
	if (mpBoss) {

		//CXPlayerのパラメータ等の２D描画は一番最後
		CBoss::GetInstance()->Render2D();
	}
	//CXPlayerのパラメータ等の２D描画は一番最後
	CXPlayer::GetInstance()->Render2D();
}

CScene::EScene CSceneGame::GetNextScene()
{
	return mScene;
}
