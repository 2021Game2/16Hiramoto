#ifndef CTASK_H
#define CTASK_H


class CTask {    //タスクリスト
public: 
	CTask* mpNext;//次のポインタ
	  CTask* mpPrev;//前のポインタ
	  int mPriority;//優先度
	  bool mEnabled;//有効フラグ
	  //デフォルトコンストラクタ
	  CTask() :
		  mpNext(0), mpPrev(0), mPriority(0), mEnabled(true) {}
	  virtual ~CTask() {}
	  //更新
	  virtual void Update() {}
	  //描画
	  virtual void Render() {}
	  //衝突処理
	  virtual void TaskCollision(){}
};
#endif