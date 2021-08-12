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
	void		OnUpdate(const Util::Timer& timer)			noexcept;
	void		OnLateUpdate(const Util::Timer& timer)		noexcept;
	void		OnRender(const Util::Timer& timer)			noexcept;
	void		Clear();

	void		CreateDeviceDependentResources();
	void		CreateWindowSizeDependentResources();

	void		SetupImGui(const HWND& window)		noexcept;
	void		CreateObjects()						noexcept;


	Util::Timer								mTimer;
	double									mCheckingDelay;

	Util::ObjectCollection					mObjectCollection;
	Util::SharedContext						mSharedContext;

	std::shared_ptr<UI::UIManager>			mUIManager;

	//std::shared_ptr<NetworkManager>			mNetworkManager;
	//std::shared_ptr<DialogManager>			mDialogManager;
};