
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CField.h"
#include"CCollider.h"
#define OBJ "3DModel\\Field\\Field.obj"
#define MTL "3DModel\\Field\\Field.mtl"

CModel CField::mModel;//���f���f�[�^�쐬

CField::CField(const CVector& position, const CVector& rotation, const CVector& scale)

{

	mPosition = position;
	mRotation = rotation;
	mScale = scale;

	//�D��x��2�ɕύX����
	mPriority = 2;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
	CTransform::Update();//�s��̍X�V

   //���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
	mColliderMesh.Set(this, &mMatrix, &mModel);//���f�����R���C�_�ɂ���
	mTag = EFIELD;
}
void CField::Update() {
	//CCharacter�̍X�V
	CTransform::Update();
}

void CField::Render() {
	//�e�̕`�揈��
	CCharacter::Render();
}
void CField::TaskCollision() {

}
