#include"CMap.h"
CMap::CMap() {
    mPosition = CVector(0.0f, 1.0f, 0.0f);//CTransform����
	mScale = CVector(1.0f, 1.0f, 1.0f);//CTransform����
	mModel.Load("sky.obj", "sky.mtl");

	
	
	CTransform::Update();//CCharacter::Render�̍X�V
	mpModel = &mModel;//�|�C���^�ǉ�
}