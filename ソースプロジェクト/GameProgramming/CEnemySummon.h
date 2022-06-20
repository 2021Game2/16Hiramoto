#ifndef CENEMYSUMMON_H
#define CENEMYSUMMON_H



#include"CCharacter.h"
#include"CCollider.h"

class CEnemySummon:public CCharacter {
private:

	//�R���C�_
	CCollider mCollider;
	
	int mDamageCount;
	int mEffectCount;
public:
	int mHp;
	//���f���f�[�^
	static CModel mModel;
	CEnemySummon();
	//CEnemy2(�ʒu�A��]�A�g�k�j
	CEnemySummon(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_�P�A�R���C�_�Q�j
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	
};




#endif

