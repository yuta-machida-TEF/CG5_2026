#pragma once
// WorldTransformクラスの利用
#include "C:\KamataEngine-CG5\External\KamataEngine\include\3d\WorldTransform.h"
#include "C:\KamataEngine-CG5\External\KamataEngine\include\math\MathUtility.h"

class WorldTransformEx : public KamataEngine::WorldTransform {
public:
	// Affine変換行列の生成と定数バッファへの転送を行う
	void UpdateMatrix();

	// Affine変換行列の生成
	KamataEngine::Matrix4x4 MakeAffineMatrix();
};