#include "Enemy.h"
#include <numbers>
#include <cassert>
#include "WorldtransfomUpdate.h"
#include "Math.h"
#include "Player.h"
#include "GameScene.h"

using namespace KamataEngine;



void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	// モデル
	model_ = model;

	//カメラ
	camera_ = camera;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 角度調整
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	//速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	// 変更リクエストがあったら
	if (behaviorRequest_ != Behavior::kUnknown) {
		// 振るまいを変更する
		behavior_ = behaviorRequest_;

		// 各振るまいごとの初期化を実行
		switch (behavior_) {
		case Behavior::kDefeated:
		default:
			counter_ = 0;
			break;
		}

		// 振るまいリクエストをリセット
		behaviorRequest_ = Behavior::kUnknown;
	}

	

	switch (behavior_) {
	case Enemy::Behavior::kWalk:
		// 移動
		worldTransform_.translation_.x += velocity_.x;

		// タイマーを加算
		walkTimer_ += 1.0f / 60.0f;

		// 回転アニメーション
		worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);

		// 行列更新
		worldTransformUpdate(worldTransform_);
		break;
	case Enemy::Behavior::kDefeated:
		counter_ += 1.0f / 60.0f;

		worldTransform_.rotation_.y += 0.3f;
		worldTransform_.rotation_.x = EaseOut(ToRadians(kDefeatedMotionAngleStart), ToRadians(kDefeatedMotionAngleEnd), counter_ / kDefeatedTime);
		
		// 行列更新
		worldTransformUpdate(worldTransform_);

		if (counter_ >= kDefeatedTime) {
			isDead_ = true;
		}
		break;
	}

}

void Enemy::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_, *camera_); 
	Model::PostDraw();
}



AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision(const Player* player) { 
	//敵がやられているなら何もしない
	if (behavior_ == Behavior::kDefeated) {
		return;
	}

	//プレイヤーが攻撃中なら敵が死ぬ
	if (player->IsAttack()) {
		if (gameScene_) {

			Vector3 pos = player->GetWorldPosition();
			// 敵と自キャラの中間位置にエフェクトを生成
			Vector3 effectPos;
			effectPos.x = (GetWorldPosition() + pos).x / 2.0f;
			effectPos.y = (GetWorldPosition() + pos).y / 2.0f;
			effectPos.z = (GetWorldPosition() + pos).z / 2.0f;
			gameScene_->CreateHitEffect(effectPos);
		}

		//敵の振る舞いをデス演出に変更
		behaviorRequest_ = Behavior::kDefeated;

		isCollisionDisabled_ = true;

		
	}
}
