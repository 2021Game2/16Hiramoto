#include"CMap.h"
CMap::CMap() {
    mPosition = CVector(0.0f, 1.0f, 0.0f);//CTransformから
	mScale = CVector(1.0f, 1.0f, 1.0f);//CTransformから
	mModel.Load("sky.obj", "sky.mtl");

	
	
	CTransform::Update();//CCharacter::Renderの更新
	mpModel = &mModel;//ポインタ追加
}