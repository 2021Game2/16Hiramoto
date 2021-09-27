#ifndef CCOLLIDERLINE_H
#define CCOLLIDERLINE_H

#include"CCollider.h"

class CColliderLine :public CCollider {
public:
	CColliderLine(){}
	//コンストラクタ(線分コライダ)
	//親はプレイヤー
	//CColliderLine(親、親行列、頂点１、頂点２）
	CColliderLine(CCharacter* parent, CMatrix* matrix, const CVector& v0, const CVector& v1);
	//線分コライダの設定
	//Set(親、親行列、頂点１、頂点２）
	void Set(CCharacter* parent, CMatrix* matrix, const CVector& v0, const CVector& v1);
	//描画
	void Render();
	void ChangePriority();
};
#endif
