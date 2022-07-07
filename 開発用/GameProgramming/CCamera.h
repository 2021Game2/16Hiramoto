#ifndef CCAMERA_H
#define CCAMERA_H
#include "CVector.h"

#include"CCollisionManager.h"
#include "CColliderLine.h"
/*
カメラクラス
*/
#define DEF_CAMERA_DIST 8.0f
#define DEF_CAMERA_HEAD_ADJUST 3.0f
class CCamera :public CCharacter{
public:

	

	CCamera();
	//視点
	CVector mEye;
	//注視点
	CVector mCenter;
	//上方向
	CVector mUp;
	CColliderLine mColliderLine;
	CMatrix mMatrix;
	

	//重複するがカメラ制御用
	CVector mPos;		//位置
	CVector mTarget;	//ターゲット
	float	mAngleX;	//アングル
	float	mAngleY;	//アングル
	float	mDist;	//距離

	//カメラの設定
	//Set(視点, 注視点, 上方向)
	void Set(const CVector &eye, const CVector &center,
		const CVector &up);

	void SetTarget(const CVector& target);
	void Render();
	//カメラ初期化
	void Init();
	//カメラ更新処理
	void Update();
	//カメラ適用
	//void Render();
    void CameraRender();
	void TaskCollision();
	//staticでポインタを作る
	static CCamera* mpCameraInstance;
	//staticで処理を作る
	static CCamera* GetInstance();
	void Collision(CCollider* m, CCollider* o);
	//void CollisionTriangleLine(CCollider* triangle, CCollider* line, CVector* adjust);
	//ベクトル取得
	CMatrix GetMat();

	bool mSkip;
	//スクリーン座標変換
	//戻り値 表示範囲
	//pOut 結果格納用
	//pos 2Dに変換したいワールド座標
	bool WorldToScreen(CVector* pOut, const CVector& pos);

};

//カメラの外部参照
extern CCamera* Camera;
#endif
