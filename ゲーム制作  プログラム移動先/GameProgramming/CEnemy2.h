#ifndef CENEMY2_H
#define CENEMY2_H
//キャラクタクラスのインクルード
#include"CCharacter.h"
#include"CXPlayer.h"
#include "CXCharacter.h"
//コライダクラスのインクルード
#include"CCollider.h"
#include"CText.h"
/*エネミークラス
キャラクタクラスを継承	*/
class CEnemy2 :public CXCharacter {
public:
	//モデルデータ
	static CModel mModel;
	//コライダ
	CCollider mCollider;
	//コンストラクタ	
	CEnemy2();
	CVector mPoint;//目標地点
	int mHp;//体力
	float mJump;
	int mJump2;
	CText mText;
	int mMove;
	int mMove2;
	
	float mColliderCount;
	static int mEnemy2AttackCount;
	float mGravity;//重力
	float mTime;//ジャンプする時の時間を計測
	int mEnemy2StopCount;//プレイヤーのESTOPPERに当たっている間増加
	void Init(CModelX* model);
	CCollider mColSphereHead;
	CCollider mColSphereRight;
	CCollider mColSphereLeft;
	
	CVector mCollisionEnemy;
	//CEnemy2(位置、回転、拡縮）
	CEnemy2(const CVector& position, const CVector& rotation, const CVector& scale);
	CCollider mColSearch;//サーチ用コライダ
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	float mPlayerMarkingX;//プレイヤーと敵のX座標の差
	float mPlayerMarkingZ;//プレイヤーと敵のZ座標の差
	float mRotationCount;
	int mEnemyDamage;

	void Idle();		//待機処理
	void AutoMove();	//移動処理
	void Attack();	//攻撃処理
	void Damaged();		//被弾処理
	void Death();		//死亡処理
	//敵の状態
	enum EEnemy2State
	{
		EIDLE,		//待機
		EAUTOMOVE,	//移動
		EATTACK,	//攻撃
		EDAMAGED,	//被弾
		EDEATH,		//死亡
	};
	EEnemy2State mState;
};
#endif