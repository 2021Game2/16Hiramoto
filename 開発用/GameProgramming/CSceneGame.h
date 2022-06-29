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
#include <time.h>
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
	//�}�b�v�̃C���X�^���X
	CModel mModelC5;
	CShadowMap mShadowMap;
	EScene mNextScene;
	clock_t mStartTime, mEndTime; //�v���J�n�����A�v���I������
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
	std::vector<CEnemy2*> mEnemy2List;//Enemy2��p�̕��������
	std::vector<CEnemy3*> mEnemy3List;
	 int mSpawn;//�G�����������܂ł̎���
	 int mSpawn2;
	 int mTimeCount;
	 bool mSceneChange;
	 bool mBgmStartStopper;
	 bool mBgmBattleStopper;
	 bool mBgmBossStopper;
	 bool mBgmOverStopper;
	 bool mBgmClearStopper;
	 bool mBgmCountCheck2;//BGM���A���ōĐ����Ȃ��悤�ɂ���t���O
	 bool mGameClear;
	 bool mGameOver;
	 bool mCountStart; //�N���A���Ԍv���J�n�p

	
	 int mTimeMinute; 
	 //static�Ń|�C���^�����
	 static CSceneGame* mpSceneGameInstance;
	
public: 
	 float mClearTime;
	 bool mEnemy2Bgm;
	 bool mBgmCountCheck;
	 bool mVoiceSwitch;//BGM SE�̃I���I�t�؂�ւ� 
	 bool mBossSwitch;
     bool mBossGaugeSwitch;//BGM�𗬂����~�߂邩������t���O
	 int mBgmCount;
	void SetBgmCount(int v) {
		if (v < 0) return;//< �����Ƀu���[�N�|�C���g��u���ΒN���Ɛl�Ȃ̂��킩��
		this->mBgmCount = v;
	}
	int GetBgmCount() {
		return mBgmCount;
	}
	int mTimeSecond;
	void SetTimeSecond(int v) {
		if (v < 0) return;//< �����Ƀu���[�N�|�C���g��u���ΒN���Ɛl�Ȃ̂��킩��
		this->mTimeSecond = v;
	}
	int GetTimeSecond() {
		return mTimeSecond;
	}
	int mEnemy2Count;//����������Ă���G2�̐�
	void SetEnemy2Count(int v) {
		if (v < 0) return;//< �����Ƀu���[�N�|�C���g��u���ΒN���Ɛl�Ȃ̂��킩��
		this->mEnemy2Count = v;
	}
	int GetEnemy2Count() {
		return mEnemy2Count;
	}
	int mEnemy3Count;//����������Ă���G3�̐�
	void SetEnemy3Count(int v) {
		if (v < 0) return;//< �����Ƀu���[�N�|�C���g��u���ΒN���Ɛl�Ȃ̂��킩��
		this->mEnemy3Count = v;
	}
	int GetEnemy3Count() {
		return mEnemy3Count;
	}
	int mEnemy2CountStopper;//�����ł���G2�̌��x
	void SetEnemy2CountStopper(int v) {
		if (v < 0) return;//< �����Ƀu���[�N�|�C���g��u���ΒN���Ɛl�Ȃ̂��킩��
		this->mEnemy2CountStopper = v;
	}
	int  GetEnemy2CountStopper() {
		return mEnemy2CountStopper;
	}
	int mEnemy3CountStopper;//�����ł���G3�̌��x
	void SetEnemy3CountStopper(int v) {
		if (v < 0) return;//< �����Ƀu���[�N�|�C���g��u���ΒN���Ɛl�Ȃ̂��킩��
		this->mEnemy3CountStopper = v;
	}
	int  GetEnemy3CountStopper() {
		return mEnemy3CountStopper;
	}
	//static�ŏ��������
	static CSceneGame* GetInstance();
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
