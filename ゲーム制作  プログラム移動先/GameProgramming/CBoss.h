#ifndef CBOSS_H
#define CBOSS_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter.h"

#include "CXCharacter.h"
//�R���C�_�N���X�̃C���N���[�h
#include"CCollider.h"
#include"CText.h"
/*�G�l�~�[�N���X
�L�����N�^�N���X���p��	*/
class CBoss :public CXCharacter {
public:
	//���f���f�[�^
	static CModel mModel;
	//�R���C�_
	CCollider mCollider;
	//�R���X�g���N�^	
	CBoss();
	CVector mPoint;//�ڕW�n�_
	int mHp;//�̗�
	float mJump;
	int mJump2;
	CText mText;
	int mMove;
	int mMove2;
	float mColliderCount;
	 int mBossAttackCount;
	float mGravity;//�d��
	float mTime;//�W�����v���鎞�̎��Ԃ��v��
	int mEnemy2StopCount;//�v���C���[��ESTOPPER�ɓ������Ă���ԑ���
	void Init(CModelX* model);
	CCollider mColSphereHead;
	CCollider mColSphereRight;
	CCollider mColSphereLeft;
	int mAttackPercent;
	CVector mCollisionEnemy;
	//CEnemy2(�ʒu�A��]�A�g�k�j
	CBoss(const CVector& position, const CVector& rotation, const CVector& scale);
	CCollider mColSearch;//�T�[�`�p�R���C�_
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_�P�A�R���C�_�Q�j
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	CCharacter* mpPlayer;//�v���C���[�̃|�C���^
	int mEnemyDamage;

	void Idle();		//�ҋ@����
	void AutoMove();	//�ړ�����
	void Attack();	//�U������
	void Damaged();		//��e����
	void Death();		//���S����
	void Attack2();
	//�G�̏��
	enum EBossState
	{
		EIDLE,		//�ҋ@
		EAUTOMOVE,	//�ړ�
		EATTACK,	//�U��
		EDAMAGED,	//��e
		EDEATH,		//���S
		EATTACK2,
	};
	EBossState mState;
};
#endif