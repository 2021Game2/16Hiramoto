
#include"CModel.h"
#include"CCharacter.h"
#include"CXPlayer.h"
#ifndef CTARGET_H
#define CTARGET_H

class CTarget : public CCharacter {
private:

	
public:

	CVector mPoint;
	static bool mItemAttackHit;
	//���f���f�[�^
	static CModel mModel;
	CTarget();

	//CEnemy2(�ʒu�A��]�A�g�k�j
	CTarget(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();

};
#endif
