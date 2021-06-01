#pragma once

namespace DX
{
	__interface IDeviceNotify
	{
		void OnDeviceLost();
		void OnDeviceRestored();
	};

	class DeviceResources
	{
	public:
		DeviceResources() noexcept;
		~DeviceResources() = default;

		DeviceResources(DeviceResources&&) = default;
		DeviceResources& operator= (DeviceResources&&) = default;

		DeviceResources(DeviceResources const&) = delete;
		DeviceResources& operator= (DeviceResources const&) = delete;

		auto			GetDXGIFactory()			const	noexcept		{ return mDxgiFactory.Get(); }
		auto			GetDXGISwapChain()			const	noexcept		{ return mDxgiSwapChain.Get(); }

		auto			GetD3DDevice()				const	noexcept		{ return mD3DDevice.Get(); }
		auto			GetD3DDeviceContext()		const	noexcept		{ return mD3DDeviceContext.Get(); }

		auto			GetD3DRenderTarget()		const	noexcept		{ return mD3DRenderTarget.Get(); }
		auto			GetD3DDepthStencil()		const	noexcept		{ return mD3DDepthStencil.Get(); }

		auto			GetD3DRenderTargetView()	const	noexcept		{ return mD3DRenderTargetView.Get(); }
		auto			GetD3DDepthStencilView()	const	noexcept		{ return mD3DDepthStencilView.Get(); }

		HWND			GetWindow()					const	noexcept		{ return mWindow; }
		RECT			GetOutputSize()				const	noexcept		{ return mOutputSize; }
		D3D11_VIEWPORT	GetScreenViewport()			const	noexcept		{ return mScreenViewport; }

		void			CreateDeviceResoruces()							noexcept;
		void			CreateWindowSizeDependentResources()			noexcept;
		void			SetWindow(HWND window, int width, int height)	noexcept;
		bool			WindowSizeChanged(int width, int height)		noexcept;
		void			HandleDeviceLost()								noexcept;
		void			RegisterDeviceNotify(IDeviceNotify* device)		noexcept;
		void			Present()										noexcept;

		// Performance events
		void PIXBeginEvent(_In_z_ const wchar_t* name)
		{
			mD3DAnnotation->BeginEvent(name);
		}

		void PIXEndEvent()
		{
			mD3DAnnotation->EndEvent();
		}

		void PIXSetMarker(_In_z_ const wchar_t* name)
		{
			mD3DAnnotation->SetMarker(name);
		}

	private:
		void	CreateFactory()									noexcept;
		void	GetHardwareAdapter(IDXGIAdapter1** ppAdapter)	noexcept;

		// DXGI objects.
		Microsoft::WRL::ComPtr<IDXGIFactory2>				mDxgiFactory;
		Microsoft::WRL::ComPtr<IDXGISwapChain1>				mDxgiSwapChain;
		
		// Direct3D objects.
		Microsoft::WRL::ComPtr<ID3D11Device1>				mD3DDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1>		mD3DDeviceContext;
		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation>	mD3DAnnotation;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>				mD3DRenderTarget;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				mD3DDepthStencil;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		mD3DRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		mD3DDepthStencilView;
		D3D11_VIEWPORT										mScreenViewport;

		// Direct3D properties.
		DXGI_FORMAT											mBackBufferFormat;
		DXGI_FORMAT											mDepthBufferFormat;

		// Windows objects.
		HWND												mWindow;
		RECT												mOutputSize;

		D3D_FEATURE_LEVEL									mD3DFeatureLevel;

		IDeviceNotify*										mDeviceNotify;
	};
}

extern std::unique_ptr<DX::DeviceResources>	gDeviceResources;