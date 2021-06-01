#include "pch.h"
#include "BotLine.h"

BotLine::BotLine() noexcept : 
	mCheckingDelay(0.0)
{
	gDeviceResources->RegisterDeviceNotify(this);
	mNetworkManager = std::make_unique<NetworkManager>();
	mTimer = Utility::Timer();
}

BotLine::~BotLine()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	WSACleanup();
}

void BotLine::Initialize(HWND window, int width, int height) noexcept
{
	// Network Initialize
	WSADATA wsaData{ 0 };
	int result = WSAStartup(sVersionRequested, &wsaData);
	if (result != 0)
	{
		return;
	}

	mTimer.SetFixedTimeStep(true);
	mNetworkManager->Initialize();

	gDeviceResources->SetWindow(window, width, height);

	gDeviceResources->CreateDeviceResoruces();
	gDeviceResources->CreateWindowSizeDependentResources();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(window);
	auto device = gDeviceResources->GetD3DDevice();
	auto deviceContext = gDeviceResources->GetD3DDeviceContext();
	ImGui_ImplDX11_Init(device, deviceContext);
}

void BotLine::Tick() noexcept
{
	mTimer.Tick([this]()
		{
			this->OnUpdate(this->mTimer);
			this->OnLateUpdate(this->mTimer);
		}
	);

	OnRender(mTimer);
}

void BotLine::OnDeviceLost()
{
}

void BotLine::OnDeviceRestored()
{
}

void BotLine::OnActivated()
{
}

void BotLine::OnDeactivated()
{
}

void BotLine::OnSuspending()
{
}

void BotLine::OnResuming()
{
}

void BotLine::OnWindowMoved() noexcept
{
	auto rect = gDeviceResources->GetOutputSize();
	gDeviceResources->WindowSizeChanged(rect.right, rect.bottom);
}

void BotLine::OnWindowSizeChanged(int width, int height) noexcept
{
	if (!gDeviceResources->WindowSizeChanged(width, height))
		return;

	CreateDeviceDependentResources();
}

void BotLine::GetDefaultSize(int& width, int& height) const noexcept
{
	width = 1920;
	height = 1080;
}

void BotLine::OnUpdate(const Utility::Timer& timer) noexcept
{
	// 패킷 처리
	mNetworkManager->ProcessIncomingPackets(timer);
}

void BotLine::OnLateUpdate(const Utility::Timer& timer) noexcept
{
	// 연결 확인
	mNetworkManager->CheckForDisconnect();

	if (mCheckingDelay >= sCheckCycle)
	{
		mNetworkManager->VerifyConnection();
		mCheckingDelay = 0.0;
	}
	mCheckingDelay += timer.GetElapsedSeconds();
}

void BotLine::OnRender(const Utility::Timer& timer) noexcept
{
	Clear();

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	mNetworkManager->OnRender(timer);

	ImPlot::ShowDemoWindow();
	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	gDeviceResources->Present();
}

void BotLine::Clear()
{
	gDeviceResources->PIXBeginEvent(L"Clear");

	// Clear the views.
	auto context = gDeviceResources->GetD3DDeviceContext();
	auto renderTargetView = gDeviceResources->GetD3DRenderTargetView();
	auto depthStencilView = gDeviceResources->GetD3DDepthStencilView();

	context->ClearRenderTargetView(renderTargetView, DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// Set the viewport.
	auto viewport = gDeviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	gDeviceResources->PIXEndEvent();
}

void BotLine::CreateDeviceDependentResources()
{
}

void BotLine::CreateWindowSizeDependentResources()
{
}
