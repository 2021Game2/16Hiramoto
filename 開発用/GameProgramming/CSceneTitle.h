#include "CScene.h"
#ifndef CSCENETITLE_H
#define CSCENETITLE_H
class CSceneTitle :public CScene {
private:
	enum ESelect {
		EBACKGROUND = 0,//�w�i
		EGAMESTART,		//�Q�[���X�^�[�g
	};
	int mSelect;
	float mRecord;
	CTexture mImageTitle;
	CTexture mImageButtonBack; //�{�^���w�i�摜
	CTexture mImageBackground;
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
