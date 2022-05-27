#ifndef CBOSS_H
#define CBOSS_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter.h"

#include "CXCharacter.h"
//�R���C�_�N���X�̃C���N���[�h
#include"CCollider.h"
#include"CText.h"
#include"CEffect2.h"
#include"CSound.h"
/*�G�l�~�[�N���X
�L�����N�^�N���X���p��	*/
class CBoss :public CXCharacter {
private:
    CText mText;
	CVector mPoint;//�ڕW�n�_
	CCollider mColSphereHead;
	CCollider mColSphereRightFront;
	CCollider mColSphereLeftFront;
	CEffect2* mBossEffect;
	CVector mCollisionEnemy;
	CCharacter* mpPlayer;//�v���C���[�̃|�C���^
	int mJump2;
	int mMove;
	int mMove2;
	int mBossDamageCount;
	int mEnemy2StopCount;//�v���C���[��ESTOPPER�ɓ������Ă���ԑ���
    int mAttackPercent;
	int mEnemyDamage;
	float mColliderCount;
	float mGravity;//�d��
	float mTime;//�W�����v���鎞�̎��Ԃ��v��
	bool mBossBgm;
	bool mBossBgmDeath;
public:
	CCollider mColSearch;//�T�[�`�p�R���C�_
	bool mColSearchCount;
	bool mBossAttackHit;
	//���f���f�[�^
	static CModel mModel;
	static CBoss* GetInstance();
	static CBoss* mpBossInstance;
	static int mBossAttackCount;
	static int mHp;//�̗�
	int mEffectCount;
	//�R���X�g���N�^	
	CBoss();
	void Init(CModelX* model);
	//CEnemy2(�ʒu�A��]�A�g�k�j
	CBoss(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_�P�A�R���C�_�Q�j
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
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