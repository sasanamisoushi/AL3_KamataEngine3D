#pragma once
#include "KamataEngine.h"
#include "Math.h"

#include <array>
#include <numbers>
#include <algorithm>

class DeathPerticles {
public:

	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	bool IsFinished() { return isFinished_; }


private:
	
	//02_11スライド10ページパーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;

	//モデル
	Model* model_ = nullptr;

	//カメラ
	Camera* camera_ = nullptr;

	//存続時間<秒>
	static inline const float kDuration = 2.0f;

	//移動の速さ
	static inline const float kSpeed = 0.05f;

	//分割した1個分の角度
	static inline const float kAngleUnit = 2.0f * std::numbers::pi_v<float> / kNumParticles;


	//終了グラフ
	bool isFinished_ = false;

	//経過時間カウント
	float counter_ = 0.0f;

	//色変更オブジェクト
	ObjectColor objectColor_;

	//色の数値
	Vector4 color_;
};
