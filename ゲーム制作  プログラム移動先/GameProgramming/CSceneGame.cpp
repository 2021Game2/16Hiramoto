#include "CSceneGame.h"
#include "CCamera.h"
#include "CUtil.h"
#include "CRes.h"
#include "Ckey.h"
#include "CMaterial.h"
#include"CScene.h"
#include "CCollisionManager.h"
#include"CEnemy2.h"
#include"CItem.h"
#include"CRock.h"
#include"CSound.h"
#include"CTarget.h"
#define HP 30
#define ENEMY2COUNT 10 //��x�ɏo����G�Q�̐�
#define ENEMY2MINCOUNT 4 //�G�Q���Đ���������Ƃ��̓G�Q�̐��̉���
#define ENEMY3COUNT 1//��x�ɏo����G�R�̐�
#define ENEMY3MINCOUNT 4 //�G�R���Đ���������Ƃ��̓G�R�̐��̉���

#define TEX_BUTTON "Resource\\png,tga\\MoveKey.png"
#define TEX_BUTTON2 "Resource\\png,tga\\CKey.png"
#define TEX_BUTTON3 "Resource\\png,tga\\Mouse.png"
#define TEX_DUSH "Resource\\png,tga\\Dush.png"
#define TEX_WORK "Resource\\png,tga\\Work.png"
#define BGMSTART "BGM\\BGMSTART.wav" //BGM
#define BGMBATTLE "BGM\\BGMBATTLE.wav" //�o�g������BGM
#define BGMBOSSBATTLE "BGM\\BGMBOSSBATTLE.wav" // �{�X�Ɛ���Ă���Ƃ���BGM
#define BGMGAMECLEAR "BGM\\BGMGAMECLEAR.wav" //�Q�[���N���A����BGM
#define BGMGAMEOVER "BGM\\BGMGAMEOVER.wav" //�Q�[���I�[�o�[����BGM
#define ATTACK1 "SE\\PlayerAttack1.wav" //�v���C���[�̍U��SE�P
#define ATTACK2 "SE\\PlayerAttack2.wav" //�v���C���[�̍U��SE�Q
#define ATTACK3 "SE\\PlayerAttack3.wav" //�v���C���[�̍U��SE�R
#define ATTACKSP "SE\\PlayerAttacksp.wav" //�v���C���[�̃W�����v�U��SE
#define DAMAGE "SE\\Damage.wav"  //�v���C���[�_���[�W����SE
#define FRY "SE\\Beefly.wav"  //�G�R�̈ړ���SE
#define BOSSVOICE "SE\\BossVoice.wav"//�{�X�̖���
#define BOSSWORK "SE\\BossWork.wav" //�{�X�̑���
#define ENEMYVOICE "SE\\ScopionVoice.wav" //�G�Q�̖���
#define FONT "Resource\\png,tga\\FontG.png"  //�Q�[���Ɏg���Ă��镶���̃f�[�^
#define SCOPION "3DModel\\scorpid\\scorpid-monster-X-animated.X" //�G�Q��X�t�@�C��
#define BOSS "3DModel\\Boss\\monster-animated-character-X.X" // �{�X��X�t�@�C��
#define KNIGHT "3DModel\\knight\\knight_low.x"
#define TEXWIDTH  8192  //�e�N�X�`����
#define TEXHEIGHT  6144  //�e�N�X�`������

//CMatrix Matrix;
int CSceneGame::mEnemy2Count = 0;
int CSceneGame::mEnemy2CountStopper = ENEMY2COUNT;
int CSceneGame::mEnemy3Count = 0;
int CSceneGame::mEnemy3CountStopper = ENEMY3COUNT;
int CSceneGame::mBgmCount = 1;//BGM�̐؂�ւ��ԍ�
bool CSceneGame::mBgmCountCheck = true;//BGM�𗬂����~�߂邩������t���O
bool CSceneGame::mVoiceSwitch =false;//false�F�����Ȃ� true�F��������

