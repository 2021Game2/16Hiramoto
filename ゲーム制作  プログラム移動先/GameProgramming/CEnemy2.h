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
	CVector mPoint;//�ڕW�n�_
	
	float mJump;//�_���[�W���󂯂��Ƃ��ɔ�яオ��
	int mMove;//�ҋ@�[���U���Ɉڂ�܂ł̐�
	int mDamageCount;//���G����
	int mEffectCount;//�G�t�F�N�g��\�������鎞��
	float mColliderCount;//�q�b�g�o�b�N�������
	float mTime;//�W�����v���鎞�̎��Ԃ��v��
	int mEnemy2StopCount;//�v���C���[��ESTOPPER�ɓ������Ă���ԑ���
	CCollider mColSphereRight;//�E�n�T�~�̃R���C�_
	CCollider mColSphereLeft;//���n�T�~�̃R���C�_
	CCollider mColSphereBody;//�̂̃R���C�_
	CVector mCollisionEnemy;
	float mPlayerMarkingX;//�v���C���[�ƓG��X���W�̍�
	float mPlayerMarkingZ;//�v���C���[�ƓG��Z���W�̍�
	float mRotationCount;
	int mEnemyDamage;
	int mEnemyVoice;
	float CurveCount;
	CCharacter* mpPlayer;//�v���C���[�̃|�C���^
	bool mEnemy2Bgm;
public:
	unsigned int mEnemyLevel;
	float mEnemyHpPercent;
	bool mMoveCount;
	int mHp;//�̗�
	bool mEnemy2AttackHit;
	//���f���f�[�^
	static CModel mModel;
	//�R���X�g���N�^	
	CEnemy2();
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