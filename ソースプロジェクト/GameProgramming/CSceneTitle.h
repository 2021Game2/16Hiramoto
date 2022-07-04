#include "CScene.h"
#ifndef CSCENETITLE_H
#define CSCENETITLE_H
class CSceneTitle :public CScene {
private:
	enum ESelect {
		EBACKGROUND = 0,//îwåi
		EGAMESTART,		//ÉQÅ[ÉÄÉXÉ^Å[Ég
	};
	int mSelect;
	float mRecord;
	CTexture mImageTitle;
	CTexture mImageButtonBack; //É{É^ÉìîwåiâÊëú
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