CSound PlayerFirstAttack;
CSound PlayerSecondAttack;
CSound PlayerThirdAttack;
CSound PlayerJumpAttack;
CSound PlayerDamage;
CSound Enemy3Fry;
CSound BossVoice;
CSound BossMove;
CSound Enemy2Voice;
CSceneGame::CSceneGame() 
	:mTimeCount(0)
	,mTimeSecond(0)
	,mTimeMinute(0)
	,mSpawn(0)
    , mSpawn2(0)
	
	,mBgmCountCheck2(true)
	,mBgmStartStopper(true)
	,mBgmBattleStopper(true)
	,mBgmBossStopper(true)
    ,mBgmOverStopper(true)
    ,mBgmClearStopper(true)
{

}
CSceneGame::~CSceneGame() {
	Sleep(2000);

}
void ShadowRender() {
	//�e�̉e�����󂯂�p�ɂȂ�
	CTaskManager::Get()->Render();
}
void CSceneGame::Init()

{
	//�T�E���h(wav)�t�@�C���̓ǂݍ���
	mScene = EGAME;
	//BGM,SE�̓ǂݍ���
	PlayerFirstAttack.Load(ATTACK1);
	PlayerSecondAttack.Load(ATTACK2);
	PlayerThirdAttack.Load(ATTACK3);
	PlayerJumpAttack.Load(ATTACKSP);
	PlayerDamage.Load(DAMAGE);
	Enemy3Fry.Load(FRY);
	BossVoice.Load(BOSSVOICE);
	BossMove.Load(BOSSWORK);
	Enemy2Voice.Load(ENEMYVOICE);
	mBgmStart.Load(BGMSTART);
	mBgmBattle.Load(BGMBATTLE);
	mBgmBossBattle.Load(BGMBOSSBATTLE);
	mBgmGameClear.Load(BGMGAMECLEAR);
	mBgmGameOver.Load(BGMGAMEOVER);
	mImageMouse.Load(TEX_BUTTON3);
	mImageCkey.Load(TEX_BUTTON2);
	mImageWork.Load(TEX_WORK);
	mImageDush.Load(TEX_DUSH);
	mImageMoveKey.Load(TEX_BUTTON);
	if (mVoiceSwitch == true) {

	 mBgmStart.Repeat();
	}
	//�e�L�X�g�t�H���g�̓ǂݍ��݂Ɛݒ�
	mFont.LoadTexture(FONT, 1, 4096 / 64);

	CRes::sModelX.Load(MODEL_FILE);
	//�L�����N�^�[�Ƀ��f����ݒ�
	mPlayer.Init(&CRes::sModelX);
	mPlayer.mPosition = CVector(-63.0f, 1.0f, -150.0f);

	//mPlayer.mPosition = CVector(-56.0f, 5.0f, -49.0f);
	/*
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
	*/
	//�J����������
	Camera.Init();

	CRes::sScorp.Load(SCOPION);
	CRes::sScorp.SeparateAnimationSet(0, 0, 72, "walk");
	CRes::sScorp.SeparateAnimationSet(0, 72, 120, "strafe left");
	CRes::sScorp.SeparateAnimationSet(0, 120, 168, "strafe right");
	CRes::sScorp.SeparateAnimationSet(0, 168, 220, "attack");
	CRes::sScorp.SeparateAnimationSet(0, 292, 350, "attack2");
	CRes::sScorp.SeparateAnimationSet(0, 350, 440, "attack3");
	CRes::sScorp.SeparateAnimationSet(0, 220, 292, "death");
	CRes::sScorp.SeparateAnimationSet(0, 660, 760, "idle");
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
	CRes::sBoss.SeparateAnimationSet(0, 500, 550, "death - 02");
	CRes::sBoss.SeparateAnimationSet(0, 565, 650, "death - 03");
	//�V�������
	mpBoss = new CBoss(CVector(0.0f, 10.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(0.5f, 0.5f, 0.5f));
	//�ǂݍ��܂���
	mpBoss->Init(&CRes::sBoss);
	//�{�X�̔z�u
	mpBoss->mPosition = CVector(3.0f, 10.0f, 100.0f);

	new CItem(CVector(-20.0f, 2.0f, -10.0f),
		CVector(), CVector(1.5f, 1.5f, 1.5f));
	new CTarget(mPlayer.mPosition,
		CVector(), CVector(0.5f, 0.5f, 0.5f));
	mpEnemySummon = new CEnemySummon(CVector(-36.0f, -2.0f, -59.0f),
		CVector(), CVector(0.5f, 0.5f, 0.5f));

	mpEnemySummon2 = new CEnemySummon(CVector(6.0f, 8.0f, 14.0f),
		CVector(), CVector(0.5f, 0.5f, 0.5f));

	mpRock = new CRock(CVector(0.0f, 0.0f, -100.0f),
		CVector(0.0f, 180.0f, 0.0f), CVector(0.5f, 0.5f, 0.5f));

	mpTree = new CTree(CVector(0.0f, 0.0f, 0.0f),
		CVector(), CVector(10.5f, 10.5f, 10.5f));

	float shadowColor[] = { 0.4f, 0.4f, 0.4f, 0.2f };  //�e�̐F
	float lightPos[] = { 50.0f, 160.0f, 50.0f };  //�����̈ʒu
	mShadowMap.Init(TEXWIDTH, TEXHEIGHT, ShadowRender, shadowColor, lightPos);//�e�̏�����
	CEffect2::TexPreLoad();
}

void CSceneGame::BgmBattle() {

	if (mVoiceSwitch == true) {
		if (mBgmCountCheck == true) {
			if (mBgmBattleStopper == true) {
				mBgmBattle.Repeat();
				mBgmBattleStopper = false;
			}
		
		}

	}
}
void CSceneGame::BgmBoss() {

	if (mVoiceSwitch == true) {
		if (mBgmCountCheck == true) {
			if (mBgmBossStopper==true) {
				mBgmBossBattle.Repeat();
				mBgmBossStopper = false;
			}
				
		}
	}
}
void CSceneGame::BgmGameOver() {
	if (mVoiceSwitch == true) {
		if (mBgmCountCheck == true) {
			if (mBgmOverStopper == true) {
				mBgmGameOver.Repeat();
				mBgmOverStopper = false;
			}
		}
		
	}
}
void CSceneGame::BgmGameClear() {

	if (mVoiceSwitch == true) {
		if (mBgmCountCheck == true) {
			if (mBgmClearStopper == true) {
				mBgmGameClear.Repeat();
				mBgmClearStopper = false;

			}
		}
	}
}
void CSceneGame::Update() {
	
		switch (mBgmCount) {
			
		
		case 2:
			 BgmBattle();
			   break;
		case 3:
			 BgmBoss();
			break;
		case 4:
			 BgmGameClear();
			   break;
		case 5:
			 BgmGameOver();
            break;
		}
	
	mTimeCount++;
	if (mTimeCount % 60 == 0) {
		mTimeSecond++;
	}
	if (mTimeSecond==60) {
		mTimeMinute++;
		mTimeSecond = 0;
	}
	//�G�̐����Ԋu
	if (mSpawn >= 0) {
		mSpawn--;
	}
	//�G�̐����Ԋu
	if (mSpawn2 >= 0) {
		mSpawn2--;
	}
	if (mpEnemySummon->mHp > 0) {


		//mEnemy2CountStopper�ɐݒ肵���������G�𐶐�
		if (mEnemy2Count < mEnemy2CountStopper) {
			//�Q�b���Ƃɐ���
			if (mSpawn <= 0) {
				mpEnemy2 = new CEnemy2(mpEnemySummon->mPosition,
					CVector(), CVector(0.75f, 0.75f, 0.75f));
				mpEnemy2->Init(&CRes::sScorp);
				mEnemy2Count++;
				mSpawn = 60;
			}
		}
	}

	//�G�����̐�����܂ōĐ������Ȃ�
	 if( mEnemy2CountStopper<= ENEMY2MINCOUNT) {
		mEnemy2CountStopper = ENEMY2COUNT;
	}

	if (mpEnemySummon2->mHp > 0) {
		//mEnemy3CountStopper�ɐݒ肵���������G�𐶐�
		if (mEnemy3Count < mEnemy3CountStopper) {
			//�Q�b���Ƃɐ���
			if (mSpawn2 <= 0) {
				mpEnemy3 = new CEnemy3(mpEnemySummon2->mPosition, CVector(0.0f, 0.0f, 0.0f),
					CVector(1000.5f, 1000.5f, 1000.5f));
				mEnemy3Count++;
				mSpawn2 = 120;
			}
		}
	}

	//�G�����̐�����܂ōĐ������Ȃ�
	 if (mEnemy3CountStopper <= ENEMY3MINCOUNT) {
		mEnemy3CountStopper = ENEMY3COUNT;
	}
	//�G�X�P�[�v�L�[�ŏI��
	if (CKey::Push(VK_ESCAPE)) {
		exit(0);
	}
	if (mpBoss->mHp <= 0) {
			mpEnemy2->mHp = 0;
			mpEnemy3->mHp = 0;
	}
		if (mBgmCountCheck ==false) {
			mBgmStart.Stop();
			mBgmBattle.Stop();
			mBgmBossBattle.Stop();
			mBgmGameClear.Stop();
			mBgmGameOver.Stop();
			mBgmCountCheck = true;
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
	//CTaskManager::Get()->Render();
	Camera.CameraRender();//�J�����ݒ�
	mShadowMap.Render();//�e�ݒ�
	//�R���C�_�̕`��
	//�������R�����g�ɂ���Ƃ��ׂẴR���C�_��\��
	CCollisionManager::Get()->Render();
	//2D�`��J�n
	CUtil::Start2D(0, 800, 0, 600);
	char buf[64];
	if (CBoss::mHp>0||mPlayer.mHp<0) {
		if (mTimeMinute<10) {
			sprintf(buf, "0%d:", mTimeMinute);
			mFont.DrawString(buf, 170, 500, 8, 16);
		}
	else {
			sprintf(buf, "%d:", mTimeMinute);
			mFont.DrawString(buf, 170, 500, 8, 16);
	}
		if (mTimeSecond < 10) {
		  sprintf(buf, "0%d", mTimeSecond);
		 mFont.DrawString(buf, 230, 500, 8, 16);
		}
		else {
			sprintf(buf, "%d", mTimeSecond);
			mFont.DrawString(buf, 230, 500, 8, 16);
	    }
		sprintf(buf, "SPECIAL:%10d", CXPlayer::mSpAttack);
		//mFont.DrawString(buf, 20, 100, 8, 16);
		if (mPlayer.mPosition.mX > 0) {
		 sprintf(buf, ":%f", mPlayer.mPosition.mX);
		//mFont.DrawString(buf, 20, 200, 8, 16);
		}
		else {
			sprintf(buf, "X:%f", mPlayer.mPosition.mX);
			//mFont.DrawString(buf, 20, 200, 8, 16);
		}
		if (mPlayer.mPosition.mY > 0) {
		
		 sprintf(buf, "Y:%f", mPlayer.mPosition.mY);
		 //mFont.DrawString(buf, 20, 250, 8, 16);
		}
		else {
			sprintf(buf, "Y:-%f", mPlayer.mPosition.mY);
			//mFont.DrawString(buf, 20, 250, 8, 16);
		}
		if (mPlayer.mPosition.mZ > 0) {
			sprintf(buf, "Z:%f", mPlayer.mPosition.mZ);
			//mFont.DrawString(buf, 20, 300, 8, 16);
		}
		else {
			sprintf(buf, "Z:-%f", mPlayer.mPosition.mZ);
			//mFont.DrawString(buf, 20, 300, 8, 16);

		}
	}
	else if (CBoss::mHp <= 0) {
	sprintf(buf, "GAMECLEAR" );
	mFont.DrawString(buf, 20, 300, 16, 32);
	}
	if (mPlayer.mHp < 0) {
		sprintf(buf, "GAMEOVER");
		mFont.DrawString(buf, 20, 300, 16, 32);
	}
	mImageMouse.Draw(600, 800, 0, 200, 0, 500, 500, 0);
	mImageMoveKey.Draw(0, 150, 150, 300, 0, 500, 500, 0);
	mImageCkey.Draw(150, 300, 200, 300, 0, 500, 500, 0);
	mImageWork.Draw(0, 200, 0, 200, 0, 250, 250, 0);
	mImageDush.Draw(200, 300, 50, 190, 0, 200, 200, 0);
	//2D�̕`��I��
	CUtil::End2D();
	//CXPlayer�̃p�����[�^���̂QD�`��͈�ԍŌ�
	CXPlayer::GetInstance()->Render2D();
}

CScene::EScene CSceneGame::GetNextScene()
{
	return mScene;
}
