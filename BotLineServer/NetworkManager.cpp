#include "stdafx.h"
#include "NetworkManager.h"

constexpr int PORT = 8000;
constexpr int BUFFER_SIZE = 2048;
constexpr int MAX_PACKETS_PER_FRAME_COUNT = 10;

bool NetworkManager::Init(uint16_t inPort)
{
    mSocket = std::make_unique<UDPSocket>();

    bool result = mSocket->Create();
    if (result != NO_ERROR) {
        return false;
    }

    result = mSocket->Bind(SocketAddress(INADDR_ANY, inPort));
    if (result != NO_ERROR) {
        return false;
    }

    result = mSocket->SetNonBlockingMode(true);
    if (result != NO_ERROR) {
        return false;
    }

    return true;
}

void NetworkManager::ProcessIncomingPackets()
{
    this->ReadIncomingPacketsIntoQueue();

    this->ProcessQueuedPackets();
}

void NetworkManager::SendPacket(const OutputMemoryBitStream& inOutputStream, const SocketAddress& inFromAddress)
{
    int sentByteCount = mSocket->SendTo(inOutputStream.GetBufferPtr(), inOutputStream.GetByteLength(), inFromAddress);
}

void NetworkManager::ReadIncomingPacketsIntoQueue()
{
    char buffer[BUFFER_SIZE] = {};
    int bufferSize = sizeof(buffer);
    InputMemoryBitStream inputStream(buffer, bufferSize * 8);
    SocketAddress fromAddress;

    int receivedPackedCount = 0;

    int readByteCount = mSocket->ReceiveFrom(buffer, bufferSize, fromAddress);
    if (readByteCount == 0) {
        return;
    }
    else if (readByteCount == ~WSAECONNRESET) {
        return;
    }
    else if (readByteCount > 0) {
        inputStream.ResetToCapacity(readByteCount);
        ++receivedPackedCount;

        mPacketQueue.emplace(0.0f, inputStream, fromAddress);
    }
}

void NetworkManager::ProcessQueuedPackets()
{
    while (mPacketQueue.empty() == false) {
        ReceivedPacket& nextPacket = mPacketQueue.front();
        mPacketQueue.pop();

        // Process Packet...
        InputMemoryBitStream inputStream = nextPacket.GetPacketBuffer();
    }
}
