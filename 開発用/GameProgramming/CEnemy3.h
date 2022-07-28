#ifndef CENEMY3_H
#define CENEMY3_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CXCharacter.h"
//�R���C�_�N���X�̃C���N���[�h
#include"CCollider.h"
#include"CText.h"
#include"CBullet.h"
#include"CSound.h"
/*�G�l�~�[�N���X
�L�����N�^�N���X���p��	*/
class CEnemy3 :public CCharacter {
public:
	//�G�̏��
	enum EEnemy3State
	{
		EIDLE,		//�ҋ@
		EMOVE1,//�ړ��P
		EMOVE2,//�ړ��Q
		EMOVE3,//�ړ��R
		EMOVE4,//�ړ��S
		EMOVE5,//�ړ��T
		EDAMAGED,	//��e
		EDEATH,		//���S
	};
private:
	float mJump;
	int mJump2;
	int mMove2;
	int mColliderCount;
	int mCount;
	int mFireCount;
	int mEnemy3Fry;
	int mEnemyDamage;
    int mMoveCount;
	int mEffectCount;//�G�t�F�N�g��\�������鎞��
    int mHp;//�̗�
	int mStopCount;
	bool mDeathSwitch;
	CCollider mCollider;
	CBullet* mpBullet;
	CVector mPoint;//�ڕW�n�_
	CVector mCollisionEnemy;
	CText mText;
	CCharacter* mpPlayer;//�v���C���[�̃|�C���^
	CCollider mColSearch2;//�T�[�`�p�R���C�_
public:
	//���f���f�[�^
	static CModel mModel;
	//�R���X�g���N�^
	CEnemy3();
	CEnemy3(const CVector& position, const CVector& rotation, const CVector& scale);
	void Move1();
	void Move2();
	void Move3();
	void Move4();
	void Move5();
	void Death();
	void Idle();
	//�X�V����
	void Update();
	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	EEnemy3State mState;
};

#endif