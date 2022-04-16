#ifndef CENEMY3_H
#define CENEMY3_H
//キャラクタクラスのインクルード
#include"CXCharacter.h"
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
	void AutoMove1();
	void AutoMove2();
	void Damage();
	void Death();
	void Attack();
	void Idle();
	static int mMoveCount;
	//CEnemy2(位置、回転、拡縮）
	CEnemy3(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	//敵の状態
	enum EEnemy3State
	{
		EIDLE,		//待機
		EAUTOMOVE1,	//移動
		EAUTOMOVE2,
		EATTACK,	//攻撃
		EDAMAGED,	//被弾
		EDEATH,		//死亡
	};
	EEnemy3State mState;
};

#endif