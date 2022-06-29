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
private:

	
	int mJump;
	int mJump2;
	int mMove2;
	int mColliderCount;
	int mCount;
	int mFireCount;
	int mEnemy3Fry;
	int mEnemyDamage;
    int mMoveCount;
    int mHp;//�̗�
	CCollider mCollider;
	CBullet* mpBullet;
	CVector mPoint;//�ڕW�n�_
	CVector mCollisionEnemy;
	CText mText;
	CCharacter* mpPlayer;//�v���C���[�̃|�C���^

public:
	CCollider mColSearch2;//�T�[�`�p�R���C�_
	//���f���f�[�^
	static CModel mModel;
	//�R���X�g���N�^
	CEnemy3();
	CEnemy3(const CVector& position, const CVector& rotation, const CVector& scale);
	void AutoMove1();
	void AutoMove2();
	void Damage();
	void Death();
	void Attack();
	void Idle();
	//�X�V����
	void Update();
	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	//�G�̏��
	enum EEnemy3State
	{
		EIDLE,		//�ҋ@
		EAUTOMOVE1,	//�ړ�
		EAUTOMOVE2,
		EATTACK,	//�U��
		EDAMAGED,	//��e
		EDEATH,		//���S
	};
	EEnemy3State mState;
};

#endif