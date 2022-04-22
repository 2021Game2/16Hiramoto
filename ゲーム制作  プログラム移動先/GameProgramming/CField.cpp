
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CField.h"
#include"CCollider.h"
#define OBJ "3DModel\\Field\\Field.obj"
#define MTL "3DModel\\Field\\Field.mtl"

CModel CField::mModel;//モデルデータ作成

CField::CField(const CVector& position, const CVector& rotation, const CVector& scale)

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
	mTag = EFIELD;
}
void CField::Update() {
	//CCharacterの更新
	CTransform::Update();
}

void CField::Render() {
	//親の描画処理
	CCharacter::Render();
}
void CField::TaskCollision() {

}
