#pragma once
#include "stdafx.h"

class SocketAddress
{
public:
	SocketAddress() noexcept {
		this->GetSockAddrIn()->sin_family = AF_INET;
		this->GetSockAddrIn()->sin_addr.s_addr = INADDR_ANY;
		this->GetSockAddrIn()->sin_port = 0;
	}

	SocketAddress(uint32_t address, uint16_t port) noexcept {
		this->GetSockAddrIn()->sin_family = AF_INET;
		this->GetSockAddrIn()->sin_addr.s_addr = htonl(address);
		this->GetSockAddrIn()->sin_port = htons(port);
	}

	SocketAddress(const SOCKADDR& sockAddr) noexcept {
		memcpy_s(&this->mSockAddr, sizeof(SOCKADDR), &sockAddr, sizeof(SOCKADDR));
	}

	std::string ToString() const noexcept {
		const SOCKADDR_IN* addr = this->GetSockAddrIn();
		char dest[128] = { 0 };

		inet_ntop(AF_INET, const_cast<IN_ADDR*>(&addr->sin_addr), dest, sizeof(dest));

		std::stringstream ss{};
		ss << dest << ':' << ntohs(addr->sin_port);

		return ss.str();
	}

	const	size_t			GetSockAddrSize()	const	noexcept	{ return sizeof(mSockAddr); }
														 
	const	SOCKADDR&		GetSockAddr()		const	noexcept	{ return mSockAddr; }
			SOCKADDR&		GetSockAddr()				noexcept	{ return mSockAddr; }
														 
private:												 
	const	uint32_t&		GetIPv4Ref()		const	noexcept	{ return *reinterpret_cast<const uint32_t*>(&this->GetSockAddrIn()->sin_addr.s_addr); }
			uint32_t&		GetIPv4Ref()				noexcept	{ return *reinterpret_cast<uint32_t*>(&this->GetSockAddrIn()->sin_addr.s_addr); }
														 
	const	SOCKADDR_IN*	GetSockAddrIn()		const	noexcept	{ return reinterpret_cast<const SOCKADDR_IN*>(&this->mSockAddr); }
			SOCKADDR_IN*	GetSockAddrIn()				noexcept	{ return reinterpret_cast<SOCKADDR_IN*>(&this->mSockAddr); }

	SOCKADDR	mSockAddr;
};