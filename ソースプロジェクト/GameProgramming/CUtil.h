#ifndef CUTIL_H
#define CUTIL_H
/*
���[�e�B���e�B�N���X
���ʂɎg�p�ł���v���O����
*/
typedef struct {
	float dot;//����
	float cross;//�O��
	float dist;//����
	float cos;//�p�x
	float turn;//��]
}Check;

class CUtil
{
public:

public:
	//2D�`��X�^�[�g
	//Start2D(�����W, �E���W, �����W, ����W)
	static void Start2D(float left, float right, float bottom, float top);
	static void End2D();

	//�[��2�����x�N�g���v�Z�@angle��Radian�p�x�ɕϊ����ē����
	static Check GetCheck2D(float tx, float tz, float x, float z, float angle);
};

#endif
