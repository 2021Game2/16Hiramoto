#ifndef CENEMY3_H
#define CENEMY3_H
//キャラクタクラスのインクルード
#include"CCharacter.h"
//コライダクラスのインクルード
#include"CCollider.h"
#include"CText.h"
#include"CBullet.h"
/*エネミークラス
キャラクタクラスを継承	*/
class CEnemy3 :public CCharacter {
public:
	//モデルデータ
	static CModel mModel;
	//コライダ
	CCollider mCollider;
	CBullet* mpBullet;
	//コンストラクタ	
	CEnemy3();
	CVector mPoint;//目標地点
	int mHp;//体力
	int mJump;
	int mJump2;
	CText mText;
	static int mMoveCount;
	int mMove2;
	int mColliderCount;
	int mCount;
	CVector mCollisionEnemy;
	//CEnemy2(位置、回転、拡縮）
	CEnemy3(const CVector& position, const CVector& rotation, const CVector& scale);
	CCollider mColSearch;//サーチ用コライダ
	CCollider mColSearch2;//サーチ用コライダ
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	CCharacter* mpPlayer;//プレイヤーのポインタ
	int mEnemyDamage;
};

#endif