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
#include"CSound.h"
#define ENEMYCOUNT 10 //��x�ɏo����G�̐�
#define BGM "SE\\BGM.wav" //BGM
#define ATTACK1 "SE\\�ꌂ��.wav" //�v���C���[�̍U��SE�P
#define ATTACK2 "SE\\�񌂖�.wav" //�v���C���[�̍U��SE�Q
#define ATTACK3 "SE\\�O����.wav" //�v���C���[�̍U��SE�R
#define ATTACK4 "SE\\�W�����v�U��.wav" //�v���C���[�̃W�����v�U��SE
#define DAMAGE "SE\\�_���[�W.wav"  //�v���C���[�_���[�W����SE
#define FRY "SE\\�n�`�̉H�΂���.wav"   //�G�R�̈ړ���SE
#define VOICE "SE\\�{�X����.wav"
#define VOICE2 "SE\\�{�X�̑���.wav"
#define MOVE "SE\\�T�\������.wav"
#define FONT "FontG.png"
#define SCOPION "3DModel\\scorpid\\scorpid-monster-X-animated.X"
#define BOSS "3DModel\\Boss\\monster-animated-character-X.X"
#define KNIGHT "3DModel\\knight\\knight_low.x"


//CMatrix Matrix;
int CSceneGame::mEnemy2Count = 0;
int CSceneGame::mEnemy2CountStopper = ENEMYCOUNT;
int CSceneGame::mVoiceSwitch =0 ;//0�F�����Ȃ� �P�F��������

CSound FirstAttack;
CSound SecondAttack;
CSound ThirdAttack;
CSound JumpAttack;
CSound Damage;
CSound Enemy3Fry;
CSound BossVoice;
CSound BossMove;
CSound Enemy2Voice;

CSceneGame::~CSceneGame() {
	Sleep(2000);

}

void CSceneGame::Init() {
	//�T�E���h(wav)�t�@�C���̓ǂݍ���
	
	Bgm.Load(BGM);
	if (mVoiceSwitch == 1) {

	  Bgm.Repeat();
	}
	FirstAttack.Load(ATTACK1);
	SecondAttack.Load(ATTACK2);
	ThirdAttack.Load(ATTACK3);
	JumpAttack.Load(ATTACK4);
	Damage.Load(DAMAGE);
	Enemy3Fry.Load(FRY);
	BossVoice.Load(VOICE);
	BossMove.Load(MOVE);
	Enemy2Voice.Load(VOICE2);
   
	//�e�L�X�g�t�H���g�̓ǂݍ��݂Ɛݒ�
	mFont.LoadTexture(FONT, 1, 4096 / 64);
	
	CRes::sModelX.Load(MODEL_FILE);
     //�L�����N�^�[�Ƀ��f����ݒ�
	mPlayer.Init(&CRes::sModelX);
	mPlayer.mPosition = CVector(-63.0f, 5.0f, -150.0f);
	CRes::sKnight.Load(KNIGHT);
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
	

	//�G�̏����ݒ�
	mEnemy.Init(&CRes::sKnight);
	mEnemy.mAnimationFrameSize = 1024;
	//�G�̔z�u
	mEnemy.mPosition = CVector(700.0f, 0.0f, 0.0f);
	
	//�J����������
	Camera.Init();

    CRes::sScorp.Load(SCOPION);
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


	//�A�j���[�V������ǂݍ���
 	CRes::sBoss.Load(BOSS);
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
	//�V�������
	mpBoss = new CBoss(CVector(0.0f, 10.0f, 0.0f), 
		CVector(0.0f, 0.0f, 0.0f), CVector(0.5f, 0.5f, 0.5f));
	//�ǂݍ��܂���
	mpBoss->Init(&CRes::sBoss);
	//�{�X�̔z�u
	mpBoss->mPosition = CVector(3.0f, 10.0f, 100.0f);
	
	new CItem(CVector(-20.0f, 2.0f, -10.0f) ,
		CVector(), CVector(1.5f, 1.5f, 1.5f));
	mpEnemySummon = new CEnemySummon(CVector(17.0f, 1.0f, 32.0f),
		CVector(), CVector(0.5f, 0.5f, 0.5f));

	mpEnemySummon2 = new CEnemySummon(CVector(17.0f, 1.0f, 50.0f),
		CVector(), CVector(0.5f, 0.5f, 0.5f));
	mpRock=new CRock(CVector(0.0f, 5.5f, -100.0f),
		CVector(0.0f,180.0f,0.0f), CVector(0.5f, 0.5f, 0.5f));
	mpTree = new CTree(CVector(70.0f, 0.0f, 0.0f),
		CVector(), CVector(50.0f, 50.0f, 50.0f));
	mpEnemy3=new CEnemy3(CVector(-20.0f, 50.0f, 100.0f),
		CVector(), CVector(1000.5f, 1000.5f, 1000.5f));

}


