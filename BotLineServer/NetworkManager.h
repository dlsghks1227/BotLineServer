#pragma once
#include "stdafx.h"

class NetworkManager
{
public:
	NetworkManager() :
		mBytesSentThisFrame(0),
		mDropPacketChance(0.0f),
		mSimulatedLatency(0.0f) {}
	virtual ~NetworkManager() = default;

	bool		Init(uint16_t inPort);
	void		ProcessIncomingPackets();
	
	void		SendPacket(const OutputMemoryBitStream& inOutputStream, const SocketAddress& inFromAddress);
	
private:
	class ReceivedPacket {
	public:
		ReceivedPacket(float inReceivedTime, InputMemoryBitStream& inInputMemoryBitStream, const SocketAddress& inAddress) :
			mReceivedTime(inReceivedTime),
			mPacketBuffer(inInputMemoryBitStream),
			mFromAddress(inAddress) {}
			
			
		const	SocketAddress&			GetFromAddress()	const	{ return mFromAddress; }
				InputMemoryBitStream&	GetPacketBuffer()			{ return mPacketBuffer;}
				float					GetReceivedTime()	const	{ return mReceivedTime; }

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