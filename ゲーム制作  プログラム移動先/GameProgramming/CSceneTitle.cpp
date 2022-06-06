#include"CSceneTitle.h"
#include "CSceneTitle.h"
#include "CKey.h"
#include "CUtil.h"
#include "CKey.h"
#include <stdio.h>

#include "CInput.h"

#define FONT "Resource\\png,tga\\FontG.png" //フォント
#define IMAGE_BUTTONBACK "Resource\\png,tga\\Gauge.png" //ボタン背景画像

CSceneTitle::CSceneTitle()
	:mSelect(EBACKGROUND)
{
	mFont.LoadTexture(FONT, 1, 4096 / 64);
	mImageButtonBack.Load(IMAGE_BUTTONBACK);
}

void CSceneTitle::Init()
{
	mScene = ETITLE;
}

void CSceneTitle::Update()
{
	int mouseX, mouseY;
	CInput::GetMousePosW(&mouseX, &mouseY);
	mouseY = 600 - mouseY;

	mSelect = EBACKGROUND;
	//指定した範囲をまとめる(左端、右端、下、上）
	if ((mouseX >= 240 - 20 && mouseX <= (240 - 20) + (40 * 9)) &&
		(mouseY >= 220 - 20 && mouseY <= 220 + 20)) {
		mSelect = EGAMESTART;
	}
	if (CKey::Once(VK_RETURN)) {
		mScene = EGAME;
	}
	
	if (CKey::Once(VK_LBUTTON)) {
		switch (mSelect) {
		case EGAMESTART:
			mScene = EGAME;
			break;

		default:
			break;
		}
	}

	CUtil::Start2D(0, 800, 0, 600);

	//カーソルをあわせたとき
	switch (mSelect) {
	case EGAMESTART:
		//ボタンの後ろに影を描画
		mImageButtonBack.Draw(220, 580, 200, 240, 338, 338, 20, 20);
		break;
		

	default:
		break;
	}

	//タイトル
	mFont.DrawString("CREATURE", 120, 500, 40, 40);
	mFont.DrawString("HUNTER", 200, 400, 40, 40);
	mFont.DrawString("GAMESTART", 240, 220, 20, 20);
	mFont.DrawString("RECORD TIME", 240, 120, 10, 10);

	CUtil::End2D();
}

void CSceneTitle::Render(){

	//2D描画開始
	CUtil::Start2D(0, 800, 0, 600);
	//2Dの描画終了
	CUtil::End2D();
}
CScene::EScene CSceneTitle::GetNextScene() {
	return mScene;
}