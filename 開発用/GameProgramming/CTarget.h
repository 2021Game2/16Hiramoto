
#include"CModel.h"
#include"CCharacter.h"
#include"CXPlayer.h"
#ifndef CTARGET_H
#define CTARGET_H

class CTarget : public CCharacter {
private:

	CVector mPoint;
public:
	//���f���f�[�^
	static CModel mModel;
	CTarget();
	//CTarget(�ʒu�A��]�A�g�k�j
	CTarget(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();

};
#endif
