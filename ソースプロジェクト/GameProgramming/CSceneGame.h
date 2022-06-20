#ifndef CSCENEGAME_H
#define CSCENEGAME_H

#include "CScene.h"
#include "CXPlayer.h"
#include "CBillBoard2.h"
#include "CSound.h"
#include "CMap.h"
#include"CEnemySummon.h"
#include"CEnemy2.h"
#include"CBillBoard2.h"
#include"CRock.h"
#include"CFlag.h"
#include"CEnemy3.h"
#include"CSound.h"
#include"CBoss.h"
#include"CShadowMap.h"
#include"CTarget.h"
#include"CItem.h"
/*
�Q�[���̃V�[��
*/

class CSceneGame : public CScene {
private:
	enum ESceneSelect
	{
		EGAME,
		ETITLE,
	};
	ESceneSelect mSceneSelect;
	//�}�b�v�̃C���X�^���X
	CModel mModelC5;
	int mSpawn;//�G�����������܂ł̎���
	int mSpawn2;
	static bool mSceneCount;
	static int mTimeMinute;
	static int mTimeSecond;
	int mTimeCount;
	CShadowMap mShadowMap;
	CMap mMap;//�t�B�[���h	
	//�L�����N�^�̃C���X�^���X
	CXPlayer* mpPlayer;//�v���C���[

	
	CRock*mpRock;//����̊�
	CMap* mpMap;
	CFlag* mpFlag;//��
	//�G�̃C���X�^���X
	CEnemy2* mpEnemy2;//�G�Q
	CEnemy3* mpEnemy3;//�G�R
	CEnemySummon* mpEnemySummon;//�G�Q�̐����ꏊ
	CEnemySummon* mpEnemySummon2;//�G�R�̐����ꏊ
	CTarget* mpTarget;
	CItem* mpItem;
	CBoss* mpBoss;//�{�X
	bool mSceneChange;
	EScene mNextScene;
public:
	std::vector<CEnemy2*> mEnemy2List;//Enemy2��p�̕��������
	std::vector<CEnemy3*> mEnemy3List;
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
	 static bool mGameClear;
	 static bool mGameOver;
	static bool mBgmCountCheck;
	static bool mBossSwitch;
	static bool mBossGaugeSwitch;
//BGM�𗬂����~�߂邩������t���O
	bool mBgmCountCheck2;//BGM���A���ōĐ����Ȃ��悤�ɂ���t���O
	static bool mVoiceSwitch;//BGM SE�̃I���I�t�؂�ւ�
	static int mEnemy2Count;//����������Ă���G2�̐�
	static int mEnemy3Count;//����������Ă���G3�̐�
	static int mEnemy2CountStopper;//�����ł���G2�̌��x
	static int mEnemy3CountStopper;//�����ł���G3�̌��x
	static bool mEnemy2Bgm;
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
