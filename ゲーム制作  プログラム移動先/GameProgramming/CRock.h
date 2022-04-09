#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CColliderMesh.h"
#ifndef CROCK_H
#define CROCK_H
class CRock : public CCharacter {
private:

public:
	CColliderMesh mColliderMesh;
	//���f���f�[�^
	static CModel mModel;
	//�R���C�_
	CRock(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	void TaskCollision();
	void Render();
};
#endif