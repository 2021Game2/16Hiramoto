#ifndef CSCENEGAME_H
#define CSCENEGAME_H

#include "CScene.h"
#include "CXPlayer.h"
#include "CBillBoard2.h"
#include "CSound.h"
#include "CMap.h"
#include"CBossStage.h"
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
	CBossStage* mpBossStage;
	CMap* mpMap;//�t�B�[���h
	CFlag* mpFlag;//��
	//�G�̃C���X�^���X
	CEnemy2* mpEnemy2;//�G�Q
	CEnemy3* mpEnemy3;//�G�R
	CEnemySummon* mpEnemySummon;//�G�Q�̐����ꏊ
	CEnemySummon* mpEnemySummon2;//�G�R�̐����ꏊ
	CTarget* mpTarget;//�ړI�n�̕������������
	CItem* mpItem;//�n���}�[
	CBoss* mpBoss;//�{�X
	//�}�b�v�̃C���X�^���X
	
	CShadowMap mShadowMap;//�V���h�E�}�b�v�i�e�j
	EScene mNextScene;//���̃V�[���̐ݒ�
	clock_t mStartTime, mEndTime; //�v���J�n�����A�v���I������
	CTexture mImageMouse;//�}�E�X�̃e�N�X�`��
	CTexture mImageMoveKey;//�ړ��L�[�iASWD�j�̃e�N�X�`��
	CTexture mImageCkey;//C�L�[�̃e�N�X�`��
	CTexture mImageWork;//�����Ă���l�̃e�N�X�`��
	CTexture mImageDush;//�����Ă���l�̃e�N�X�`��
    CSound mBgmStart;//�ŏ���BGM
	CSound mBgmBattle;//�G�Ɛ���Ă���Ƃ���BGM
	CSound mBgmBossBattle;//�{�X�Ɛ���Ă���Ƃ���BGM
	CSound mBgmGameClear;//�Q�[���N���A����BGM
	CSound mBgmGameOver;//�Q�[���I�[�o�[����BGM
	std::vector<CEnemy2*> mEnemy2List;//Enemy2��p�̕��������
	std::vector<CEnemy3*> mEnemy3List;//Enemy3��p�̕��������
	 int mSpawn;//�G2�����������܂ł̎���
	 int mSpawn2;//�G3�����������܂ł̎���
	
	 bool mSceneChange;//�V�[���̐؂�ւ�

	 bool mBgmStartStopper;//BGM���~�߂�
	 bool mBgmBattleStopper;//BGM���~�߂�
	 bool mBgmBossStopper;//BGM���~�߂�
	 bool mBgmOverStopper;//BGM���~�߂�
	 bool mBgmClearStopper;//BGM���~�߂�
	 bool mBgmCountCheck2;//BGM���A���ōĐ����Ȃ��悤�ɂ���t���O
	 bool mGameClear;
	 bool mGameOver;
	 bool mCountStart; //�N���A���Ԍv���J�n�p
	
	 int mTimeMinute; 
	 //static�Ń|�C���^�����
	 static CSceneGame* mpSceneGameInstance;
	
public: 
	CVector mBossStageCenter;
	CVector mBossStageEnd;
	float mBossStageLengthX;//�{�X�̍s���͈͂�X���̒���
	float mBossStageLengthZ;//�{�X�̍s���͈͂�Z���̒���
	float mBossStageLengthSum;//�{�X�̍s���͈͂̔��a
	float mBossStageCircle;//�{�X�̍s���\�͈�
	 float mClearTime;
	 bool mEnemy2Bgm;
	 bool mBgmCountCheck;
	 bool mVoiceSwitch;//BGM SE�̃I���I�t�؂�ւ� 
	 bool mBossSwitch;
     //�{�X�֌W�̃Q�[�W�̕\���E��\��
	 //�{�X�̐����A�폜�̐؂�ւ����ɗ��p
     bool mBossGaugeSwitch;
	 bool mBossBattleStage;
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
	void SetEnemy3Count(int Enemy3Count) {
		if (Enemy3Count < 0) return;//< �����Ƀu���[�N�|�C���g��u���ΒN���Ɛl�Ȃ̂��킩��
		this->mEnemy3Count = Enemy3Count;
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
