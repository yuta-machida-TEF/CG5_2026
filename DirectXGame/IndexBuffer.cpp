#include "IndexBuffer.h"
#include"KamataEngine.h"

#include<d3d12.h>
#include<cassert>

using namespace KamataEngine;

//生成
void IndexBuffer::Create(const UINT size, const UINT stride)
{
   //strideの値によって、1つのインデックスのフォーマットを決める
	assert(stride == 2 || stride == 4); //2byte or 4byteのみ受け付ける
   DXGI_FORMAT format = (stride == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;

   //クラス
   DirectXCommon* dxCommon = DirectXCommon::GetInstance();

   //インデックスリソースの生成
   //  頂点リソース用のヒープの設定
   D3D12_HEAP_PROPERTIES uploadHeapProperties{};
   uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // CPUから書き込むヒープ
   // 頂点リソースの設定
   D3D12_RESOURCE_DESC vertexResourceDesc{};
   vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
   vertexResourceDesc.Width = size;
   // バッファの場合はこれらは1にする決まり
   vertexResourceDesc.Height = 1;
   vertexResourceDesc.DepthOrArraySize = 1;
   vertexResourceDesc.MipLevels = 1;
   vertexResourceDesc.SampleDesc.Count = 1;
   // バッファの場合はこれにする決まり
   vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

   // 実際に頂点リソースを生成する
   ID3D12Resource* vertexResource = nullptr;
   [[maybe_unused]]
   HRESULT hr =
	   dxCommon->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
   assert(SUCCEEDED(hr));

   // 生成した頂点リーソスをとっておく
   indexBuffer_ = vertexResource;

   // VertexBufferViewを作成する
   D3D12_INDEX_BUFFER_VIEW indexBufferView{};
   // リソースの先頭アドレスから使う
   indexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
   // 使用するリソースのサイズは頂点sizeのサイズ
   indexBufferView.SizeInBytes = size;
   //// 1つの頂点のサイズ
   //vertexBufferView.StrideInBytes = stride;

   //インデックスのフォーマット
   indexBufferView.Format = format;//インデックス1つ分のサイズ

   // VertexBufferViewをとっておく
   indexBufferView_ = indexBufferView;

}

//生成したインデックスバッファを返す
ID3D12Resource* IndexBuffer::Get()
{
	return indexBuffer_; 
}

//用意済みのインデックスバッファビューを返す
D3D12_INDEX_BUFFER_VIEW* IndexBuffer::GetView()
{ 
	return &indexBufferView_; 
}

//コンストラクタ
IndexBuffer::IndexBuffer()
{
}

//デストラクタ
IndexBuffer::~IndexBuffer()
{
	if (indexBuffer_) 
	{
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
}