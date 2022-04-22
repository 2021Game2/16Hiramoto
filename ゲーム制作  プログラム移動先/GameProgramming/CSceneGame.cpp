#include "CSceneGame.h"
//
#include "CCamera.h"
//
#include "CUtil.h"
//
#include "CRes.h"

#include "Ckey.h"
//
#include "CMaterial.h"
#include"CScene.h"
#include "CCollisionManager.h"
#include"CEnemy2.h"
#include"CItem.h"
#include"CRock.h"
#include"CSound.h"
#define ENEMY2COUNT 1 //一度に出せる敵２の数
#define ENEMY2MINCOUNT 4 //敵２を再生成させるときの敵２の数の下限
#define ENEMY3COUNT 5//一度に出せる敵３の数
#define ENEMY3MINCOUNT 4 //敵３を再生成させるときの敵３の数の下限
#define BGM "SE\\BGM.wav" //BGM
#define ATTACK1 "SE\\一撃目.wav" //プレイヤーの攻撃SE１
#define ATTACK2 "SE\\二撃目.wav" //プレイヤーの攻撃SE２
#define ATTACK3 "SE\\三撃目.wav" //プレイヤーの攻撃SE３
#define ATTACK4 "SE\\ジャンプ攻撃.wav" //プレイヤーのジャンプ攻撃SE
#define DAMAGE "SE\\ダメージ.wav"  //プレイヤーダメージ時のSE
#define FRY "SE\\ハチの羽ばたき.wav"   //敵３の移動中SE
#define VOICE "SE\\ボス鳴き声.wav"
#define VOICE2 "SE\\ボスの足音.wav"
#define MOVE "SE\\サソリ鳴き声.wav"
#define FONT "FontG.png"
#define SCOPION "3DModel\\scorpid\\scorpid-monster-X-animated.X"
#define BOSS "3DModel\\Boss\\monster-animated-character-X.X"
#define KNIGHT "3DModel\\knight\\knight_low.x"
#define TEXWIDTH  8192  //テクスチャ幅
#define TEXHEIGHT  6144  //テクスチャ高さ

//CMatrix Matrix;
int CSceneGame::mEnemy2Count = 0;
int CSceneGame::mEnemy2CountStopper = ENEMY2COUNT;
int CSceneGame::mEnemy3Count = 0;
int CSceneGame::mEnemy3CountStopper = ENEMY3COUNT;
int CSceneGame::mVoiceSwitch =0 ;//0：音声なし １：音声あり

CSound FirstAttack;
CSound SecondAttack;
CSound ThirdAttack;
CSound JumpAttack;
CSound Damage;
CSound Enemy3Fry;
CSound BossVoice;
CSound BossMove;
CSound Enemy2Voice;
CSceneGame::CSceneGame() 
	:mTimeCount(0)
	,mTimeSecond(0)
	,mTimeMinute(0)
	,mSpawn(0)
    , mSpawn2(0)
{

}
CSceneGame::~CSceneGame() {
	Sleep(2000);

}
void ShadowRender() {
	//影の影響を受ける用になる
	CTaskManager::Get()->Render();
}
void CSceneGame::Init()

{
	//サウンド(wav)ファイルの読み込み

	mScene = EGAME;
	Bgm.Load(BGM);
	if (mVoiceSwitch == 1) {

	  Bgm.Repeat();
	}
	FirstAttack.Load(ATTACK1);
	SecondAttack.Load(ATTACK2);
	ThirdAttack.Load(ATTACK3);
	JumpAttack.Load(ATTACK4);
	Damage.Load(DAMAGE);
	Enemy3Fry.Load(FRY);
	BossVoice.Load(VOICE);
	BossMove.Load(MOVE);
	Enemy2Voice.Load(VOICE2);
   
	//テキストフォントの読み込みと設定
	mFont.LoadTexture(FONT, 1, 4096 / 64);
	
	CRes::sModelX.Load(MODEL_FILE);
     //キャラクターにモデルを設定
	mPlayer.Init(&CRes::sModelX);
	mPlayer.mPosition = CVector(-63.0f, 5.0f, -150.0f);
	/*
	CRes::sKnight.Load(KNIGHT);
    CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//1:移動
	CRes::sKnight.SeparateAnimationSet(0, 1530, 1830, "idle1");//2:待機
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//3:ダミー
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//4:ダミー
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//5:ダミー
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//6:ダミー
	CRes::sKnight.SeparateAnimationSet(0, 440, 520, "attack1");//7:Attack1
	CRes::sKnight.SeparateAnimationSet(0, 520, 615, "attack2");//8:Attack2
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//9:ダミー
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//10:ダミー
	CRes::sKnight.SeparateAnimationSet(0, 1160, 1260, "death1");//11:ダウン
	

	//敵の初期設定
	mEnemy.Init(&CRes::sKnight);
	mEnemy.mAnimationFrameSize = 1024;
	//敵の配置
	mEnemy.mPosition = CVector(700.0f, 0.0f, 0.0f);
	*/
	//カメラ初期化
	Camera.Init();

    CRes::sScorp.Load(SCOPION);
	CRes::sScorp.SeparateAnimationSet(0, 0, 72, "walk");
	CRes::sScorp.SeparateAnimationSet(0, 72, 120, "strafe left");
	CRes::sScorp.SeparateAnimationSet(0, 120, 168, "strafe right");
	CRes::sScorp.SeparateAnimationSet(0, 168, 220, "attack");
	CRes::sScorp.SeparateAnimationSet(0, 292, 350, "attack2");
	CRes::sScorp.SeparateAnimationSet(0, 350, 440, "attack3");
	CRes::sScorp.SeparateAnimationSet(0, 220,292, "death");
	CRes::sScorp.SeparateAnimationSet(0, 660,760, "idle");
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
	CRes::sBoss.SeparateAnimationSet(0, 500, 550, "death - 02" );
	CRes::sBoss.SeparateAnimationSet(0, 565, 650, "death - 03");
	//新しく作る
	mpBoss = new CBoss(CVector(0.0f, 10.0f, 0.0f), 
		CVector(0.0f, 0.0f, 0.0f), CVector(0.5f, 0.5f, 0.5f));
	//読み込ませる
	mpBoss->Init(&CRes::sBoss);
	//ボスの配置
	mpBoss->mPosition = CVector(3.0f, 10.0f, 100.0f);
	
	new CItem(CVector(-20.0f, 2.0f, -10.0f) ,
		CVector(), CVector(1.5f, 1.5f, 1.5f));
	mpEnemySummon = new CEnemySummon(CVector(-36.0f, 1.0f,-59.0f),
		CVector(), CVector(0.5f, 0.5f, 0.5f));

	mpEnemySummon2 = new CEnemySummon(CVector(6.0f, 8.0f, 14.0f),
		CVector(), CVector(0.5f, 0.5f, 0.5f));
	/*
	mpRock=new CRock(CVector(0.0f, 0.0f, -100.0f),
		CVector(0.0f,180.0f,0.0f), CVector(0.5f, 0.5f, 0.5f));*/
	/*
	mpField = new CField(CVector(0.0f, 0.0f, 0.0f),
		CVector(), CVector(0.5f, 0.5f, 0.5f));
	*/

	float shadowColor[] = { 0.4f, 0.4f, 0.4f, 0.2f };  //影の色
	float lightPos[] = { 50.0f, 160.0f, 50.0f };  //光源の位置
	mShadowMap.Init(TEXWIDTH, TEXHEIGHT, ShadowRender, shadowColor, lightPos);//影の初期化
}


