#include "CSceneGame.h"
//
#include "CCamera.h"
//
#include "CUtil.h"
//
#include "CRes.h"

#include "Ckey.h"
//
#include "CMaterial.h"
//
#include "CCollisionManager.h"
#include"CEnemy2.h"
#include"CItem.h"
#include"CRock.h"
//CMatrix Matrix;
int CSceneGame::mEnemyCount = 0;
CSceneGame::~CSceneGame() {
	
}

void CSceneGame::Init() {
	//�T�E���h(wav)�t�@�C���̓ǂݍ���
	Bgm.Load("BGM.wav");
	mJump.Load("jump.wav");
    Sleep(2000);
	//mBillBoard.Set(CVector(0.0f, 5.0f, 0.0f), 1.0f, 1.0f);
	//Bgm.Repeat();
	//�e�L�X�g�t�H���g�̓ǂݍ��݂Ɛݒ�
	mFont.LoadTexture("FontG.png", 1, 4096 / 64);
	 
	CRes::sModelX.Load(MODEL_FILE);
	CRes::sKnight.Load("knight\\knight_low.x");
    CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//1:�ړ�
	CRes::sKnight.SeparateAnimationSet(0, 1530, 1830, "idle1");//2:�ҋ@

	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//3:�_�~�[
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//4:�_�~�[
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//5:�_�~�[
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//6:�_�~�[
	CRes::sKnight.SeparateAnimationSet(0, 440, 520, "attack1");//7:Attack1
	CRes::sKnight.SeparateAnimationSet(0, 520, 615, "attack2");//8:Attack2
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//9:�_�~�[
	CRes::sKnight.SeparateAnimationSet(0, 10, 80, "walk");//10:�_�~�[
	CRes::sKnight.SeparateAnimationSet(0, 1160, 1260, "death1");//11:�_�E��
	//�L�����N�^�[�Ƀ��f����ݒ�
	mPlayer.Init(&CRes::sModelX);

	//�G�̏����ݒ�
	mEnemy.Init(&CRes::sKnight);
	mEnemy.mAnimationFrameSize = 1024;
	//�G�̔z�u
	mEnemy.mPosition = CVector(7.0f, 0.0f, 0.0f);
	mEnemy.ChangeAnimation(2, true, 200);
	

	//mEnemy2.Init(&CRes::sScorp);
	//�J����������
	Camera.Init();


    
    CRes::sScorp.Load("scorpid-monster-X-animated.X");
	CRes::sScorp.SeparateAnimationSet(0, 0, 72, "walk");
	CRes::sScorp.SeparateAnimationSet(0, 72, 120, "strafe left");
	CRes::sScorp.SeparateAnimationSet(0, 120, 168, "strafe right");
	CRes::sScorp.SeparateAnimationSet(0, 168, 220, "attack");
	CRes::sScorp.SeparateAnimationSet(0, 292, 350, "attack2");
	CRes::sScorp.SeparateAnimationSet(0, 350, 440, "attack3");
	CRes::sScorp.SeparateAnimationSet(0, 220,292, "death");
	CRes::sScorp.SeparateAnimationSet(0, 660,760, "idle");
	CRes::sScorp.SeparateAnimationSet(0, 761, 849, "idle2");
	CRes::sScorp.SeparateAnimationSet(0, 850, 880, "gethit");
	CRes::sScorp.SeparateAnimationSet(0, 880, 950, "gethit2");
	CRes::sScorp.SeparateAnimationSet(0, 951, 1015, "jump");


	mBoss.Init(&CRes::sBoss);
	//�{�X�̔z�u
	mBoss.mPosition = CVector(70.0f, 0.0f, 70.0f);
	CRes::sBoss.Load("monster-animated-character-X.X");
	CRes::sBoss.SeparateAnimationSet(0, 0, 30, "walk");
	CRes::sBoss.SeparateAnimationSet(0, 0, 120, "walk");
	CRes::sBoss.SeparateAnimationSet(0, 150, 190, "run");
	CRes::sBoss.SeparateAnimationSet(0, 150, 210, "run");
	CRes::sBoss.SeparateAnimationSet(0, 250, 333, "attack - 01");
	CRes::sBoss.SeparateAnimationSet(0, 320, 400, "attack - 02");
	CRes::sBoss.SeparateAnimationSet(0, 390, 418, "death - 01");
	CRes::sBoss.SeparateAnimationSet(0, 478, 500, "growl");
	CRes::sBoss.SeparateAnimationSet(0, 500, 550, "death - 02" );
	CRes::sBoss.SeparateAnimationSet(0, 565, 650, "death - 03");


	new CItem(CVector(-20.0f, 2.0f, -10.0f) ,
		CVector(), CVector(1.5f, 1.5f, 1.5f));
	mpEnemySummon = new CEnemySummon(CVector(-40.0f, 1.0f, 0.0f),
		CVector(), CVector(0.5f, 0.5f, 0.5f));
	mpRock=new CRock(CVector(-100.0f, 0.0f, 50.0f),
		CVector(), CVector(50.0f, 50.0f, 50.0f));
	mpRock = new CRock(CVector(-100.0f, 0.0f, -100.0f),
		CVector(), CVector(50.0f, 50.0f, 50.0f));
	mpRock = new CRock(CVector(100.0f, 0.0f, 50.0f),
		CVector(), CVector(50.0f, 50.0f, 50.0f));
	mpRock = new CRock(CVector(100.0f, 0.0f, -100.0f),
		CVector(), CVector(50.0f, 50.0f, 50.0f));
	mpTree = new CTree(CVector(70.0f, 0.0f, 0.0f),
		CVector(), CVector(50.0f, 50.0f, 50.0f));
	mpEnemy3=new CEnemy3(CVector(-20.0f, 5.0f, 100.0f),
		CVector(), CVector(1000.5f, 1000.5f, 1000.5f));

}


