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
/*
�Q�[���̃V�[��
*/

class CSceneGame : public CScene {
private:


	//�}�b�v�̃C���X�^���X
	CModel mModelC5;
	int mSpawn;//�G�����������܂ł̎���
	CShadowMap mShadowMap;
	CMap mMap;//�t�B�[���h	
	CRock*mpRock;//����̊�
	CTree* mpTree;//��
	//�L�����N�^�̃C���X�^���X
	CXPlayer mPlayer;//�v���C���[
	//�G�̃C���X�^���X
	CXEnemy mEnemy;//�G�P
	CEnemy2* mpEnemy2;//�G�Q
	CEnemy3* mpEnemy3;//�G�R
	CEnemySummon* mpEnemySummon;//�G�Q�̐����ꏊ
	CEnemySummon* mpEnemySummon2;//�G�R�̐����ꏊ
	CBoss* mpBoss;//�{�X
public:
    CSound Bgm;
	static int mVoiceSwitch;//BGM SE�̃I���I�t�؂�ւ�
	static int mEnemy2Count;//����������Ă���G2�̐�
	static int mEnemy3Count;//����������Ă���G3�̐�
	static int mEnemy2CountStopper;//�����ł���G2�̌��x
	static int mEnemy3CountStopper;//�����ł���G3�̌��x
	~CSceneGame();
	//�����������̃I�[�o�[���C�h
	void Init();
	//�X�V�����̃I�[�o�[���C�h
	void Update();
	//�`�揈��
	void Render();

};

#endif
