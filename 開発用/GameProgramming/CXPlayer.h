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

class CXPlayer : public CXCharacter
{
private:
	CTexture mImageGauge; //�Q�[�W�摜
	int mAnimationCount;//�A�j���[�V�������r���ŕς��Ȃ��悤�ɂ���
	void SetAnimationCount(int v);
	int GetAnimationCount();
	int mDamageCount;//�_���[�W���󂯂�����̖��G����
	void SetDamageCount(int v);
	int GetDamageCount();
	bool mPlayerBgm;
	bool mAnimationFrameLock;
	bool mMoveCheck;//�_�b�V�����_�b�V������Ȃ���
	bool mSpaceCount1;//1��ڂ̍U���̃t���O
	bool mSpaceCount2;//�Q��ڂ̍U���̃t���O
	bool mSpaceCount3;//�R��ڂ̍U���̃t���O
	bool mEffectStopper;
	bool mJumpStopper;
	float mSpeed;
	float mColliderCount;//������΂��ꂽ���̏����x
	float mJump;//�W�����v���鎞�̏����x
	float mStep;//�U�����A�����O�i
	static int mStamina;//�X�^�~�i�̐��l
	void SetStamina(int v);
	int GetStamina();
	
	static int mAttackCount;//�U���̃A�j���[�V���������x���Đ�����Ȃ��悤��
	CBoss* mpBoss;
	CEffect2* mEffect1;
	CEffect2* mEffect2;
	CEffect2* mEffect3;
	CEffect2* mEffectSp;
	//�R���C�_�̐錾
	CCollider mColSphereSword;	//��
	CCollider  mColSphereFoot;//�_���[�W���ʂ铖���蔻��
	CCollider mColliderSwordSp;//�W�����v�U�����̕���̓����蔻��
	CColliderLine mColEscapeStopperLine;
    CVector mCollisionEnemy;//�v���C���[�ƓG�̃R���C�_�[�̈ʒu�̍�
	CTexture mTexture;
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
	EPLAYERState mState;
public:
	bool mGaugeEnabled;
    bool mAttackHit;
	static int mSpAttack;//�W�����v�U�������邽�߂̃Q�[�W
	static int mHp;//�̗�
	//�ʂ̃N���X�ŕϐ����Ăяo���ꍇ
	//static�Ń|�C���^�����
	static CXPlayer* mpPlayerInstance;
	//static�ŏ��������
	static CXPlayer* GetInstance();
	CXPlayer();
	void TaskCollision();
	/*
	������(X���f���N���X�̃|�C���^)
	*/
	void Init(CModelX* model);
	void Update();
	void Collision(CCollider* m, CCollider* o);
	void Render2D();
};
void CXPlayer::SetAnimationCount(int v)
{
	if (v < 0) return;//< �����Ƀu���[�N�|�C���g��u���ΒN���Ɛl�Ȃ̂��킩��
	this->mAnimationCount = v;
}
int CXPlayer::GetAnimationCount()
{
	return this->mAnimationCount;
}
void CXPlayer::SetDamageCount(int v) {
	if (v < 0) return;//< �����Ƀu���[�N�|�C���g��u���ΒN���Ɛl�Ȃ̂��킩��
	this->mDamageCount = v;
}

int CXPlayer::GetDamageCount()
{
	return this->mDamageCount;
}
void CXPlayer::SetStamina(int v) {
	if (v < 0) return;//< �����Ƀu���[�N�|�C���g��u���ΒN���Ɛl�Ȃ̂��킩��
	this->mStamina = v;
}
int CXPlayer::GetStamina()
{
	return this->mStamina;
}
#endif
