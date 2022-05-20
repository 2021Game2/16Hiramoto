#include "CCharacter.h"
#include "CTaskManager.h"

//�`�揈��
void CCharacter::Render()
{
	

	mpModel->Render(mMatrix);
	
}

CCharacter::~CCharacter() {
	//�^�X�N���X�g����폜
	CTaskManager::Get()->Remove(this);
}

CCharacter::CCharacter()
:mTag(EZERO)
{
	//�^�X�N���X�g�ɒǉ�
	CTaskManager::Get()->Add(this);
}
CCharacter::CCharacter(int Priority)
	: CTask(Priority), mTag(EZERO)
{
	//�^�X�N���X�g�ɒǉ�
	CTaskManager::Get()->Add(this);
}

