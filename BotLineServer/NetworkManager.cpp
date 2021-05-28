#include "stdafx.h"
#include "NetworkManager.h"

constexpr int PORT = 8000;
constexpr int BUFFER_SIZE = 2048;
constexpr int MAX_PACKETS_PER_FRAME_COUNT = 10;

void NetworkManager::Initialize(uint16_t inPort) noexcept(false)
{
    mSocket = std::make_unique<UDPSocket>();

    mSocket->Initialize();

    mSocket->Bind(SocketAddress(INADDR_ANY, inPort));
    mSocket->SetNonBlockingMode(true);
}

void NetworkManager::ProcessIncomingPackets() noexcept
{
    this->ReadIncomingPacketsIntoQueue();

    this->ProcessQueuedPackets();
}

void NetworkManager::SendPacket(const OutputMemoryBitStream& inOutputStream, const SocketAddress& inFromAddress) noexcept
{
    int sentByteCount = mSocket->SendTo(inOutputStream.GetBufferPtr(), inOutputStream.GetByteLength(), inFromAddress);
}

void NetworkManager::ReadIncomingPacketsIntoQueue()
{
    char buffer[BUFFER_SIZE] = {};
    int bufferSize = sizeof(buffer);
    InputMemoryBitStream inputStream(buffer, bufferSize * 8);
    SocketAddress fromAddress;

    int readByteCount = mSocket->ReceiveFrom(buffer, bufferSize, fromAddress);
    if (readByteCount == 0) {
        return;
    }
    else if (readByteCount == ~WSAECONNRESET) {
        return;
    }
    else if (readByteCount > 0) {
        inputStream.ResetToCapacity(readByteCount);

        mPacketQueue.emplace(0.0f, inputStream, fromAddress);
    }
}

void NetworkManager::ProcessQueuedPackets()
{
    while (mPacketQueue.empty() == false) {
        ReceivedPacket& nextPacket = mPacketQueue.front();

        // Process Packet...
        InputMemoryBitStream inputStream = nextPacket.GetPacketBuffer();

        this->PacketProcessing(nextPacket.GetPacketBuffer(), nextPacket.GetFromAddress());

        mPacketQueue.pop();
    }
}

void NetworkManager::PacketProcessing(InputMemoryBitStream& input, const SocketAddress& address)
{
    //COMMAND command = COMMAND::DEFAULT;

    //input.Read(command);

    //OutputMemoryBitStream outputStream = OutputMemoryBitStream();
    //switch (command) {
    //case COMMAND::JETBOT_CONNECT:
    //    outputStream.Write(COMMAND::JETBOT_CONNECT);
    //    mSocket->SendTo(outputStream.GetBufferPtr(), outputStream.GetBitLength(), address);
    //    std::cout << outputStream.GetBufferPtr() << '\n';
    //    break;
    //case COMMAND::JETBOT_DISCONNECT:
    //    break;
    //case COMMAND::CONTROL_CONNECT:
    //    break;
    //case COMMAND::CONTROL_DISCONNECT:
    //    break;
    //}
}