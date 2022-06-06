
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CTree.h"
#include"CCollider.h"
#define OBJ "Resource\\3DModel\\Tree\\Tree.obj"
#define MTL "Resource\\3DModel\\Tree\\Tree.mtl"

CModel CTree::mModel;//���f���f�[�^�쐬

CTree::CTree(const CVector& position, const CVector& rotation, const CVector& scale)

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
	mTag = ETREE;
}
void CTree::Update() {
	//CCharacter�̍X�V
	CTransform::Update();
}

void CTree::Render() {
	//�e�̕`�揈��
	CCharacter::Render();
}
void CTree::TaskCollision() {

}
