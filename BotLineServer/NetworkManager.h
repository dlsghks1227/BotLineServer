#pragma once
#include "stdafx.h"

class NetworkManager final
{
public:
	NetworkManager() noexcept :
		mBytesSentThisFrame(0),
		mDropPacketChance(0.0f),
		mSimulatedLatency(0.0f) {}
	~NetworkManager() = default;

	static	constexpr	uint16_t	sPort = 8000;
	static	constexpr	uint32_t	sBufferSize = 2048;

	void		Initialize(uint16_t inPort = 8000)	noexcept(false);
	void		ProcessIncomingPackets()			noexcept;
	
	void		SendPacket(const OutputMemoryBitStream& inOutputStream, const SocketAddress& inFromAddress) noexcept;
	
private:
	class ReceivedPacket {
	public:
		ReceivedPacket(float inReceivedTime, InputMemoryBitStream& inInputMemoryBitStream, const SocketAddress& inAddress) noexcept :
			mReceivedTime(inReceivedTime),
			mPacketBuffer(inInputMemoryBitStream),
			mFromAddress(inAddress) {}
			
			
		const	SocketAddress&			GetFromAddress()	const	noexcept	{ return mFromAddress; }
				InputMemoryBitStream&	GetPacketBuffer()			noexcept	{ return mPacketBuffer;}
				float					GetReceivedTime()	const	noexcept	{ return mReceivedTime; }

	private:
		SocketAddress			mFromAddress;
		InputMemoryBitStream	mPacketBuffer;
		float					mReceivedTime;
	};

	void		ReadIncomingPacketsIntoQueue();
	void		ProcessQueuedPackets();
	void		PacketProcessing(InputMemoryBitStream& input, const SocketAddress& address);

	std::queue<ReceivedPacket, std::list<ReceivedPacket>>	mPacketQueue;
	std::unique_ptr<UDPSocket>								mSocket;

	int			mBytesSentThisFrame;

	float		mDropPacketChance;
	float		mSimulatedLatency;

};