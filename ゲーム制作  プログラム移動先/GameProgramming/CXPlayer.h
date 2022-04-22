#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"
#include"CColliderLine.h"
#include"CCollisionManager.h"
#include"CTaskManager.h"
#include"CVector.h"

#include"CTexture.h"
#include"CSound.h"
class CXPlayer : public CXCharacter
{
private:

	CTexture mImageGauge; //ゲージ画像
public:

	 bool mAttackHit;
	//コライダの宣言
	CCollider mColSphereSword;	//剣
	CCollider  mColSphereFoot;//ダメージが通る当たり判定
	CCollider mCollider2;//アイテムを拾った時の武器の当たり判定
    CVector mCollisionEnemy;//プレイヤーと敵のコライダーの位置の差

	CTexture mTexture;
	int mSpaceCount1;//一回目の攻撃の総フレーム
	int mSpaceCount2;//２回めの攻撃の総フレーム
	int mSpaceCount3;//３回めの攻撃の総フレーム
	static int mStamina;//スタミナの数値
	int mDamageCount;//ダメージを受けた直後の無敵時間
	float mJump;//ジャンプする時の初速度
	float mStep;//攻撃時、少し前進
	float mGravity;//重力
	//別のクラスで変数を呼び出す場合
	//staticでポインタを作る
	static CXPlayer* mpPlayerInstance;
	//staticで処理を作る
	static CXPlayer* GetInstance();
	
    static int mSpAttack;//ジャンプ攻撃をするためのゲージ
	static int mAttackCount;//武器の当たり判定が適用される時間
	float mColliderCount;//吹き飛ばされた時の初速度
	int mAnimationCount;//アニメーションが途中で変わらないようにする
	static int mHp;//体力
	float mTime;//ジャンプする時の時間を計測
	float mSpeed;
	
	//敵の状態
	enum EPLAYERState
	{
		EIDLE,		//待機
		EMOVE,	//移動
		EDUSH,
		EESCAPE,
		EATTACK1,	//攻撃1
		EATTACK2,
		EATTACK3,
		EATTACKSP,
		EDAMAGED,	//被弾
		EDEATH,		//死亡
	};
	EPLAYERState mState;
	CXPlayer();

	void TaskCollision();
	
	
	/*
	初期化(Xモデルクラスのポインタ)
	*/
	void Init(CModelX* model);

	void Update();
	void Collision(CCollider* m, CCollider* o);
	void Render2D();
};

#endif
