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
#include"CRock.h"
#include"CTree.h"
#include"CEnemy3.h"
#include"CSound.h"
#include"CBoss.h"
//#include"CShadowMap.h"
/*
�Q�[���̃V�[��
*/

class CSceneGame : public CScene {
private:

	CSound mJump;
	//CBillBoard2 mBillBoard;

	//�}�b�v�̃C���X�^���X
	CMap mMap;
	CModel mModelC5;
	CRock*mpRock;
	CTree* mpTree;
	int mSpawn;
	CEnemySummon* mpEnemySummon;
	CEnemy2* mpEnemy2;
	CBoss* mpBoss;
	//CShadowMap mShadowMap;
	//�L�����N�^�̃C���X�^���X
	CXPlayer mPlayer;
	//�G�̃C���X�^���X
	CXEnemy mEnemy;
	CEnemy3* mpEnemy3;
public:

	 CSound Bgm;
	// static CSound Enemy2Voice;
	static int mEnemy2Count;
	static int mEnemy2CountStopper;

	~CSceneGame();
	//�����������̃I�[�o�[���C�h
	void Init();
	//�X�V�����̃I�[�o�[���C�h
	void Update();
	//�`�揈��
	void Render();

};

#endif