void CSceneGame::Update() {
	//�G�̃X�|�[���Ԋu
	if (mSpawn >= 0) {
		mSpawn--;
	}
	if (mSpawn <= 0) {
		mpEnemy2 = new CEnemy2(mpEnemySummon->mPosition, CVector(0.0f, 0.1f, 0.0f),
			CVector(1.5f, 1.5f, 1.5f));
	  
		mpEnemy2->Init(&CRes::sScorp);
		
		mEnemyCount++;
		mSpawn = 120;
	}
	if (CKey::Push(VK_ESCAPE)) {
		exit(0);
	}

	//�X�V
	CTaskManager::Get()->Update();

	//�Փˏ���
	CCollisionManager::Get()->Collision();

	Camera.Update();

	//mJump.Play();


	//mBillBoard.Update();




	return;
}

void CSceneGame::Render() {

	//mBillBoard.Render();

	//���f���`��
//	CRes::sModelX.Render();
	//mPlayer.Render();
	////�G�`��
	//mEnemy.Render();

	
	Camera.Render();

	//�^�X�N�̕`��
	CTaskManager::Get()->Render();

	//�R���C�_�̕`��
	CCollisionManager::Get()->Render();
	//2D�`��J�n
	CUtil::Start2D(0, 800, 0, 600);

	mFont.DrawString("3D PROGRAMMING", 20, 20, 10, 12);
	char buf[64];

	sprintf(buf, "SPECIAL:%10d", CXPlayer::mSpAttack);
	mFont.DrawString(buf, 20, 100, 8, 16);
	sprintf(buf, "STAMINA:%10d", CXPlayer::mStamina);
	mFont.DrawString(buf, 20, 150, 8, 16);
	sprintf(buf, "MOVE:%10d", CEnemy3::mMoveCount);
	mFont.DrawString(buf, 20, 200, 8, 16);

	//2D�̕`��I��
	CUtil::End2D();

}
