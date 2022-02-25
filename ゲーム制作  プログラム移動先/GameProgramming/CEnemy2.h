#ifndef CENEMY2_H
#define CENEMY2_H
//キャラクタクラスのインクルード
#include"CCharacter.h"
#include"CXPlayer.h"
#include "CXCharacter.h"
//コライダクラスのインクルード
#include"CCollider.h"
#include"CText.h"

#include"CSound.h"
/*エネミークラス
キャラクタクラスを継承	*/
class CEnemy2 :public CXCharacter {
private:

	//コライダ
	CCollider mCollider;

	CVector mPoint;//目標地点
	int mHp;//体力
	float mJump;
	int mJump2;
	CText mText;
	int mMove;
	int mMove2;
	int mDamageCount;

	float mColliderCount;

	float mGravity;//重力
	float mTime;//ジャンプする時の時間を計測
	int mEnemy2StopCount;//プレイヤーのESTOPPERに当たっている間増加

	CCollider mColSphereHead;
	CCollider mColSphereRight;
	CCollider mColSphereLeft;
	CVector mCollisionEnemy;

	CCollider mColSearch;//サーチ用コライダ

	float mPlayerMarkingX;//プレイヤーと敵のX座標の差
	float mPlayerMarkingZ;//プレイヤーと敵のZ座標の差
	float mRotationCount;
	int mEnemyDamage;
	int mEnemyVoice;
	float CurveCount;
	CCharacter* mpPlayer;//プレイヤーのポインタ
	//CXPlayer* mpPointPlayer;
public:
	//モデルデータ
	static CModel mModel;
	//コンストラクタ	
	CEnemy2();
	static int mEnemy2AttackCount;
	void Init(CModelX* model);
	
	//CEnemy2(位置、回転、拡縮）
	CEnemy2(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
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