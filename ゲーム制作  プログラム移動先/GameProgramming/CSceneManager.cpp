#include <stdio.h>
#include "CSceneManager.h"
#include "CSceneGame.h"
#include"CSceneTitle.h"
#include"CUtil.h"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
//コンストラクタ
CSceneManager::CSceneManager()
: mpScene(0)
{
	mpFade = new CFade;
	
}
//デストラクタ（削除されるときに実行されます）
CSceneManager::~CSceneManager() {

	//シーンがあれば削除
	if (mpScene)
		//シーンの削除
		delete mpScene;
	mpScene = 0;
	if (mpFade) {
		delete mpFade;
		mpFade = 0;
	}
}
//初期化処理
void CSceneManager::Init() {
	mScene = CScene::ETITLE;
	//シーンを生成し、ポインタを設定する
	mpScene = new CSceneTitle();
	//生成したクラスのメソッドが呼ばれる
	mpScene->Init();
}
//更新処理
void CSceneManager::Update() {
	//ポインタのUpdateを呼ぶ
	mpScene->Update();
	mpFade->Update();
	//次のシーンを取得し異なるか判定
	if (mScene != mpScene->GetNextScene()) {
		mScene = mpScene->GetNextScene();
		delete mpScene;//今のシーン削除
		//該当するシーンを生成
		switch (mScene) {
		case CScene::EGAME:
			mpScene = new CSceneGame();
			mpScene->Init();
			break;
		case CScene::ETITLE:
			mpScene = new CSceneTitle();
			mpScene->Init();
			break;
		
		}
	}
}
void CSceneManager::Render() {
	if (mpScene) {
		mpScene->Render();
	}
	CUtil::Start2D(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	mpFade->Render();
	CUtil::End2D();
}
