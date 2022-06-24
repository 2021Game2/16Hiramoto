
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CFlag.h"
#include"CCollider.h"
#include"CSceneGame.h"
#include"CXPlayer.h"
#define OBJ "Resource\\3DModel\\Flag\\Flag.obj"
#define MTL "Resource\\3DModel\\Flag\\Flag.mtl"

CModel CFlag::mModel;//���f���f�[�^�쐬
CFlag* CFlag::mpFlagInstance;
//�t���O�̃|�C���^��Ԃ����ƂŁA���W�Ȃǂ��Q�Ƃł���悤�ɂȂ�
CFlag* CFlag::GetInstance()
{
	return mpFlagInstance;
}
CFlag::CFlag(const CVector& position, const CVector& rotation, const CVector& scale)
	:CFlag()
{

	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mpFlagInstance = this;
	//�D��x��2�ɕύX����
	mPriority = 2;
	mColliderFlag.mTag = CCollider::EFLAGCOLLIDER;
	mTag = EFLAG;
	CTransform::Update();//�s��̍X�V
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
	
	
}
CFlag::CFlag()
	:mColliderFlag(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 5.0f)
{
//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
}
void CFlag::Update() {
	//CCharacter�̍X�V
	CTransform::Update();
}
void CFlag::Collision(CCollider* m, CCollider* o) {
	
	if (m->mType == CCollider::ESPHERE) {
	   if (m->mpParent->mTag == EFLAG) {
		  if (m->mTag == CCollider::EFLAGCOLLIDER) {
			 if (o->mType == CCollider::ESPHERE) {
				if (o->mpParent->mTag == EPLAYER) {
					if (o->mTag == CCollider::EPLAYERBODY) {
						if (CCollider::Collision(m, o)) {
							CSceneGame* tSceneGame = CSceneGame::GetInstance();
							tSceneGame->mBossSwitch = true;
							tSceneGame->mBossGaugeSwitch = true;
							mEnabled = false;

						}
					}
				}
			 } 
		  }
	   }

	}

}
void CFlag::TaskCollision() {
	//�R���C�_�̗D��x�ύX
	mColliderFlag.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mColliderFlag, COLLISIONRANGE);
}
