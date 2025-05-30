#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include "Player.h"

//前方宣言
class Player;

class CameraController {
public:
	void Initialize(Camera* camera);

	void Update();

	void SetTarget(Player* target) { 
		
		target_ = target;
	}

	void Reset();


	//矩形
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	void SetMovableArea(Rect area) { movableArea_ = area; }


private:
	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	Player* target_ = nullptr;

	//追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = {0, 0, -15.0f};

	//カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	//カメラの目的座標
	Vector3 targetPos_;

	//座標補間割合
	static inline const float kInterpolationRate = 0.1f;

	//速度掛け率
	static inline const float kVelocityBias = 1.0f;

	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};

};
