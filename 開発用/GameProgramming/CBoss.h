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
#include"CColliderLine.h"
/*�G�l�~�[�N���X
�L�����N�^�N���X���p��	*/
class CBoss :public CXCharacter {
public:
//�G�̏��
	enum EBossState
	{
		EIDLE,		//�ҋ@
		EAUTOMOVE,	//�ړ�
		EATTACK,	//�U��
        EATTACK2,
		EATTACK3,
		EATTACK4,
		EDAMAGED,	//��e
		EDEATH,		//���S
		
	};
private:
    CText mText;
	CVector mPoint;//�ڕW�n�_
	CCollider mColSphereHead;
	CCollider mColSphereRightFront;
	CCollider mColSphereLeftFront;
	CCollider mColSphereAttack;
	CCollider mColSearch;//�T�[�`�p�R���C�_
	CColliderLine mColliderLine;
	CEffect2* mBossEffect;
	CVector mCollisionEnemy;
	CVector mJumpCount;
	CVector mMoveRecord;
	CCharacter* mpPlayer;//�v���C���[�̃|�C���^
	CText mFont;
	CTexture mImageGauge; //�Q�[�W�摜
	char buf[64];
	bool mBossBgm;
	bool mBossBgmDeath;
	bool mColSearchCount;
	bool mJumpStopper;
	bool mBossGaugeSwitch;
	bool mBossStopper;
	bool mBossAttackHit;
	int mHp;//�̗�
	int mMove;
	int mMove2;
	int mBossDamageCount;
	int mEnemy2StopCount;//�v���C���[��ESTOPPER�ɓ������Ă���ԑ���
    int mAttackPercent;
	int mEnemyDamage;
	int mBossJumpCount;
	int mBossColliderCheck;
	int mAttack4MoveCount;
	int mAttack4Count;
	int mAttack4directionCount;
	int mEffectCount;
	int mBossAttackMove;
	int mBossPositionLengthCount;//�{�X�̍s���͈͂��v��
	float mJump;
	float mJumpZ;
	float mColliderCount;
	float mTime;//�W�����v���鎞�̎��Ԃ��v��
	float mAttack4MoveX;
	float mAttack4MoveZ;
	float mAttack4RotationCount;
	float mAttackRotation;
	float mRotationCount;
	float mBossPositionLengthX;//�{�X�̍s���͈͂�X��
	float mBossPositionLengthZ;//�{�X�̍s���͈͂�Z��
	float mBossPositionLengthSum;//�{�X�̍s���͈͂̔��a
	float mBossCircle;//�{�X�̍s���͈�
public:

	EBossState mState;
	//���f���f�[�^
	static CModel mModel;
	static CBoss* GetInstance();
	static CBoss* mpBossInstance;
	bool GetBossAttackHit() {
		return mBossAttackHit;
	}
	int GetHp() {
		 return mHp;
	}
	//�R���X�g���N�^	
	CBoss();
	//CEnemy2(�ʒu�A��]�A�g�k�j
	CBoss(const CVector& position, const CVector& rotation, const CVector& scale);
	void Init(CModelX* model);
	//�X�V����
	void Update();
	void Render2D();
	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	void Idle();//�ҋ@����
	void AutoMove();//�ړ�����
	void Attack();//�U������1
    void Attack2();//�U������2
	void Attack3();//�U������3
	void Attack4();//�U������4
	void Damaged();//��e����
	void Death();//���S����
	
	
};
#endif