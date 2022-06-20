#ifndef CBILLBOARD2_H
#define CBILLBOARD2_H
#include "CCharacter.h"
/*
�r���{�[�h�N���X
��ɃJ�����̕��������l�p�`
*/
class CBillBoard2 : public CCharacter {
public:
	//�O�p�`2��
	CTriangle mT[2];
	//�}�e���A��
	CMaterial mMaterial;
	//�g�k
	float mBillBoardScale;
	//UV
	CVector mUv[4];
	//�@��
	CVector mN[4];
	//�R���X�g���N�^
	CBillBoard2();
	//CBillBoard(�ʒu, �X�P�[��, ��])
	CBillBoard2(CVector pos, float scale, float rotate);
	//�ʒu�Ƒ傫���̐ݒ�
	//Set(�ʒu, �X�P�[��, ��])
	void Set(CVector pos, float scale, float rotate);
	//�X�V
	void Update();
	//�`��
	void Render();
	void Render(CMaterial *mpMaterial);
};
#endif
