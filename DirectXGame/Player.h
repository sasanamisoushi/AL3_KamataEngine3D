#pragma once
#include "KamataEngine.h"

class Player {
public:

	//初期化
	void Initilize();

	//更新
	void Update();

	//描画
	void Draw();

private:
	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	//モデル
	KamataEngine::Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	void Initialize(Model* model, uint32_t textureHandle);
};
