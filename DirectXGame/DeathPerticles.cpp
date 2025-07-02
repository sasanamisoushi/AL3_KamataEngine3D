#include "DeathPerticles.h"
#include "WorldtransfomUpdate.h"



void DeathPerticles::Initialize(Model* model, Camera* camera, const Vector3& position) {

	//モデル
	model_ = model;
	//カメラ
	camera_ = camera;

	// ワールド変換の初期化
	for (KamataEngine::WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeathPerticles::Update() {

	//終了なら何もしない
	if (isFinnished_) {
		return;
	}

	//カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	//存在時間の上限に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		//終了扱いする
		isFinnished_ = true;
	}

	


	for (uint32_t i = 0; i < kNumParticles; ++i) {
		//基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};
		//回転角を計算する
		float angle = kAngleUnit * i;
		//z軸周り回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		//基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		//移動量
		worldTransforms_[i].translation_ += velocity;
	}

	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	//色変更オブジェクトに色の数値を設定する
	 objectColor_.SetColor(color_);

	// 02_11の12ページ
	for (auto& worldTransform : worldTransforms_) {
		// ワールド行列更新（アフィン変換～DirectXに転送）
		worldTransformUpdate(worldTransform);
	}
}

void DeathPerticles::Draw() {
	//終了なら何もしない
	if (isFinnished_) {
		return;
	}

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	// 02_11の13ページ
	for (auto& worldTransform : worldTransforms_) {
		// 02_11_33枚目で&objectColor_を追加
		model_->Draw(worldTransform, *camera_,&objectColor_);
	}
	Model::PostDraw();
}
