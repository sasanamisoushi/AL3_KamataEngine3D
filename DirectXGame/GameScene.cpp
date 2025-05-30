#include "GameScene.h"




using namespace KamataEngine;



void GameScene::Initialize() {
	
	//3Dモデルの生成(自機）
	model_ = Model::CreateFromOBJ("player", true);
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//カメラの初期化
	camera_.Initialize();

	

	//自キャラの更新
	player_ = new Player();

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);

	//自キャラの初期化
	player_->Initilize(model_, &camera_, playerPosition);
	// 3Dモデルの生成(ブロック）
	blockModel_ = Model::CreateFromOBJ("block",true);

	//マップチップの更新
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	//表示ブロックの生成
	GenerateBlocks();

	

	

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//02_03天球
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//天球の生成
	skydome_ = new Skydome();
	//天球の初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	//カメラコントローラの生成
	cameraController_ = new CameraController();
	// カメラコントローラの初期化
	cameraController_->Initialize(&camera_);
	//追従対象をセット
	cameraController_->SetTarget(player_);
	//リセット
	cameraController_->Reset();
	//移動範囲の指定
	CameraController::Rect camerArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(camerArea);

}

void GameScene::Update() {
	//自キャラの更新
	player_->Update();

	// 天球の更新
	skydome_->Update();

	//カメラコントローラの更新
	cameraController_->Update();

	

	//ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			//worldTransform(* worldTransformBlock);
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}

	#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	#endif // DEBUG

	if (isDebugCameraActive_) {

		//デバックカメラの更新
		debugCamera_->Update();

		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		camera_.UpdateMatrix();
	}
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			blockModel_->Draw(*worldTransformBlock, camera_);
		}
	}

	// 自キャラの描画
	player_->Draw();

	Model::PostDraw();

	//天球の描画
	skydome_->Draw();
}

GameScene::~GameScene() {
	//3Dモデルデータの解放(自機)
	delete model_;

	//自キャラの解放
	delete player_;

	// 3Dモデルデータの解放(ブロック)
	delete blockModel_;

	//可変個配列の解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	//デバックカメラ
	delete debugCamera_;

	//天球
	delete modelSkydome_;
	delete skydome_;

	//マップチップの解放
	delete mapChipField_;
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical=mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	
	// 要素数を変更する
	worldTransformBlocks_.resize(numBlockVirtical);

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlockHorizontal);
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kblock) {

				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j]=worldTransform;
				worldTransformBlocks_[i][j]->translation_= mapChipField_->GetMapChipPositionByIndex(j,i);
			}
		}
	}
}
