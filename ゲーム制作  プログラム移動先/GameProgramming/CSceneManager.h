#ifndef CSCENEMANAGER_H
#define CSCENEMANAGER_H
#include "CScene.h"
#include"CFade.h"
/*
�V�[���}�l�[�W���N���X
*/
class CSceneManager {
public:
	CScene *mpScene; //�V�[���ւ̃|�C���^
	CFade* mpFade;
	//�R���X�g���N�^
	CSceneManager();
	//�f�X�g���N�^�i�폜�����Ƃ��Ɏ��s����܂��j
	~CSceneManager();
	//����������
	void Init();
	//�X�V����
	void Update();
	void Render();

	CScene::EScene mScene;
};

#endif
