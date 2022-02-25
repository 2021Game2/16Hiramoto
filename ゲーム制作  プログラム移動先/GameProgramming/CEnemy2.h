#ifndef CENEMY2_H
#define CENEMY2_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter.h"
#include"CXPlayer.h"
#include "CXCharacter.h"
//�R���C�_�N���X�̃C���N���[�h
#include"CCollider.h"
#include"CText.h"

#include"CSound.h"
/*�G�l�~�[�N���X
�L�����N�^�N���X���p��	*/
class CEnemy2 :public CXCharacter {
private:

	//�R���C�_
	CCollider mCollider;

	CVector mPoint;//�ڕW�n�_
	int mHp;//�̗�
	float mJump;
	int mJump2;
	CText mText;
	int mMove;
	int mMove2;
	int mDamageCount;

	float mColliderCount;

	float mGravity;//�d��
	float mTime;//�W�����v���鎞�̎��Ԃ��v��
	int mEnemy2StopCount;//�v���C���[��ESTOPPER�ɓ������Ă���ԑ���

	CCollider mColSphereHead;
	CCollider mColSphereRight;
	CCollider mColSphereLeft;
	CVector mCollisionEnemy;

	CCollider mColSearch;//�T�[�`�p�R���C�_

	float mPlayerMarkingX;//�v���C���[�ƓG��X���W�̍�
	float mPlayerMarkingZ;//�v���C���[�ƓG��Z���W�̍�
	float mRotationCount;
	int mEnemyDamage;
	int mEnemyVoice;
	float CurveCount;
	CCharacter* mpPlayer;//�v���C���[�̃|�C���^
	//CXPlayer* mpPointPlayer;
public:
	//���f���f�[�^
	static CModel mModel;
	//�R���X�g���N�^	
	CEnemy2();
	static int mEnemy2AttackCount;
	void Init(CModelX* model);
	
	//CEnemy2(�ʒu�A��]�A�g�k�j
	CEnemy2(const CVector& position, const CVector& rotation, const CVector& scale);
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
	//�G�̏��
	enum EEnemy2State
	{
		EIDLE,		//�ҋ@
		EAUTOMOVE,	//�ړ�
		EATTACK,	//�U��
		EDAMAGED,	//��e
		EDEATH,		//���S
	};
	EEnemy2State mState;
};
#endif