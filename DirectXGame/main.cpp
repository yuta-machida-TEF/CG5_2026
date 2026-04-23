#include <Windows.h>
#include"KamataEngine.h"
#include<d3dcompiler.h>
using namespace KamataEngine; 

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	// エンジンの初期化
	KamataEngine::Initialize(L"LE3D_25_マチダ_ユウタ");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// DirectXCommonクラスが管理している、ウィンドウの幅と高さの値の取得
	int32_t w = dxCommon->GetBackBufferWidth();
	int32_t h = dxCommon->GetBackBufferHeight();
	DebugText::GetInstance()->ConsolePrintf(std::format("width: height: {}\n", w, h).c_str());

	// DirectXCommonクラスが管理している、コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();

	// RootSignature作成
	// 構造体をデータを用意する
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlog = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlog);

	if (FAILED(hr)) {
		DebugText::GetInstance()->ConsolePrintf(reinterpret_cast<char*>(errorBlog->GetBufferPointer()));
		assert(false);
	}

	// パソナリをもとに作成
	ID3D12RootSignature* rootSignature = nullptr;
	hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSTION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// BlendState
	D3D12_BLEND_DESC blendDesc{};

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 表面(反時計回り)をカリングする
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// コンパイル
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	//頂点シェーダー
	std::wstring vsFile = L"Resource/shaders/TestVS.hlsl";
	hr = D3DCompileFromFile(
		vsFile.c_str(), 
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", 
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &vsBlob, &errorBlob);

	if (FAILED(hr)) 
	{
		DebugText::GetInstance()->ConsolePrintf(
			std::system_category().message(hr).c_str());
		if (errorBlob)
		{
			DebugText::GetInstance()->ConsolePrintf(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		}
		assert(false);
	}

	//PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStaticDesc{};
	graphicsPipelineStaticDesc.pRootSignature = rootSignature;
	graphicsPipelineStaticDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStaticDesc.VS = {vsBlob->GetBufferPointer(), vsBlob->GetBufferSize()};
	graphicsPipelineStaticDesc.PS = {psBlob->GetBufferPointer(), psBlob->GetBufferSize()};
	graphicsPipelineStaticDesc.BlendState = blendDesc;
	graphicsPipelineStaticDesc.RasterizerState = rasterizerDesc;



	//メインループ
	while (true)
	{
		//エンジンの更新
		if (KamataEngine::Update()) 
		{
			break;
		}

		// 描画開始
		dxCommon->PreDraw();

		//描画終了
		dxCommon->PostDraw();

	}


	//エンジンの終了処理
	KamataEngine::Finalize();
	
	return 0;
}
