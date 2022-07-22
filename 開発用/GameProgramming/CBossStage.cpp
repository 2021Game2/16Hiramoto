#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CBossStage.h"
#include"CCollider.h"
#define OBJ "Resource\\3DModel\\BossStage\\BossStage.obj"
#define MTL "Resource\\3DModel\\BossStage\\BossStage.mtl"
CModel CBossStage::mModel;//モデルデータ作成
CBossStage::CBossStage(const CVector& position, const CVector& rotation, const CVector& scale)
{
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	//優先度を2に変更する
	mPriority = 2;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
	CTransform::Update();//行列の更新
   //モデルのポインタ設定
	mpModel = &mModel;
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}
	mColliderMesh.Set(this, &mMatrix, &mModel);//モデルをコライダにする
	mTag = EBOSSSTAGE;
}
void CBossStage::Update() {
	//CCharacterの更新
	CTransform::Update();
}

void CBossStage::Render() {
	//親の描画処理
	CCharacter::Render();
}