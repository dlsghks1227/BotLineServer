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

void NetworkManager::ProcessIncomingPackets(const Utility::Timer& timer) noexcept
{
    ReadIncomingPacketsIntoQueue(timer.GetTotalSeconds());

    ProcessQueuedPackets();
}

void NetworkManager::CheckForDisconnect(const Utility::Timer& timer) noexcept
{
}

void NetworkManager::SendPacket(const OutputMemoryBitStream& inOutputStream, const SocketAddress& inFromAddress) noexcept
{
    int sentByteCount = mSocket->SendTo(inOutputStream.GetBufferPtr(), inOutputStream.GetByteLength(), inFromAddress);
}

void NetworkManager::ReadIncomingPacketsIntoQueue(double inReceviedTime) noexcept
{
    char buffer[BUFFER_SIZE] = {};
    int bufferSize = sizeof(buffer);
    InputMemoryBitStream inputStream(buffer, bufferSize * 8);
    SocketAddress fromAddress;

    int readByteCount = mSocket->ReceiveFrom(buffer, bufferSize, fromAddress);
    if (readByteCount == 0)
    {
        return;
    }
    else if (readByteCount == ~WSAECONNRESET)
    {
        return;
    }
    else if (readByteCount > 0)
    {
        inputStream.ResetToCapacity(readByteCount);

        mPacketQueue.emplace(inReceviedTime, inputStream, fromAddress);
    }
}

void NetworkManager::ProcessQueuedPackets() noexcept
{
    while (mPacketQueue.empty() == false)
    {
        ReceivedPacket& nextPacket = mPacketQueue.front();

        // Process Packet...
        double receivedTime = nextPacket.GetReceivedTime();
        InputMemoryBitStream inputStream = nextPacket.GetPacketBuffer();

        PacketProcessing(receivedTime, nextPacket.GetPacketBuffer(), nextPacket.GetFromAddress());

        mPacketQueue.pop();
    }
}

void NetworkManager::PacketProcessing(double receivedTime, InputMemoryBitStream& input, const SocketAddress& address) noexcept
{
    auto itr = mBotLineObject.find(address);
    if (itr == mBotLineObject.cend())
    {
        // ���� ����� ������Ʈ ó��
        HandlePacketFromNewObject(input, address);
    }
    else
    {
        // ������ ����� ������Ʈ ó��
        PacketProcessingFromObject(receivedTime, input, (*itr).second);
    }
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

void NetworkManager::PacketProcessingFromObject(double receivedTime, InputMemoryBitStream& input, const BotLineObjectPtr& object) noexcept
{
    // Read command
}

void NetworkManager::HandlePacketFromNewObject(InputMemoryBitStream& input, const SocketAddress& address) noexcept
{
    // ���� ������ ��ü�� map�� ����

    // send connection success packet
}

void NetworkManager::HandleObjectDisconnect(const BotLineObjectPtr& object) noexcept
{
    //mBotLineObject.erase(object.GetSocketAddress());
}