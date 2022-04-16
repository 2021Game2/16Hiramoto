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

	CCollider mCollider;
	CBullet* mpBullet;
	CVector mPoint;//�ڕW�n�_
	int mHp;//�̗�
	int mJump;
	int mJump2;
	CText mText;
	int mMove2;
	int mColliderCount;
	int mCount;
	int mFireCount;
	int mEnemy3Fry;
	CVector mCollisionEnemy;
	CCollider mColSearch;//�T�[�`�p�R���C�_
	CCollider mColSearch2;//�T�[�`�p�R���C�_

	int mEnemyDamage;

	CCharacter* mpPlayer;//�v���C���[�̃|�C���^
public:
	//���f���f�[�^
	static CModel mModel;
	//�R���C�_
	//�R���X�g���N�^	
	CEnemy3();
	void AutoMove1();
	void AutoMove2();
	void Damage();
	void Death();
	void Attack();
	void Idle();
	static int mMoveCount;
	//CEnemy2(�ʒu�A��]�A�g�k�j
	CEnemy3(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_�P�A�R���C�_�Q�j
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