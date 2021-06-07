#pragma once
#include "../../framework.h"

enum class ObjectType : uint8_t
{
	DEFAULT = 0U,
	JETBOT,
	CONTROLLER,
	XAVIER,
};

class SocketAddress;
class BotLineObject
{
public:
	BotLineObject(const SocketAddress& address) noexcept :
		mType(ObjectType::DEFAULT),
		mSocketAddress(address),
		mLastPacketTime(std::chrono::system_clock::now()),
		mLastMessageType(MessageType::DEFAULT) {}
	virtual ~BotLineObject() = default;

	const	ObjectType&		GetObjectType()			const				noexcept	{ return mType; }
			void			SetObjectType(const ObjectType& type)		noexcept	{ mType = type;}

	const	std::string&	GetObjectName()			const				noexcept	{ return mObjectName; }
			void			SetObjectName(const std::string& name)		noexcept	{ mObjectName = name; }

	const	SocketAddress&	GetSocketAddress()		const				noexcept	{ return mSocketAddress; }
	const	SystemTime&		GetLastPacketTime()		const				noexcept	{ return mLastPacketTime; }

			void			UpdateLastPacketTime()						noexcept	{ mLastPacketTime = std::chrono::system_clock::now();}

			const			MessageType& GetLastMessageType() const		noexcept { return mLastMessageType; }
			void			SetLastMessageType(const MessageType& type)	noexcept { mLastMessageType = type; }

protected:
	ObjectType		mType;
	std::string		mObjectName;
	SocketAddress	mSocketAddress;
	SystemTime		mLastPacketTime;
	MessageType		mLastMessageType;
};