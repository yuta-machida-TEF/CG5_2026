#include <Windows.h>
#include"KamataEngine.h"
#include"Shader.h"
#include"RootSignature.h"
#include"PipelineState.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
using namespace KamataEngine; 

//関数プロとタイプ宣言
void SetupPiPelineState(PipelineState& pipelineState, RootSignature& rs, Shader& vs, Shader& ps)
{
	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
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

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStaticDesc{};
	graphicsPipelineStaticDesc.pRootSignature = rs.Get();
	graphicsPipelineStaticDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStaticDesc.VS = {vs.GetDxcBlob()->GetBufferPointer(), vs.GetDxcBlob()->GetBufferSize()};
	graphicsPipelineStaticDesc.PS = {ps.GetDxcBlob()->GetBufferPointer(), ps.GetDxcBlob()->GetBufferSize()};
	graphicsPipelineStaticDesc.BlendState = blendDesc;
	graphicsPipelineStaticDesc.RasterizerState = rasterizerDesc;

	// 書き込むRTVの情報
	graphicsPipelineStaticDesc.NumRenderTargets = 1;
	graphicsPipelineStaticDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ(形状)タイプ。三角形
	graphicsPipelineStaticDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込みかの設定
	graphicsPipelineStaticDesc.SampleDesc.Count = 1;
	graphicsPipelineStaticDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// 準備を整った。PSOを生成する
	pipelineState.Create(graphicsPipelineStaticDesc);

	//RenderTextrueResourceの生成
	ID3D12Resource* CreateRenderTextureResource(ID3D12Device * device, uint32_t width, uint32_t height,
	DXGI_FORMAT format, const FLOAT* clearColor);

}

// RenderTextureResourceの生成
ID3D12Resource* CreateRenderTextureResource(ID3D12Device* device, uint32_t width, uint32_t height, DXGI_FORMAT clearFormat, const FLOAT* clearColor) {
	// 1.生成するRederTextureの Descの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(width);                             // RenderTextureの幅
	resourceDesc.Height = UINT(height);                           // RenderTextureの高さ
	resourceDesc.MipLevels = 1;                                   // mipmapの数
	resourceDesc.DepthOrArraySize = 1;                            // 奥行 or 配列Textureceの配列数
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;        // TextureのFormat
	resourceDesc.SampleDesc.Count = 1;                            // サンプリングカウント 1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;  // Textrueの次元数。普通使っているのは 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // RenderTargetとして使う通知

	// 2利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	// 3. ClaarValueの用意
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = clearFormat;
	clearValue.Color[0] = clearColor[0];
	clearValue.Color[1]	= clearColor[1];
	clearValue.Color[2]	= clearColor[2];
	clearValue.Color[3]	= clearColor[3];

	//4. RenderTextureResourceの生成
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
	    &heapProperties, // Heapの設定
	    D3D12_HEAP_FLAG_NONE,//Heapの特殊な設定
		&resourceDesc,//Resourceの設定
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,//Pixel Shader でアクセスできるようにする
		&clearValue,//Clear最適値
		IID_PPV_ARGS(&resource)//作成するResourceポインタへのポインタ
	);

	assert(SUCCEEDED(hr));

	return resource;
}

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
	RootSignature rs;
	rs.Create();


	//頂点シェーダー
	Shader vs;
	vs.LoadDxc(L"Resources/shaders/TestVS.hlsl", L"vs_6_0");
	assert(vs.GetDxcBlob() != nullptr);

	
	// ピクセルシェーダーの読み込みとコンパイル
	Shader ps;
	ps.LoadDxc(L"Resources/shaders/TestPS.hlsl", L"ps_6_0");
	assert(ps.GetDxcBlob() != nullptr);

	//PipelineState作成
	PipelineState peipelineState;
	SetupPiPelineState(peipelineState, rs, vs, ps);

	//リソースの確保含む
	//Vertex4 => VertexData に変更して利用する
	struct VertexData 
	{
		Vector4 position;
		Vector2 texcoord;
	};

	// 頂点インデックスデータを準備
	uint16_t indices[] = 
	{
	    0,1,2,
		2,1,3
	};

	//頂点データの準備
	VertexData vertices[] = {

		{{-1.0f,  1.0f, 0.0f,1.0f},{0.0f, 0.0f}}, //  左上
	    {{ 1.0f,  1.0f, 0.0f,1.0f},{1.0f, 0.0f}}, //  右上
	    {{-1.0f, -1.0f, 0.0f,1.0f},{0.0f, 1.0f}}, //  左下
	    {{ 1.0f, -1.0f, 0.0f,1.0f},{1.0f, 1.0f}}, //  右下
	    	
	};

	// IndexBuffer(VertexResource,VertexResourceView)の作成
	VertexBuffer vb;
	vb.Create(sizeof(vertices), sizeof(vertices[0]));

	//頂点リソースにデータを書き込み
	VertexData* pGpuVertices = nullptr;
	vb.Get()->Map(0, nullptr, reinterpret_cast<void**>(&pGpuVertices));

	for (int i = 0; i < _countof(vertices); i++) 
	{
		pGpuVertices[i] = vertices[i];
	}

	//IndexBuffer(VertexResource,VertexResourceView)の作成
	IndexBuffer ib;
	ib.Create(sizeof(indices), sizeof(indices[0]));

	//頂点リソースにデータを書き込み
	uint16_t* pGpuIndices = nullptr;
	ib.Get()->Map(0, nullptr, reinterpret_cast<void**>(&pGpuIndices));

	for (int i = 0; i < _countof(indices); i++) 
	{
		pGpuIndices[i] = indices[i];
	}

	//Resource生成、Heap生成、View生成で再利用される変数の準備
	ID3D12Device* device = dxCommon->GetInstance();

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

		//コマンドを積む
		commandList->SetGraphicsRootSignature(rs.Get());//RootSignatureの設定
		commandList->SetPipelineState(peipelineState.Get());//PSOの設定する
		commandList->IASetVertexBuffers(0, 1, vb.GetView());//VBVの設定する
		commandList->IASetIndexBuffer(ib.GetView());//IBVの設定する
		//トポロジの設定
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//commandList->DrawInstanced(3, 1, 0, 0); //頂点数, インデックス数, インデックスの開始位置, インデックスのオフセット
		commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);

		//描画終了
		dxCommon->PostDraw();

	}


	//エンジンの終了処理
	KamataEngine::Finalize();
	
	return 0;
}