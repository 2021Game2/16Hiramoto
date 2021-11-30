#include"CEnemy2.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CXCharacter.h"
#include"CUtil.h"
#include"CText.h"
#define OBJ "mini.obj"//���f���̃t�@�C��
#define MTL "mini.mtl"//���f���̃}�e���A���t�@�C��
#define HP 1
#define VELOCITY 0.2f //�}�N��

#define JUMP 4.0f
#define G 0.1f
int CEnemy2::mEnemy2AttackCount = 0;
CModel CEnemy2::mModel;//���f���f�[�^�쐬
//�f�t�H���g�R���X�g���N�^
CEnemy2::CEnemy2()
//�R���C�_�̐ݒ�
	:mCollider(this,&mMatrix,CVector(-0.5f,0.0f,-1.0f),1.0f)
	,mColSearch(this,&mMatrix,CVector(0.0f,0.0f,0.0f),200.0f)
	,mColSphereHead(this,&mMatrix,CVector(0.0f,0.5f,-1.0f),1.0f)
	, mColSphereRight(this, &mMatrix, CVector(1.5f, 3.0f, 0.5f), 1.0f)
	, mColSphereLeft(this, &mMatrix, CVector(-1.0f, 0.5f, 0.0f), 1.0f)
	
	,mpPlayer(0)
	,mHp(HP)
	,mJump(0)
	,mJump2(0)
	, mEnemyDamage(60)
	,mMove(0)
	,mMove2(0)
	, mColliderCount(0)
{
	
	
	mTag = EENEMY2;
	mColSearch.mTag = CCollider::ESEARCH;//�^�O�ݒ�
	//mCollider.mTag = CCollider::EENEMY2COLLIDER;
	mColSphereHead.mTag= CCollider::EENEMY2COLLIDER;
		mColSphereRight.mTag= CCollider::EENEMY2COLLIDERATTACK;
		mColSphereLeft.mTag= CCollider::EENEMY2COLLIDERATTACK;
}

//CEnemy(�ʒu�A��]�A�g�k�j
CEnemy2::CEnemy2(const CVector& position, const CVector& rotation, const CVector& scale)
:CEnemy2()
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position;//�ʒu�̐ݒ�
	mRotation = rotation;//��]�̐ݒ�
	mScale = scale;//�g�k�̐ݒ�
	CTransform::Update();//�s��̍X�V
	//�ڕW�n�_�̐ݒ�
	mPoint =mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}
//�X�V����
void CEnemy2::Update() {
	
	switch (mAnimationIndex) {
	case(4):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(5, false,60);
		}
		break;
	case(5):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(0, true, 60);
		}
		break;
	case(7):
		if (mAnimationFrame >= mAnimationFrameSize)
		{
			ChangeAnimation(0, true, 60);
		}
		break;
	}
	//if(mPosition.mY<=mpPlayer->mPosition.mY)
	//�������iX���j�̃x�N�g�������߂�
	CVector vx = CVector(1.0f, 0.0f, 0.0f)*mMatrixRotate;
	//������iY���j�̃x�N�g�������߂�
	CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//�O�����iZ���j�̃x�N�g�������߂�
	CVector vz = CVector(0.0f, 0.0f, 5.0f) * mMatrixRotate;
	//�ڕW�n�_�܂ł̃x�N�g�������߂�
	CVector vp = mPoint - mPosition;
	//���x�N�g���Ƃ̓��ς����߂�
	float dx = vp.Dot(vx);
	//��x�N�g���Ƃ̓��ς����߂�
	float dy = vp.Dot(vy);
	//�O�x�N�g���Ƃ̓��ς����߂�
	float dz = vp.Dot(vz);
	float margin = 0.1f;
	//���E�����։�]
	if (dx > margin) {
		mRotation.mY += 1.0f;//���։�]
	}
		else if (dx < -margin) {
		mRotation.mY -= 1.0f;//�E�։�]
        }
	
	CTransform::Update();//�s��X�V
	
		
        
			
		//����
		if (mEnemy2AttackCount <= 0) {
             mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
			// ChangeAnimation(8, true, 60);
			 ChangeAnimation(4, true, 60);
			 ChangeAnimation(5, true, 60);
		}
		
		
	int r = rand() % 60; //rand()�͐����̗�����Ԃ�
	          
    //%180�͂P�W�O�Ŋ������]������߂�
	if (r == 0) {
		if (mpPlayer) {
			mPoint = mpPlayer->mPosition;

		}
		else {
			mPoint = mPoint * CMatrix().RotateY(80);
		}
	}
	mpPlayer = 0;
	if (mHp <= 0) {
		mHp--;
		//15�t���[�����ƂɃG�t�F�N�g
		if (mHp % 15 == 0) {
            //�G�t�F�N�g����
			new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		CTransform::Update();
	}
	if (mHp <= -70) {
		mEnabled = false;
		CSceneGame::mEnemyCount -= 1;

	}
	if (mJump > 0) {

     //mPosition.mZ -= mJump;
	 mJump--;
	}
	//������ԁiX,Z��)
	if (mColliderCount > 0) {
		mColliderCount--;
		mPosition = mPosition + mCollisionEnemy * mColliderCount;
	}
	//������ԁiY���j
	if (mJump2 > 0) {
		mPosition.mY += mJump;
		mJump2--;
	}
	//�d��
	if (mPosition.mY > 1.0f) {
      mPosition.mY -= G;
      
	}
	//�P�Q�O�ɂȂ�܂ŏՓ˂���������
	if (mMove >= 120) {
		//�����Ă��鎞�Ԃ��������蔻��
		mEnemy2AttackCount = 60;
		ChangeAnimation(3, false, 60);
		mMove = 0;
	}
	//�������ł���ԉ�]
	if (mPosition.mY > 3.0f) {
     mRotation.mX += 20.0f;   
	} 
	//�����蔻�肪�K�p����鎞��
	if (mEnemy2AttackCount > 0) {
		mEnemy2AttackCount--;
	}
	CXCharacter::Update();
}

