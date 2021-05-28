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

	void		Initialize(uint16_t inPort = 8000)					noexcept(false);
	void		ProcessIncomingPackets(const Utility::Timer& timer)	noexcept;
	void		CheckForDisconnect(const Utility::Timer& timer)		noexcept;
	
	void		SendPacket(const OutputMemoryBitStream& inOutputStream, const SocketAddress& inFromAddress) noexcept;
	
private:
	using		BotLineObjectPtr = std::shared_ptr<BotLineObject>;

	class ReceivedPacket
	{
	public:
		ReceivedPacket(double inReceivedTime, InputMemoryBitStream& inInputMemoryBitStream, const SocketAddress& inAddress) noexcept :
			mReceivedTime(inReceivedTime),
			mPacketBuffer(inInputMemoryBitStream),
			mFromAddress(inAddress) {}
			
			
		const	SocketAddress&			GetFromAddress()	const	noexcept	{ return mFromAddress; }
				InputMemoryBitStream&	GetPacketBuffer()			noexcept	{ return mPacketBuffer;}
				double					GetReceivedTime()	const	noexcept	{ return mReceivedTime; }

	private:
		SocketAddress			mFromAddress;
		InputMemoryBitStream	mPacketBuffer;
		double					mReceivedTime;
	};

	void		ReadIncomingPacketsIntoQueue(double inReceviedTime)	noexcept;
	void		ProcessQueuedPackets()								noexcept;
	void		PacketProcessing(double receivedTime, InputMemoryBitStream& input, const SocketAddress& address)			noexcept;
	void		PacketProcessingFromObject(double receivedTime, InputMemoryBitStream& input, const BotLineObjectPtr& object)	noexcept;


	// 연결된 오브젝트 처리
	void		HandlePacketFromNewObject(InputMemoryBitStream& input, const SocketAddress& address)	noexcept;
	// 끊어진 오브젝트 처리
	void		HandleObjectDisconnect(const BotLineObjectPtr& object)		noexcept;

	std::queue<ReceivedPacket, std::list<ReceivedPacket>>				mPacketQueue;
	std::unique_ptr<UDPSocket>											mSocket;

	std::unordered_map<SocketAddress, BotLineObjectPtr>	mBotLineObject;

	int			mBytesSentThisFrame;

	float		mDropPacketChance;
	float		mSimulatedLatency;

};