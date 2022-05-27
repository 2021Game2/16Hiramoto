#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CXPlayer.h"
#ifndef CITEM_H
#define CITEM_H

class CItem : public CCharacter {
private:
	//�R���C�_
	CCollider mCollider;
	CCollider mCollider2;
public:
	static bool mItemAttackHit;
	//���f���f�[�^
	static CModel mModel;
	static int mItemCount;
	CItem();
	//�e�ւ̃|�C���^
	CCharacter* mpParent;
	CCharacter* mpPlayer;//�v���C���[�̃|�C���^
	//CEnemy2(�ʒu�A��]�A�g�k�j
	CItem(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_�P�A�R���C�_�Q�j
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	
};
#endif
