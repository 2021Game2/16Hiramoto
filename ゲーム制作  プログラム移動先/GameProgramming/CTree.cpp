
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CTree.h"
#define OBJ "tree.obj"
#define MTL "tree.mtl"
CModel CTree::mModel;//���f���f�[�^�쐬
CTree::CTree(const CVector& position, const CVector& rotation, const CVector& scale)
	: CTree()
{

	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mCollider.mTag = CCollider::EROCKCOLLIDER;
	mTag = ETREE;
	CTransform::Update();//�s��̍X�V
	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}
CTree::CTree()
	: mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)

{
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
	mRotation.mX += 30.0f;
}
void CTree::Update() {

	//CCharacter�̍X�V
	CTransform::Update();
}
void CTree::Collision(CCollider* m, CCollider* o) {
	switch (m->mType) {
	case CCollider::ESPHERE:
		if (o->mType == CCollider::ESPHERE) {
			if (o->mpParent->mTag == EPLAYER) {

				//�Փ˂��Ă���Ƃ�
				if (CCollider::Collision(m, o)) {


				}

			}
		}
	}

}
void CTree::Render() {
	//�e�̕`�揈��
	CCharacter::Render();
}
void CTree::TaskCollision() {
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}