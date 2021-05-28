#pragma once
#include "stdafx.h"

class BotLine final
{
public:
	BotLine() noexcept;
	~BotLine();

	static	constexpr	WORD	sVersionRequested = MAKEWORD(2, 2);

	void	Initialize()	noexcept(false);

	void	Tick()			noexcept;

private:
	void	OnUpdate(const Utility::Timer& timer)		noexcept;
	void	OnLateUpdate(const Utility::Timer& timer)	noexcept;

	void	OnDestory()		noexcept;

	std::unique_ptr<NetworkManager>		mNetworkManager;

	Utility::Timer						mTimer;
};