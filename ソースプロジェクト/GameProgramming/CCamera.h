#ifndef CCAMERA_H
#define CCAMERA_H
#include "CVector.h"

#include"CCollisionManager.h"
#include "CColliderLine.h"
/*
�J�����N���X
*/
#define DEF_CAMERA_DIST 8.0f
#define DEF_CAMERA_HEAD_ADJUST 3.0f
class CCamera :public CCharacter{
public:

	

	CCamera();
	//���_
	CVector mEye;
	//�����_
	CVector mCenter;
	//�����
	CVector mUp;
	CColliderLine mColliderLine;
	CMatrix mMatrix;
	

	//�d�����邪�J��������p
	CVector mPos;		//�ʒu
	CVector mTarget;	//�^�[�Q�b�g
	float	mAngleX;	//�A���O��
	float	mAngleY;	//�A���O��
	float	mDist;	//����

	//�J�����̐ݒ�
	//Set(���_, �����_, �����)
	void Set(const CVector &eye, const CVector &center,
		const CVector &up);

	void SetTarget(const CVector& target);
	void Render();
	//�J����������
	void Init();
	//�J�����X�V����
	void Update();
	//�J�����K�p
	//void Render();
    void CameraRender();
	void TaskCollision();
	//static�Ń|�C���^�����
	static CCamera* mpCameraInstance;
	//static�ŏ��������
	static CCamera* GetInstance();
	void Collision(CCollider* m, CCollider* o);
	//void CollisionTriangleLine(CCollider* triangle, CCollider* line, CVector* adjust);
	//�x�N�g���擾
	CMatrix GetMat();

	bool mSkip;
	//�X�N���[�����W�ϊ�
	//�߂�l �\���͈�
	//pOut ���ʊi�[�p
	//pos 2D�ɕϊ����������[���h���W
	bool WorldToScreen(CVector* pOut, const CVector& pos);

};

//�J�����̊O���Q��
extern CCamera* Camera;
#endif
