#ifndef CCAMERA_H
#define CCAMERA_H
#include"CVector.h"
#include"CTransform.h"
/*カメラクラス（プレイヤーの視点に対する処理）*/
class CCamera {
public:
	//視点
	CVector mEye;
	//注視点
	CVector mCenter;
	//上方向
	CVector mUp;
	CVector mPosition;
	//カメラの設定
	//Set(視点、注視点(向く方向）、上方向）
	void Set(const CVector& eye, const CVector& center,
		const CVector& up);
		void Update();
	//カメラ適用
	void Render();
};
//カメラの外部参照
extern CCamera Camera;
#endif
