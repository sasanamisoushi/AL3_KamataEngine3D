#pragma once
#include "KamataEngine.h"

class Fade {
public:
	void Initialize();

	void Update();

	void Draw();

	enum class Status {
		None,      //フェード無し
		FadeIn,    //フェードイン中
		FedeOut,   //フェードアウト中
	};

	// フェード開始
	void Start(Status status, float duration);

	//フェード停止
	void Stop();

	//フェード終了判定
	bool IsFinished() const;

private:
	KamataEngine::Sprite* sprite_ = nullptr;

	//現在のフェード状態
	Status status_ = Status::None;

	//フェードの持続時間
	float duration_ = 0.0f;

	//経過時間カウンター
	float counter_ = 0.0f;

	
};
