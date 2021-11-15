#ifndef CENEMY3_H
#define CENEMY3_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter.h"
//�R���C�_�N���X�̃C���N���[�h
#include"CCollider.h"
#include"CText.h"
#include"CBullet.h"
/*�G�l�~�[�N���X
�L�����N�^�N���X���p��	*/
class CEnemy3 :public CCharacter {
public:
	//���f���f�[�^
	static CModel mModel;
	//�R���C�_
	CCollider mCollider;
	CBullet* mpBullet;
	//�R���X�g���N�^	
	CEnemy3();
	CVector mPoint;//�ڕW�n�_
	int mHp;//�̗�
	int mJump;
	int mJump2;
	CText mText;
	static int mMoveCount;
	int mMove2;
	int mColliderCount;
	int mCount;
	CVector mCollisionEnemy;
	//CEnemy2(�ʒu�A��]�A�g�k�j
	CEnemy3(const CVector& position, const CVector& rotation, const CVector& scale);
	CCollider mColSearch;//�T�[�`�p�R���C�_
	CCollider mColSearch2;//�T�[�`�p�R���C�_
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_�P�A�R���C�_�Q�j
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	CCharacter* mpPlayer;//�v���C���[�̃|�C���^
	int mEnemyDamage;
};

#endif