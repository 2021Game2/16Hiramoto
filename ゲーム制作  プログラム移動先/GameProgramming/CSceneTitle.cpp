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
#include"CSceneManager.h"
#define FONT "Resource\\png,tga\\FontG.png" //�t�H���g
#define IMAGE_BUTTONBACK "Resource\\png,tga\\Gauge.png" //�{�^���w�i�摜

CSceneTitle::CSceneTitle()
	:mSelect(EBACKGROUND)
{
	mFont.LoadTexture(FONT, 1, 4096 / 64);
	mImageButtonBack.Load(IMAGE_BUTTONBACK);
}

void CSceneTitle::Init()
{
	mScene = ETITLE;
	CFade::SetFade(CFade::FADE_IN);
}

void CSceneTitle::Update()
{
	int mouseX, mouseY;
	CInput::GetMousePosW(&mouseX, &mouseY);
	mouseY = 600 - mouseY;

	mSelect = EBACKGROUND;
	//�w�肵���͈͂��܂Ƃ߂�(���[�A�E�[�A���A��j
	if ((mouseX >= 240 - 20 && mouseX <= (240 - 20) + (40 * 9)) &&
		(mouseY >= 220 - 20 && mouseY <= 220 + 20)) {
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

	//�J�[�\�������킹���Ƃ�
	switch (mSelect) {
	case EGAMESTART:
		//�{�^���̌��ɉe��`��
		mImageButtonBack.Draw(220, 580, 200, 240, 338, 338, 20, 20);
		break;
		

	default:
		break;
	}

	//�^�C�g��
	mFont.DrawString("CREATURE", 120, 500, 40, 40);
	mFont.DrawString("HUNTER", 200, 400, 40, 40);

	mFont.DrawString("GAMESTART", 240, 220, 20, 20);
	
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