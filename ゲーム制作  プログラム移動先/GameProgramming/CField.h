#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CColliderMesh.h"
#ifndef CFIELD_H
#define CFIELD_H
class CField : public CCharacter {
private:

public:
	CColliderMesh mColliderMesh;//��̂RD���f���̃R���C�_
	//���f���f�[�^
	static CModel mModel;
	//�R���C�_
	CField(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	void TaskCollision();
	void Render();
};
#endif
