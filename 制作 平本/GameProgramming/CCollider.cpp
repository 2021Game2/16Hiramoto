#include"CCollider.h"
#include"CCollisionManager.h"
#include"CColliderLine.h"
//コライダー・・・３Dモデル内での当たり判定。球の形
//任意のモデルに貼り付けることで
CCollider::CCollider(CCharacter* parent, CMatrix* matrix, CVector position, float radius)
	:CCollider()
{
	//親設定
	mpParent = parent;
	//親行列設定
	mpMatrix = matrix;
	//CTransform設定
	mPosition = position;//位置
	//半径設定
	mRadius = radius;
	//Collisionリストに追加
	//CCollisionManager::Get()->Add(this);
	

}
CCollider::~CCollider() {
	//Collisionリストから削除
	CCollisionManager::Get()->Remove(this);
}
CCollider::CCollider()
:mpParent(0)
,mpMatrix(&mMatrix)
,mType(ESPHERE)
,mTag(EBODY)
{
	//コリジョンマネージャに追加
	CCollisionManager::Get()->Add(this);

}
void CCollider::Render() {
	glPushMatrix();
	//コライダの中心座標を計算
	//自分の座標*親の変数行列をかける
	CVector pos = mPosition * *mpMatrix;//*ポインタ,,,実体
	//中心座標へ移動
	glMultMatrixf(CMatrix().Translate(pos.mX, pos.mY, pos.mZ). mM[0]);
	//DIFFUSE赤色設定
	float c[] = { 1.0f,0.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//球描画(当たり判定のある弾）glutWireSphere(半径、分割数、分割数）
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();
}
//衝突判定
//CCollision(コライダ１，コライダ２）
//return:true(衝突している）　false(衝突していない）
bool CCollider::Collision(CCollider* m, CCollider* o) {
	//各コライダの中心座標を求める
	//原点*コライダの変換行列*親の変換行列
	CVector mpos = m->mPosition * *m->mpMatrix;
	CVector opos = o->mPosition * *o->mpMatrix;
	//中心から中心へのベクトルを求める
	mpos = mpos - opos;
	//中心の距離が半径の合計より小さいと衝突
	if (m->mRadius + o->mRadius > mpos.Length()) {
		//衝突している
		return true;
	}
	//衝突していない
	return false;
}
//線分コライダの衝突判定
bool CCollider::CollisionTriangleLine(CCollider* t, CCollider* l, CVector* a) {
	CVector v[3], sv, ev;  //sv,,,始点、ev,,,終点
	//各コライダの頂点をワールド座標へ変換
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;

	sv = l->mV[0] * *l->mpMatrix;
	ev = l->mV[1] * *l->mpMatrix;
	//面の法線を、外積を正規化してもとめる
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	//三角の頂点から線分始点へのベクトルを求める
	CVector v0sv = sv - v[0];
	//三角の頂点から線分終点へのベクトルを求める
	CVector v0ev = ev - v[0];
	//線分が面と交差しているか確認する
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//プラスは交差していない（面は高さ０の位置にあるため）
	if (dots * dote >= 0.0f) {
		//衝突していない(調整不要）
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//線分は面と交差している


	//面と線分の交点を求める
	//交点の計算
	//始点+（始点から終点までの距離）*(v0svと法線の内積/始点から終点までの内積の和
	CVector cross = sv + (ev - sv) * (abs(dots) / (abs(dots) + abs(dote)));

	//交点が三角形内なら衝突している（三角形なので頂点３つ分の衝突処理）
	//外積、、、２つのベクトルの交点から垂直に伸びる直線
	//内積、、ベクトルの絶対値*ベクトルｂの絶対値*２つのベクトルからなるCosθ
	//頂点１～頂点２ベクトルと頂点1～交点ベクトルとの交点を求め、法線との内積がマイナスなら三角形の外
	//(１～２のベクトルの長さ）、（１～交点の長さ）、（法線との内積）
	if ((v[1] - v[0]).Cross(cross - v[0]).Dot(normal) < 0.0f) {
		//衝突していない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点２～頂点３ベクトルと頂点２～交点ベクトルとの外積を求め、法線との内積がマイナスなら三角形の外
	if ((v[2] - v[1]).Cross(cross - v[1]).Dot(normal) < 0.0f) {
		//衝突していない
		//*a、、、線分コライダを戻す値
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点３～頂点１ベクトルと、頂点３～交点ベクトルとの外積を求め、法線との外積がマイナスなら三角形の外
	if ((v[0] - v[2]).Cross(cross - v[2]).Dot(normal) < 0.0f) {
		//衝突していない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//調整値計算（衝突しない位置まで戻す)

	if (dots < 0.0f) {
		//始点が裏面
		*a = normal * -dots;
	}
	else {
		//終点が裏面
		*a = normal * -dote;
	}
	return true;
}
//球と三角形の衝突判定を行う処理
bool CCollider::CollisionTriangleSphere(CCollider* t, CCollider* s, CVector* a) {
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	//面の法線を,外積を正規化して求める
	CVector normal=(v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	//線コライダをワールド座標で作成
	sv = s->mPosition * *s->mpMatrix + normal * s->mRadius;
	ev = s->mPosition * *s->mpMatrix - normal * s->mRadius;
	CColliderLine line(NULL, NULL, sv, ev);
	//三角コライダと線コライダの衝突処理
	return CollisionTriangleLine(t, &line, a);

}
//優先度の変更
void CCollider::ChangePriority() {
	//自分の座標*親の変換行列を掛けてワールド変数を求める
	CVector pos = mPosition * *mpMatrix;
	//べぅトルの長さが優先度
	mPriority = pos.Length();
	CCollisionManager::Get()->Remove(this);//一旦削除
	CCollisionManager::Get()->Add(this);//追加
}
