#ifndef CBULLET_H
#define CBULLET_H
#include"CCharacter.h"
#include"CCollider.h"
class CBullet : public CCharacter {
public:
	CBullet();
	int mLife;

	//三角形CTriangleをmTとする
	CTriangle mT;
	//w,dの値を設定（値は変えられる）
	void Set(float w, float d);
	//w;//幅
	//d;//長さ
	//三角形を次の幅、高さに設定
	//三角形の法線を上向きに設定
	//mScaleを全て１倍にする
	void Update();
	//CCharacter::Updateを実行
	CCollider mCollider4;

	void CBullet::Render();
	
	//衝突処理
//Collision(CCollider*m,CCollider*o)
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();

};
#endif
