#include"CTaskManager.h"
#include"CTask.h"
//タスクマネージャのインスタンス
CTaskManager* CTaskManager::mpInstance = 0;
//インスタンスの取得
CTaskManager* CTaskManager::Get() {
	//インスタンスがなければ
	if (mpInstance == 0) {
		//インスタンスを生成する
		mpInstance = new CTaskManager();
	}
	return mpInstance;
}

CTaskManager::CTaskManager() {
	mHead.mpNext = &mTail;
	mTail.mpPrev = &mHead;



}
//デストラクタ
CTaskManager::~CTaskManager() {

}
//リストに追加
//Add（タスクのポインタ）
void CTaskManager::Add(CTask* addTask) {
	//mHeadの次から検索
	CTask* task = mHead.mpNext;
	//優先度の大きさ順にタスクを挿入
	//挿入位置の検索(優先度が同じか小さいタスクの前）大きいものを先に処理
	//ｍPriority>=0のこと
	while (addTask->mPriority < task->mPriority)
	{
		task = task->mpNext;//次へ
	}
	//弾は出された瞬間ポインタを移動することで消えることなく移動できる

	//mpPrev=前のポインタ
	//mpNext＝次のポインタ
	//CTaskをポインタに変換し、mTailの前に追加
	//addTask->task->addTask,,,となる
	//addTaskの次をtask
	addTask->mpNext = task;
	//addTaskの前をtaskの前に
	addTask->mpPrev = task->mpPrev;
	//addTaskの前の次をaddTaskに
	addTask->mpPrev->mpNext = addTask;
	//taskの前をaddTaskに
	task->mpPrev = addTask;
}
//更新
void CTaskManager::Update() {
	//先頭から最後まで繰り返し
	CTask* task = mHead.mpNext;
	while (task->mpNext) {
		//更新処理を呼ぶ
		task->Update();
		//次へ
		task = task->mpNext;
	}
}
void CTaskManager::Render() {
	//先頭から最後まで繰り返し
	CTask* task = mHead.mpNext;
	while (task->mpNext) {
		//描画処理を呼ぶ
		task->Render();
		//次へ
		task = task->mpNext;
	}
}
//リストから削除
void CTaskManager::Remove(CTask* task) {
	//タスクの前の次をタスクの次にする
	task->mpPrev->mpNext = task->mpNext;
	//タスクの次の前をタスクの前にする
	task->mpNext->mpPrev = task->mpPrev;

}
void CTaskManager::Delete() {
	//先頭から最後までくりかえし
	CTask* task = mHead.mpNext;
	while (task->mpNext) {
		//taskをdelとする
		CTask* del = task;
		//次へ
		task = task->mpNext;
		//ｍEnabledがfalseなら削除
		if (del->mEnabled == false) {
			delete del;
		}
	}
}
void CTaskManager::TaskCollision() {


	//先頭から最後まで繰り替えし
	CTask* task = mHead.mpNext;
	while (task->mpNext) {
		//衝突処理を呼ぶ
		task->TaskCollision();
		//次へ
		task = task->mpNext;
	}
}