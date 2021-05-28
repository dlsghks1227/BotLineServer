#pragma once
#include "stdafx.h"

class SocketAddress;
class BotLineObject
{
public:
	BotLineObject() noexcept :
		mLastPacketTime(0.0) {}
	virtual ~BotLineObject() = default;

			void				UpdateLastPacketTime()		noexcept	{}

	const	SocketAddress&		GetSocketAddress()	const	noexcept	{ return mSocketAddress; }

private:
	SocketAddress		mSocketAddress;
	double				mLastPacketTime;
};