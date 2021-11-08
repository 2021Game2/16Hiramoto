#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#ifndef CTREE_H
#define CTREE_H
class CTree : public CCharacter {
public:
	//���f���f�[�^
	static CModel mModel;
	//�R���C�_
	CCollider mCollider;
	CTree();
	//CEnemy2(�ʒu�A��]�A�g�k�j
	CTree(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_�P�A�R���C�_�Q�j
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	void Render();
};
#endif
