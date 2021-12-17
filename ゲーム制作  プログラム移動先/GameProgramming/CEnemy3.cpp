#include"CEnemy3.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CBullet.h"
#include"CSceneGame.h"
#include"CUtil.h"
#include"CText.h"
#include"CXCharacter.h"
#define OBJ "3DModel\\Bee\\BEE1.obj"//���f���̃t�@�C��
#define MTL "3DModel\\Bee\\BEE1.mtl"//���f���̃}�e���A���t�@�C��
#define HP 1
#define VELOCITY 0.05f //�s���P
#define VELOCITY1  -0.1f//�s���P

#define VELOCITY2 0.1f

#define VELOCITY3 0.2f
#define JUMP 4.0f
#define G 0.1f
int CEnemy3::mMoveCount = 0;
CModel CEnemy3::mModel;//���f���f�[�^�쐬
//�f�t�H���g�R���X�g���N�^
CEnemy3::CEnemy3()
//�R���C�_�̐ݒ�
	:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 20.0f)
	, mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 70.0f)
	, mColSearch2(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f),35.0f)
	, mpPlayer(0)
	, mHp(HP)
	, mJump(0)
	, mJump2(0)
	, mEnemyDamage(60)
	
	, mMove2(0)
	, mColliderCount(0)
	,mCount(0)
	,mFireCount(0)
{
	mRotation.mY += 90.0f;
	mTag = EENEMY3;
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	mTag = EENEMY2;
    mCollider.mTag = CCollider::EENEMY3COLLIDER;
	mColSearch.mTag = CCollider::ESEARCH;//�^�O�ݒ�
	mColSearch2.mTag = CCollider::ESEARCH2;//�^�O�ݒ�
}

