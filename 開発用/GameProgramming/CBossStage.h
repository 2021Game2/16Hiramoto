#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CColliderMesh.h"
#ifndef CBOSSSTAGE_H
#define CBOSSSTAGE_H
class CBossStage : public CCharacter {
private:

public:
	CColliderMesh mColliderMesh;//��̂RD���f���̃R���C�_
	//���f���f�[�^
	static CModel mModel;
	//�R���C�_
	CBossStage(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���

	void Render();
};
#endif
