#ifndef CSCENEGAME_H
#define CSCENEGAME_H
#include "CScene.h"
#include "CXPlayer.h"
#include "CXEnemy.h"
#include "CBillBoard2.h"
#include "CSound.h"
#include "CMap.h"
#include"CEnemySummon.h"
#include"CEnemy2.h"
#include"CBillBoard2.h"
/*
�Q�[���̃V�[��
*/
class CSceneGame : public CScene {
	CSound mJump;
	//CBillBoard2 mBillBoard;

public:
	//�}�b�v�̃C���X�^���X
	CMap mMap;
	int mSpawn;
	CEnemySummon* mpEnemySummon;
	static int mEnemyCount;

	//�L�����N�^�̃C���X�^���X
	CXPlayer mPlayer;
	//�G�̃C���X�^���X
	CXEnemy mEnemy;

	~CSceneGame();
	//�����������̃I�[�o�[���C�h
	void Init();
	//�X�V�����̃I�[�o�[���C�h
	void Update();
	//�`�揈��
	void Render();

};

#endif
