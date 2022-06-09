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
#include"CTexture.h"
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
	CText mFont;
	char buf[64];
	float mJump;
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
	int  mBossColliderCheck;

	CTexture mImageGauge; //�Q�[�W�摜
public:
	CCollider mColSearch;//�T�[�`�p�R���C�_
	bool mColSearchCount;
	bool mBossAttackHit;
	bool mJumpStopper;
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
	void Render2D();
	//�Փˏ���
	//Collision(�R���C�_�P�A�R���C�_�Q�j
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	void Idle();		//�ҋ@����
	void AutoMove();	//�ړ�����
	void Attack();	//�U������
    void Attack2();
	void Attack3();
	void Damaged();		//��e����
	void Death();		//���S����
	
	//�G�̏��
	enum EBossState
	{
		EIDLE,		//�ҋ@
		EAUTOMOVE,	//�ړ�
		EATTACK,	//�U��
        EATTACK2,
		EATTACK3,
		EDAMAGED,	//��e
		EDEATH,		//���S
		
	};
	EBossState mState;
};
#endif