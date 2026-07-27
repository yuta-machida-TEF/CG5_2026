#include "PipelineState.h"
#include"KamataEngine.h"
#include<cassert>

using namespace KamataEngine;

//PipelineStateを生成する
void PipelineState::Create(D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicPipelineStateDesc)
{
	//クラス内で取得するために追加
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	ID3D12PipelineState* graphicsPipleLineState = nullptr;
	[[maybe_unused]]
	HRESULT hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(
		         &graphicPipelineStateDesc, IID_PPV_ARGS(&graphicsPipleLineState));

    assert(SUCCEEDED(hr));

	//生成した PipelineStateをとっておく
	pipelineState_ = graphicsPipleLineState;

}

//生成した PipelineState を返す
ID3D12PipelineState* PipelineState::Get()
{ 
	return pipelineState_; 
}

//PipelineStateのコンストラクタ
PipelineState::PipelineState()
{
}

//PipelineStateのデストラクタ
PipelineState::~PipelineState()
{
	if (pipelineState_)
	{
		pipelineState_->Release();
		pipelineState_ = nullptr;
	}
}

