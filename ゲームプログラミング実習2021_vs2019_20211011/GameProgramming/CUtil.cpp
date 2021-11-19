#include "CUtil.h"
#include "glut.h"
#include <corecrt_math.h>

//2D�`��X�^�[�g
//Start2D(�����W, �E���W, �����W, ����W)
void CUtil::Start2D(float left, float right, float bottom, float top)
{
	//���f���r���[�s��̑ޔ�
	glPushMatrix();
	//���f���r���[�s��̏�����
	glLoadIdentity();

	//���f���r���[�s�񂩂�
	//�v���W�F�N�V�����s��֐؂�ւ�
	glMatrixMode(GL_PROJECTION);
	//�v���W�F�N�V�����s��̑ޔ�
	glPushMatrix();
	//�v���W�F�N�V�����s��̏�����
	glLoadIdentity();
	//2D�`��̐ݒ�
	gluOrtho2D(left, right, bottom, top);
	//Depth�e�X�g�I�t
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void CUtil::End2D()
{
	//�v���W�F�N�V�����s���߂�
	glPopMatrix();
	//���f���r���[���[�h�֐؂�ւ�
	glMatrixMode(GL_MODELVIEW);
	//���f���r���[�s���߂�
	glPopMatrix();
	//Depth�e�X�g�I��
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

Check CUtil::GetCheck2D(float tx, float tz, float x, float z, float angle)
{
	Check localcheck;
	float v1x, v1z;
	float v2x, v2z;


	v1x = tx - x;
	v1z = tz - z;
	v2x = sinf(angle);//�i�s�p�x��
	v2z = cosf(angle);//�i�s�p�x��


	//�O�ςō��E����
	localcheck.cross = v1x * v2z - v1z * v2x;
	//���ςőO�㔻��
	localcheck.dot = v1x * v2x + v1z * v2z;

	localcheck.dist = sqrtf(v1x * v1x + v1z * v1z);//����
	localcheck.cos = localcheck.dot / localcheck.dist;//2�������ς������Ŋ���ƃR�T�C���̒l�ɂȂ�

	localcheck.turn = (1 - localcheck.cos);

	return localcheck;
}

