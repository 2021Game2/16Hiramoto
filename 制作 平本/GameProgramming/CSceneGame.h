#ifndef CSCENEGAME_H
#define CSCENEGAME_H
#include "CScene.h"
#include"CVector.h"
#include"CModel.h"
#include"CCharacter.h"
#include "CPlayer.h"
#include"CEnemy.h"
#include"CCollisionManager.h"
#include"CItem.h"
#include"CBillBoard.h"
#include"CCamera.h"
#include"CColliderTriangle.h"
#include"CColliderMesh.h"
#include"CSound.h"
#include"CEnemySummon.h"
#include"CTexture.h"
/*
�Q�[���̃V�[��
*/

class CSceneGame : public CScene {
public:
	
	CVector mEye; //�J�������_�̒ǉ�
	CModel mModel;//CModel�N���X�̃C���X�^���X�ϐ�
	CModel mBackGround;//�w�i���f��
	CMatrix mMatrixRotate;//��]�s��
	CMatrix mMatrix;//�����s��
	CMatrix mBackGroundMatrix;
	CPlayer mPlayer;
	CTexture Texture;
	CSound Bgm;
	CEnemySummon* mpEnemySummon;
	//���f������R���C�_����
      CColliderMesh mColliderMesh;
	  int mMapCount;
	  static int mEnemyCount;
	//�G���f���̃C���X�^���X�i�N���X�Ɠ������͎̂g���Ȃ��j
	CModel mModelC5;
	CModel mModelc5;
	CModel mModelI;
	CModel mModelE;
	//�O�p�R���C�_�̍쐬
	CColliderTriangle mColliderTriangle;
	CColliderTriangle mColliderTriangle2;

	int mSpawn;

	//�����������̃I�[�o�[���C�h

	void Init();
	
	//�X�V�����̃I�[�o�[���C�h
	void Update();
	void Render();
};

#endif