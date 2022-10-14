#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CColliderMesh.h"
#ifndef CFLAG_H
#define CFLAG_H
class CFlag : public CCharacter {
private:
	CCollider mColliderFlag;
public:
	
	//���f���f�[�^
	static CModel mModel;
	static CFlag* GetInstance();
	static CFlag* mpFlagInstance;
	//�R���C�_
	CFlag(const CVector& position, const CVector& rotation, const CVector& scale);
	CFlag();
	//�X�V����
	void Update();
	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
};
#endif
