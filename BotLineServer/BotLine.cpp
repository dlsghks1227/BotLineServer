#include "stdafx.h"
#include "BotLine.h"

BotLine::BotLine() noexcept :
	mCheckDelay(0.0)
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
	// ��Ŷ ó��
	mNetworkManager->ProcessIncomingPackets(timer);
}

void BotLine::OnLateUpdate(const Utility::Timer& timer) noexcept
{
	// ���� Ȯ��
	if (mCheckDelay >= sCheckCycle)
	{
		mNetworkManager->SendForConnectCheck();
		mCheckDelay = 0.0;
	}

	// ����� ��� alive test
	mNetworkManager->CheckForDisconnect();

	mCheckDelay += timer.GetElapsedSeconds();
}

void BotLine::OnDestory() noexcept
{
	WSACleanup();
}
