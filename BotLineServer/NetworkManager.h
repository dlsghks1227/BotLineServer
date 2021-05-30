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
	static	constexpr	double		sTimeout = 3.0;

	void		Initialize(uint16_t inPort = 8000)					noexcept(false);
	void		ProcessIncomingPackets(const Utility::Timer& timer)	noexcept;

	void		CheckForDisconnect()		noexcept;
	void		VerifyConnection()			noexcept;
	void		SendJetbotInfomation()		noexcept;
	
	void		SendPacket(const OutputMemoryBitStream& inOutputStream, const SocketAddress& inFromAddress) noexcept;
	
private:
	using		BotLineObjectPtr	= std::shared_ptr<BotLineObject>;
	using		JetbotObjectPtr		= std::shared_ptr<JetbotObject>;
	using		ControllerObjectPtr = std::shared_ptr<ControllerObject>;

	class ReceivedPacket
	{
	public:
		ReceivedPacket(const SystemTime& receivedTime, InputMemoryBitStream& inputMemoryBitStream, const SocketAddress& address) noexcept :
			mReceivedTime(receivedTime),
			mPacketBuffer(inputMemoryBitStream),
			mFromAddress(address) {}
			
			
		const	SocketAddress&			GetFromAddress()	const	noexcept	{ return mFromAddress; }
				InputMemoryBitStream&	GetPacketBuffer()			noexcept	{ return mPacketBuffer;}
				SystemTime				GetReceivedTime()	const	noexcept	{ return mReceivedTime; }

	private:
		SocketAddress			mFromAddress;
		InputMemoryBitStream	mPacketBuffer;
		SystemTime				mReceivedTime;
	};

	void		ReadIncomingPacketsIntoQueue()	noexcept;
	void		ProcessQueuedPackets()			noexcept;
	void		PacketProcessing(InputMemoryBitStream& input, const SocketAddress& address)				noexcept;
	void		PacketProcessingFromObject(InputMemoryBitStream& input, const BotLineObjectPtr& object)	noexcept;


	// 연결된 오브젝트 처리
	void		HandlePacketFromNewObject(InputMemoryBitStream& input, const SocketAddress& address)	noexcept;
	// 끊어진 오브젝트 처리
	void		HandleObjectDisconnect(const BotLineObjectPtr& object)		noexcept;

	std::queue<ReceivedPacket, std::list<ReceivedPacket>>				mPacketQueue;
	std::unique_ptr<UDPSocket>											mSocket;

	std::unordered_map<SocketAddress, BotLineObjectPtr>		mBotLineObjects;
	std::unordered_map<SocketAddress, JetbotObjectPtr>		mJetBotObjects;
	std::unordered_map<SocketAddress, ControllerObjectPtr>	mControllerObjects;

	int			mBytesSentThisFrame;

	float		mDropPacketChance;
	float		mSimulatedLatency;

};