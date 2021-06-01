#include "pch.h"
#include "DeviceResources.h"

std::unique_ptr<DX::DeviceResources>	gDeviceResources = std::make_unique<DX::DeviceResources>();

DX::DeviceResources::DeviceResources() noexcept :
	mScreenViewport{},
	mBackBufferFormat(DXGI_FORMAT_B8G8R8A8_UNORM),
	mDepthBufferFormat(DXGI_FORMAT_D32_FLOAT),
	mWindow(nullptr),
	mD3DFeatureLevel(D3D_FEATURE_LEVEL_9_1),
	mOutputSize{0, 0, 1, 1},
	mDeviceNotify(nullptr)
{
}

void DX::DeviceResources::CreateDeviceResoruces() noexcept
{
	// API 기본값과 다른 색상 채널 순서를 가진 표면에 대한 지원을 추가합니다.
	// Direct3D 리소스와 Direct2D 상호 호환성을 위해 필요
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	CreateFactory();

	static const D3D_FEATURE_LEVEL sFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
	GetHardwareAdapter(adapter.GetAddressOf());

	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		deviceContext;

	HRESULT hr = E_FAIL;
	if (adapter)
	{
		hr = D3D11CreateDevice(
			adapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			creationFlags,
			sFeatureLevels,
			ARRAYSIZE(sFeatureLevels),
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&mD3DFeatureLevel,
			deviceContext.GetAddressOf()
		);
	}

	if (FAILED(hr))
	{
		hr = D3D11CreateDevice(
			adapter.Get(),
			D3D_DRIVER_TYPE_WARP,
			nullptr,
			creationFlags,
			sFeatureLevels,
			ARRAYSIZE(sFeatureLevels),
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&mD3DFeatureLevel,
			deviceContext.GetAddressOf()
		);
	}

	DX::ThrowIfFailed(device.As(&mD3DDevice));
	DX::ThrowIfFailed(deviceContext.As(&mD3DDeviceContext));
	DX::ThrowIfFailed(deviceContext.As(&mD3DAnnotation));
}

// 윈도우가 변경될 때 마다 다시 그려준다.
void DX::DeviceResources::CreateWindowSizeDependentResources() noexcept
{
	// 초기화
	ID3D11RenderTargetView* nullViews[] = { nullptr };

	mD3DDeviceContext->OMSetRenderTargets(static_cast<UINT>(std::size(nullViews)), nullViews, nullptr);
	mD3DRenderTargetView.Reset();
	mD3DDepthStencilView.Reset();
	mD3DRenderTarget.Reset();
	mD3DDepthStencil.Reset();
	mD3DDeviceContext->Flush();

	// Back buffer Size
	const UINT backBufferWidth	= std::max<UINT>(static_cast<UINT>(mOutputSize.right  - mOutputSize.left), 1u);
	const UINT backBufferHeight = std::max<UINT>(static_cast<UINT>(mOutputSize.bottom - mOutputSize.top),  1u);

	// 스왑 체인이 생성 된 경우
	if (mDxgiSwapChain)
	{
		HRESULT hr = mDxgiSwapChain->ResizeBuffers(
			2,
			backBufferWidth,
			backBufferHeight,
			mBackBufferFormat,
			0u
		);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			HandleDeviceLost();
			return;
		}
		else
		{
			ThrowIfFailed(hr);
		}
	}
	else
	{
		// Swap Chain
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = backBufferWidth;
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = mBackBufferFormat;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;	// 이중 or 삼중 버퍼링 설정
		swapChainDesc.SampleDesc.Count = 1;	// 멀티 샘플링 사용하지 않음
		swapChainDesc.SampleDesc.Quality = 0;	// 품질 수준 0
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapChainDesc.Flags = 0u;

		// 전체 화면에 대한 Swap Chain
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC	fullscreenSwapChainDesc = {};
		fullscreenSwapChainDesc.Windowed = TRUE;

		// Win32 window으로부터 스왑체인 생성
		DX::ThrowIfFailed(mDxgiFactory->CreateSwapChainForHwnd(
			mD3DDevice.Get(),
			mWindow,
			&swapChainDesc,
			&fullscreenSwapChainDesc,
			nullptr,
			mDxgiSwapChain.ReleaseAndGetAddressOf()
		));

		ThrowIfFailed(mDxgiFactory->MakeWindowAssociation(mWindow, DXGI_MWA_NO_ALT_ENTER));
	}

	// 스왑체인의 0번째 버퍼
	DX::ThrowIfFailed(mDxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(mD3DRenderTarget.ReleaseAndGetAddressOf())));

	// Back buffer 렌더링
	CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2D, mBackBufferFormat);
	DX::ThrowIfFailed(mD3DDevice->CreateRenderTargetView(
		mD3DRenderTarget.Get(),
		&renderTargetViewDesc,
		mD3DRenderTargetView.ReleaseAndGetAddressOf()
	));

	// 깊이 스텐스 설정
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		mDepthBufferFormat,
		backBufferWidth,
		backBufferHeight,
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
	);

	DX::ThrowIfFailed(mD3DDevice->CreateTexture2D(
		&depthStencilDesc,
		nullptr,
		mD3DDepthStencil.ReleaseAndGetAddressOf()
	));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(mD3DDevice->CreateDepthStencilView(
		mD3DDepthStencil.Get(),
		&depthStencilViewDesc,
		mD3DDepthStencilView.ReleaseAndGetAddressOf()
	));

	mScreenViewport = CD3D11_VIEWPORT(
		0.0f,
		0.0f,
		static_cast<float>(backBufferWidth),
		static_cast<float>(backBufferHeight)
	);
}

