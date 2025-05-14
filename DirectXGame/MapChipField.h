#pragma once
#include <cstdint>
#include<string>
#include <vector>
#include <map>
#include <fstream>
#include<sstream>
#include "KamataEngine.h"
#include "Math.h"

enum class MapChipType {
	kblank,  //空白
	kblock,  //ブロック
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
public:
	//1ブッロクのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	
	void ResetMapChipData();
	
	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);


private:

	//ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

};
