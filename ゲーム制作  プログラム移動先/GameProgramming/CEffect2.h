#ifndef CEFFECT2_H
#define CEFFECT2_H
//�r���{�[�h�N���X�̃C���N���[�h
#include "CBillBoard2.h"
/*
�G�t�F�N�g�N���X
�e�N�X�`���̃A�j���[�V����
*/
class CEffect2 : public CBillBoard2 {

public:
	enum EffType {
		EFF_ATTACK ,
		EFF_ATTACK2,
		EFF_ATTACK3,
		EFF_ATTACKSP,
		EFF_DAMAGE,
		EFF_EXP,
		EFF_MAX,
	};
private:
	EffType mEffType;
	CVector	mMove;
	CVector	mPower;
	float	mRotate;
	float	mStretch;
	float	mAngle;

public:
	//�s��
	int mRows;
	//��
	int mCols;
	//1�R�}�̃t���[����
	int mFps;
	//�t���[���J�E���^
	int mFrame;
	//�}�e���A��
	static CMaterial sMaterial[EFF_MAX];
	
	//�}�e���A���̎��O�ǂݍ���
	static void TexPreLoad();
	//�R���X�g���N�^
	//CEffect(�ʒu, ��, ����, �e�N�X�`����, �s��, ��, 1�R�}������̃t���[����)�@�s���A�񐔁A�t���[�����̓f�t�H���g�����Ōďo�����ȗ��\
	CEffect2(const CVector& pos, float w, float h, EffType efftype, int row = 1, int col = 1, int fps = 1);
	~CEffect2();
	//�ړ��X�V
	void MoveUpdate();
	//�X�V
	void Update();
	//�`��
	void Render();
};
#endif

