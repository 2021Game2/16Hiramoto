#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H

#include "CTaskManager.h"
#include "CCollider.h"

#define COLLISIONRANGEFIELD 20 //�Փˏ����͈͂�藣��Ă���R���C�_�͏Փˏ������Ȃ�
#define COLLISIONRANGE 2
class CCollisionManager : public CTaskManager
{
	//�}�l�[�W���̃C���X�^���X
	static CCollisionManager* mpInstance;
public:
	//�C���X�^���X�̎擾
	static CCollisionManager* Get();
	//�Փˏ���
	void Collision();
	void Collision(CCollider *collider, int range);
};

#endif
