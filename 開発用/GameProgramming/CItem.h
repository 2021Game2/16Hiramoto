
#ifndef CITEM_H
#define CITEM_H
#include"CCollider.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CXPlayer.h"
class CItem : public CCharacter {
private:
    int mItemCount;
	//�R���C�_
	CCollider mCollider;
	CCollider mCollider2;
	CCharacter* mpParent;//�e�ւ̃|�C���^
	CCharacter* mpPlayer;//�v���C���[�̃|�C���^
public:
	void SetItemCount(int ItemCount) {
		mItemCount = ItemCount;
	}
	 bool mItemAttackHit;
	static CItem* mpItemInstance;
	//static�ŏ��������
	static CItem* GetInstance();
	//���f���f�[�^
	static CModel mModel;
	
	CItem();
	//CItem(�ʒu�A��]�A�g�k�j
	CItem(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	
};
#endif
