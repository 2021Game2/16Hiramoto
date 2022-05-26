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
#include"CShadowMap.h"
#include"CTarget.h"
/*
�Q�[���̃V�[��
*/

class CSceneGame : public CScene {
private:


	//�}�b�v�̃C���X�^���X
	CModel mModelC5;
	int mSpawn;//�G�����������܂ł̎���
	int mSpawn2;
	int mTimeMinute;
	int mTimeSecond;
	int mTimeCount;
	CShadowMap mShadowMap;
	CMap mMap;//�t�B�[���h	
	CRock*mpRock;//����̊�
	CTree* mpTree;//��
	//�L�����N�^�̃C���X�^���X
	CXPlayer mPlayer;//�v���C���[

	//�G�̃C���X�^���X
	//CXEnemy mEnemy;//�G�P
	CEnemy2* mpEnemy2;//�G�Q
	CEnemy3* mpEnemy3;//�G�R
	CEnemySummon* mpEnemySummon;//�G�Q�̐����ꏊ
	CEnemySummon* mpEnemySummon2;//�G�R�̐����ꏊ
	CBoss* mpBoss;//�{�X
public:
	CTexture mImageMouse;
	CTexture mImageMoveKey;
	CTexture mImageCkey;
	CTexture mImageWork;
	CTexture mImageDush;

    CSound mBgmStart;
	CSound mBgmBattle;
	CSound mBgmBossBattle;
	CSound mBgmGameClear;
	CSound mBgmGameOver;
	 static int mBgmCount;
	 bool mBgmStartStopper;
	 bool mBgmBattleStopper;
	 bool mBgmBossStopper;
	 bool mBgmOverStopper;
	 bool mBgmClearStopper;
	static bool mBgmCountCheck;
//BGM�𗬂����~�߂邩������t���O
	bool mBgmCountCheck2;//BGM���A���ōĐ����Ȃ��悤�ɂ���t���O
	static bool mVoiceSwitch;//BGM SE�̃I���I�t�؂�ւ�
	static int mEnemy2Count;//����������Ă���G2�̐�
	static int mEnemy3Count;//����������Ă���G3�̐�
	static int mEnemy2CountStopper;//�����ł���G2�̌��x
	static int mEnemy3CountStopper;//�����ł���G3�̌��x
	CSceneGame();
	~CSceneGame();
	//�����������̃I�[�o�[���C�h
	void Init();
	//�X�V�����̃I�[�o�[���C�h
	void Update();
	//�`�揈��
	void Render();
	void BgmStart();
	void BgmBattle();
	void BgmBoss();
	void BgmGameOver();
	void BgmGameClear();
	EScene GetNextScene();
};

#endif
