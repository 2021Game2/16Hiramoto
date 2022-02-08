#ifndef CENEMY3_H
#define CENEMY3_H
//キャラクタクラスのインクルード
#include"CCharacter.h"
//コライダクラスのインクルード
#include"CCollider.h"
#include"CText.h"
#include"CBullet.h"

#include"CSound.h"
/*エネミークラス
キャラクタクラスを継承	*/
class CEnemy3 :public CCharacter {
private:

	CCollider mCollider;
	CBullet* mpBullet;
	CVector mPoint;//目標地点
	int mHp;//体力
	int mJump;
	int mJump2;
	CText mText;
	int mMove2;
	int mColliderCount;
	int mCount;
	int mFireCount;
	int mEnemy3Fry;
	CVector mCollisionEnemy;
	CCollider mColSearch;//サーチ用コライダ
	CCollider mColSearch2;//サーチ用コライダ

	int mEnemyDamage;

	CCharacter* mpPlayer;//プレイヤーのポインタ
public:
	//モデルデータ
	static CModel mModel;
	//コライダ
	//コンストラクタ	
	CEnemy3();
	static int mMoveCount;
	//CEnemy2(位置、回転、拡縮）
	CEnemy3(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
};

#endif