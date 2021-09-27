#ifndef CENEMY_H
#define CENEMY_H
#include"CCharacter.h"
#include"CCollider.h"
class CEnemy :public CCharacter {
public:
	//コンストラクタ
	//CEnemy(モデル、位置、回転、拡縮）

	CEnemy(CModel* model, CVector position, CVector rotation, CVector scale);
void Update();
//コライダ
int mHp;
CCollider mCollider1;
CCollider mCollider2;
CCollider mCollider3;
CVector mPoint;//目標地点
CCollider mColSearch;//サーチ用コライダ
CCharacter* mpPlayer;//プレイヤーのポインタ
int mFireCount;
//衝突処理
//Collision(CCollider*m,CCollider*o)
void Collision(CCollider* m, CCollider* o);
int mDamageCount;
void TaskCollision();

}; 
#endif