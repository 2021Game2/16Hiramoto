#ifndef CCAMERA_H
#define CCAMERA_H
#include "CVector.h"
/*
�J�����N���X
*/
#define DEF_CAMERA_DIST 10.0f
#define DEF_CAMERA_HEAD_ADJUST 3.0f
class CCamera {
public:
	//���_
	CVector mEye;
	//�����_
	CVector mCenter;
	//�����
	CVector mUp;

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

	//�J����������
	void Init();
	//�J�����X�V����
	void Update();

	//�J�����K�p
	void Render();

	//�x�N�g���擾
	CMatrix GetMat();
};

//�J�����̊O���Q��
extern CCamera Camera;
#endif
