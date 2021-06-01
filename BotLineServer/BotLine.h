#pragma once
#include "framework.h"
#include "DeviceResources.h"


class BotLine : public DX::IDeviceNotify
{
public:
	BotLine() noexcept;
	~BotLine();

	// delete / default Å°¿öµå
	// https://blankspace-dev.tistory.com/350
	BotLine(BotLine&&) = default;
	BotLine& operator = (BotLine&&) = default;

	BotLine(BotLine const&) = delete;
	BotLine& operator = (BotLine const&) = delete;

	static	constexpr	WORD	sVersionRequested = MAKEWORD(2, 2);
	static	constexpr	double	sCheckCycle = 1.0;

	void		Initialize(HWND window, int width, int height)	noexcept;
	void		Tick()											noexcept;

	void		OnDeviceLost()		override;
	void		OnDeviceRestored()	override;

	void		OnActivated();
	void		OnDeactivated();
	void		OnSuspending();
	void		OnResuming();
	void		OnWindowMoved()									noexcept;
	void		OnWindowSizeChanged(int width, int height)		noexcept;

	void		GetDefaultSize(int& width, int& height) const	noexcept;


private:
	void		OnUpdate(const Utility::Timer& timer)			noexcept;
	void		OnLateUpdate(const Utility::Timer& timer)		noexcept;
	void		OnRender(const Utility::Timer& timer)			noexcept;
	void		Clear();

	void		CreateDeviceDependentResources();
	void		CreateWindowSizeDependentResources();


	Utility::Timer						mTimer;
	double								mCheckingDelay;

	std::unique_ptr<NetworkManager>		mNetworkManager;
};