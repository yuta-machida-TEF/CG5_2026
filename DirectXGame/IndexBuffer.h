#pragma once

#include<d3d12.h>//ID3D12Resource

class IndexBuffer {
public:
	// IndexBuffer生成
	void Create(const UINT size, const UINT stride);

	// ゲッター
	ID3D12Resource* Get();              // インデックスバッファ
	D3D12_INDEX_BUFFER_VIEW* GetView(); // インデックスバッファビュー

	IndexBuffer();  // コンストラクタ
	~IndexBuffer(); // デストラクタ

private:
	ID3D12Resource* indexBuffer_ = nullptr; // インデックスバッファ
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};//インデックスバッファビュー

};