void DX::DeviceResources::SetWindow(HWND window, int width, int height) noexcept
{
	mWindow = window;

	mOutputSize.left	= mOutputSize.top = 0;
	mOutputSize.right	= width;
	mOutputSize.bottom	= height;
}

bool DX::DeviceResources::WindowSizeChanged(int width, int height) noexcept
{
	RECT rect{ 0, 0, width, height };
	if (rect == mOutputSize)
	{
		return false;
	}
	mOutputSize = rect;

	CreateWindowSizeDependentResources();

	return true;
}

void DX::DeviceResources::HandleDeviceLost() noexcept
{
	if (mDeviceNotify)
	{
		mDeviceNotify->OnDeviceLost();
	}

	// 생성한 반대로 리셋
	mD3DDepthStencilView.Reset();
	mD3DRenderTargetView.Reset();
	mD3DDepthStencil.Reset();
	mD3DRenderTarget.Reset();
	mDxgiSwapChain.Reset();
	mD3DDeviceContext.Reset();
	mD3DAnnotation.Reset();

	mD3DDevice.Reset();
	mDxgiFactory.Reset();

	CreateDeviceResoruces();
	CreateWindowSizeDependentResources();

	if (mDeviceNotify)
	{
		mDeviceNotify->OnDeviceRestored();
	}
}

void DX::DeviceResources::RegisterDeviceNotify(IDeviceNotify* device) noexcept
{
	mDeviceNotify = device;
}

void DX::DeviceResources::Present() noexcept
{
	HRESULT hr = mDxgiSwapChain->Present(1, 0);

	mD3DDeviceContext->DiscardView(mD3DRenderTargetView.Get());
	mD3DDeviceContext->DiscardView(mD3DDepthStencilView.Get());

	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		HandleDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
		if (!mDxgiFactory->IsCurrent())
		{
			CreateFactory();
		}
	}
}

void DX::DeviceResources::CreateFactory() noexcept
{
	// DXGI 팩토리 생성
	DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(mDxgiFactory.ReleaseAndGetAddressOf())));
}

void DX::DeviceResources::GetHardwareAdapter(IDXGIAdapter1** ppAdapter) noexcept
{
	// 연결된 하드웨어 정보
	*ppAdapter = nullptr;

	Microsoft::WRL::ComPtr<IDXGIAdapter1>	adapter;
	Microsoft::WRL::ComPtr<IDXGIFactory6>	factory6;
	HRESULT hr = mDxgiFactory.As(&factory6);
	if (SUCCEEDED(hr))
	{
		// 최대 성능 또는 최소 전력별로 어댑터 열거
		for (UINT adapterIndex = 0;
			SUCCEEDED(factory6->EnumAdapterByGpuPreference(
				adapterIndex, 
				DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, 
				IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf())));
			adapterIndex++)
		{
			DXGI_ADAPTER_DESC1 desc;
			DX::ThrowIfFailed(adapter->GetDesc1(&desc));

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			break;
		}
	}

	*ppAdapter = adapter.Detach();
}