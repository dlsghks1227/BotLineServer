#pragma once
#include "../framework.h"

namespace Component
{
	class NetworkComponent : public Util::Component
	{
	public:
		NetworkComponent(Util::Object* owner)		noexcept;
		virtual ~NetworkComponent();

		using	BotLineObjectPtr = std::shared_ptr<BotLineObject>;

		static constexpr		uint16_t	sPort		= 8000;
		static constexpr		uint32_t	sBufferSize = 2048;
		static constexpr		double		sTimeout	= 5.0;

		void		OnCreate()								noexcept	override;
		void		OnUpdate(const Util::Timer& timer)		noexcept	override;
		void		OnLateUpdate(const Util::Timer& timer)	noexcept	override;
		void		OnRender(const Util::Timer& timer)		noexcept	override;

		void		SendPacket(const OutputMemoryBitStream& outputStream, const SocketAddress& fromAddress)		noexcept;
		void		HandlePacketFromNewObject(const BotLineObjectPtr& object, const SocketAddress& fromAddress)	noexcept;


		const	std::unordered_map<SocketAddress, BotLineObjectPtr>&	GetBotLineObjects()		const	noexcept	{ return mBotLineObjects; }
	private:
		class ReceivedPacket
		{
		public:
			ReceivedPacket(const SystemTime& receivedTime, const InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress)	noexcept :
				mReceivedTime(receivedTime),
				mPacketBuffer(inputMemoryStream),
				mFromAddress(fromAddress)
			{
			}

			const	SystemTime&				GetReceivedTime()		const	noexcept	{ return mReceivedTime; }
			const	SocketAddress&			GetFromAddress()		const	noexcept	{ return mFromAddress; }
					InputMemoryBitStream&	GetPacketBuffer()				noexcept	{ return mPacketBuffer; }

		private:
			SocketAddress			mFromAddress;
			InputMemoryBitStream	mPacketBuffer;
			SystemTime				mReceivedTime;
		};

		void		ProcessIncomingPackets(const Util::Timer& timer)	noexcept;

		void		ReadIncomingPacketsIntoQueue()	noexcept;
		void		ProcessQueuedPackets()			noexcept;

		void		CheckForDisconnect()			noexcept;
		void		VerifyConnection()				noexcept;

		void		PacketProcessing(InputMemoryBitStream& input, const SocketAddress& fromAddress)										noexcept;
		void		HandleObjectDisconnect(const BotLineObjectPtr& object)		noexcept;

		std::queue<ReceivedPacket, std::list<ReceivedPacket>>		mPacketQueue;
		std::unique_ptr<UDPSocket>									mSocket;

		std::unordered_map<SocketAddress, BotLineObjectPtr>			mBotLineObjects;

		std::shared_ptr<JetbotProcessingComponent>					mJetbotProcessingComponent;
	};
};