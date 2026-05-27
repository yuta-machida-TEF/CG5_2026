#include "Shader.h"
#include<d3dcompiler.h>//D3DCompileFromFile
#include<cassert>//assert

void Shader::Load(const std::wstring& filePath, const std::wstring& shaderModel) { 
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	//wsstring => string 文字列変換
	std::string mbShaderModel = "vs_5_0";

	HRESULT hr = D3DCompileFromFile(
	    filePath.c_str(), // シェーダーファイル名
	    nullptr,
	    D3D_COMPILE_STANDARD_FILE_INCLUDE,               // インクルード可能にする
	    "main", mbShaderModel.c_str(),                   // エントリーポイント名、シェーダーモデル指定
	    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
	    0, &shaderBlob, &errorBlob);

	//エラーが発生した場合、止める
	if (FAILED(hr)) 
	{
		if (errorBlob) {
			OutputDebugStringA(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
			errorBlob->Release();
		}
		assert(false);
	}
	//生成したshaderBlobをとっておく
	blob_ = shaderBlob;

}

void Shader::LoadDxc(const std::wstring& filePath, const std::wstring& shaderModel) {}

// コンパイル済みのシェーダーデータを返す
ID3DBlob* Shader::GetBlob() 
{
	return blob_;
}

//コンストラクタ
Shader::Shader()
{
}

//デストラクタ
Shader::~Shader()
{
	if (blob_ != nullptr)
	{
		blob_->Release();
		blob_ = nullptr;
	}
}