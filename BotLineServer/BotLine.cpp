#include "stdafx.h"
#include "BotLine.h"

BotLine::BotLine() noexcept
{
	mNetworkManager = std::make_unique<NetworkManager>();
}

BotLine::~BotLine()
{
	this->OnDestory();
}

void BotLine::Initialize() noexcept(false)
{
	WSADATA wsaData{ 0 };
	int result = WSAStartup(sVersionRequested, &wsaData);
	if (result != 0)
	{
		Utility::Throw(result);
	}

	mTimer = Utility::Timer();
	mTimer.SetFixedTimeStep(true);

	mNetworkManager->Initialize();
}

void BotLine::Tick() noexcept
{
	mTimer.Tick([this]()
		{
			this->OnUpdate(this->mTimer);
			this->OnLateUpdate(this->mTimer);
		}
	);
}

void BotLine::OnUpdate(const Utility::Timer& timer) noexcept
{
	// 패킷 처리
	mNetworkManager->ProcessIncomingPackets();
}

void BotLine::OnLateUpdate(const Utility::Timer& timer) noexcept
{
	// 연결된 기기 alive test
}

void BotLine::OnDestory() noexcept
{
	WSACleanup();
}