void CSceneGame::Update() {
	mTimeCount++;
	if (mTimeCount % 60 == 0) {
		mTimeSecond++;
	}
	if (mTimeSecond==60) {
		mTimeMinute++;
		mTimeSecond = 0;
	}
	//敵の生成間隔
	if (mSpawn >= 0) {
		mSpawn--;
	}
	//敵の生成間隔
	if (mSpawn2 >= 0) {
		mSpawn2--;
	}
	//mEnemy2CountStopperに設定した数だけ敵を生成
	if (mEnemy2Count < mEnemy2CountStopper) {
		//２秒ごとに生成
		if (mSpawn <= 0) {
			mpEnemy2 = new CEnemy2(mpEnemySummon->mPosition,
				CVector(), CVector(1.5f, 1.5f, 1.5f));
			mpEnemy2->Init(&CRes::sScorp);
			mEnemy2Count++;
			mSpawn = 120;
		}
	}

	//敵が一定の数減るまで再生成しない
	else if( mEnemy2CountStopper<= ENEMY2MINCOUNT) {
		mEnemy2CountStopper = ENEMY2COUNT;
	}
	//mEnemy3CountStopperに設定した数だけ敵を生成
	if (mEnemy3Count < mEnemy3CountStopper) {
		//２秒ごとに生成
		if (mSpawn2 <= 0) {
			mpEnemy3 = new CEnemy3(mpEnemySummon2->mPosition, CVector(0.0f, 0.0f, 0.0f),
				CVector(1000.5f, 1000.5f, 1000.5f));
			mEnemy3Count++;
			mSpawn2 = 120;
		}
	}

	//敵が一定の数減るまで再生成しない
	else if (mEnemy3CountStopper <= ENEMY3MINCOUNT) {
		mEnemy3CountStopper = ENEMY3COUNT;
	}
	//エスケープキーで終了
	if (CKey::Push(VK_ESCAPE)) {
		exit(0);
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
	Camera.CameraRender();//カメラ設定
	mShadowMap.Render();//影設定
	//コライダの描画
	//ここをコメントにするとすべてのコライダ非表示
	CCollisionManager::Get()->Render();
	//2D描画開始
	CUtil::Start2D(0, 800, 0, 600);
	char buf[64];
	if (CBoss::mHp>0) {
		sprintf(buf, "%d:", mTimeMinute);
		mFont.DrawString(buf, 200, 500, 8, 16);
		sprintf(buf, "%d", mTimeSecond);
		mFont.DrawString(buf, 250, 500, 8, 16);
		sprintf(buf, "SPECIAL:%10d", CXPlayer::mSpAttack);
		mFont.DrawString(buf, 20, 100, 8, 16);
		sprintf(buf, "PositionX:%f", mPlayer.mPosition.mX);
		mFont.DrawString(buf, 20, 200, 8, 16);
		sprintf(buf, "PositionY:%f", mPlayer.mPosition.mY);
		mFont.DrawString(buf, 20, 250, 8, 16);
		sprintf(buf, "PositionZ:%f", mPlayer.mPosition.mZ);
		mFont.DrawString(buf, 20, 300, 8, 16);
	}
	else if (CBoss::mHp <= 0) {
	sprintf(buf, "GAMECLEAR" );
	mFont.DrawString(buf, 20, 300, 16, 32);
	}
	//2Dの描画終了
	CUtil::End2D();
	//CXPlayerのパラメータ等の２D描画は一番最後
	CXPlayer::GetInstance()->Render2D();
}

CScene::EScene CSceneGame::GetNextScene()
{
	return mScene;
}
