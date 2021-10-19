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
//
#include "CCollisionManager.h"

#include"CEnemy2.h"

CMatrix Matrix;
int CSceneGame::mEnemyCount = 0;
CSceneGame::~CSceneGame() {

}

void CSceneGame::Init() {
	
	//テキストフォントの読み込みと設定
	mFont.LoadTexture("FontG.png", 1, 4096 / 64);

	CRes::sModelX.Load(MODEL_FILE);
	CRes::sKnight.Load("knight\\knight_low.x");
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

	//キャラクターにモデルを設定
	mPlayer.Init(&CRes::sModelX);
	mpEnemySummon = new CEnemySummon(CVector(0.0f, 0.0f, 0.0f),
		CVector(), CVector(0.5f, 0.5f, 0.5f));


	//敵の初期設定
	mEnemy.Init(&CRes::sKnight);
	mEnemy.mAnimationFrameSize = 1024;
	//mEnemy2 = new CXEnemy();

	//敵の配置
	mEnemy.mPosition = CVector(7.0f, 0.0f, 0.0f);
	


	mEnemy.ChangeAnimation(7, true, 200);

}


void CSceneGame::Update() {
	//CTaskManager::Get()->Render();
	//キャラクタークラスの更新
	mPlayer.Update();
	//敵の更新
	mEnemy.Update();
	if (mSpawn >= 0) {
		mSpawn--;
	}
	if (mSpawn <= 0) {
		new CEnemy2(
			mpEnemySummon->mPosition,
			CVector(), CVector(0.5f, 0.5f, 0.5f));
		mEnemyCount++;
		mSpawn = 20;
	}
	//衝突処理
	CCollisionManager::Get()->Collision();

	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//視点を求める
	e = CVector(1.0f, 2.0f, 10.0f);
	//注視点を求める
	c = CVector();
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f);

	//カメラクラスの設定
	Camera.Set(e, c, u);
	Camera.Render();

	//X軸＋回転
	/*if (CKey::Push('K')) {

		Matrix = Matrix*mPlayer.mPosition * CMatrix().RotateX(1);
	}
	if (CKey::Push('I')) {
		Matrix = Matrix *mPlayer.mPosition * CMatrix().RotateX(-1);
	}
	//Y軸＋回転
	if (CKey::Push('L')) {
		Matrix = Matrix*mPlayer.mPosition * CMatrix().RotateY(1);
	}
	if (CKey::Push('J')) {
		Matrix = Matrix* mPlayer.mPosition * CMatrix().RotateY(-1);
	}*/

	//行列設定
	glMultMatrixf(Matrix.mF);
	CTaskManager::Get()->Render();
	//モデル描画
//	CRes::sModelX.Render();
	mPlayer.Render();
	//敵描画
	mEnemy.Render();
	mMap.Render();
	//コライダの描画
	CCollisionManager::Get()->Render();

	//2D描画開始
	CUtil::Start2D(0, 800, 0, 600);

	mFont.DrawString("3D PROGRAMMING", 20, 20, 10, 12);

	//2Dの描画終了
	CUtil::End2D();

	return;
}