void CSceneGame::Update() {

	//�G�̃X�|�[���Ԋu
	if (mSpawn >= 0) {
		mSpawn--;
	}
	//Stopper�ɐݒ肵���������G�𐶐�
	if (mEnemy2Count < mEnemy2CountStopper) {
		//�Q�b���Ƃɐ���
		if (mSpawn <= 0) {
			mpEnemy2 = new CEnemy2(mpEnemySummon->mPosition, CVector(0.0f, 0.1f, 0.0f),
				CVector(1.5f, 1.5f, 1.5f));
			mpEnemy2->Init(&CRes::sScorp);
			mEnemy2Count++;
			mSpawn = 120;
		}
	}
	//�G�����̐�����܂ōĐ������Ȃ�
	else if( mEnemy2CountStopper<=4) {
		mEnemy2CountStopper = ENEMYCOUNT;
	}
	//�G�X�P�[�v�L�[�ŏI��
	if (CKey::Push(VK_ESCAPE)) {
		exit(0);
	}
	//�X�V
	CTaskManager::Get()->Update();

	//�Փˏ���(������j
	//CCollisionManager::Get()->Collision();
	CTaskManager::Get()->TaskCollision();
	
	return;
}

void CSceneGame::Render() {
	//�^�X�N�̕`��
	CTaskManager::Get()->Render();
	//�R���C�_�̕`��
	//�������R�����g�ɂ���Ƃ��ׂẴR���C�_��\��
	CCollisionManager::Get()->Render();
	//2D�`��J�n
	CUtil::Start2D(0, 800, 0, 600);
	mFont.DrawString("3D PROGRAMMING", 20, 20, 10, 12);
	char buf[64];
	if (CBoss::mHp>0) {

		sprintf(buf, "SPECIAL:%10d", CXPlayer::mSpAttack);
		mFont.DrawString(buf, 20, 100, 8, 16);
		sprintf(buf, "STAMINA:%10d", CXPlayer::mStamina);
		mFont.DrawString(buf, 20, 150, 8, 16);
		
		sprintf(buf, "PositionX:%f", mPlayer.mPosition.mX);
		mFont.DrawString(buf, 20, 200, 8, 16);
		sprintf(buf, "PositionY:%f", mPlayer.mPosition.mY);
		mFont.DrawString(buf, 20, 250, 8, 16);
		sprintf(buf, "PositionZ:%f", mPlayer.mPosition.mZ);
		mFont.DrawString(buf, 20, 300, 8, 16);
		//sprintf(buf, "MOVE:%10d", CEnemy3::mMoveCount);
		//mFont.DrawString(buf, 20, 200, 8, 16);
		//sprintf(buf, "Y:%10f", mPlayer.mPosition.mY);
		//mFont.DrawString(buf, 20, 250, 8, 16);
	}
	else if (CBoss::mHp <= 0) {
	sprintf(buf, "GAMECLEAR" );
	mFont.DrawString(buf, 20, 300, 16, 32);
	}
	//2D�̕`��I��
	CUtil::End2D();
	//CXPlayer�̃p�����[�^���̂QD�`��͈�ԍŌ�
	CXPlayer::GetInstance()->Render2D();
}
