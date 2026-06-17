#include "PipelineState.h"
#include"KamataEngine.h"

using namespace KamataEngine;

//PipelineStateを生成する
void PipelineState::Create(D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicPipelineStateDesc)
{
	//クラス内で取得するために追加
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	ID3D12PipelineState* graphicsPipleLineState = nullptr;
	HRESULT hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(
		         &graphicPipelineStateDesc, IID_PPV_ARGS(&graphicsPipleLineState));

    assert(SUCCEEDED(hr));

	//生成した PipelineStateをとっておく
	pipelineState_ = graphicsPipleLineState;

}





