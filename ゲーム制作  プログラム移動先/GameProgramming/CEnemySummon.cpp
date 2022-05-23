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
#define OBJ "3DModel\\EnemySummon\\egg.obj"
#define MTL "3DModel\\EnemySummon\\egg.mtl"
#define HP 10
#define HP_MAX 100				//体力最大値
#define GAUGE_WID_MAX 400.0f	//ゲージの幅の最大値
#define GAUGE_LEFT 20			//ゲージ描画時の左端
#define IMAGE_GAUGE "Resource\\Gauge.png"		//ゲージ画像
CModel CEnemySummon::mModel;
CEnemySummon::CEnemySummon()
    :mHp(0)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)

{
   

	//モデルのポインタ設定
	mpModel = &mModel;
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}

	mImageGauge.Load(IMAGE_GAUGE);
}
CEnemySummon::CEnemySummon(const CVector& position, const CVector& rotation, const CVector& scale)
	:CEnemySummon()
	
{
	mHp = HP;
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
void CEnemySummon::Update() {
	if (mDamageCount > 0) {
		mDamageCount--;
	}
		if(mHp<=0){
		mEnabled=false;
		}
}
//Collision(コライダ１、コライダ２）
void CEnemySummon::Collision(CCollider* m, CCollider* o) {
	if (m->mType == CCollider::ESPHERE) {
		if (o->mType == CCollider::ESPHERE) {
			if (o->mpParent->mTag ==EPLAYER) {
				if(o->mTag==CCollider::EPLAYERSWORD){
				//衝突しているとき
					if (CCollider::Collision(m, o)) {

						//プレイヤーの当たり判定が有効なとき
						if (((CXPlayer*)(o->mpParent))->mAttackHit == true) {
							if (mDamageCount <= 0) {

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
void CEnemySummon::Render() {
	//親の描画処理
	CCharacter::Render();
}
void CEnemySummon::TaskCollision() {
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}

void CEnemySummon::Render2D()
{
	//2D描画開始
	CUtil::Start2D(0, 800, 0, 600);

	CVector ret;
	CVector tpos = mPosition;
		//mpCombinedMatrix[6].GetPos();
	Camera.WorldToScreen(&ret, tpos);

	float HpRate = (float)mHp / (float)HP_MAX; //体力最大値に対する、現在の体力の割合
	float HpGaugeWid = GAUGE_WID_MAX * HpRate; //体力ゲージの幅

	//画面外の時に表示しない
	if (ret.mX > 0 && ret.mX < 800) {
		//ゲージ背景
		mImageGauge.Draw(ret.mX - GAUGE_WID_MAX, ret.mX + GAUGE_WID_MAX, ret.mY + 30.0f, ret.mY + 45.0f, 210, 290, 63, 0);
		//体力ゲージ
		mImageGauge.Draw(ret.mX - GAUGE_WID_MAX, (ret.mX - GAUGE_WID_MAX) + HpGaugeWid * 2.0f, ret.mY + 30.0f, ret.mY + 45.0f, 0, 0, 0, 0);
		//プレイヤーの攻撃対象になっているとき
		//if (mIsTarget) {
			//ターゲット画像表示
			//mImageTarget.Draw(ret.mX - 30.0f, ret.mX + 30.0f, ret.mY - 30.0f, ret.mY + 30.0f, 0, 255, 255, 0);
		//}
	}
	CUtil::End2D();
}

