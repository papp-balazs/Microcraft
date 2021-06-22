#pragma once

#include "WinConfig.h"
#include "CustomException.h"
#include "DxgiInfoManager.h"

#include <wrl.h>
#include <string>
#include <vector>
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

class Graphics
{
public:
	Graphics(HWND hWnd, size_t width, size_t height);
	Graphics(Graphics&) = delete;
	~Graphics();
	Graphics operator=(Graphics&) = delete;
	void SetCamera(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;
	void EndFrame();
	void DrawIndexed(UINT count) noexcept;
	void SetProjection(DirectX::FXMMATRIX proc) noexcept;
	void BeginFrame(float red, float green, float blue) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
#ifdef _DEBUG
	DxgiInfoManager& GetInfoManager();
#endif
private:
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;
	size_t width;
	size_t height;
#ifdef _DEBUG
	DxgiInfoManager infoManager;
#endif
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
};