#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"
#include"CColliderLine.h"
#include"CCollisionManager.h"
#include"CTaskManager.h"
#include"CVector.h"

#include"CTexture.h"
#include"CSound.h"
class CXPlayer : public CXCharacter
{
private:

	CTexture mImageGauge; //�Q�[�W�摜
public:

	 bool mAttackHit;
	//�R���C�_�̐錾
	CCollider mColSphereSword;	//��
	CCollider  mColSphereFoot;//�_���[�W���ʂ铖���蔻��
	CCollider mCollider2;//�A�C�e�����E�������̕���̓����蔻��
    CVector mCollisionEnemy;//�v���C���[�ƓG�̃R���C�_�[�̈ʒu�̍�

	CTexture mTexture;
	int mSpaceCount1;//���ڂ̍U���̑��t���[��
	int mSpaceCount2;//�Q��߂̍U���̑��t���[��
	int mSpaceCount3;//�R��߂̍U���̑��t���[��
	static int mStamina;//�X�^�~�i�̐��l
	int mDamageCount;//�_���[�W���󂯂�����̖��G����
	float mJump;//�W�����v���鎞�̏����x
	float mStep;//�U�����A�����O�i
	float mGravity;//�d��
	//�ʂ̃N���X�ŕϐ����Ăяo���ꍇ
	//static�Ń|�C���^�����
	static CXPlayer* mpPlayerInstance;
	//static�ŏ��������
	static CXPlayer* GetInstance();
	
    static int mSpAttack;//�W�����v�U�������邽�߂̃Q�[�W
	static int mAttackCount;//����̓����蔻�肪�K�p����鎞��
	float mColliderCount;//������΂��ꂽ���̏����x
	int mAnimationCount;//�A�j���[�V�������r���ŕς��Ȃ��悤�ɂ���
	static int mHp;//�̗�
	float mTime;//�W�����v���鎞�̎��Ԃ��v��
	float mSpeed;
	
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
