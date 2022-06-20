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
	int mDamageCount;//�_���[�W���󂯂�����̖��G����
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

public:
	bool mGaugeEnabled;
    bool mAttackHit;
	static int mStamina;//�X�^�~�i�̐��l
	static int mSpAttack;//�W�����v�U�������邽�߂̃Q�[�W
	static int mAttackCount;//�U���̃A�j���[�V���������x���Đ�����Ȃ��悤��
	static int mHp;//�̗�
	CBoss* mpBoss;
	CEffect2* mEffect1;
	CEffect2* mEffect2;
	CEffect2* mEffect3;
	CEffect2* mEffectSp;
	CVector mPositionZ;
	
	//�R���C�_�̐錾
	CCollider mColSphereSword;	//��
	CCollider  mColSphereFoot;//�_���[�W���ʂ铖���蔻��
	CCollider mColliderSwordSp;//�W�����v�U�����̕���̓����蔻��
	
	CColliderLine mColEscapeStopperLine;
    CVector mCollisionEnemy;//�v���C���[�ƓG�̃R���C�_�[�̈ʒu�̍�

	CTexture mTexture;
	
	//�ʂ̃N���X�ŕϐ����Ăяo���ꍇ
	//static�Ń|�C���^�����
	static CXPlayer* mpPlayerInstance;
	//static�ŏ��������
	static CXPlayer* GetInstance();
	
	//�G�̏��
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
#endif
