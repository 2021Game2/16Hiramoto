#ifndef CBILLBOARD_H
#define CBILLBOARD_H
#include "CCharacter.h"
/*
�r���{�[�h�N���X
��ɃJ�����̕��������l�p�`
*/
class CBillBoard2 : public CCharacter {
	CMatrix mMatrixModelView;
	CMatrix mCameraInverse;
public:
	//�O�p�`2��
	CTriangle mT[2];
	//�}�e���A��
	CMaterial mMaterial;
	//�R���X�g���N�^
	CBillBoard2();
	//CBillBoard(�ʒu, ��, ����)
	CBillBoard2(CVector pos, float w, float h);
	//�ʒu�Ƒ傫���̐ݒ�
	//Set(�ʒu, ��, ����)
	void Set(CVector pos, float w, float h);
	//�X�V
	void Update();
	//�`��
	void Render();
	void Render(CMaterial* mpMaterial);
};
#endif

