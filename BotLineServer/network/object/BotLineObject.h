#pragma once
#include "../../framework.h"

enum class ObjectType : uint8_t
{
	DEFAULT = 0U,
	JETBOT,
	CONTROLLER,
};

class SocketAddress;
class BotLineObject
{
public:
	BotLineObject(const SocketAddress& address) noexcept :
		mType(ObjectType::DEFAULT),
		mSocketAddress(address),
		mLastPacketTime(std::chrono::system_clock::now()) {}
	virtual ~BotLineObject() = default;

	const	ObjectType&		GetObjectType()			const			noexcept	{ return mType; }
			void			SetObjectType(const ObjectType& type)	noexcept	{ mType = type;}

	const	SocketAddress&	GetSocketAddress()		const			noexcept	{ return mSocketAddress; }
	const	SystemTime&		GetLastPacketTime()		const			noexcept	{ return mLastPacketTime; }

			void			UpdateLastPacketTime()					noexcept	{ mLastPacketTime = std::chrono::system_clock::now();}
protected:
	ObjectType		mType;
	SocketAddress	mSocketAddress;
	SystemTime		mLastPacketTime;
};