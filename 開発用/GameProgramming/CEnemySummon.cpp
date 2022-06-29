#include"CEnemySummon.h"
#include"CCharacter.h"
#include"CXCharacter.h"
#include"CCollider.h"
#include"CEnemy2.h"
#include"CModel.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CUtil.h"
#include"CCamera.h"
#include"CSceneGame.h"
#define OBJ "Resource\\3DModel\\EnemySummon\\egg.obj"
#define MTL "Resource\\3DModel\\EnemySummon\\egg.mtl"
#define HP 3
CModel CEnemySummon::mModel;
CEnemySummon::CEnemySummon(const CVector& position, const CVector& rotation, const CVector& scale)
	:CEnemySummon()
	
{
	
    mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mCollider.mTag = CCollider:: EENEMYSUMMON;
	mTag = EENEMYSUMMON;
	CTransform::Update();//行列の更新
	//優先度を１に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
}

CEnemySummon::CEnemySummon()
	: mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
	, mHp(HP)
	, mEffectCount(0)
{
	//モデルのポインタ設定
	mpModel = &mModel;
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}

}
void CEnemySummon::Update() {
	CSceneGame* tSceneGame = CSceneGame::GetInstance();
	if (mDamageCount > 0) {
		mDamageCount--;
	}

	mEffectCount--;
		if(mHp<=0|| tSceneGame->mBossGaugeSwitch == true){
			
		 mEnabled=false;
		}
		//CCharacterの更新
		CTransform::Update();
}
void CEnemySummon::Damage() {
	if (mEffectCount % 15 == 0) {
		//エフェクト生成
		new CEffect2(mPosition, 5.0f, 5.0f, CEffect2::EFF_EXP, 4, 4, 2);
	}
}
//Collision(コライダ１、コライダ２）
void CEnemySummon::Collision(CCollider* m, CCollider* o) {
	if (m->mType == CCollider::ESPHERE) {
		if (m->mTag == CCollider::EENEMYSUMMON) {
			if (o->mType == CCollider::ESPHERE) {
				if (o->mpParent->mTag == EPLAYER) {
					if (o->mTag == CCollider::EPLAYERSWORD) {
						//衝突しているとき
						if (CCollider::Collision(m, o)) {
							//プレイヤーの当たり判定が有効なとき
							if (((CXPlayer*)(o->mpParent))->mAttackHit == true) {
								if (mDamageCount <= 0) {
									//爆発エフェクト付与
									 CEnemySummon::Damage();
									mHp--;
									mDamageCount = 20;
								}
							}
						}
					}
				}
			}
		}
	}
}
void CEnemySummon::TaskCollision() {
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
