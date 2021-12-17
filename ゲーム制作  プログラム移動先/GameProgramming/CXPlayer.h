#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"
#include"CVector.h"
#include"CEnemy2.h"
#include"CBoss.h"
class CXPlayer : public CXCharacter
{
public:
	//�R���C�_�̐錾
	CCollider mColSphereBody;	//��
	CCollider mColSphereHead;	//��
	CCollider mColSphereSword;	//��


	CCollider mCollider;
	
	CCollider  mColSphereFoot;//�_���[�W���ʂ铖���蔻��
	CCollider mCollider2;//�A�C�e�����E�������̕���̓����蔻��
    CVector mCollisionEnemy;//�v���C���[�ƓG�̃R���C�_�[�̈ʒu�̍�
	int mSpaceCount1;//���ڂ̍U���̑��t���[��
	int mSpaceCount2;//�Q��߂̍U���̑��t���[��
	int mSpaceCount3;//�R��߂̍U���̑��t���[��
	static int mStamina;//�X�^�~�i�̐��l
	int mDamageCount;//�_���[�W���󂯂�����̖��G����
	float mJump;//�W�����v���鎞�̏����x
	float mStep;//�U�����A�����O�i
	float mGravity;//�d��
	
    static int mSpAttack;//�W�����v�U�������邽�߂̃Q�[�W
	static int mAttackCount;//����̓����蔻�肪�K�p����鎞��
	float mColliderCount;//������΂��ꂽ���̏����x
	int mAnimationCount;//�A�j���[�V�������r���ŕς��Ȃ��悤�ɂ���
	int mHp;//�̗�
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
	
	
	
	/*
	������(X���f���N���X�̃|�C���^)
	*/
	void Init(CModelX* model);

	void Update();
	void Collision(CCollider* m, CCollider* o);
};

#endif
