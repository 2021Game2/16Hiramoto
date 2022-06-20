
#ifndef CSHADOWMAP_H
#define CSHADOWMAP_H
#include "glew.h"
#include "glut.h"
class CShadowMap {
private:
	GLfloat mLightPos[3]; //�����̈ʒu
	GLfloat mShadowCol[4]; //�e�̐F
	int mTextureWidth;// �f�v�X�e�N�X�`���̕�
	float mTextureHeight;// �f�v�X�e�N�X�`���̍���
	GLuint mDepthTextureID; //�f�v�X�e�N�X�`��ID
	GLuint mFb; //�t���[���o�b�t�@���ʎq
	//void (*mpRender)(); //Render�֐��̃|�C���^
public:
	CShadowMap();
	~CShadowMap();
	//����������
	void Init();

	//wight// �f�v�X�e�N�X�`���̕�
	//height// �f�v�X�e�N�X�`���̍���
    //funcRender;// �`��֐��̃|�C���^
	//shadowCol;// �e�̐F
	//lightPos;// �����̈ʒu
	/*
	void Init(int width, int height, void (*funcRender)(),
		float shadouCol[], float lightPos[]);
	*/
	void Init(int width, int height, void (*funcRender)(), void (*funcEffectRender)(),
		float shadouCol[], float lightPos[]);
	void Render();
	void (*mpRender)(); //Render�֐��̃|�C���^
	void (*mpEffectRender)();//EffectRender�֐��|�C���^

};
#endif
