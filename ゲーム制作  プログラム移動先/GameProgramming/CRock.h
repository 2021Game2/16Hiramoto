#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#ifndef CROCK_H
#define CROCK_H
class CRock : public CCharacter {
public:
	//���f���f�[�^
	static CModel mModel;
	//�R���C�_
	CCollider mCollider;
	CRock();
	//CEnemy2(�ʒu�A��]�A�g�k�j
	CRock(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_�P�A�R���C�_�Q�j
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	void Render();
};
#endif