#ifndef CSCENEMANAGER_H
#define CSCENEMANAGER_H
#include "CScene.h"
#include"CFade.h"
/*
シーンマネージャクラス
*/
class CSceneManager {
public:
	CScene *mpScene; //シーンへのポインタ
	CFade* mpFade;
	//コンストラクタ
	CSceneManager();
	//デストラクタ（削除されるときに実行されます）
	~CSceneManager();
	//初期化処理
	void Init();
	//更新処理
	void Update();
	void Render();

	CScene::EScene mScene;
};

#endif