void CEnemy2::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	mCollider.mpMatrix = &mpCombinedMatrix[1];
	//��
	mColSphereHead.mpMatrix = &mpCombinedMatrix[10];
	mColSphereRight.mpMatrix = &mpCombinedMatrix[9];
	mColSphereLeft.mpMatrix = &mpCombinedMatrix[20];
	

}
//Collision(�R���C�_�P�C�R���C�_�Q�C�j
void CEnemy2::Collision(CCollider* m, CCollider* o) {
	//�������T�[�`�p�̂Ƃ�
	if (m->mTag == CCollider::ESEARCH) {
		//���肪�e�R���C�_�̂Ƃ�
		if (o->mType == CCollider::ESPHERE) {
			//���肪�v���C���[�̂Ƃ�
			if (o->mpParent->mTag == EPLAYER) {
				//�Փ˂��Ă���Ƃ�
				if (CCollider::Collision(m, o)) {
					//�v���C���[�̃|�C���^�ݒ�
					mpPlayer = o->mpParent;
				}
			}
		}
		return;
	}
	//�e�R���C�_�̂Ƃ�
	if (m->mType == CCollider::ESPHERE) {
		//EENEMY2COLLIDER�̎�
		if (m->mTag == CCollider::EENEMY2COLLIDER) {

			if (o->mType == CCollider::ESPHERE) {
				
				if (o->mpParent->mTag == EPLAYER) {
					//���肪����̂Ƃ�
					if (o->mTag == CCollider::ESWORD) {
						//�Փ˂��Ă���Ƃ�
						if (CCollider::Collision(m, o)) {
							if (CXPlayer::mAttackCount > 0) {
								mColliderCount = 5;
								mCollisionEnemy = mPosition - o->mpParent->mPosition;
								mCollisionEnemy.mY = 0;
								mCollisionEnemy = mCollisionEnemy.Normalize();

								mJump = JUMP;
								mJump2 = JUMP;
								mHp--;
							}
						}
					}
					//���肪ESTOPPER�̎�
					if (o->mTag == CCollider::ESTOPPER) {
						if (CCollider::Collision(m, o)) {
							mColliderCount = 2;
							mCollisionEnemy = mPosition - o->mpParent->mPosition;
							mCollisionEnemy.mY = 0;

							mCollisionEnemy = mCollisionEnemy.Normalize();
							mMove += 2;
							ChangeAnimation(0, true, 60);

						}
						
					}
				}
					
			}
		}
		if (o->mType == CCollider::ETRIANGLE) {
            CVector adjust;//�����l
			//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
			//adjust�A�A�A�����l
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{
				if (mPosition.mX + mPosition.mZ > 0) {
					//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
					mPosition = mPosition - adjust;
					if (mJump > 0) {
						mPosition = mPosition - adjust ;
					}
				}
				else {
					//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
					mPosition = mPosition + adjust;
					if (mJump > 0) {
						mPosition = mPosition + adjust;
					}
				}

				
			}
		}
		return;
	}
}
void CEnemy2::TaskCollision() {
    //�R���C�_�̗D��x�ύX
    mCollider.ChangePriority();
    mColSearch.ChangePriority();
	mColSphereHead.ChangePriority();
	mColSphereRight.ChangePriority();
	mColSphereLeft.ChangePriority();
	//�Փˏ��������s

	CCollisionManager::Get()->Collision(&mColSphereRight, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereLeft, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSphereHead, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
