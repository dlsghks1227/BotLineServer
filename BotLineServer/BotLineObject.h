#pragma once
#include "stdafx.h"

class SocketAddress;
class BotLineObject
{
public:
	BotLineObject(const SocketAddress& address) noexcept :
		mSocketAddress(address),
		mLastPacketTime(std::chrono::system_clock::now()) {}
	virtual ~BotLineObject() = default;

			void				UpdateLastPacketTime()		noexcept	{ mLastPacketTime = std::chrono::system_clock::now();}
	const	SystemTime&			GetLastPacketTime()	const	noexcept	{ return mLastPacketTime; }

	const	SocketAddress&		GetSocketAddress()	const	noexcept	{ return mSocketAddress; }

private:
	SocketAddress	mSocketAddress;
	SystemTime		mLastPacketTime;
};