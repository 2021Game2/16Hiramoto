#ifndef CXPLAYER_H
#define CXPLAYER_H

#include"CEffect2.h"
#include "CXCharacter.h"
#include "CCollider.h"
#include"CColliderLine.h"
#include"CCollisionManager.h"
#include"CTaskManager.h"
#include"CVector.h"

#include"CTexture.h"
#include"CSound.h"
#include"CBoss.h"
#include"CItem.h"
class CXPlayer : public CXCharacter
{
public:  
	//�v���C���[�̏��
	enum EPLAYERState
	{
		EIDLE,		//�ҋ@
		EMOVE,	//�ړ�
		EDUSH,
		EESCAPE,
		EATTACK1,	//�U��1
		EATTACK2,
		EATTACK3,
		EATTACKSP,
		EDAMAGED,	//��e
		EDEATH,		//���S
	};

private:
	CVector mMoveAttack;
	CVector mMoveEscape;

	CTexture mImageGauge; //�Q�[�W�摜
	int mAnimationCount;//�A�j���[�V�������r���ŕς��Ȃ��悤�ɂ���
	int mDamageCount;//�_���[�W���󂯂�����̖��G����
	
	bool mPlayerBgm;//�Q�[���I�[�o�[����BGM�𗬂��t���O
	bool mAnimationFrameLock;
	bool mMoveCheck;//�_�b�V�����_�b�V������Ȃ���
	bool mSpaceCount1;//1��ڂ̍U���̃t���O
	bool mSpaceCount2;//�Q��ڂ̍U���̃t���O
	bool mSpaceCount3;//�R��ڂ̍U���̃t���O
	bool mEffectStopper;//�G�t�F�N�g��\�������邽�߂Ɉꎞ�I�ɃA�j���[�V�������~�߂�
	bool mJumpStopper;//SP�U���������̏d��
	float mSpeed;//�ړ����̃X�s�[�h
	float mColliderCount;//������΂��ꂽ���̏����x
	float mJump;//�W�����v���鎞�̏����x
	float mStep;//�U�����A�����O�i
	float mEscapeFlg;
	 int mStamina;//�X�^�~�i�̐��l
	 void SetStamina(int stamina) {
		 this->mStamina = stamina;
	 }
	 int GetStamina() {
		 return mStamina;
	 }
	 int mAttackCount;//�U���̃A�j���[�V���������x���Đ�����Ȃ��悤��
	 void SetAttackCount(int attackCount) {
		
		 this->mAttackCount = attackCount;
	 }
	 int GetAttackCount() {
		 return mAttackCount;
	 }
	CBoss* mpBoss;
	CEffect2* mEffect1;
	CEffect2* mEffect2;
	CEffect2* mEffect3;
	CEffect2* mEffectSp;
	CItem* mpItem;
	//�R���C�_�̐錾
	CCollider  mColSphereFoot;//�_���[�W���ʂ铖���蔻��
	CCollider mColliderSwordSp;//�W�����v�U�����̕���̓����蔻��
	CColliderLine mColEscapeStopperLine;
    CVector mCollisionEnemy;//�v���C���[�ƓG�̃R���C�_�[�̈ʒu�̍�
	CTexture mTexture;
	
public:
	bool mAttackSp;
	CCollider mColSphereSword;	//���R���C�_
	EPLAYERState mState;
	bool mGaugeEnabled;
	void SetGaugeEnabled(bool GaugeEnabled) {
		mGaugeEnabled = GaugeEnabled;
	}
    bool mAttackHit;
    int mSpAttack;//�W�����v�U�������邽�߂̃Q�[�W

	void SetSpAttack(int SpAttack) {
		
		this->mSpAttack = SpAttack;
	}
	int GetSpAttack() {
		return mSpAttack;
	}
    int mHp;//�̗�
	void SetHp(int Hp) {
		
		this->mHp = Hp;
	}
	int GetHp() {
		return mHp;
	}
	//�ʂ̃N���X�ŕϐ����Ăяo���ꍇ
	//static�Ń|�C���^�����
	static CXPlayer* mpPlayerInstance;
	//static�ŏ��������
	static CXPlayer* GetInstance();
	CVector GetSwordColPos();
	CXPlayer();
	void TaskCollision();
	/*
	������(X���f���N���X�̃|�C���^)
	*/
	void Init(CModelX* model);
	void Update();
	void SpAttackPoint();
	void Collision(CCollider* m, CCollider* o);
	void Render2D();
};

#endif
