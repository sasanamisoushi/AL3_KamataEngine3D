#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Player {
public:

	//初期化
	void Initilize(Model* model,  Camera* camera,const Vector3& position);

	//更新
	void Update();

	//描画
	void Draw();

	// 速度
	Vector3 velocity_ = {};
	

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	//uint32_t textureHandle_ = 0u;
	//カメラ
	Camera* camera_ = nullptr;

	
	
	static inline const float kAcceleration = 5.0f;
};
