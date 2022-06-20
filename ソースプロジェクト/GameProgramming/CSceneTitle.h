#include "CScene.h"
#ifndef CSCENETITLE_H
#define CSCENETITLE_H
class CSceneTitle :public CScene {
private:
	enum ESelect {
		EBACKGROUND = 0,//背景
		EGAMESTART,		//ゲームスタート
	};
	int mSelect;

	CTexture mImageButtonBack; //ボタン背景画像
	 bool mSceneChange;
	 EScene mNextScene;
public:

	CSceneTitle();
	void Init();
	void Update();
	void Render();
	EScene GetNextScene();
};
#endif
