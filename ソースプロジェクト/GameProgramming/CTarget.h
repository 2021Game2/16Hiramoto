
#include"CModel.h"
#include"CCharacter.h"
#include"CXPlayer.h"
#ifndef CTARGET_H
#define CTARGET_H

class CTarget : public CCharacter {
private:

	CVector mPoint;
public:
	//モデルデータ
	static CModel mModel;
	CTarget();
	//CTarget(位置、回転、拡縮）
	CTarget(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();

};
#endif
