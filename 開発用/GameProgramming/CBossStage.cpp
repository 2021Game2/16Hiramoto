#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CBossStage.h"
#include"CCollider.h"
#define OBJ "Resource\\3DModel\\BossStage\\BossStage.obj"
#define MTL "Resource\\3DModel\\BossStage\\BossStage.mtl"
CModel CBossStage::mModel;//���f���f�[�^�쐬
CBossStage::CBossStage(const CVector& position, const CVector& rotation, const CVector& scale)
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
	mTag = EBOSSSTAGE;
}
void CBossStage::Update() {
	//CCharacter�̍X�V
	CTransform::Update();
}

void CBossStage::Render() {
	//�e�̕`�揈��
	CCharacter::Render();
}