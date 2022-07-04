#include"CSceneTitle.h"
#include "CSceneTitle.h"
#include "CKey.h"
#include "CUtil.h"
#include "CKey.h"
#include <stdio.h>
#include"CSceneGame.h"
#include "CInput.h"
#include"CFade.h"
#include"CScene.h"

#include "CRes.h"
#include"CSceneManager.h"
#define FONT "Resource\\png,tga\\FontG.png" //�t�H���g
#define IMAGE_BUTTONBACK "Resource\\png,tga\\Gauge.png" //�{�^���w�i�摜
#define IMAGE_TITLE "Resource\\png,tga\\Title.png"
#define IMAGE_BACKGROUND "Resource\\png,tga\\Background.png"
CSceneTitle::CSceneTitle()
	:mSelect(EBACKGROUND)
{
	
	mFont.LoadTexture(FONT, 1, 4096 / 64);
	mImageButtonBack.Load(IMAGE_BUTTONBACK);
	mImageTitle.Load(IMAGE_TITLE);
	mImageBackground.Load(IMAGE_BACKGROUND);
}

void CSceneTitle::Init()
{
	mScene = ETITLE;
	CFade::SetFade(CFade::FADE_IN);
	mSceneChange = false;
	//CSceneGame* tSceneGame = CSceneGame::GetInstance();
	
	//mRecord = tSceneGame->mClearTime;
}

void CSceneTitle::Update()
{
	int mouseX, mouseY;
	CInput::GetMousePosW(&mouseX, &mouseY);
	mouseY = 600 - mouseY;

	mSelect = EBACKGROUND;
	//�w�肵���͈͂��܂Ƃ߂�(���[�A�E�[�A���A��j
	if ((mouseX >= 240 - 20 && mouseX <= (240 - 20) + (40 * 9)) &&
		(mouseY >= 150 - 20 && mouseY <= 150 + 20)) {
		mSelect = EGAMESTART;
	}
	
	if (CKey::Once(VK_LBUTTON)) {
		switch (mSelect) {
		case EGAMESTART:
			mNextScene = CScene::EGAME;
			mSceneChange = true;
			CFade::SetFade(CFade::FADE_OUT);
			/*
			�Q�[���X�^�[�g���̌��ʉ���ǉ�����
			*/
		default:
			break;
		}
	}
	if (mSceneChange) {
		if (CFade::IsFadeEnd()) {
			mScene = mNextScene;
		}
	}
	CUtil::Start2D(0, 800, 0, 600);
	mImageBackground.Draw(0, 800, 0, 600, 0, 800, 580, 0);
	mImageTitle.Draw(0, 800, 200, 580,0, 900,1060,340);
	//�J�[�\�������킹���Ƃ�
	switch (mSelect) {
	case EGAMESTART:
		//�{�^���̌��ɉe��`��
		mImageButtonBack.Draw(220, 580, 150, 190, 338, 338, 20, 20);
		break;
		
	}

	char buf[64];
	//�^�C�g��
	//mFont.DrawString("CREATURE", 120, 500, 40, 40);
	//mFont.DrawString("HUNTER", 200, 400, 40, 40);

	mFont.DrawString("GAMESTART", 240, 170, 20, 20);

	//mFont.DrawString("RECORD", 240, 100, 15, 15);
	//sprintf(buf, "%02d:%05.2f", (int)mRecord / 60, fmod(mRecord, 60));
	//mFont.DrawString(buf, 460, 100 , 15, 20);
	
	CUtil::End2D();
}

void CSceneTitle::Render(){

	//2D�`��J�n
	CUtil::Start2D(0, 800, 0, 600);
	//2D�̕`��I��
	CUtil::End2D();
}
CScene::EScene CSceneTitle::GetNextScene() {
	return mScene;
}