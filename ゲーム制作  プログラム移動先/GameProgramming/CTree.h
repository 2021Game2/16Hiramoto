#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CColliderMesh.h"
#ifndef CTREE_H
#define CTREE_H
class CTree : public CCharacter {
private:

public:
	CColliderMesh mColliderMesh;//��̂RD���f���̃R���C�_
	//���f���f�[�^
	static CModel mModel;
	//�R���C�_
	CTree(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	void TaskCollision();
	void Render();
};
#endif
