#include <stdio.h>
#include "CSceneManager.h"
#include "CSceneGame.h"
#include"CSceneTitle.h"
#include"CUtil.h"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
//�R���X�g���N�^
CSceneManager::CSceneManager()
: mpScene(0)
{
	mpFade = new CFade;
	
}
//�f�X�g���N�^�i�폜�����Ƃ��Ɏ��s����܂��j
CSceneManager::~CSceneManager() {

	//�V�[��������΍폜
	if (mpScene)
		//�V�[���̍폜
		delete mpScene;
	mpScene = 0;
	if (mpFade) {
		delete mpFade;
		mpFade = 0;
	}
}
//����������
void CSceneManager::Init() {
	mScene = CScene::ETITLE;
	//�V�[���𐶐����A�|�C���^��ݒ肷��
	mpScene = new CSceneTitle();
	//���������N���X�̃��\�b�h���Ă΂��
	mpScene->Init();
}
//�X�V����
void CSceneManager::Update() {
	//�|�C���^��Update���Ă�
	mpScene->Update();
	mpFade->Update();
	//���̃V�[�����擾���قȂ邩����
	if (mScene != mpScene->GetNextScene()) {
		mScene = mpScene->GetNextScene();
		delete mpScene;//���̃V�[���폜
		//�Y������V�[���𐶐�
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