//CEnemy(�ʒu�A��]�A�g�k�j
CEnemy3::CEnemy3(const CVector& position, const CVector& rotation, const CVector& scale)
	:CEnemy3()
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position;//�ʒu�̐ݒ�
	mRotation = rotation;//��]�̐ݒ�
	mScale = scale;//�g�k�̐ݒ�
	CTransform::Update();//�s��̍X�V
	//�ڕW�n�_�̐ݒ�
	//mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
	//�D��x���P�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//�폜����
	CTaskManager::Get()->Add(this);//�ǉ�����
}
//�X�V����
void CEnemy3::Update() {
	//if(mPosition.mY<=mpPlayer->mPosition.mY)
	//�������iX���j�̃x�N�g�������߂�
	CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
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
	switch (mMoveCount) {
		//���V
	case(0):
       //�O����U�O�t���[��
		 if (mCount < 60) {
			
			if (mCount >= 0) {
				mPosition.mY -= 0.1f;
			}
		}
		//�U�O����P�Q�O�t���[��
		if (mCount < 120) {
			if (mCount >= 60) {
				mPosition.mY += 0.1f;
			}
		}
	    if (mCount >= 120) {
			mCount = 0;
	    }
		break;
		//�ړ��i�܂������ړ��j
	case(1):
		if (mCount < 180) {
			mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
			if (mPosition.mY > 3.0f) {
				mPosition = mPosition + CVector(0.0f,-0.1f , VELOCITY) * mMatrixRotate;
				//mPosition.mY -= 0.1f;
			}
		}
		if (mCount >= 180) {
			mMoveCount = 2;
			mCount = 0;
			
		}
		break;
		//�ړ��Q�i�E���Ɉړ��j
	case(2):
		if (mCount <= 10) {

			//mPosition.mX += 3.0f;
			//mPosition.mZ -= 1.5f;
			//mPosition.mY += 2.0f;
			mPosition = mPosition + CVector(3.0f, 2.0f,-1.5f) * mMatrixRotate;
		}
		if (mCount >= 10) {
            mMoveCount = 3;
			mCount = 0;
			
		}
		break;
		//�ړ��R�i���Ɉړ��j
	case(3):
		if (mCount <= 10) {
			//mPosition.mX -= 6.0f;
			//mPosition.mY -= 1.0f;
			mPosition = mPosition + CVector(-6.0f, -1.0f, 0.0f) * mMatrixRotate;
		}
		if (mCount >= 10) {
            mMoveCount = 4;
			mCount = 0;
			
		}
		break;
		//�ړ��S�i�E�O�Ɉړ��i���̈ʒu�ɖ߂�j
	case(4):
		if (mCount <= 30) {
			//mPosition.mX += 1.5f;
			//mPosition.mZ += 0.5f;
           // mPosition.mY += 0.1f;
			
			
			mPosition = mPosition + CVector(1.5f,VELOCITY ,0.5f ) * mMatrixRotate;
		}
		if (mCount >= 30) {
           mMoveCount = 0;
			mCount = 0;
			
		}
		break;
	}
	mCount++;
		

	int r = rand() % 60; //rand()�͐����̗�����Ԃ�

	//%180�͂P�W�O�Ŋ������]������߂�
	if (r == 0) {
		if (mpPlayer) {
			mPoint = mpPlayer->mPosition;

		}
		else {
			mPoint = mPoint * CMatrix().RotateY(80);
			//mPoint = mPoint * CMatrix().RotateX(80);
		}
	}
	mpPlayer = 0;
	if (mHp <= 0) {

		//�������(X�������j
		if (mColliderCount > 0) {
			mColliderCount--;
			mPosition = mPosition + mCollisionEnemy * mColliderCount;
		}
		//������ԁiY�������j
		if (mJump2 > 0) {
			mPosition.mY += mJump;
			mJump2--;
		}
		
		//��]
		if (mPosition.mY > 3.0f) {
			mRotation.mX += 20.0f;
		}
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
		mJump--;
	}
	if (mFireCount > 0) {
		mFireCount--;
	}
	//CTransform::Update();

	//CMatrix mMatrixCol;
	//CModel cube;
	//cube.Load("cube.obj", "cube.mtl");
	//CColliderMesh.Set(this, &mMatrix, mpModel);
	//mMatrixCol = CMatrix().Scale(10.0f,10.0f,10.0f)* mMatrix;
	//ColliderMesh.Set(this, &mMatrixCol, &cube);

}
//Collision(�R���C�_�P�C�R���C�_�Q�C�j
void CEnemy3::Collision(CCollider* m, CCollider* o) {
	//�������T�[�`�p�̂Ƃ�
	if (m->mTag == CCollider::ESEARCH) {
		//���肪�e�R���C�_�̂Ƃ�
		if (o->mType == CCollider::ESPHERE) {
			//���肪�v���C���[�̂Ƃ��A
			if (o->mpParent->mTag == EPLAYER) {
				//�Փ˂��Ă���Ƃ�
				if (CCollider::Collision(m, o)) {
					//�v���C���[�̃|�C���^�ݒ�
					mpPlayer = o->mpParent;
					if (mMoveCount<= 0) {
                          mMoveCount = 1;
						
                     
					}
					
				}
			}
		}
		return;
	}
	if (m->mTag == CCollider::ESEARCH2) {
		//���肪�e�R���C�_�̂Ƃ�
		if (o->mType == CCollider::ESPHERE) {
			//���肪�v���C���[�̂Ƃ��A
			if (o->mpParent->mTag == EPLAYER) {
				//�Փ˂��Ă���Ƃ�
				if (CCollider::Collision(m, o)) {
					//�v���C���[�̃|�C���^�ݒ�
					/*mpPlayer = o->mpParent;
					mpBullet = new CBullet;
					mpBullet->mPosition = mPosition;
					mpBullet->mScale = CVector(2.5f, 2.5f, 2.5f);*/
					mFireCount = 60;
					CBullet* bullet = new CBullet();
					bullet->Set(0.1f, 1.5f);
				bullet->mPosition = CVector(0.0f, 0.0f, 10.0f) * mMatrix;
					bullet->mRotation = mRotation;
					bullet->Update();
				}
				
			}
		}
		return;
	}
	if (m->mType == CCollider::ESPHERE) {
		if (o->mType == CCollider::ESPHERE) {
			//���肪����̂Ƃ��A
			if (o->mpParent->mTag == EPLAYER) {
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
						mPosition = mPosition - adjust;
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
void CEnemy3::TaskCollision() {
	mColSearch.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	mColSearch2.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mColSearch2, COLLISIONRANGE);
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
